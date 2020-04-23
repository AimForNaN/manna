project(manna)
cmake_minimum_required(VERSION 3.5)

LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/sword)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs/manna-server)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Sword REQUIRED)

include_directories(
    /usr/local/include/sword
    /usr/include/sword
)

add_executable(${PROJECT_NAME} main.cpp)

include(src/CMakeLists.txt)

add_dependencies(manna manna-server)

link_directories(
    /usr/lib
    /usr/local/lib
)

target_link_libraries(manna Qt5::Core)
target_link_libraries(manna sword)
target_link_libraries(manna manna-server)
