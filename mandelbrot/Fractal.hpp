/**
 * This header file is part of {{mandelbrot}}.
 * 
 * There are defined classes Fractal and its subclass MandelbrotSet.
*/
#ifndef FRACTAL
#define FRACTAL

#include <iostream>
#include <SFML/Graphics.hpp>


/**
 * Fractal is abstract class that used for creating and rendering other fractal sets and keeps information max iterations count and zooming parameter.
 * Override Fractal::iterate in your fractal class to render it correctly.
*/
class Fractal {
protected:
	unsigned maxIterations;
	double x0;
	double y0;
	double scale;
	const double startScale;
	const int width, height;
    double *iterationsArray = nullptr;

public:
	/**
	 * Main constructor of the class. 
	 * 
	 * @param width width of the window in pixels.
	 * @param height height of the window in pixels. 
	 * @param maxIterations max allowed number of iterations.
	 * @param x0 real part of center of the sample. 
	 * @param y0 imaginary part of center of the sample. 
	*/
	Fractal(unsigned width, unsigned height, unsigned maxIterations, double x0, double y0): width(width), height(height), maxIterations(maxIterations), x0(x0), y0(y0),
																							startScale(1 / (2 * 1e-6 * width)), scale(1 / (2 * 1e-6 * fmax(width, height))), iterationsArray(new double[width * height]) {}
	Fractal(unsigned width, unsigned height): Fractal(width, height, 50, 0, 0) {}
	Fractal(): Fractal(1500, 1000) {}

	Fractal(Fractal&) = default;
	Fractal(Fractal&&) = default;

    
    // Functions that return private athributes' values.

    unsigned getWidth(){ return width; }
    unsigned getHeight(){ return height; }
    unsigned getMaxIterations() { return maxIterations; }
    double* getIterationsArray() { return iterationsArray; }
    double getScale() { return scale; }
    double getStartScale() { return startScale; }

    // Functions that change private athributes' values.

    void updateMaxIterations(int change) { if (change > 0 || maxIterations > 0) maxIterations += change; }
    void updateCenterX(double x) { x0 += x / scale; }
    void updateCenterY(double y) { y0 += y / scale; }

    /**
     * Resetting athributes' values.
    */
    void reset() {
        x0 = 0;
        y0 = 0;
        scale = startScale;
    }

	// Fractal operator =(Fractal&) = delete;
	// Fractal operator =(Fractal&&) = delete;


	/**
	 * Rescales the scale parameter that is used for zooming a view.
	 * 
	 * @param k scaling parameter (in which times to increase (or decrease) the view).
	 * @throws throws exception if scaling parameter is equal to zero.
	*/
	void rescale(double k) {
		if (!k){
            if (!iterationsArray)
			    delete [] iterationsArray;
			throw std::invalid_argument("Scaling parameter cannot be zero.");
		}
        if (scale / startScale > 0.5f)
	        scale *= k;	
	}

	/**
	 * Virtual function that must be overriden for each subclass for correct rendering.
	*/
    virtual void iterate(int x, int y) = 0;
	
	/**
	 * Destructor of the class. Closes the window and deletes dynamic array of pixels.
	*/
	~Fractal(){
        if (!iterationsArray)
            delete [] iterationsArray;
	}
};

#endif

#ifndef MANDELBROT 
#define MANDELBROT

/**
 * Subclass of Fractal class that enures for rendering mandelbrot set.
*/
class MandelbrotSet final: public Fractal {
public:
	/**
	 * Overriden method that processes point (x, y) of the set and updates information about its iterations number. 
	*/
	void iterate(int x, int y) override {
        unsigned iterations = 0;
        double xc = 0;
        double yc = 0;
        double xx = 0;
        double yy = 0;

        while (xc * xc + yc * yc < 4 && iterations++ < maxIterations){
            xx = xc * xc - yc * yc + (x - width/2) / scale - x0;
            yy = 2 * xc * yc + (y - height/2) / scale  - y0;
            xc = xx;
            yc = yy;
        }
        
        iterationsArray[width * y + x] = (double)(iterations - 1) / (double)maxIterations;
    }

	/**
	 * Main constructor of the class. 
	 * 
	 * @param width width of the window in pixels.
	 * @param height height of the window in pixels. 
	 * @param maxIterations max allowed number of iterations.
	 * @param x0 real part of center of the sample. 
	 * @param y0 imaginary part of center of the sample. 
	*/
	MandelbrotSet(unsigned width, unsigned height, unsigned maxIterations, double x0, double y0): Fractal(width, height, maxIterations, x0, y0) {}
	MandelbrotSet(unsigned width, unsigned height): MandelbrotSet(width, height, 50, 0, 0) {}
	MandelbrotSet(): MandelbrotSet(1500, 1000) {}

    ~MandelbrotSet() = default;
};

#endif
