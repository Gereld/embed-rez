# embed-rez

This is the embed-rez project.

This a tool to embed assets as C++ code in a portable way.
I recommend nevertheless to use the prefered way of embedding resources of your system instead of this. But if you want to distrute a source library that depends on some files and you don't want your user to deal with them, this will do the trick.
For example: you provide a rendering library and you want to embed shaders and fonts in it. 

The assets are decribed with an xml file, see [example/res.xml](example/res.xml).

### Usage

```sh
embed-rez res.xml res.inc
```
This will parse the xml file and embed the referenced files or folders in res.inc. You can then #include "res.inc" in your project.

### Dependencies

- pugixml

### Get the source

```sh
git clone --recurse-submodules https://github.com/Gereld/embed-rez
```
or

```sh
git clone https://github.com/Gereld/embed-rez
git submodule update --init --recursive
```

## Building

Requires C++20. A Visual Studio 2022 solution is included in the vs folder. Otherwise see the [BUILDING](BUILDING.md) document.

## Licensing

This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>
