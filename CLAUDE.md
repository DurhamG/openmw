# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

OpenMW is an open-source reimplementation of the Morrowind game engine, written in C++20. It aims to be a faithful reimplementation minus crash bugs and stability issues. The project is hosted on GitLab (https://gitlab.com/OpenMW/openmw) with a GitHub mirror.

## Build Commands

CMake-based build system. Default build type is RelWithDebInfo.

### Build (this repo, Windows MSVC)
```bash
cmake --build MSVC2022_64 --config RelWithDebInfo
```
Build directory is `MSVC2022_64`. Must run from a VS Developer Command Prompt.

### Configure and build (Windows with vcpkg, generic)
```bash
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

### Configure and build (Linux)
```bash
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make -j$(nproc)
```

### Build with tests enabled
Tests are OFF by default and must be explicitly enabled:
```bash
cmake .. -DBUILD_COMPONENTS_TESTS=ON -DBUILD_OPENMW_TESTS=ON -DBUILD_OPENCS_TESTS=ON
```

### Run tests (Google Test)
```bash
./build/components-tests        # Component library tests
./build/openmw-tests            # Main game tests
./build/openmw-cs-tests         # Construction Set tests
```

Run a single test or test suite:
```bash
./build/components-tests --gtest_filter="SuiteName.TestName"
./build/openmw-tests --gtest_filter="SuiteName.*"
```

### Benchmarks
```bash
cmake .. -DBUILD_BENCHMARKS=ON
./build/openmw_detournavigator_navmeshtilescache_benchmark
./build/openmw_esm_refid_benchmark
./build/openmw_settings_access_benchmark
```

### Key CMake options
- `BUILD_OPENMW`, `BUILD_OPENCS`, `BUILD_LAUNCHER`, `BUILD_WIZARD` — main applications (ON by default)
- `BUILD_COMPONENTS_TESTS`, `BUILD_OPENMW_TESTS`, `BUILD_OPENCS_TESTS` — tests (OFF by default)
- `BUILD_BENCHMARKS` — benchmarks (OFF by default)
- `PRECOMPILE_HEADERS_WITH_MSVC` — PCH for MSVC (ON by default)

## Code Style

Enforced via `.clang-format` and `.clang-tidy`. CI runs `CI/check_clang_format.sh`.

- **Indentation**: 4 spaces, no tabs
- **Column limit**: 120
- **Braces**: Allman style (opening brace on new line for functions, classes, control statements, namespaces)
- **Namespaces**: indented (all levels)
- **Pointers**: left-aligned (`int* ptr`)
- **Constructor initializers**: break before comma

### Naming conventions (enforced by clang-tidy)
- Classes, namespaces, enums, concepts: `CamelCase`
- Local variables, parameters: `camelBack`

## Architecture

### Applications (`apps/`)
- **`openmw/`** — Main game engine
- **`opencs/`** — Construction Set (world editor), Qt-based
- **`launcher/`** — Game launcher with settings UI
- **`wizard/`** — Installation wizard
- **`navmeshtool/`** — Offline navmesh generator
- CLI tools: `bsatool`, `esmtool`, `essimporter`, `mwiniimporter`, `niftest`, `bulletobjecttool`

### Components (`components/`)
Shared libraries used across applications. Key groupings:

- **Data formats**: `esm/`, `esm3/`, `esm4/`, `nif/`, `bsa/`, `bgsm/` — file format parsing
- **Virtual filesystem**: `vfs/` — abstraction over BSA archives and loose files
- **Rendering**: `nifosg/`, `sceneutil/`, `shader/`, `terrain/`, `fx/` — OpenSceneGraph-based
- **Physics**: `nifbullet/`, `bullethelpers/` — Bullet physics integration
- **Navigation**: `detournavigator/` — Recast/Detour navmesh generation
- **Scripting**: `compiler/`, `interpreter/` (legacy MW scripts), `lua/`, `lua_ui/` (modern Lua)
- **Settings**: `settings/` — YAML-based configuration system
- **Serialization**: `serialization/`, `sqlite3/`

### Main Game Engine (`apps/openmw/`)
Organized into subsystem modules:
- **`mwworld/`** — World model: cells, objects, stores, player
- **`mwrender/`** — Rendering with OpenSceneGraph
- **`mwgui/`** — UI widgets and windows (MyGUI-based)
- **`mwinput/`** — Input handling (SDL2)
- **`mwmechanics/`** — Game mechanics, AI, combat, spells
- **`mwphysics/`** — Physics simulation (Bullet)
- **`mwscript/`** — Legacy compiled script interpreter
- **`mwlua/`** — Modern Lua scripting system (Sol2 bindings)
- **`mwsound/`** — Audio (OpenAL)
- **`mwdialogue/`** — Dialogue system
- **`mwstate/`** — Save/load game state
- **`mwclass/`** — Per-type object handlers (NPC, creature, weapon, etc.)
- **`mwbase/`** — Abstract interfaces (Environment singleton, manager base classes)

### Lua Scripting System (`apps/openmw/mwlua/`)
- Lua executes in a separate thread, in parallel with OSG Cull traversal
- Scene graph mutations must be queued via `LuaManager::addAction`, not done synchronously
- Three object types enforce access control:
  - `GObject` — global scripts (full read/write)
  - `LObject` — local scripts (read-only for other objects)
  - `SelfObject` — the object a local script is attached to (read/write self)
- Lua API specs live in `files/lua_api/` (LDT documentation format); update them when changing bindings

### Key Dependencies
- **OpenSceneGraph 3.6.5+** — rendering
- **Bullet** (double precision) — physics
- **MyGUI 3.4.3** — in-game UI
- **Qt6** — launcher, editor, wizard
- **Lua/LuaJIT + Sol2** — scripting
- **SDL2** — windowing/input
- **Boost 1.70+**, **RecastNavigation**, **FFmpeg**, **OpenAL**

## Merge Request Conventions

- Reference issues: "Bug #123" or "Fixes #123" in commit messages
- One feature or bugfix per MR
- Prefer rebase over merge when pulling from master
- Keep commit history clean (each commit should build independently for `git bisect`)
- Original engine "bugs" should not be fixed unless obviously unintended — OpenMW aims for faithful reimplementation
