/**
 * This header file is part of {{mandelbrot}}.
 * 
 * There are defined classes Fractal and its subclass MandelbrotSet.
*/
#ifndef FRACTAL_HPP 
#define FRACTAL_HPP

#include <iostream>
#include <string.h>
#include <complex.h>
#include <SFML/Graphics.hpp>


/**
 * Fractal is abstract class that used for creating and rendering other fractal sets and keeps information about window, max iterations count and zooming parameter.
 * Override Fractal::iterate and use Fractall::poll to render your fractal set.
*/
class Fractal {
protected:
	unsigned max_iterations;
	double x0;
	double y0;
	double scale;
	const double start_scale;
	const double scale_param;
	const int width, height;
    double *iterations_array = nullptr;

    friend class FractalRenderer;

	/**
	 * Rescales the scale parameter that is used for zooming a view.
	 * 
	 * @param k scaling parameter (in which times to increase (or decrease) the view).
	 * @throws throws exception if scaling parameter is equal to zero.
	*/
	void rescale(double k) {
		if (!k){
            if (!iterations_array)
			    delete [] iterations_array;
			throw std::invalid_argument("Scaling parameter cannot be zero.");
		}
	    scale *= k;	
	}

	/**
	 * Virtual function that must be overriden for each subclass for correct rendering.
	*/
    virtual void iterate(int x, int y) = 0;
   
public:
	/**
	 * Main constructor of the class. 
	 * 
	 * @param width width of the window in pixels.
	 * @param height height of the window in pixels. 
	 * @param max_iterations max allowed number of iterations.
	 * @param x0 real part of center of the sample. 
	 * @param y0 imaginary part of center of the sample. 
	 * @param title title of the window.
	*/
	Fractal(unsigned width, unsigned height, unsigned max_iterations, double x0, double y0): width(width), height(height), max_iterations(max_iterations), x0(x0), y0(y0), scale_param(2),
																												 start_scale(1 / (2 * 1e-6 * width)), scale(1 / (2 * 1e-6 * fmax(width, height))), iterations_array(new double[width * height]) {}
	Fractal(unsigned width, unsigned height): Fractal(width, height, 50, 0, 0) {}
	Fractal(): Fractal(1500, 1000) {}

	Fractal(Fractal&) = default;
	Fractal(Fractal&&) = default;

    unsigned getWidth(){ return width; }
    unsigned getHeight(){ return height; }

	// Fractal operator =(Fractal&) = delete;
	// Fractal operator =(Fractal&&) = delete;

	
	/**
	 * Destructor of the class. Closes the window and deletes dynamic array of pixels.
	*/
	~Fractal(){
        if (!iterations_array)
            delete [] iterations_array;
	}
};

#endif

#ifndef MANDELBROT 
#define MANDELBROT

/**
 * Subclass of Fractal class that enures for rendering mandelbrot set.
*/
class MandelbrotSet: public Fractal {
protected:
	/**
	 * Overriden method that processing each pixel and colors it with RGBA colorscheme.
	*/
	void iterate(int x, int y) override {
        
        unsigned iterations = 0;
        
        double xc = 0;
        double yc = 0;
        double xx = 0;
        double yy = 0;

        while (xc * xc + yc * yc < 4 && iterations++ < max_iterations){
            xx = xc * xc - yc * yc + (x - width/2) / scale - x0;
            yy = 2 * xc * yc + (y - height/2) / scale  - y0;
            xc = xx;
            yc = yy;
        }
        
        iterations_array[width * y + x] = (double)(iterations - 1) / (double)max_iterations;
    }

public:
	/**
	 * Main constructor of the class. 
	 * 
	 * @param width width of the window in pixels.
	 * @param height height of the window in pixels. 
	 * @param max_iterations max allowed number of iterations.
	 * @param x0 real part of center of the sample. 
	 * @param y0 imaginary part of center of the sample. 
	*/
	MandelbrotSet(unsigned width, unsigned height, unsigned max_iterations, double x0, double y0): Fractal(width, height, max_iterations, x0, y0) {}
	MandelbrotSet(unsigned width, unsigned height): MandelbrotSet(width, height, 50, 0, 0) {}
	MandelbrotSet(): MandelbrotSet(1500, 1000) {}

    ~MandelbrotSet() = default;
};

#endif

class FractalRenderer {
protected:
    sf::RenderWindow window;
    sf::Uint8 *pixels = nullptr;
    unsigned width, height;
    int colors[4];
    Fractal *fractal;

    virtual void rgba_colorscheme(double t) {
		colors[0] = 150 * (1 - t) * t * 4;
		colors[1] = 255 * (1 - t) * t * 4;
		colors[2] = 240 * (1 - t) * t * 4;
		colors[3] = 255;
	}

    virtual void set_color(int x, int y, double t) {
		rgba_colorscheme(t);
		pixels[4 * (width * y + x)] = colors[0];
		pixels[4 * (width * y + x) + 1] = colors[1];
		pixels[4 * (width * y + x) + 2] = colors[2];
		pixels[4 * (width * y + x) + 3] = colors[3];
	}

    void mouse_handle(){
		sf::Vector2i posPixel = sf::Mouse::getPosition(window);
		sf::Vector2f pos = window.mapPixelToCoords(posPixel);
		fractal->x0 += (-(double)pos.x + (double)width / 2) / fractal->scale;
        fractal->y0 += (-(double)pos.y + (double)height / 2) / fractal->scale;
	
		fractal->rescale(fractal->scale_param);
	}

    void keyboard_handle() {
		double k = 0.2 / fractal->scale * fractal->start_scale ;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			fractal->x0-=k;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			fractal->x0+=k;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			fractal->y0 += k;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			fractal->y0 -= k;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
			fractal->max_iterations += 10;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
			if (fractal->max_iterations > 0)
				fractal->max_iterations -= 10;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			fractal->rescale(fractal->scale_param);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) && fractal->scale / fractal->start_scale > 0.5f)
			fractal->rescale(1 / fractal->scale_param);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			fractal->scale = fractal->start_scale;
			fractal->x0 = 0;
			fractal->y0 = 0;
		}
	}

    // MandelbrotSet manSet;

public: 
    FractalRenderer(Fractal *fractal): fractal(fractal), height(fractal->getHeight()), width(fractal->getWidth()), window(sf::VideoMode(fractal->getWidth(), fractal->getHeight()), "title"), pixels(new sf::Uint8[fractal->getWidth() * fractal->getHeight() * 4]) {}

    void set_pixels() {
        for (unsigned y = 0; y < height; y++)
            for (unsigned x = 0; x < width; x++)
                set_pixel(x, y);
    }

    void set_pixel(int x, int y) {
        fractal->iterate(x, y);
        
        set_color(x, y, (double) (fractal->iterations_array[width * y + x]));

    }

    int poll(){
		sf::Font font;
		font.loadFromFile("arial.ttf");

		sf::Text zoomText, precText;
		zoomText.setFont(font);
		precText.setFont(font);
		zoomText.setFillColor(sf::Color::White);
		precText.setFillColor(sf::Color::White);
		zoomText.setCharacterSize(24);
		precText.setCharacterSize(24);

        sf::Texture texture;
        texture.create(width, height);
        sf::Sprite sprite;

		set_pixels();
	// std::cout << "test" << std::endl;

		while (window.isOpen())
    	{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				else if (event.type == sf::Event::KeyPressed)
				{
					keyboard_handle();
                    set_pixels();
				}

                else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					mouse_handle();
                    set_pixels();
				}
			}

            window.clear();

            texture.update(pixels);

            sprite.setTexture(texture);
            window.draw(sprite);

			zoomText.setString("Zoom: " + std::to_string(fractal->scale / fractal->start_scale));
			precText.setString("Max. iterations: " + std::to_string(fractal->max_iterations));
			precText.setPosition(sf::Vector2f(0, 32));
			window.draw(zoomText);
			window.draw(precText);	
            
			window.display();
    	}
		return EXIT_SUCCESS;
	}
    ~FractalRenderer() {
        window.close();
        delete [] pixels;
    }
};