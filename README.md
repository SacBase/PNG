About
=====

This repository contains the PNG and SimpleImage SaC modules.

Build
-----

Requirements:
 * [CMake](https://cmake.org/) >= 3.3
 * The `libpng` library
 * an installed and/or compiled SaC [Stdlib](https://github.com/SacBase/Stdlib)

 Steps:

```sh
$ cd Png
$ git submodule init
$ git submodule update
$ mkdir build
$ cd build
$ cmake ..
$ make -j4
$ make install
```
