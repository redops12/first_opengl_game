# first_opengl_game

A small C++23 OpenGL sandbox for learning modern graphics programming. It renders a first-person 3D scene with loaded models, textured meshes, and point lighting.

## What it is

This project is an early OpenGL game / renderer experiment. It sets up an OpenGL 4.6 core context, loads GLB models, and lets you walk around a lit room with free-look camera controls.

Right now it focuses on the rendering and camera loop rather than gameplay systems.

## Capabilities

- **Window & context** — GLFW + GLAD, OpenGL 4.6 core profile, depth testing, resize handling
- **First-person player** — WASD movement on the XZ plane, Space / Shift for vertical move, mouse look with pitch clamp
- **Model loading** — Assimp-based `Model` / `Mesh` pipeline for GLB assets (positions, normals, UVs, material textures)
- **Shaders** — Vertex/fragment programs with a typed uniform helper (`ShaderProgram::set`)
- **Lighting** — Point light with ambient / diffuse / specular terms, distance falloff, per-model roughness, and preset light color temperatures (candle, tungsten, fluorescent, sunlight, etc.)
- **Scene composition** — Multiple placed models (room, pool table, katana, bookshelf, light fixture) drawn each frame with view/projection from the player camera

## Controls

| Input | Action |
| --- | --- |
| W / A / S / D | Move |
| Mouse | Look |
| Space / Left Shift | Up / down |
| Escape | Quit |

## Build

Dependencies are pulled in via git submodules (`glfw`, `assimp`) plus vendored headers under `thirdparty/` (GLAD, GLM, stb_image).

```bash
git submodule update --init --recursive
cmake --preset debug
cmake --build build
./build/first_game
```

Use `--preset release` for an optimized build.

## Project layout

```
src/          # Application, player, model, shader, window code
shaders/      # GLSL vertex/fragment shaders
thirdparty/   # GLFW, Assimp, GLAD, and other deps
```

## Notes

Shader and asset paths in `src/main.cpp` are currently absolute paths on the local machine. Point them at your own `.vert` / `.frag` files and `.glb` models before running elsewhere.
