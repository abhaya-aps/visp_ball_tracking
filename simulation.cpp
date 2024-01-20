#include <iostream>
//#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>
#include <imgui.h>
//#include "imgui_impl_glfw.h"

#ifndef PLAYBACK_ABS_FILEPATH
    #define PLAYBACK_ABS_FILEPATH "error" //this one is defined in CmakeLists
#endif
// Function to estimate distance (in m) based on the depth information
double estimateDistance(const rs2::depth_frame& depth_frame, int cx, int cy) {
    // Get the depth value at the specified pixel
    float depth_meters = depth_frame.get_distance(cx, cy);

    return depth_meters;
}

int main() {
    // Initialize the RealSense pipeline
    ImGui::Text("Hello, world %d", 123);
if (ImGui::Button("Save"))
    MySaveFunction();
ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
ImGui::SliderFloat("float", &f, 0.0f, 1.0f);

    rs2::pipeline pipe;
    rs2::config cfg;
    //make sure to download the file using ./download_sample_bag_files.sh script
    //PLAYBACK_ABS_FILEPATH is defined in CMakeLists.txt do not edit this value!
    //don't forget to run "cmake . && make" after redefining the source *.bag file
    cfg.enable_device_from_file(PLAYBACK_ABS_FILEPATH);

    //cfg.enable_stream(RS2_STREAM_COLOR, 640, 480, RS2_FORMAT_BGR8, 30);
    //cfg.enable_stream(RS2_STREAM_DEPTH, 640, 480, RS2_FORMAT_Z16, 30);
    pipe.start(cfg);

    // Create a colorizer for the depth stream
    //rs2::colorizer color_map;
}