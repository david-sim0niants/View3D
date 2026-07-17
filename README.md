# View3D

A lightweight OpenGL 3D model viewer and rendering library written in C++23.

## Features

- Load and render 3D models via [Assimp](https://github.com/assimp/assimp)
- Interactive first-person camera (mouse look + keyboard movement)
- Reference grid rendering
- Phong-style lighting with a configurable `Light` object
- Usable as a standalone CLI tool or as a linkable C++ library (`View3D::view3d`)

## Dependencies

| Dependency | Purpose |
|---|---|
| OpenGL 4.6 | Rendering backend |
| [GLFW3](https://www.glfw.org/) | Window and input |
| [GLEW](https://glew.sourceforge.net/) | OpenGL extension loading |
| [GLM](https://github.com/g-truc/glm) | Math (vectors, matrices) |
| [Assimp](https://github.com/assimp/assimp) | 3D asset loading |
| [CLI11](https://github.com/CLIUtils/CLI11) | CLI argument parsing (auto-fetched) |

Install system dependencies:

**Arch Linux**
```sh
sudo pacman -S glfw glew glm assimp
```

**Debian/Ubuntu**
```sh
sudo apt install libglfw3-dev libglew-dev libglm-dev libassimp-dev
```

**Fedora/RHEL**
```sh
sudo dnf install glfw-devel glew-devel glm-devel assimp-devel
```

## Building

```sh
cmake -B build
cmake --build build
```

### Options

| Option | Default | Description |
|---|---|---|
| `BUILD_CLI` | `ON` | Build the `view3d` CLI executable |
| `BUILD_SHARED_LIBS` | `OFF` | Build `libview3d` as a shared library |

Example — library only, shared:

```sh
cmake -B build -DBUILD_CLI=OFF -DBUILD_SHARED_LIBS=ON
cmake --build build
```

## Running

```sh
./build/view3d <model_file>
```

Any format supported by Assimp works (`.obj`, `.fbx`, `.gltf`, `.ply`, etc.).

### Controls

| Input | Action |
|---|---|
| `W / S` | Move forward / backward |
| `A / D` | Strafe left / right |
| `E / Q` | Move up / down |
| Right mouse button + drag | Look around |
| `Escape` | Quit |

## Library API

Include the single aggregate header:

```cpp
#include <view3d.hpp>
```

All symbols live in the `view3d` namespace.

### Resource management

Objects are heap-allocated through a pool and returned as raw pointers. Use
`createResource<T>(...)` / `destroyResource<T>(ptr)` to manage lifetime.

```cpp
Camera* camera = view3d::createResource<Camera>(16.0f / 9.0f);
Light*  light  = view3d::createResource<Light>(glm::vec3(1.0f), 1.0f);
Mesh3D* mesh   = view3d::createResource<Mesh3D>(view3d::loadMesh3D("model.obj"));
Shader  shader = view3d::loadBuiltinShader("basic_vertex.glsl", "basic_fragment.glsl");
Object* object = view3d::createResource<Object>(*mesh, shader, glm::vec3{1.0f});
```

### Scene

```cpp
Scene scene;
scene.setCamera(camera);
scene.setLight(light);
scene.addObject(object);   // returns the pointer back
scene.removeObject(object);
bool present = scene.hasObject(object);
```

### Renderer

```cpp
Renderer renderer;
renderer.setViewport(width, height);
renderer.render(scene);    // call each frame
```

### Camera

`Camera` extends `Transform` and exposes:

```cpp
camera->setAspectRatio(16.0f / 9.0f);
camera->setFOV(45.0f);
camera->setNearPlane(0.1f);
camera->setFarPlane(1000.0f);
glm::mat4 view = camera->getViewMatrix();
glm::mat4 proj = camera->getProjectionMatrix();
```

### Controller

Binds keyboard/mouse input to any `Transform` target (typically the camera):

```cpp
Controller controller(window, camera);

ControllerSettings& s = controller.getSettings();
s.move_speed          = 0.2f;
s.mouse_x_sensitivity = 20.0f;
s.mouse_y_sensitivity = 20.0f;

// each frame:
controller.update();
```

### Asset loading

```cpp
Mesh3D mesh       = view3d::loadMesh3D("path/to/model.obj");
Shader shader     = view3d::loadShader("vert.glsl", "frag.glsl");
Shader builtin    = view3d::loadBuiltinShader("basic_vertex.glsl", "basic_fragment.glsl");
```

### Shader uniforms

```cpp
shader.setUniform("model",     glm::mat4{...});
shader.setUniform("color",     glm::vec3{...});
shader.setUniform("intensity", 1.0f);
```

### Mesh

`Mesh<T, N, FN>` is a generic template; `Mesh3D` is `Mesh<float, 3, 3>` (3-D,
triangles). Normals can be computed on the CPU when both vertex and face buffers
are in host memory:

```cpp
mesh->computeNormals();
```

## License

See [LICENSE](LICENSE).
