README
======

This package includes the following files.

|-- Eigen [This directory contains files for use of the Eigen library]
|-- raytracer.cc [This program reads a driver file and outputs an image from the data processed]
|-- Camera.cc/.h, Model.cc/.h, Light.cc/.h, Material.cc/.h, Face.cc/.h, Sphere.cc/.h, Ray.cc/.h [Classes referenced in raytracer.cc]
|-- Makefile [For compiling, cleaning and taring]
|-- README.txt [This file]
|-- driver00.txt, driver01.txt[Driver files created by student]
|-- driver00.ppm, driver01.ppm [Images rendered using program]
|-- board.obj, cube_centered.obj [OBJ files referenced in drivers]
|-- board.mtl, cube_centered.mtl [MTL files references in objects]

To compile: 'make'
To run: 'raytracer [driverFileName] [outputFileName]'