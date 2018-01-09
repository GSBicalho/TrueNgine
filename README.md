# TrueNgine - A N-Dimensional Renderer

This is an implementation an N-Dimensional Renderer, to allow the visualization of N-Dimensional shapes through the cut of the space and perspective rendering.

# Table of Contents

- [Introduction](#introduction)
- [Cutting the Space](#cutting-the-space)
- [Perspective Rendering](#perspective-rendering)
- [Build Process](#build-process)
- [Examples](#examples)
- [Generating Shapes](#generating-shapes)
- [Acknowledgments](#acknowledgments)

## Introduction
We, human beings, have brains adapted for observing, interacting and imagining in 3 Dimensions. Interaction is limited to height, width and depth, so, when imagining objects of such dimensions, it is simple to create a mental image and understand it. However, in mathematics, we are not limited in such a way, and it can be difficult to imagine a construct with more dimensions than we are used to. That's why this program exists, to try to allow the further understanding and comprehension of higher dimensional spaces, functions and shapes.

As such, the resulting program allows the user to combine two ways of visualizing the extra dimensions. First, the Cut is applied to the first K Dimensions, and then the remaining ones are rendered using perspective.

It allows the user to rotate the shape, move around it, and also move the location and directions of all the cameras generated.

The polytopes in the examples were largely generated using [this](https://github.com/GSBicalho/Function-Approximation) code by Professor [Antonio Castlo Filho](http://conteudo.icmc.usp.br/Portal/Pessoas/Detalhes.php?id=132404).
## Cutting the Space
This part of the rendering takes the ND model and, given a position in the last dimension, slices it so that only the part that intersects the current viewed space is seen.

![A Sphere, being sliced in three different locations, generating different shapes.](https://i.imgur.com/egQLeih.png)

As seen above, the 2D cut of a 3D sphere is given by a circle, which increases in radius the closer to the center the cut is. In the same manner, the 3D cut of a 4D Hypersphere generates a sphere, which increases in size the closer to the center the cut is applied. 

## Perspective Rendering
This part of the rendering utilizes perspective projection to show the user the N dimensions above 3D look like. It means that objects "further away" in the Nth dimension will be smaller, and objects "closer" in the Nth dimension will appear bigger,

The perspective projection was made following [this](https://ef.gy/linear-algebra:perspective-projections) incredible article by Magnus Deininger.

Some adjustments have been made, given the different libraries used and other factors, but it remains largely the same.

Do note that, given an N-Dimensional space, N-2 cameras will be required.

## Build Process

### Prerequisites
Two libraries were used to create this project, [Eigen](http://eigen.tuxfamily.org/) and [Qt](https://www.qt.io/).

Eigen is a simple template library, therefore it does not need installation. Qt, on the other hand, is a bit more complex and requires you to go through its paces to get it working on your computer.

After downloading and installing both libraries, set the Environment Variables `EIGENDIR` and `QTDIR` to the Eigen directory and to the folder containing your Qt `bin` folder, respectively.

### Building
Open the project in Visual Studio 2017 and build it.

## Examples
The program should open showing the classic view of the Hypercube. To the left, there will be the Properties Window, which shows the current position of the 3D and 4D cameras. It also allows you to select whether to display only the wireframe, activate Face-Culling (in progress), and also to define in which plane to rotate the shape.

![The starting screen of the program, showing the perspective projection of a Hypercube.](https://i.imgur.com/IzJxjYX.png)

By clicking on the main window, you will be able to move around using WASD and rotate the shape using Q and E.

![The Hypercube rotated 45 degrees, with perspective projection.](https://i.imgur.com/Uukl4qO.png)
In the image above, the Hypercube was rotated 45 degrees around the XW plane, for example.

By going into `Options > Set Cut Number`, you can set the number of cuts that will be applied to the shape. Bear in mind that, given that the cut removes one dimension and that we need at least 3 dimensions for our main camera to exist, it will cut only up to the 3D. That means we can only cut the Hypercube once. By also rotating it on the YW plane by 35 degrees, we obtain the following shape:

![A Hypercube, rotated on the XW plane by 45 degrees, on the YW plane by 35 degrees, then cut at the point 0](https://i.imgur.com/VWPHt88.png)

It is also possible to adjust the location of the cut through the properties window.

Clicking in `File > Open` you can open one of the other shapes found in the `Polytopes` folder. Further examples can be found [here](https://imgur.com/a/Taevi).

## Generating Shapes

There are two formats of files in the Polytopes folder. They can both be used to create the same shapes, but are generated differently.

### NDP Files
NDP Files were generated (mostly) by hand. As an example here, we will use the file `Polytopes\hypercube.ndp`.

The first line contains two numbers, N and K.
```
4 4
```
N indicates the number of dimensions of the Space. K indicates the amount of dimensions the shapes we will describe will have. This allows us to create 3D cubes in a 4D space, or a 4D cube defined simply by its 3D faces, which would make it "hollow". In a 3D analogy, think of a cube defined only by its faces, instead of it being defined as a "filled" shape.

In this case, we have a Hypercube in a 4D space.

Then comes
```
16
-0.5 -0.5 -0.5 -0.5
-0.5 -0.5 -0.5  0.5
-0.5 -0.5  0.5 -0.5
-0.5 -0.5  0.5  0.5
-0.5  0.5 -0.5 -0.5
-0.5  0.5 -0.5  0.5
-0.5  0.5  0.5 -0.5
-0.5  0.5  0.5  0.5
 0.5 -0.5 -0.5 -0.5
 0.5 -0.5 -0.5  0.5
 0.5 -0.5  0.5 -0.5
 0.5 -0.5  0.5  0.5
 0.5  0.5 -0.5 -0.5
 0.5  0.5 -0.5  0.5
 0.5  0.5  0.5 -0.5
 0.5  0.5  0.5  0.5
```
The first number indicates the number of vertices the shape will have. Then follows the coordinates of each of the vertices.

```
32
 0  1 
 1  3 
 2  3 
 0  2
 4  5 
 5  7 
 6  7 
 4  6 
 0  4 
 1  5 
 2  6 
 3  7
 8  9
 9 11 
10 11
 8 10
12 13 
13 15 
14 15 
12 14 
 8 12 
 9 13 
10 14
11 15
 0  8 
 1  9 
 2 10 
 3 11
 4 12 
 5 13 
 6 14 
 7 15
```
This defines the edges of the Hypercube. We define that our Hypercube has 32 edges, and that the edge 0 is composed of the vertices 0 and 1, edge 1 is composed by the vertices 1 and 3, and so on.

Then comes the faces:
```
24
     0     1     2     3
     4     5     6     7
     0     4     8     9
     2     6    10    11
     3     7     8    10 
     1     5     9    11 
    12    13    14    15
    16    17    18    19
    12    16    20    21
    14    18    22    23
    15    19    20    22
    13    17    21    23
     0    12    24    25 
     2    14    26    27
     3    15    24    26
     1    13    25    27
     4    16    28    29
     6    18    30    31
     7    19    28    30
     5    17    29    31
     8    20    24    28
     9    21    25    29
    10    22    26    30
    11    23    27    31 
```
We define that our Hypercube has 24 faces and that the face 0 is composed of the edges 0, 1, 2 and 3, face 1 is composed by the edges 4, 5, 6, 7, etc. Note that, just as the Edges (1D Shape) reference the Vertices (0D Shape), the Faces (2D Shape) reference the Edges (1D Shape).

This goes on for the remainder of the dimensions, so the Spaces we define will reference the list of Faces:
```
8
 0  1  2  3  4  5
 6  7  8  9 10 11
 0  6 12 13 14 15
 1  7 16 17 18 19
 2  8 12 16 20 21
 3  9 13 17 22 23
 4 10 14 18 20 22
 5 11 15 19 21 23
```
Here we define our Hypercube's "3D faces", which we will call Spaces. There are 8 of them, and the Space 0 is defined by the Faces 0, 1, 2, 3, 4 and 5, Space 1 by 6, 7, 8, 9, 10 and 11, etc.

```
1
 0 1 2 3 4 5 6 7
```
Finally, we reach our Kth shape, the Hypercube itself, of which we have only one, and is formed by the eight cubes we defined before.

Do note that all of the shapes defined MUST be convex, otherwise the Dimensional Cut will generate erroneous shapes.

### POL Files
POL Files were generated by [this](https://github.com/GSBicalho/Function-Approximation) MATLAB code, which was created by Professor Antonio Castelo Filho and uploaded here with his permission. 

A full description of the program, its workings and output can be seen on the repository linked above.

## Acknowledgments
* [Professor Antonio Castelo Filho](http://conteudo.icmc.usp.br/Portal/Pessoas/Detalhes.php?id=132404) who created the [program](https://github.com/GSBicalho/Function-Approximation) used to generate the POL files.
* [Marc Ten Bosch](http://marctenbosch.com/), creator of [Miegakure](http://miegakure.com/), which inspired this project.
* [Marcus Deininger](https://ef.gy/site), who wrote articles on [Perspective Rendering](https://ef.gy/linear-algebra:perspective-projections) and [Normal Vectors in Higher Dimensions](https://ef.gy/linear-algebra:normal-vectors-in-higher-dimensional-spaces) both of which were used to create this program.
