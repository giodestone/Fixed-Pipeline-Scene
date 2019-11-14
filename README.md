# Fixed-Pipeline-Scene
A 3D scene with procedurally generated shapes, models, reflections, and some shadows made using the fixed function (OpenGL 1.x) pipeline.

![Image of scene](https://github.com/giodestone/Fixed-Pipeline-Scene/blob/master/Images/Image1.jpg)
![GIF of night-time](https://github.com/giodestone/Fixed-Pipeline-Scene/blob/master/Images/GIF.gif)

# Running
[Download Here](https://github.com/giodestone/Fixed-Pipeline-Scene/releases) Or alternatively clone the repository and look inside of the Executable/ folder.

## Controls
* W, S, A, D for movement.
* Q, E for roll.
* R, F to go up and down.
* O for night-time.
* P for wire-frame view.
* X to un/lock mouse.
* Z to cycle views between lighthouse, spinning, and free.

# Scene elements
* Procedurally Generated Shapes - pretty much all of simple shapes have been generate by the code.
* Models - Loaded in using a custom made *.obj decoder.
* Lighting - Per vertex lighting (fixed function doesn't allow more unfortunately). Contains point, directional, and spot lights.
* Shadows - Impostor shadows below trees, and planar shadows (flatten then project backward).
* Reflection - Done by re-rendering the scene upside down.
* Transparency - The item in the centre of the town has four transparent walls which are dynamically sorted depending on the camera position to create transparency.
* Skybox - rendered without the z-buffer.
* 'Night-time' - Lit by a spinning spotlight in the lighthouse.

# Code Structure
The code is structured with a simple GameEntity system where each item in the scene inherits from and adds logic to the GameEntity. They are later managed by the GameEntityManager.

GameEntityManager manages the lifetime (through smart pointers) as well as the draw order, initialization, and is responsible for calling draw, update functions on the objects, as well as deleting them.

The trees and their size will vary from run to run (as their trunk and canopy sizes as well as their position are decided when launching the application). In addition the house roof textures are randomly picked out when launching the game.

ModelData contains data as well as the logic (through static functions) for generating the relevant shapes (discs, cubes, spheres, cones, cylinders).

AssetManager is responsible for loading and storing data for textures and models. The underlying data structure is a map as to be programmer friendly as the relevant assets can be gotten by an alias.

Imposter and Planar Shadow Managers are for rendering shadows underneath trees and buildings, respectively.

# Images
![Scene from dock](https://github.com/giodestone/Fixed-Pipeline-Scene/blob/master/Images/Image2.jpg)
![Scene from forest with imposter shadows](https://github.com/giodestone/Fixed-Pipeline-Scene/blob/master/Images/Image3.jpg)
![Scene from mountains](https://github.com/giodestone/Fixed-Pipeline-Scene/blob/master/Images/Image4.jpg)
![Houses with planar shadows](https://github.com/giodestone/Fixed-Pipeline-Scene/blob/master/Images/Image5.jpg)
*Last image is of planar shadows.*