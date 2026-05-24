# Trading Engine

Minimal C++17 trading backtest engine skeleton.

## Build and Run

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/trading_engine
```

## Module Layout

```text
app        program orchestration entry
data       data ingestion
market     market data models
strategy   strategy decisions, factors, and ranking
backtest   backtest simulation
portfolio  account and positions
execution  order execution abstractions
risk       risk constraints
report     result reports
infra      shared infrastructure and core types
```

Project-specific Codex conventions live in `.codex/skills/trading-system`.
