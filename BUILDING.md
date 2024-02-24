# Building with CMake

## Build

This project doesn't require any special command-line flags to build to keep
things simple.

Here are the steps for building in release mode with a single-configuration
generator, like the Unix Makefiles one:

```sh
cmake -S . -B build -D CMAKE_BUILD_TYPE=Release
cmake --build build
```

Here are the steps for building in release mode with a multi-configuration
generator, like the Visual Studio ones:

```sh
cmake -S . -B build
cmake --build build --config Release
```

# Building with Premake 5

## Generate

If you prefer Preamke 5 to generate the project, here is the command:

```sh
premake5 generator_of_your_choice --to=location_of_generated_files
```

For example, to generate a Visual Studio solution in the vs2022 folder:

```sh
premake5 vs2022 --to=vs2022
```

You can also specifiy a compiler to use with cc parameter. To generate a Makefile project using clang in the build folder:

```sh
premake5 gmake2 --cc=clang --to=build
```
