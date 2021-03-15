cmake_minimum_required(VERSION 2.8)

project(wechat)

find_package(CURL REQUIRED)

include_directories(. ${CURL_INCLUDE_DIRS})

aux_source_directory(. FILES)

MESSAGE(STATUS ${FILES})

set(CMAKE_CXX_FLAGS "$ENV{CMAKE_CXX_FLAGS} -std=c++11 -g -ggdb")
add_executable(wechat ${FILES})

add_subdirectory(base)
add_subdirectory(core)

target_link_libraries(wechat core base ${CURL_LIBRARIES})

install(TARGETS wechat RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/publish/)
