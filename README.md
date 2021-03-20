# Project 3 - Models
Whitworth University - CS-357 Computer Graphics  
Last edited: 3/20/2021 by Scott  

## Overview
Let's start making scenes! We can define complex shapes in Blender (or other modeling tools). We can import them into our program. We can layout where things go. We can manage a camera space. This project puts all of that together.

You are welcome (and encouraged) to use provided code, both from me and from sb7 source code. Always be clear about your citations.

As this is a 300 level course, I expect adherence to style and appropriate and complete commenting.

As always: you can do this assignment in whatever Graphics Language you like, but you need to approach the theme of the original assignment if not doing it in OpenGL.

## Grade

| Portion             | Points Possible |
|---------------------|-----------------|
| Model Creation      | 15              |
| Scene Generation    | 30              |
| Camera Interactions | 20              |
| Extras              | 15              |
| Proof               | 20              |

Please only upload original code and documentation! I don't want to see whole libraries uploaded or zip files unrelated to what you did. You should use a good `.gitignore` file, if you don't know what that is, let me know and I can show you.

## Part 1: Model Creation
You need to make at least 3 original models in a modeling language of your choice to import into your scene. Make sure you document this process (see Proof).

Don't go overboard (unless you want to). We are looking for something distinguishable from your other models not Pixar quality.

## Part 2: Scene Generation
Get your models into your graphics pipeline and set them up into a reasonable scene. 

You are going to need to submit a layout, so now might be a good time to do that *before* you start the next part. You should have a good idea of what you are trying to accomplish before starting.

You can decide on color options. We are not really going to have the ability to do lighting or textures yet, but if you want to try you can. Shading in the z dimension is not the worst idea to give the illusion of depth.

## Part 3: Camera Interactions
You are going to need to map either (or both) keybindings or mouse positions to your camera space. 

As an example: keep your camera fixed on (0,0,0) world but the x direction of your mouse maps to 'horizontal' rotation and y mouse direction maps to 'vertical' rotation. You could also set up a 'fly-through' with the keyboard, w -> moves down a path, s -> backs up on that path.

You can decide, but you do need to interact with a more complicated camera transform.

## Part 4: Extras

Be creative! Add features!

You can add elements to move each model independently of the others, you could introduce camera operations like changing the perspective or position. This is where you need to flex those creative muscles. 

Like before: you are welcome to come talk to me to brainstorm.

## Part 5: Proof
Show and tell! Show off what you did!

Include:
* Screenshots of your models in whatever tool you used to make them
  * You should also probably at least upload the .obj or other files for your objects.
* Sketch/diagram of your scene (this can be digital or physical, I care more about layout than detail)
* Video or .gif recording of an interaction with your program. Show off the features!
