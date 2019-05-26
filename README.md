# Basic OpenGL application
The original version was provided by Kevin Harris. Manuel Menezes (Informatics Institute - UFRGS) provided a modified version. I converted it to used glutKeyboardFunc on linux.

# Dependencies

        $ sudo apt-get install freeglut3-dev
 
        $ sudo apt-get install libglew-dev

# Compiling:

        g++ basic-opengl.cpp -o basic-opengl -lGL -lGLU -lglut

# Control Keys: 
* Up         - View moves forward
* Down       - View moves backward
* Left       - View strafes left
* Right      - View strafes Right
* Left Mouse - Perform looking
* Mouse      - Look about the scene
* Ctrl+H     - View moves up
* Ctrl+E     - View moves down

