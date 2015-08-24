The project relies on Qt and OpenCV. Please be sure to install them before continuing.

# Prepare Libs #
The project relies on two libraries:
  * [asmlib-opencv](http://code.google.com/p/asmlib-opencv/)
  * [imgwarp-opencv](http://code.google.com/p/imgwarp-opencv/)

Checkout codes for them from corresponding SVN repositories, and move `src/lib` to `dep/` dir in FaceWorkshop's source directory. So that the directory structure will look this:

```
faceworkshop/
  ...
  gui_paramdeform.cpp
  ...
  dep/
    asmlib/ (moved from src/lib from asmlib)
      asmmodel.h
      ...
    imgwarp/ (moved from src/lib from imgwarp-opencv)
      delaunay.h
      ...
  ...
```

Then you can build with CMake. For example, in Linux, you can:

```
$ cd faceworkshop/
$ mkdir build
$ cd build
$ cmake ..
$ make
```

If you use Windows or Mac, please just use CMake's GUI.


# Run #
I feel sorry but some paths are hard coded...
```
# Assume you are in build/
$ mkdir data
```
Find `grayall_asm.model` and `haarcascade_frontalface_alt.xml` from `asmlib-opencv -> data/` and move them to `build/data`

Then you should be able to run the demo.

Note information needed to do parameter based deformation is pre-generated for `grayall_asm.model` and hard coded:( So currently although you can use better models such as `muct76.model` for landmark point detecting and free deformation, you won't be able to do Parameterized Deformation. I will fix this when I have time, sorry...