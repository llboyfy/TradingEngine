cmake_minimum_required(VERSION 3.16)

project(trading_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(trading_engine
    tools/run_backtest/main.cpp
)

target_include_directories(trading_engine
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
