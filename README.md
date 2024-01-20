# About

Visual tracking of objects using Realsense D410 camera and UR10 robot.

# Installation steps

1. Install [VISP](https://visp-doc.inria.fr/doxygen/visp-daily/tutorial-install-ubuntu.html) as described in the documentation.
2. ```cd $VISP_WS```
3. Proceed with RealSense library installation using vcpkg as described [here](https://github.com/IntelRealSense/librealsense).
4. ```cmake . && make```


# Downloading sample *.bag files

```
chmod +x download_sample_bag_files.sh
./download_sample_bag_files.sh
```
Files will be saved in _./video_samples_ directory. 

*WARNING: Significant amount of data!*

