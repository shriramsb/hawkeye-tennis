cmake_minimum_required(VERSION 2.8)
project( ITSP_detection)
set(OpenCV_DIR '/home/shriram/opencv-3.1.0/build')
find_package(OpenCV 3.1 REQUIRED)
include_directories( include /usr/include )

add_executable( calibration src/calibration.cpp )
target_link_libraries( calibration ${OpenCV_LIBS})

add_executable( detection src/detection_main.cpp src/Camera.cpp
				src/locator.cpp src/CamFrame.cpp )
target_link_libraries( detection ${OpenCV_LIBS})


