cmake_minimum_required( VERSION 2.8.11 )
project( ITSP_interpolation )
find_package( OpenCV REQUIRED )
include_directories( include )
set( SPLINE_SRCS
    src/spline.cpp
    src/main.cpp )
add_executable( spline ${SPLINE_SRCS} )
target_link_libraries( spline ${OpenCV_LIBS} )
