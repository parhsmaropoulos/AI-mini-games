cmake_minimum_required(VERSION 2.8)
project( Digit-Detection )
dire
find_package( "C:/opencv/build/x64/vc15/lib/OpenCVConfig" REQUIRED )
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable( Digit-Detection readimage.cpp )

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)