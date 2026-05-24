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
data       data ingestion and local development datasets
```

Local development datasets live under `data/dev/` and are ignored by Git.
Project-specific Codex conventions live in `.codex/skills/trading-system`.
