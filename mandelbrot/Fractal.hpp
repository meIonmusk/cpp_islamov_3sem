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
	sf::RenderWindow window;
	unsigned max_iterations;
	double x0;
	double y0;
	double scale;
	const double start_scale;
	const double scale_param;
	const int width, height;
    sf::Uint8 *pixels;
    sf::Image image;
	int colors[4];
	/**
	 * Rescales the scale parameter that is used for zooming a view.
	 * 
	 * @param k scaling parameter (in which times to increase (or decrease) the view).
	 * @throws throws exception if scaling parameter is equal to zero.
	*/
	void rescale(double k) {
		if (!k){
			delete [] pixels;
			throw std::invalid_argument("Scaling parameter cannot be zero.");
		}
	    scale *= k;	
	}

	/**
	 * Handles keyboard events. It can move the center of view, change max iterations count, rescale the view and reset settings. 
	*/
	void keyboard_handle() {
		double k = 0.2 / scale * start_scale ;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			x0-=k;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			x0+=k;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			y0 += k;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			y0 -= k;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
			max_iterations += 10;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
			if (max_iterations > 0)
				max_iterations -= 10;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			rescale(scale_param);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift) && scale / start_scale > 0.5f)
			rescale(1 / scale_param);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			scale = start_scale;
			x0 = 0;
			y0 = 0;
		}
	}

	/**
	 * Handles mouse events. Can zoom and move the view to the cursor where the button was pressed.
	*/
	void mouse_handle(){
		sf::Vector2i posPixel = sf::Mouse::getPosition(window);
		sf::Vector2f pos = window.mapPixelToCoords(posPixel);
		x0 += (-(double)pos.x + (double)window.getSize().x / 2) / scale;
        y0 += (-(double)pos.y + (double)window.getSize().y / 2) / scale;
	
		rescale(scale_param);
	}

	/**
	 * Virtual function that must be overriden for each subclass for correct rendering.
	*/
    virtual void iterate() = 0;
    
	/**
	 * Sets the color of pixel with coordinate (x, y) associated with rgb_colorscheme.
	 * 
	 * @param x horisontal coordinate of pixel.
	 * @param y vertical coordinate of pixel.
	 * @param iterations number of iterations that was made at this point.
	*/
	virtual void set_color(int x, int y, unsigned iterations) {
		double t = (double)(iterations - 1)/(double)max_iterations;

		rgba_colorscheme(t);
		pixels[4 * (width * y + x)] = colors[0];
		pixels[4 * (width * y + x) + 1] = colors[1];
		pixels[4 * (width * y + x) + 2] = colors[2];
		pixels[4 * (width * y + x) + 3] = colors[3];
	}

	/** 
	 * Maps some value to the RGBA colorscheme.
	 * 
	 * @param t the value which will be mapped to the color.
	 * 
	 * @return int[4] array that keeps RGBA color.
	*/
	virtual int* rgba_colorscheme(double t) {
		colors[0] = 150 * (1 - t) * t * 4;
		colors[1] = 255 * (1 - t) * t * 4;
		colors[2] = 240 * (1 - t) * t * 4;
		colors[3] = 255;

		return colors;
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
	 * @param title title of the window.
	*/
	Fractal(unsigned width, unsigned height, unsigned max_iterations, double x0, double y0,  std::string title): width(width), height(height), window(sf::VideoMode(width, height), title), max_iterations(max_iterations), x0(x0), y0(y0), scale_param(2),
																												 start_scale(1 / (2 * 1e-6 * width)), scale(1 / (2 * 1e-6 * fmax(width, height))), pixels(new sf::Uint8[width * height * 4]) {}
	Fractal(unsigned width, unsigned height): Fractal(width, height, 50, 0, 0, "Mandelbrot set") {}
	Fractal(): Fractal(1500, 1000) {}

	Fractal(Fractal&) = delete;
	Fractal(Fractal&&) = delete;
	// Fractal operator =(Fractal&) = delete;
	// Fractal operator =(Fractal&&) = delete;

	/**
	 * The main method that polls the image and responds to any events.
	 * 
	 * @return information about successfully ending the polling (if program was aborted it doesn't return anything).
	*/
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

		iterate();

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
                    iterate();
				}

                else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					mouse_handle();
                    iterate();
				}
			}

            window.clear();

            texture.update(pixels);

            sprite.setTexture(texture);
            window.draw(sprite);

			zoomText.setString("Zoom: " + std::to_string(scale / start_scale));
			precText.setString("Max. iterations: " + std::to_string(max_iterations));
			precText.setPosition(sf::Vector2f(0, 32));
			window.draw(zoomText);
			window.draw(precText);	
            
			window.display();
    	}
		return EXIT_SUCCESS;
	}

	/**
	 * Destructor of the class. Closes the window and deletes the pixels dynamic array.
	*/
	~Fractal(){
		window.close();
        delete [] pixels;
	}
};

#endif

#ifndef MANDELBROT 
#define MANDELBROT

/**
 * Subclass for Fractal class that renders mandelbrot set.
*/
class MandelbrotSet: public Fractal {
protected:
	/**
	 * Overriden method that processing each pixel and colors it with RGBA colorscheme.
	*/
	void iterate() override {
        int y_max = height;
        int x_max = width;

        for (int y = 0; y < y_max; y++){
            for (int x = 0; x < x_max; x++){
                unsigned iterations = 0;
               
                double xc = 0;
                double yc = 0;
                double xx = 0;
                double yy = 0;

                while (xc * xc + yc * yc < 4 && iterations++ < max_iterations){
                    xx = xc * xc - yc * yc + (x - x_max/2) / scale - x0;
                    yy = 2 * xc * yc + (y - y_max/2) / scale  - y0;
                    xc = xx;
                    yc = yy;
                }

				set_color(x, y, iterations);
            }
        }
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
	 * @param title title of the window.
	*/
	MandelbrotSet(unsigned width, unsigned height, unsigned max_iterations, double x0, double y0,  std::string title): Fractal(width, height, max_iterations, x0, y0, title) {}
	MandelbrotSet(unsigned width, unsigned height): MandelbrotSet(width, height, 50, 0, 0, "Mandelbrot set") {}
	MandelbrotSet(): MandelbrotSet(1500, 1000) {}
};

#endif