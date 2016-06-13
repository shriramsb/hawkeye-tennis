cmake_minimum_required( VERSION 2.8.11 )
project( ITSP_interpolation )
find_package( OpenCV REQUIRED )
include_directories( include /usr/include)

set( SPLINE_FIT_SRCS
    src/spline_fit.cpp
    src/main_fit.cpp )
add_executable( spline_fit ${SPLINE_FIT_SRCS} )
target_link_libraries( spline_fit ${OpenCV_LIBS})
