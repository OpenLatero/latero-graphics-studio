# latero-graphics-studio

A program that allows editing of 2D tactile graphics rendered using the latero-graphics library. See [OpenLatero.org](http://OpenLatero.org) for more information.

## Installation

### MacOS

Tested on MacOS Tahoe 26.3.1.

Install the latero and latero-graphics libraries.

#### Build and run the app

The app can be compiled with the following commands:

```
cmake --preset default
cmake --build build
```

and ran with this command:

```
build/src/latero-graphics-studio
```

or in a single command:

```
cmake --preset default && cmake --build build && build/src/latero-graphics-studio
```

## Authors

Vincent Levesque <vleves@cim.mcgill.ca>
