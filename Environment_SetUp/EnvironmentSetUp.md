# CS357 Graphics Environment Set Up
2/16/2021
Scott Griffith

## Goal
We are trying to build off of the Whitworth CS standard install. This assumes you have a working Visual Studio Code and a working MinGW-based g++ compiler installed.

At the moment, this is just for Windows, but you should be able to follow a similar (possibly easier) path on Mac and Linux.

Check Points:
* Compile Required Libraries (GLFW and SuperBible)
* Compile included sample code (GLFW and SuperBible)
* Compile and run your own arbitrary code

There are some other libraries that I will include references to, they might be helpful in future Graphics work! 

## References:
Heavy reference to:
https://medium.com/@bhargav.chippada/how-to-setup-opengl-on-mingw-w64-in-windows-10-64-bits-b77f350cea7e

## Sources:
Cmake (Package installer)
https://cmake.org/  

OpenGL SupberBible source code:  
https://github.com/openglsuperbible/sb7code

OpenGL Media:
http://openglsuperbible.com/files/superbible7-media.zip  

GLEW (OpenGL Extension Wrangler Library):
http://glew.sourceforge.net/  
https://github.com/nigels-com/glew  

GLFW:
https://github.com/glfw/glfw


## Steps

In general I am using the 'normal' cmd terminal to do most of these operations. You can likely get Git Bash to handle them, but for ease of paths and cross compatability, CMD works fine. You can 'Select Default Shell' in VSC's terminal window -> click the down arrow on the current terminal -> select different shell -> make new shell, and it should be the 'other' shell. There are complicated feature differences between all of the possible shells you could be using

### Get the Stuff

Somewhere where you can find it (possibly a top level 'graphics' folder) pull the SuperBible source code using:
`git clone https://github.com/openglsuperbible/sb7code`  

It should be noted that this has a version of GLFW 'inside' it, specifically GLFW-3.0.4. GLFW is a library for OpenGl. This makes it kind of a API to an API. (my understanding is that) The SuperBible code utilizes some GLFW interfaces.

You could also download a newer 3.3.2 version of GLFW. See the github above.

### CMake
Download and install CMake (https://cmake.org/)  
CMake is like a super compiler, it sets up compile/build commands for complex dependencies.

Hint from Levi: make sure to add `cmake` to your PATH.

### Generate Build Scripts - GLFW
From the sb7code/extern/glfw-3.0.4/ directory run:
`cmake -G "MinGW Makefiles" -S . -B . -DCMAKE_INSTALL_PREFIX=C:\mingw-w64\mingw64\x86_64-w64-mingw32`

A couple things to point out:
* If you are going to be using visual studio (instead of mingw) as your compiler, you are going to want to change that -G option.
* Pay attention to the DCMAKE_INSTALL_PREFIX. This argument will set the target for the build to be your mingw installation path. This path is (likely) to be unique to the Whitworth CS set up! Make sure you double check this path first!
* The `-S . -B .` sets the source and destination to be this file.
* If you get an error to the effect of 'directory <> does not appear to contain CMakeLists.txt', make sure you are 'in' the correct directory.

Cmake is a stat-full application. If you run it, it makes files (a lot of them). If you stop it, or get an error, and re-run it will keep its old files around. This is both a good and bad thing. You may need to clear out the Cmake-made files if you set something wrong.

If this step is successful you should see the following files either change or be created inside the GLFW-3.0.4 directory:
* ./CMakeFiles
* cmake_install.cmake
* CMakeCache.txt
* Makefile
  
### Generate Build Scripts - OpenGL SuperBible Code
From the top level sb7code directory run:
`cmake -G "MinGW Makefiles" -S . -B . -DCMAKE_INSTALL_PREFIX=C:\mingw-w64\mingw64\x86_64-w64-mingw32`

Same thing to note as above

If this step is successful you should see the following files either change or be created at the high level:
* ./CMakeFiles
* cmake_install.cmake
* CMakeCache.txt
* Makefile

### Build Libraries - GLFW
Starting in the sb7code/extern/glfw-3.0.4/ directory run:  
`mingw32-make all`

This should compile all of the stuff! This is the beauty of a MakeFile. The first things are the library dependencies, the second set are examples that you can go run (it is pretty fun to see some of the things!). If you get errors here, you should for sure slow down and fix them. If this step does not finish, the rest are going to be fraught.

We then need to get the required libraries into our mingw libraries:
`mingw32-make install`

Notice it moves some .h files around, those are going to be used by our programs to know how to interface with the library (which is the libglfw3.a file that is copied over)

### Build Libraries - OpenGL SuperBible Code
Same thing, but up a couple of levels. Remember that you can use `cd ..` to go 'up' a level and `dir` to display a directory.  
`mingw32-make all`

This should take a bit longer, but should run through and compile a bunch of examples.

I don't believe there is anything that needs to be 'installed' from here, so `mingw32-make install` should not need to be run (and should give you a 'No rule' message if you try).

Go try to run one of those examples, they are in `bin` folder at the top level. You should see some random graphics examples (try grass.exe and julia.exe). 

If they don't work it is probably because the media is missing from /bin/media/. You can get the required media from: http://openglsuperbible.com/files/superbible7-media.zip  
Try out springmass.exe, rimlight.exe and toonshading.exe

## Make your own code

This still needs to be filled in! Come back for an update.

At the moment I am adding a file to the `sb7code/src` folder and modifying CMakeLists.txt:

Line ~47:
...
```
set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)

set(EXAMPLES
  <project name>
  alienrain
  basicfbo
  bindlesstex
  blendmatrix
  blinnphong

```

Then compiling with with `mingw32-make <project name>`