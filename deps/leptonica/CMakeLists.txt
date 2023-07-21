

find_package(Leptonica REQUIRED)

if (Leptonica_FOUND)
    message(leptonica Version: ${Leptonica_VERSION})
    add_library(leptonica_184 INTERFACE)
    target_include_directories(leptonica_184 INTERFACE ${Leptonica_INCLUDE_DIRS})
    target_link_libraries(leptonica_184 INTERFACE ${Leptonica_LIBRARIES})
endif ()






