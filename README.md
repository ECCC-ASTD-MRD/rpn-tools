Collection of tools

# At CMC

## Build dependencies

- CMake 3.20+
- librmn

## Environment

Source the right file from the `ECCI_ENV` variable, depending on the desired
architecture.  This will load the specified compiler, set the
`ECCI_DATA_DIR` variable for the test datasets, and set the
`EC_CMAKE_MODULE_PATH` variable for the `cmake_rpn` modules.

- Example for PPP5:

```
. $ECCI_ENV/latest/ppp5/inteloneapi-2022.1.2.sh
```

- Example for CMC network and gnu 11.4.0:

```
. $ECCI_ENV/latest/ubuntu-22.04-amd-64/gnu.sh
```

You will also need a version of librmn: either load it from ssm
(. r.load.dot rpn/libs/...) or provide the path to cmake install directories
if you compiled them.

## Build and install

```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=[your  install path] [-Drmn_ROOT=[path to librmn]]
make -j 4
make package
```

# Outside CMC (external users)

## Build dependencies

- CMake 3.20+
- librmn with shared libraries (https://github.com/ECCC-ASTD-MRD/librmn/)

`cmake_rpn` is included as a git submodule.  Please clone with the
`--recursive` option or run `git submodule update --init --recursive` in the
git repo after having cloned.

## Build and install

```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=${your_choice} -Drmn_ROOT=${librmn_install_path}
make -j 4
make install
```
