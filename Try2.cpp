#include <iostream>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

// Function to estimate distance (in m) based on the depth information
double estimateDistance(const rs2::depth_frame& depth_frame, int cx, int cy) {
    // Get the depth value at the specified pixel
    float depth_meters = depth_frame.get_distance(cx, cy);

    return depth_meters;
}

int main() {
    // Initialize the RealSense pipeline
    rs2::pipeline pipe;
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    pipe.start(cfg);

    // Create a colorizer for the depth stream
    rs2::colorizer color_map;

    // Main loop
    while (true) {
        // Wait for frames from the RealSense camera
        rs2::frameset frames = pipe.wait_for_frames();

        // Align the depth frame to the color frame
        rs2::align align_to_color(RS2_STREAM_COLOR);
        auto processed = align_to_color.process(frames);
        rs2::frame color_frame = processed.get_color_frame();
        rs2::depth_frame depth_frame = processed.get_depth_frame();

        // Convert the RealSense color frame to an OpenCV format
        cv::Mat rgbFrame(cv::Size(640, 480), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);

        // Convert RGB frame from BGR to HSV color space
        cv::Mat hsv;
        cv::cvtColor(rgbFrame, hsv, cv::COLOR_BGR2HSV);

        // Define the range of green color in HSV
        cv::Scalar lower_green(40, 40, 40);
        cv::Scalar upper_green(80, 255, 255);

        // Define the range of orange color in HSV
        cv::Scalar lower_orange(0, 100, 100);
        cv::Scalar upper_orange(20, 255, 255);

        // Define the range of pink color in HSV
        cv::Scalar lower_pink(160, 100, 100);
        cv::Scalar upper_pink(190, 255, 255);

        // Threshold the image to get binary masks of green, orange, and pink regions
        cv::Mat mask_green, mask_orange, mask_pink;
        cv::inRange(hsv, lower_green, upper_green, mask_green);
        cv::inRange(hsv, lower_orange, upper_orange, mask_orange);
        cv::inRange(hsv, lower_pink, upper_pink, mask_pink);

        // Find contours in the binary masks
        std::vector<std::vector<cv::Point>> contours_green, contours_orange, contours_pink;
        cv::findContours(mask_green, contours_green, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::findContours(mask_orange, contours_orange, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::findContours(mask_pink, contours_pink, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        // Counters for each color
        int green_count = 0, orange_count = 0, pink_count = 0;


        // Process green contours
        for (const auto& contour : contours_green) {
            // Find the moments of the contour
            cv::Moments moments = cv::moments(contour);

            // Calculate the area of the contour
            double area = moments.m00;

            // Only consider contours with areas within a certain range as valid green balls
            if (area > 1500 && area < 5000) {
                // Increment green count
                green_count++;

                // Calculate the centroid of the contour
                double cx = moments.m10 / area;
                double cy = moments.m01 / area;

                // Estimate distance based on the aligned depth information
                double distance = estimateDistance(depth_frame, static_cast<int>(cx), static_cast<int>(cy));

                 // Draw a circle around the centroid with green color
                cv::circle(rgbFrame, cv::Point(static_cast<int>(cx), static_cast<int>(cy)), 10, cv::Scalar(0, 255, 0), -1);

                // Display the distance on the RGB frame
                cv::putText(rgbFrame, "Distance: " + std::to_string(distance) + " m",
                            cv::Point(static_cast<int>(cx), static_cast<int>(cy) - 20),
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
            }
        }

          // Process orange contours
        for (const auto& contour : contours_orange) {
            // Find the moments of the contour
            cv::Moments moments = cv::moments(contour);

            // Calculate the area of the contour
            double area = moments.m00;

            // Only consider contours with areas within a certain range as valid orange balls
            if (area > 1500 && area < 50000) {
                // Increment orange count
                orange_count++;

                // Calculate the centroid of the contour
                double cx = moments.m10 / area;
                double cy = moments.m01 / area;

                // Estimate distance based on the aligned depth information
                double distance = estimateDistance(depth_frame, static_cast<int>(cx), static_cast<int>(cy));

                // Draw a circle around the centroid with orange color
                cv::circle(rgbFrame, cv::Point(static_cast<int>(cx), static_cast<int>(cy)), 10, cv::Scalar(0, 165, 255), -1);

                // Display the distance on the RGB frame
                cv::putText(rgbFrame, "Distance: " + std::to_string(distance) + " cm",
                            cv::Point(static_cast<int>(cx), static_cast<int>(cy) - 20),
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 165, 255), 1);
            }
        }

        // Process pink contours
        for (const auto& contour : contours_pink) {
            // Find the moments of the contour
            cv::Moments moments = cv::moments(contour);

            // Calculate the area of the contour
            double area = moments.m00;

            // Only consider contours with areas within a certain range as valid pink balls
            if (area > 1500 && area < 50000) {
                // Increment pink count
                pink_count++;

                // Calculate the centroid of the contour
                double cx = moments.m10 / area;
                double cy = moments.m01 / area;

                // Estimate distance based on the aligned depth information
                double distance = estimateDistance(depth_frame, static_cast<int>(cx), static_cast<int>(cy));

                // Draw a circle around the centroid with pink color
                cv::circle(rgbFrame, cv::Point(static_cast<int>(cx), static_cast<int>(cy)), 10, cv::Scalar(255, 182, 193), -1);

                // Display the distance on the RGB frame
                cv::putText(rgbFrame, "Distance: " + std::to_string(distance) + " cm",
                            cv::Point(static_cast<int>(cx), static_cast<int>(cy) - 20),
                            cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 182, 193), 1);
            }
        }

        // Display the original RGB frame with circular markers, counts, and distances
        cv::putText(rgbFrame, "Green Balls: " + std::to_string(green_count), cv::Point(10, 30),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
        cv::putText(rgbFrame, "Orange Balls: " + std::to_string(orange_count), cv::Point(10, 70),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 165, 255), 2);
        cv::putText(rgbFrame, "Pink Balls: " + std::to_string(pink_count), cv::Point(10, 110),
                    cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 182, 193), 2);

        cv::imshow("Ball Detection", rgbFrame);

        // Break the loop if the 'Esc' key is pressed
        if (cv::waitKey(1) == 27) {
            break;
        }
    }

    // Stop the RealSense pipeline and close the OpenCV windows
    pipe.stop();
    cv::destroyAllWindows();

    return 0;
}
