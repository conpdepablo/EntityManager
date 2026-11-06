# TinyEngine::EntityManager

A compact C++ entity-component system built around a templated `EntityManager` module. It stores component data in type-specific vectors, tracks per-entity bitmasks, and supports creating, querying, removing, and clearing entities efficiently.

## Overview

This project is a simple ECS prototype for experimenting with:

- Type-safe component registration via template parameters
- Bitmask-based component lookup
- Component indexing per entity
- Fast entity removal using swap-with-last semantics
- A modular C++ implementation using `import`/`export module`

## Project structure

- `src/EntityManager.cxx` — module definition for `TinyEngine.EntityManager`
- `tests/EntityManagerTests.cpp` — verification and usage examples
- `CMakeLists.txt` — top-level project configuration
- `src/CMakeLists.txt` — module target creation
- `tests/CMakeLists.txt` — test executable and registration

## Requirements

- CMake 3.25+
- A compiler with modern C++ modules support
- GCC/Clang toolchain suitable for C++20+ module code

The project is configured with CMake presets and currently builds with the GCC debug preset.

## Build

Configure and build the project:

```bash
cmake --preset g++-debug
cmake --build --preset g++-debug
```

If you prefer a manual build from the generated build directory:

```bash
cmake -S . -B build/g++-debug
cmake --build build/g++-debug
```

Run the test binary:

```bash
ctest --test-dir build/g++-debug --output-on-failure
```

## Usage example

```cpp
import TinyEngine.EntityManager;

using TinyEngine::EntityManager;

auto ecs = EntityManager<float, int, char>{};

auto player = ecs.CreateEntity(100.0f, 42, 'A');

if (ecs.HasComponent<float>(player))
{
    float& health = ecs.GetComponent<float>(player);
    health -= 10.0f;
}

if (ecs.HasComponent<int>(player))
{
    int& level = ecs.GetComponent<int>(player);
    level += 1;
}

ecs.RemoveEntity(player);
```

## API summary

### `CreateEntity`

Creates a new entity and stores any provided components:

```cpp
auto id = ecs.CreateEntity(componentA, componentB, componentC);
```

### `HasComponent<T>(entityId)`

Checks whether the entity owns a component of type `T`.

### `GetComponent<T>(entityId)`

Returns a reference to the component instance for the given entity.

### `RemoveEntity(entityId)`

Removes the entity and reorders storage to keep the component arrays compact.

### `Clear()`

Clears all entities and component data.

## Notes

This repository is intentionally small and educational. The implementation demonstrates a compact ECS approach using bitmasks and component vectors, and it is a good base for extending with systems, transforms, prefabs, or more advanced memory management.
