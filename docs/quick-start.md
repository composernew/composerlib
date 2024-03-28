---
layout: default
title: Quick start
nav_order: 2
has_children: false
has_toc: false
---
# Quick start

First, make sure you have the latest version of CMake in you system. Then, proceed with the following steps:

1. Clone the composerlib repository from GitHub using the following command:

```
git clone git@github.com:composernew/composerlib.git
```

2. Enter the composerlib directory:

```
cd composerlib
```

3. Create a directory to build the project:

```
mkdir build
cd build
```

4. Configure the build script

```
cmake .. -DCMAKE_C_COMPILER=/usr/bin/gcc-10 -DCMAKE_CXX_COMPILER=/usr/bin/g++-10  -DCMAKE_CXX_FLAGS="-O2" -DCMAKE_BUILD_TYPE=Release
```

5. Build all targets:

```
cmake --build . -j 2 --config Release
```

6. Start with the `optimizer` example:

```
cd examples
./example_optimizer
```




<!-- Generated with mdsplit: https://github.com/alandefreitas/mdsplit -->
