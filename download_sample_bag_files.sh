#!/bin/bash

#links from sample data as described in here: https://github.com/IntelRealSense/librealsense/blob/master/doc/sample-data.md
links=(
    "https://librealsense.intel.com/rs-tests/TestData/outdoors.bag" 
    "https://librealsense.intel.com/rs-tests/TestData/stairs.bag"
    "https://librealsense.intel.com/rs-tests/TestData/structured.bag"
    "https://librealsense.intel.com/rs-tests/TestData/depth_under_water.bag"
)

# Function to extract filenames from URLs
extract_filenames() {
    urls=("$@")

    # Initialize an empty array for filenames
    declare -a filenames

    # Loop through the URLs and extract filenames
    for url in "${urls[@]}"; do
        # Use basename to extract the filename
        filename=$(basename "$url")
        filenames+=("$filename")
    done

    # Return the array of filenames
    echo "${filenames[@]}"
}

# Function to list files in a folder and return an array
list_files() {
    folder_path="$1"

    # Check if the folder exists
    if [ ! -d "$folder_path" ]; then
        echo "Error: Folder '$folder_path' does not exist."
        return 1
    fi

    # Use ls command to list files and store in an array
    files=($(ls "$folder_path"))

    # Return the array of files
    echo "${files[@]}"
}

# Function to find the first occurrence of a filename in links
find_first_entry() {
    filename="$1"
    
    # Iterate through the links
    # Iterate through the links
    for link in "${links[@]}"; do
        # Extract filename from the link
        link_filename=$(basename "$link")

        # Check if the filename matches
        if [ "$link_filename" == "$filename" ]; then
            echo "$link"  # Return the link
            return 0  # Return success
        fi
    done

    echo "Filename '$filename' not found in any links."
    return 1  # Return failure
}

# Function to check if files in a folder are in the filename array
check_files_in_folder() {
    folder_path="$1"
    url_filenames=("$@")

    # Check if the folder exists
    if [ ! -d "$folder_path" ]; then
        echo "Error: Folder '$folder_path' does not exist."
        return 1
    fi

    # Loop through the files in the folder
    for file in "$folder_path"/*; do
        if [ -f "$file" ]; then
            # Check if the filename is in the array
            if [[ " ${url_filenames[@]} " =~ " $(basename "$file") " ]]; then
                echo "File '$file' is in the filename array."
            else
                echo "File '$file' is not in the filename array."
            fi
        else
            for download_file in "${url_filenames[@]}"; do
                url=$(find_first_entry "$download_file")
                #echo $url
                #url=find_first_entry "$download_file"
                # Check the exit status of the function
                if ! [ $? -eq 0 ]; then
                    echo "File ${download_file} is already downloaded!"
                else
                    #url=$(find_first_entry "$download_file")
                    echo "Filename not found!"
                    echo "Downloading '${download_file}'... "
                    curl -o "$folder_path/$download_file" "$url"
                    #curl -o "$folder_path/$download_file" "$url"
                fi
            done
        fi
    done
}

folder_name="video_samples"

# Call the function with the array of links and store the result in an array
filename_array=($(extract_filenames "${links[@]}"))

#echo $filename_array

if [ -d "$folder_name" ]; then
    echo "Folder '$folder_name' already exists."
    # Example usage
    file_list=($(list_files "$folder_name"))

    # Call the function with the list of files
    check_files_in_folder "$folder_name" "${filename_array[@]}"
else
    mkdir "$folder_name"
    echo "Folder '$folder_name' created successfully."
    # Example usage
    file_list=($(list_files "$folder_name"))

    # Call the function with the list of files
    check_files_in_folder "$folder_name" "${filename_array[@]}"
fi
