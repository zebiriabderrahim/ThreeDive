# 3Dive: A Modern C++ Meshmixer Clone

3Dive is an ambitious project aiming to recreate the functionality of Autodesk Meshmixer using modern C++17 and OpenGL. It features a custom 3D graphics engine with minimal dependencies and a flexible render API design for potential future support of Vulkan or DirectX.
## Demo
![Demo](demo.gif)
## Development Environment

- Language: C++17
- Compiler: Clang
- Operating System: macOS 14 (Tested only on this platform)

## Features

- Custom OpenGL render API abstraction
- Event system for efficient state management
- Advanced camera system for 3D navigation
- Scene system utilizing ECS architecture
- Model loading capabilities
- Meshmixer-style scene grid with dynamic detail levels
- Ongoing GUI development


## Dependencies

3Dive is built with minimal external dependencies:

- GLFW
- GLAD
- GLM
- Dear ImGui
- Assimp
- EnTT
- stb
- spdlog (to be replaced with std::format)

## Building the Project

3Dive uses Conan for package management. To build the project:

```bash
conan profile detect --force
conan install . -s build_type=Debug --build=missing
conan build
```
Note: If conan profile detect --force doesn't correctly create a profile, use the provided setup script to build a profile:
```bash
chmod +x setup_conan_profile.sh
./setup_conan_profile.sh
```

Part 3: Development Roadmap

```markdown
## Development Roadmap

### Completed (First Iteration)
- [x] OpenGL render API abstraction
- [x] Event system
- [x] Camera system
- [x] Scene system (ECS architecture)
- [x] Model loading
- [x] Meshmixer-style scene grid with dynamic details

### In Progress
- [ ] GUI system

### Future Goals
- [ ] Mesh manipulation tools
- [ ] Advanced 3D modeling features
```

## Testing

3Dive employs Google Test (gtest) for unit testing. The test suite is currently under development.

## Contributing

We welcome contributions to 3Dive!

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- Inspired by Autodesk Meshmixer
- Built with modern C++ practices and OpenGL



