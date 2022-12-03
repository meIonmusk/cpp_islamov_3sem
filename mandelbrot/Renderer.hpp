/**
 * This header file is part of {{mandelbrot}}.
 * 
 * There are defined classes FractalRenderer for rendering fractals.
*/
#include <iostream>
#include <string.h>
#include <SFML/Graphics.hpp>
#include "Fractal.hpp"

#ifndef RENDERER 
#define RENDERER
/**
 * Renderer for written fractals. 
*/
class FractalRenderer final {
private:
    sf::RenderWindow window;
    sf::Uint8 *pixels = nullptr;
    unsigned width, height;
    int colors[4];
    Fractal *fractal;
    const double scale_param;

    /** 
	 * Maps some value to the RGBA colorscheme.
	 * 
	 * @param t the value which will be mapped to the color.
	 * 
	*/
    virtual void rgbaColorscheme(double t) {
		colors[0] = 150 * (1 - t) * t * 4;
		colors[1] = 255 * (1 - t) * t * 4;
		colors[2] = 240 * (1 - t) * t * 4;
		colors[3] = 255;
	}

    /**
	 * Sets the color of pixel with coordinate (x, y) associated with rgb_colorscheme.
	 * 
	 * @param x horisontal coordinate of pixel.
	 * @param y vertical coordinate of pixel.
	 * @param t the value which will be mapped to the color (depends on iterations number).
	*/
    void setColor(int x, int y, double t) {
		rgbaColorscheme(t);
		pixels[4 * (width * y + x)] = colors[0];
		pixels[4 * (width * y + x) + 1] = colors[1];
		pixels[4 * (width * y + x) + 2] = colors[2];
		pixels[4 * (width * y + x) + 3] = colors[3];
	}

	/**
	 * Handles mouse events. Can zoom and move the view to the cursor where the button was pressed.
	*/
    void mouseHandle(){
		sf::Vector2i posPixel = sf::Mouse::getPosition(window);
		sf::Vector2f pos = window.mapPixelToCoords(posPixel);
		fractal->updateCenterX((-(double)pos.x + (double)width / 2));
		fractal->updateCenterY((-(double)pos.y + (double)height / 2));
		fractal->rescale(scale_param);
	}

	/**
	 * Handles keyboard events. It can move the center of view, change max iterations count, rescale the view and reset settings. 
	*/
    void keyboardHandle() {
		double k = 0.2 * fractal->getStartScale();
        
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			fractal->updateCenterX(-k);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			fractal->updateCenterX(k);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			fractal->updateCenterY(k);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			fractal->updateCenterY(-k);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal))
			fractal->updateMaxIterations(10);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
			fractal->updateMaxIterations(-10);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
			fractal->rescale(scale_param);
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			fractal->rescale(1 / scale_param);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            fractal->reset();
	}

public: 
    /**
	 * Constructor of the class. 
	 * 
	 * @param fractal pointer to object of Fractal (or its subclass) type
     * @param title title of the window (unnecessary)
	*/
    FractalRenderer(Fractal *fractal, std::string title): fractal(fractal), height(fractal->getHeight()), width(fractal->getWidth()), window(sf::VideoMode(fractal->getWidth(), fractal->getHeight()), title), 
                                                          pixels(new sf::Uint8[fractal->getWidth() * fractal->getHeight() * 4]), scale_param(2) {}

    FractalRenderer(Fractal *fractal): FractalRenderer(fractal, "Fractal Rendering") {}

    /**
     * Sets all pixels' colors.
    */
    void setPixels() {
        for (unsigned y = 0; y < height; y++)
            for (unsigned x = 0; x < width; x++)
                setPixel(x, y);
    }

    /**
     * Sets color of pixel with (x, y) coordinates.
    */
    void setPixel(int x, int y) {
        fractal->iterate(x, y);
        
        setColor(x, y, (double) (fractal->getIterationsArray()[width * y + x]));
    }

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

		setPixels();

		while (window.isOpen())
    	{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				else if (event.type == sf::Event::KeyPressed)
				{
					keyboardHandle();
                    setPixels();
				}

                else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					mouseHandle();
                    setPixels();
				}
			}

            window.clear();

            texture.update(pixels);

            sprite.setTexture(texture);
            window.draw(sprite);

			zoomText.setString("Zoom: " + std::to_string(fractal->getScale() / fractal->getStartScale()));
			precText.setString("Max. iterations: " + std::to_string(fractal->getMaxIterations()));
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

#endif