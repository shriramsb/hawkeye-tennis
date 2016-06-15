cmake_minimum_required( VERSION 2.8.11 )
project( ITSP_detection )
find_package( OpenCV REQUIRED )
include_directories( include /usr/include)

add_executable( calibration src/calib.cpp )
target_link_libraries( calibration ${OpenCV_LIBS})

add_executable( detection src/detector.cpp src/main_detec.cpp)
target_link_libraries( detection ${OpenCV_LIBS})
