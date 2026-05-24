cmake_minimum_required(VERSION 3.16)

project(trading_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(trading_engine
    app/main.cpp
    data/src/offline_data_loader.cpp
    data/src/online_data_loader.cpp
)

target_include_directories(trading_engine
    PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/data/include
)
