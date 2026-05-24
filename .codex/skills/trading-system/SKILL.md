---
name: trading-system
description: TradingEngine project architecture and workflow conventions for the C++17 trading backtest engine. Use when modifying this repository's minimal app/data layout, adding engine features, deciding where code belongs, updating CMake targets, or preserving the current low-friction development structure.
---

# Trading System

## Overview

Use this skill to keep TradingEngine changes aligned with the repository's current minimal architecture. Prefer small, compile-safe edits and avoid adding top-level module directories until the implementation needs them.

## Module Boundaries

- `app`: Program orchestration and executable entry points. Keep CLI/bootstrap code here.
- `data`: Data ingestion work and local development datasets. Keep large development-only datasets under `data/dev/`, which is ignored by Git.

## File Placement

Place orchestration entry files directly under `app/`.
Do not wire local development data into code until the user explicitly asks for integration.
Keep data ingestion headers directly under `data/include/` and implementations under `data/src/`.
Use short module include paths such as `"offline_data_loader.h"` and keep `data/include/` configured as an include root in CMake and editor settings.

## Build Rules

Keep the project buildable with:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/trading_engine
```

When adding a new `.cpp`, update `CMakeLists.txt` so it is compiled into the `trading_engine` target. Keep generated files in `build/` and local datasets in `data/dev/`.

## Comment Style

- Describe intent or domain meaning; do not repeat the commented function, variable, or class name.
- Place member-variable comments at the end of the declaration line.
- Place class and struct comments in headers immediately before the declaration.
- Prefer short comments only where they reduce ambiguity.

## Naming Style

- Use PascalCase for public methods.
- Use lowerCamelCase for private methods and helper functions.
- Do not use underscores as word separators in identifiers; use camel case.
- Keep member variables private when possible, name them lowerCamelCase, and add a trailing `_`.

## Change Workflow

1. Read the touched module headers and neighboring implementations before editing.
2. Keep responsibilities inside `app` or `data` unless a new top-level module is clearly needed.
3. Avoid committing files from `data/dev/`.
4. Verify with CMake build after structural or C++ changes.
