cmake_minimum_required( VERSION 2.8.11 )
project( ITSP_interpolation )
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
find_package( OpenCV REQUIRED )
include_directories( include /usr/include)

set( SPLINE_FIT_SRCS
    src/spline_fit.cpp
    src/main_time.cpp )
add_executable( spline_fit ${SPLINE_FIT_SRCS} )
target_link_libraries( spline_fit ${OpenCV_LIBS})

set( SPLINE_FIT_REAL_SRCS
    src/spline_fit.cpp
    src/point_details.cpp
    src/main_real.cpp )
add_executable( spline_fit_real ${SPLINE_FIT_REAL_SRCS} )
target_link_libraries( spline_fit_real ${OpenCV_LIBS})
