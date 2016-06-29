cmake_minimum_required(VERSION 2.8)
project( ITSP_detection)
find_package(OpenCV REQUIRED)
include_directories( include /usr/include )

add_executable( calibration src/calibration.cpp )
target_link_libraries( calibration ${OpenCV_LIBS})

add_executable( detection src/detection_main.cpp src/Camera.cpp
				src/locator.cpp src/CamFrame.cpp src/point_details.cpp )
target_link_libraries( detection ${OpenCV_LIBS})

add_executable( framepicker src/framepicker.cpp )
target_link_libraries( framepicker ${OpenCV_LIBS})

add_executable( fund_mat src/fund_mat.cpp )
target_link_libraries( fund_mat ${OpenCV_LIBS})

add_executable (find_undetected src/find_undetected.cpp src/point_details.cpp )
target_link_libraries( find_undetected ${OpenCV_LIBS})

add_executable (manual_detection src/manual_detection.cpp src/point_details.cpp )
target_link_libraries( manual_detection ${OpenCV_LIBS})
