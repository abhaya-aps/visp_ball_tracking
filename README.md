# About

Visual tracking of objects using Realsense D410 camera and UR10 robot.

# GUI

The project will most likely use [ImGUI](https://github.com/ocornut/imgui) as Realsense seems to depend on it.

# Installation steps

1. Install [VISP](https://visp-doc.inria.fr/doxygen/visp-daily/tutorial-install-ubuntu.html) as described in the documentation.
2. ```cd $VISP_WS```
3. Proceed with RealSense library installation using vcpkg as described [here](https://github.com/IntelRealSense/librealsense).
4. ```cmake . && make```

# Installing libraries

```
cd vcpkg
./vcpkg install imgui
sudo apt update
sudo apt install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev pkg-config libglfw3 libglfw3-dev
./vcpkg install glfw3
./vcpkg install opengl
./vcpkg integrate install

```

# Downloading sample *.bag files

```
chmod +x download_sample_bag_files.sh
./download_sample_bag_files.sh
```
Files will be saved in _./video_samples_ directory. 

*WARNING: Significant amount of data!*

