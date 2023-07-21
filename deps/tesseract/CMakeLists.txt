


find_package(Tesseract REQUIRED)

if (Tesseract_FOUND)
    add_library(tesseract_532 INTERFACE)
    target_link_directories(tesseract_532 INTERFACE ${Tesseract_LIBRARY_DIRS})
    target_include_directories(tesseract_532 INTERFACE ${Tesseract_INCLUDE_DIRS})
    message("Tesseract Version: ${Tesseract_VERSION}")
endif ()
