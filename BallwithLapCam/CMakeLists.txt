cmake_minimum_required(VERSION 2.8)
project( BallWithLapCam )
add_executable( BallWithLapCam BallWithLapCam.cpp )
target_link_libraries( BallWithLapCam ${OpenCV_LIBRARIES} )
