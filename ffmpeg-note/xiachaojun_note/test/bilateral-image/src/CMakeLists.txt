cmake_minimum_required(VERSION 3.13)
project(image-bilateral)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)

#set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing OpenCVConfig.cmake file
# via the command line or GUI
find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

## Declare the executable target built from your sources
#add_executable(opencv_example example.cpp)
#
## Link your application with OpenCV libraries
#target_link_libraries(opencv_example LINK_PRIVATE ${OpenCV_LIBS})

#set(INC_DIR ../include)
#set(LINK_DIR ../lib)

#include_directories(${INC_DIR})
#link_directories(${LINK_DIR})
#link_libraries("libopencv_dnn.3.4.dylib"
#        "libopencv_core.3.4.dylib"
#        "libopencv_highgui.3.4.dylib"
#        "libopencv_imgproc.3.4.dylib")

add_executable(image-bilateral ${SOURCE_FILES})

target_link_libraries(image-bilateral LINK_PRIVATE ${OpenCV_LIBS})

#target_link_libraries(image-bilateral
#        opencv_core opencv_dnn opencv_highgui opencv_imgproc
#        opencv_ml opencv_objdetect opencv_shape opencv_stitching
#        )
