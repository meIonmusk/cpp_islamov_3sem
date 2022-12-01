# Mandelbrot Set

Code allows you to create, render and zoom in Mandelbrot set or your own written fractal.
Here manual for launch and using but also documentation and instruction are given in the code.

### Launch
To compile (MacOS) the code use following command

``
g++ -std=c++17 window.cpp -I/opt/local/include/ /opt/local/lib/libsfml-graphics.dylib /opt/local/lib/libsfml-audio.dylib  /opt/local/lib/libsfml-window.dylib /opt/local/lib/libsfml-system.dylib
``

### Using
 * Arrows: 					    to move across the window.
 * Right Shift: 			  to zoom out of the center of the view
 * Enter: 					    to zoom into the center of the view
 * Equal: 					    to increase max iterations number
 * Backspace: 				  to decrease max iterations number
 * Left mouse button: 	to zoom into the cursor point
 * Escape:					    to reset the view (max iterations number will be saved)


