// to execute g++ -std=c++17 window.cpp -I/opt/local/include/ /opt/local/lib/libsfml-graphics.dylib /opt/local/lib/libsfml-audio.dylib  /opt/local/lib/libsfml-window.dylib /opt/local/lib/libsfml-system.dylib

#include <iostream>
#include <string.h>
#include <complex.h>
#include <SFML/Graphics.hpp>

class Fractal {
private:
	sf::RenderWindow window;
	sf::View view;
	float scale;
public:
	Fractal(unsigned width, unsigned height, std::string title): window(sf::VideoMode(width, height), title), view(window.getView()), scale(1.f) {
		// window.setFramerateLimit(120);
		}
	Fractal(unsigned width, unsigned height): Fractal(width, height, "Mandelbrot set") {}
	Fractal(): Fractal(800, 600, "Mandelbrot set") {};

	void show(){

	}

	void zoom(float rescale){
		scale /= rescale;
		view.zoom(scale);
		window.setView(view);
	}

    void iterate(int x0=0, int y0=0){
        double y_max = view.getSize().y;
        double x_max = view.getSize().x;
        for (int y = 0; y < y_max; y++){
            for (int x = 0; x < x_max; x++){
                unsigned iterations = 0;
                std::complex<double> z(0);
                while (std::norm(z) < 4 && iterations < 100){
                    z = z * z + std::complex<double>(x - x_max/2, y - y_max/2) * (double)0.0002f;
                    iterations++;
                }
                int r = 255 * iterations * 0.01;
                int g = 255 * iterations * 0.007;
                int b = 255 * iterations * 0.01;
                sf::Vertex point(sf::Vector2f(x, y), sf::Color(r, g, b));
                window.draw(&point, 1, sf::Points);
                // std::cout << x << ' ' << y << std::endl;
            }
        }
    }

	int poll(){
		// sf::Texture texture;
		// if (!texture.loadFromFile("cute_image.jpeg"))
		// 	return EXIT_FAILURE;
		// sf::Sprite sprite(texture);
		// // Create a graphical text to display
		// sf::Font font;
		// if (!font.loadFromFile("arial.ttf"))
		// 	return EXIT_FAILURE;
		// sf::Text text("Hello SFML", font, 50);
		float scale_param = 1.001f;
		float rescale = scale_param;
		bool pressed = false;
                

		while (window.isOpen())
    	{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				else if (event.type == sf::Event::KeyPressed)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
						rescale = scale_param;
					else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
						rescale = 2 - scale_param;
					else
						rescale = 1.f;
					zoom(rescale);
				}
				else if (event.type == sf::Event::MouseWheelScrolled){

					auto delta = event.mouseWheelScroll.delta;
					
					rescale = (delta > 0) ? scale_param : 2 - scale_param;
			
					for (int i = 0; i < abs(delta); i++)
						zoom(rescale);
				}
				// if (event.type == sf::Mouse::isButtonPressed(sf::Mouse::Left))
				// {
				// 	// pressed = true;

				// }

				// // else if (! (event.type == sf::Mouse::isButtonPressed(sf::Mouse::Left)) and pressed){
				// else if (event.type == sf::Mouse::isButtonReleased(sf::Mouse::Left)){

				// }
					
			}
            iterate();

            			// window.draw(sprite);
			// window.draw(text);
            window.display();
			// window.clear();

			
    	}
		return EXIT_SUCCESS;
	}

	~Fractal(){
		window.clear();
		window.close();
	}

};

int main(){
	Fractal mandelbrot;
	return mandelbrot.poll();
}