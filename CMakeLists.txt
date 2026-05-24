cmake_minimum_required(VERSION 3.16)

project(trading_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TRADING_ENGINE_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/backtest/include
    ${PROJECT_SOURCE_DIR}/data/include
    ${PROJECT_SOURCE_DIR}/execution/include
    ${PROJECT_SOURCE_DIR}/infra/include
    ${PROJECT_SOURCE_DIR}/market/include
    ${PROJECT_SOURCE_DIR}/portfolio/include
    ${PROJECT_SOURCE_DIR}/report/include
    ${PROJECT_SOURCE_DIR}/risk/include
    ${PROJECT_SOURCE_DIR}/strategy/include
)

add_executable(trading_engine
    app/main.cpp
    backtest/src/backtest_engine.cpp
    backtest/src/rebalance_scheduler.cpp
    data/src/csv_bar_loader.cpp
    execution/src/order_simulator.cpp
    market/src/market_data.cpp
    portfolio/src/portfolio.cpp
    report/src/metrics.cpp
    report/src/report_writer.cpp
    risk/src/risk_rule.cpp
    strategy/src/factor_calculator.cpp
    strategy/src/factor_ranker.cpp
    strategy/src/monthly_rotation_strategy.cpp
)

target_include_directories(trading_engine
    PRIVATE
        ${TRADING_ENGINE_INCLUDE_DIRS}
)
