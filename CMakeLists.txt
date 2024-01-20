cmake_minimum_required(VERSION 3.5)

project(visp_ball_tracking_test)


include($ENV{VISP_WS}/vcpkg/scripts/buildsystems/vcpkg.cmake)
#include()
#include_directories(${VISP_INCLUDE_DIRS})
#include_directories($ENV{VISP_WS}/vcpkg/installed/x64-linux/lib/)
#include_directories($ENV{VISP_WS}/vcpkg/installed/x64-linux/include/librealsense2/)
 
find_package(VISP REQUIRED visp_robot visp_io)
include(CMakePrintHelpers)

# search for vcpkg managed realsense library
#set(realsense2_DIR "$ENV{VISP_WS}/vcpkg/installed/x64-linux/lib/realsense2/")
find_package(realsense2 CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)


# this is heuristically generated, and may not be correct
find_package(imgui CONFIG REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ./build)

#Create definition of a filepath to be used in simulation.cpp
set(simulation_file "video_samples/stairs.bag")

get_filename_component(PLAYBACK_ABS_FILEPATH "${simulation_file}" ABSOLUTE)

#message(${PLAYBACK_ABS_FILEPATH})
foreach(cpp ${example_cpp})
  visp_add_target(${cpp})
  if(COMMAND visp_add_dependency)
    visp_add_dependency(${cpp} "examples")
    visp_set_source_file_compile_flag(${cpp} ${CXX_FLAGS_MUTE_WARNINGS})
  endif()
endforeach()

if(VISP_HAVE_UR_RTDE)
  # Add specific build flag to turn off warnings coming from ur_rtde 3rd party
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "-Wno-unused-parameter")
endif()
if(VISP_HAVE_REALSENSE2)
  # Add specific build flag to turn off warnings coming from librealsense 3rd party
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "/wd4244")
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "/wd4267")
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "-Wno-deprecated-copy")
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "-Wno-deprecated-declarations")
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "-Wno-ignored-qualifiers")
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "-Wno-overloaded-virtual")
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "-Wno-reorder")
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "-Wno-sign-compare")
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "-Wno-unused-function")
  list(APPEND CXX_FLAGS_MUTE_WARNINGS "-Wno-unqualified-std-cast-call")
endif()

# Add abs_path as a preprocessor macro
add_definitions(-DPLAYBACK_ABS_FILEPATH="${PLAYBACK_ABS_FILEPATH}")

add_executable(${CMAKE_PROJECT_NAME} gui_test.cpp imgui_impl_opengl3.cpp imgui_impl_glfw.cpp)
#add_executable(${CMAKE_PROJECT_NAME} )

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE realsense2::fw realsense2::usb realsense2::rsutils realsense2::realsense2)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${VISP_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE glfw)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})