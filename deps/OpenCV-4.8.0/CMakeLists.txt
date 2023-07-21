include(OpenCVConfig.cmake)

add_library(opencv_48 INTERFACE)

target_include_directories(opencv_48 INTERFACE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(opencv_48 INTERFACE ${OpenCV_LIBS})
