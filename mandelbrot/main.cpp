// to execute use g++ -std=c++17 window.cpp -I/opt/local/include/ /opt/local/lib/libsfml-graphics.dylib /opt/local/lib/libsfml-audio.dylib  /opt/local/lib/libsfml-window.dylib /opt/local/lib/libsfml-system.dylib


/**
 * @brief File is a main part of {{mandelbrot}}. Compile and launch this file to render mandelbrot set.
 * 
 * to execute use g++ -std=c++17 main.cpp -I/opt/local/include/ /opt/local/lib/libsfml-graphics.dylib /opt/local/lib/libsfml-audio.dylib  /opt/local/lib/libsfml-window.dylib /opt/local/lib/libsfml-system.dylib
 * 
 * 	***MANUAL***
 * use 
 * Arrows: 					to move across the window.
 * Right Shift: 			to zoom out of the center of the view
 * Enter: 					to zoom into the center of the view
 * Equal: 					to increase max iterations number
 * Backspace: 				to decrease max iterations number
 * Left mouse button: 		to zoom into the cursor point
 * Escape:					to reset the view (max iterations number will be saved)
*/

#include "Fractal.hpp"
#include "Renderer.hpp"

int main(){
	MandelbrotSet *mandelbrot = new MandelbrotSet(1500, 1000, 50, 0, 0);
	FractalRenderer fractalrenderer(mandelbrot);

	int code = fractalrenderer.poll();
	delete mandelbrot;
	return code;
}