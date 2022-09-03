cmake_minimum_required(VERSION 3.12.4 FATAL_ERROR)

project(xbrz)

find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)

include_directories(src)

set(SOURCE_FILES
    CImg.h
    main.cpp
    xbrzlib/xbrz.cpp
    xbrzlib/xbrz.h
    xbrzlib/xbrz_config.h
    xbrzlib/xbrz_tools.h
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE PNG::PNG ${JPEG_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${JPEG_INCLUDE_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
