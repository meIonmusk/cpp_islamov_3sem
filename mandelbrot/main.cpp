// to execute g++ -std=c++17 window.cpp -I/opt/local/include/ /opt/local/lib/libsfml-graphics.dylib /opt/local/lib/libsfml-audio.dylib  /opt/local/lib/libsfml-window.dylib /opt/local/lib/libsfml-system.dylib

#include <iostream>
#include <string.h>
#include <complex.h>
#include <SFML/Graphics.hpp>

class Fractal {
private:
	sf::RenderWindow window;
	unsigned max_iterations;
	double x0;
	double y0;
	double scale;
	double start_scale;
	double scale_param;
	int width, height;
    sf::Uint8 *pixels;
    sf::Image image;

public:
	Fractal(unsigned width, unsigned height, unsigned max_iterations, double x0, double y0,  std::string title): width(width), height(height), window(sf::VideoMode(width, height), title), max_iterations(max_iterations), x0(x0), y0(y0), scale_param(1.5),
																												 start_scale(1 / (2 * 1e-6 * width)), scale(1 / (2 * 1e-6 * fmax(width, height))), pixels(new sf::Uint8[width * height * 4]) {}
	Fractal(unsigned width, unsigned height): Fractal(width, height, 50, 0, 0, "Mandelbrot set") {}
	Fractal(): Fractal(1500, 1000) {}

	Fractal(Fractal&) = delete;
	Fractal(Fractal&&) = delete;
	Fractal operator =(Fractal&) = delete;
	Fractal operator =(Fractal&&) = delete;

	void rescale(double k){
		scale *= k;	
	}

	void keyboard_handle(){
		double k = 0.1 / scale * start_scale ;

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
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			scale /= scale_param;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			scale = start_scale;
			x0 = 0;
			y0 = 0;
		}
	}

	void mouse_handle(){
		sf::Vector2i posPixel = sf::Mouse::getPosition(window);
		sf::Vector2f pos = window.mapPixelToCoords(posPixel);
		x0 += (-(double)pos.x + (double)window.getSize().x / 2) / scale;
        y0 += (-(double)pos.y + (double)window.getSize().y / 2) / scale;
	
		rescale(scale_param);
	}

    void iterate(){
        int y_max = window.getSize().y;
        int x_max = window.getSize().x;

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

				double t = (double)(iterations - 1)/(double)max_iterations;

                int g = 255 * (1 - t) * t * 4;
                int r = 150 * (1 - t) * t * 4;
                int b = 240 * (1 - t) * t * 4;

                pixels[4 * (x_max * y + x)] = r;
                pixels[4 * (x_max * y + x) + 1] = g;
                pixels[4 * (x_max * y + x) + 2] = b;
                pixels[4 * (x_max * y + x) + 3] = 255;
            }
        }
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
        texture.create(window.getSize().x, window.getSize().y);
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
			precText.setString("Max. Iterations: " + std::to_string(max_iterations));
			precText.setPosition(sf::Vector2f(0, 32));
			window.draw(zoomText);
			window.draw(precText);	
            
			window.display();
    	}
		return EXIT_SUCCESS;
	}

	~Fractal(){
		window.clear();
		window.close();
        delete [] pixels;
	}
};

int main(){
	Fractal mandelbrot(1500, 1000, 50, 0, 0, "Mandelbrot");
	return mandelbrot.poll();
}