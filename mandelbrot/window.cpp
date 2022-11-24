// to execute g++ -std=c++17 window.cpp -I/opt/local/include/ /opt/local/lib/libsfml-graphics.dylib /opt/local/lib/libsfml-audio.dylib  /opt/local/lib/libsfml-window.dylib /opt/local/lib/libsfml-system.dylib

#include <iostream>
#include <string.h>
#include <complex.h>
#include <SFML/Graphics.hpp>

class Fractal {
private:
	sf::RenderWindow window;
	sf::View view;
	unsigned max_iterations;
	double x0;
	double y0;
	double scale;
public:
	Fractal(unsigned width, unsigned height, unsigned max_iterations, int x0, int y0,  std::string title): window(sf::VideoMode(width, height), title), view(window.getView()), max_iterations(max_iterations), x0(x0), y0(y0), scale(1 / (2 * 1e-6 * width)) {}
	Fractal(unsigned width, unsigned height): Fractal(width, height, 50, 0, 0, "Mandelbrot set") {}
	Fractal(): Fractal(1500, 1000) {}

	// void zoom(float rescale){
	// 	scale /= rescale;
	// 	view.zoom(scale);
	// 	window.setView(view);
	// }

	void zoom_wheel(int delta){

		// auto delta = event.mouseWheelScroll.delta;
		
		// double scale_param = 0.999;
		// double rescale = (delta < 0) ? scale_param : 2 - scale_param;
		// scale += rescale;
		// if (scale > delta * 0.00001 )
		if (scale + delta > 0)
			scale += delta;
		// std::cout << 5 * std::log2(scale) << std::endl;

			// scale *= (delta < 0) ? 1.5 : 0.666;
			// scale += delta * 1e5;

		// for (int i = 0; i < abs(delta); i++){
		// 	window.clear();
		// 	scale *= rescale;
		// 	iterate();
		// 	window.display();
		// }		
	}

	void fase_key(){
		// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			
		// 				rescale = scale_param;
		// 			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		// 				rescale = 2 - scale_param;
		// 			else
		// 				rescale = 1.f;
		// 			zoom(rescale);
		double k = 0.2;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			x0-=k;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			x0+=k;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			y0+=k;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			y0-=k;
	}

    void iterate(){
		// view = window.getView();
        double y_max = window.getSize().y;
        double x_max = window.getSize().x;
        for (int y = 0; y < y_max; y++){
            for (int x = 0; x < x_max; x++){
                unsigned iterations = 0;
                std::complex<double> z(0);
				// max_iterations = 5  * std::log2(scale);
                while (std::norm(z) < 4 && iterations < max_iterations){
                    z = z * z + std::complex<double>(x - x_max/2 , y - y_max/2) / scale - std::complex<double>(x0, y0);
                    iterations++;
                }
                int r = 170 * iterations / max_iterations + 1;
                int g = 255 * iterations / max_iterations + 1;
                int b = 240 * iterations / max_iterations + 1;
				
                sf::Vertex point(sf::Vector2f(x, y), sf::Color(r, g, b));
                window.draw(&point, 1, sf::Points);
				// view.move(x_max/2 + x0, y_max/2 + y0);
				// view.setCenter(center);
				// window.setView(view);
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
		// float scale_param = 1.001f;
		// float rescale = scale_param;
		// bool pressed = false;
                

		while (window.isOpen())
    	{
			sf::Event event;
			while (window.pollEvent(event))
			{
                // попробуй скейлинг сделать не через зум а через изменение х0 у0 и коэффициента при умножении
				if (event.type == sf::Event::Closed)
					window.close();
				else if (event.type == sf::Event::KeyPressed)
				{
					// double k = 0.5;
					// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					// 	x0-=k;
					// else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					// 	x0+=k;
					// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					// 	y0+=k;
					// else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					// 	y0-=k;
					fase_key();
				}
				else if (event.type == sf::Event::MouseWheelScrolled){
					zoom_wheel(event.mouseWheelScroll.delta);
				}
				

            	
				// if (event.type == sf::Mouse::isButtonPressed(sf::Mouse::Left))
				// {
				// 	// pressed = true;

				// }

				// if ( (event.type == sf::Mouse::isButtonReleased(sf::Mouse::Left))){
				// else if (event.type == sf::Mouse::isButtonReleased(sf::Mouse::Left)){

				// }
					
			}
            iterate();

			window.display();

			window.clear();


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