---
name: trading-system
description: TradingEngine project architecture and workflow conventions for the C++17 trading backtest engine. Use when modifying this repository's module layout, adding engine features, deciding where code belongs, updating CMake targets, or preserving the expected trading-system structure across data, market, strategy, backtest, portfolio, execution, risk, report, infra, and app modules.
---

# Trading System

## Overview

Use this skill to keep TradingEngine changes aligned with the repository's intended engine architecture. Prefer small, compile-safe edits that preserve the module boundaries below.

## Module Boundaries

- `app`: Program orchestration and executable entry points. Keep CLI/bootstrap code here.
- `data`: Data ingestion, file/API loaders, normalization into domain models, and local sample data.
- `market`: Market data models and market data access abstractions.
- `strategy`: Strategy decisions, factor calculation, ranking, and portfolio target generation.
- `backtest`: Simulation loop, rebalance scheduling, event/time progression, and coordination of strategy/execution/portfolio.
- `portfolio`: Account state, cash, positions, trades, and accounting logic.
- `execution`: Order, fill, and broker/execution simulation abstractions.
- `risk`: Risk constraints, pre-trade checks, exposure limits, and guardrails.
- `report`: Metrics, reporting outputs, summaries, and persistence of backtest results.
- `infra`: Shared infrastructure and low-level cross-module types/utilities.

## File Placement

Place public headers under `<module>/include/trading/<domain>/`.
Place implementations under `<module>/src/`.
Place orchestration entry files directly under `app/`.

Current domain paths:

- `trading/backtest`
- `trading/core`
- `trading/data`
- `trading/execution`
- `trading/factor`
- `trading/market`
- `trading/portfolio`
- `trading/ranking`
- `trading/report`
- `trading/risk`
- `trading/strategy`

## Build Rules

Keep the project buildable with:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/trading_engine
```

When adding a new `.cpp`, update `CMakeLists.txt` so it is compiled into the `trading_engine` target. Keep generated files in `build/`.

## Change Workflow

1. Read the touched module headers and neighboring implementations before editing.
2. Keep responsibilities inside the module boundary; introduce cross-module dependencies deliberately.
3. Prefer domain names that match the existing `trading/<domain>` include paths.
4. Verify with CMake build after structural or C++ changes.
