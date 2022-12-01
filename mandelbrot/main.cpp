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
	double start_scale;
    sf::Uint8 *pixels;
    sf::Image image;
public:
	Fractal(unsigned width, unsigned height, unsigned max_iterations, double x0, double y0,  std::string title): window(sf::VideoMode(width, height), title), view(window.getView()), max_iterations(max_iterations), x0(x0), y0(y0), start_scale(1 / (2 * 1e-6 * width)), scale(1 / (2 * 1e-6 * width)) ,
    pixels(new sf::Uint8[width * height * 4]) {}
	Fractal(unsigned width, unsigned height): Fractal(width, height, 50, 0, 0, "Mandelbrot set") {}
	Fractal(): Fractal(1500, 1000) {}

	// void zoom(float rescale){
	// 	scale /= rescale;
	// 	view.zoom(scale);
	// 	window.setView(view);
	// }

	void zoom_wheel(){

		// auto delta = event.mouseWheelScroll.delta;
		
		// double scale_param = 0.999;
		// double rescale = (delta < 0) ? scale_param : 2 - scale_param;
		// scale += rescale;
		// if (scale > delta * 0.00001 )

		// if (scale + delta > 0)
			// scale += delta;
		
		scale *= 1.5;
		// scale += ;
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
		double k = 0.1 / scale * start_scale ;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			x0-=k;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			x0+=k;
		// if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			// y0+=k;
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
			zoom_wheel();
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
			scale /= 1.5;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			scale = start_scale;
			x0 = 0;
			y0 = 0;
		}
		

		// std::cout << x0 << ' ' << y0 << std::endl;
	}

    void iterate(){

		// view = window.getView();
        int y_max = window.getSize().y;
        int x_max = window.getSize().x;
        for (int y = 0; y < y_max; y++){
            for (int x = 0; x < x_max; x++){
                unsigned iterations = 0;
                // std::complex<double> z(0);
                // {
                // double xc = 0;
                // double yc = 0;
                // double xx = 0;
                // double yy = 0;
                // double xy = 0;
				// // max_iterations = 5  * std::log2(scale);
                // while (xc + yc < 4 && iterations++ < max_iterations){
                //     xy = xc * yc;
                //     xx = xc * xc;
                //     yy = yc * yc;
                //     xc = xx - yy + x0;
                //     yc = xy + xy + y0;
                // }
                // }
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
                // while (std::norm(z) < 4 && iterations++ < max_iterations){
                //     z = z * z + std::complex<double>(x - x_max/2 , y - y_max/2) / scale - std::complex<double>(x0, y0);
                //     // iterations++;
                // }
				// int r = 255 * (1 - 0.3 + iterations * 0.03 * 0.2); //расчет 
          		// int g = 255 * (1 - 0.1 + iterations * 0.03 * 0.3); //значений
          		// int b = 255 * (1 - 0.2 + iterations * 0.03 * 0.1); //для раскраски множества

				double t = (double)(iterations - 1)/(double)max_iterations;

			 	// Use smooth polynomials for r, g, b
			 	// int r = (int)(9*(1-t)*(1-t)*t*t*255);
			 	// int b = (int)(15*(1-t)*(1-t)*(t-1)*t*255);
				// int r = b;
			 	// int g =  (int)(8.5*(1-t)*(1-t)*t*t*255);

                int g = 255 * (1 - t) * t * 4;
                int r = 150 * (1 - t) * t * 4;
                int b = 240 * (1 - t) * t * 4;

                pixels[4 * (x_max * y + x)] = r;
                pixels[4 * (x_max * y + x) + 1] = g;
                pixels[4 * (x_max * y + x) + 2] = b;
                pixels[4 * (x_max * y + x) + 3] = 255;
                // std::cout << "boom" << std::endl;
                // sf::Vertex point(sf::Vector2f(x, y), sf::Color(r, g, b));
                // window.draw(&point, 1, sf::Points);
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

		sf::Font font;
		font.loadFromFile("arial.ttf");

		sf::Text zoomText, precText;
		zoomText.setFont(font);
		precText.setFont(font);
		zoomText.setFillColor(sf::Color::White);
		precText.setFillColor(sf::Color::White);
		zoomText.setCharacterSize(24);
		precText.setCharacterSize(24);

        sf::Clock clock;
        iterate();
        sf::Texture texture;
        texture.create(window.getSize().x, window.getSize().y);
        sf::Sprite sprite;
        int k = 0;
		while (window.isOpen())
    	{
            k++;
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
                    iterate();
				}
				// else if (sf::Event::MouseWheelScrolled){
				// 	zoom_wheel(1000*event.mouseWheelScroll.delta);
                //     // std::cout << event.mouseWheelScroll.delta << std::endl;
                //     iterate();
				// }
                else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					sf::Vector2i posPixel = sf::Mouse::getPosition(window);
					sf::Vector2f pos = window.mapPixelToCoords(posPixel);
					x0 += (-(double)pos.x + (double)window.getSize().x / 2) / scale;
        			y0 += (-(double)pos.y + (double)window.getSize().y / 2) / scale;
					// std::cout << "pos: " << pos.x << ' ' << pos.y << std::endl;
					// std::cout << "center: " << x0 << ' ' << y0 << std::endl;
					// x0 =  pos.x / scale;
					// y0 = pos.y / scale;
	
					zoom_wheel();
                    iterate();
                    // std::cout << clock.getElapsedTime().asSeconds() << std::endl;
				}
				

            	
				// if (event.type == sf::Mouse::isButtonPressed(sf::Mouse::Left))
				// {
				// 	// pressed = true;

				// }

				// if ( (event.type == sf::Mouse::isButtonReleased(sf::Mouse::Left))){
				// else if (event.type == sf::Mouse::isButtonReleased(sf::Mouse::Left)){

				// }
					
			}
            // iterate();
            // image.create(window.getSize().x, window.getSize().y, pixels);

            // texture.setTexture()
		    // if (!texture.loadFromImage(image))
			//     return EXIT_FAILURE;
            window.clear();

            texture.update(pixels);
            // std::cout << "update texture " << clock.getElapsedTime().asSeconds() << std::endl;

            sprite.setTexture(texture);
            window.draw(sprite);

			zoomText.setString("Zoom: " + std::to_string(scale / start_scale));
			precText.setString("Max. Iterations: " + std::to_string(max_iterations));
			precText.setPosition(sf::Vector2f(0, 32));
			window.draw(zoomText);
			window.draw(precText);	
            
			window.display();

            // clock.restart();


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