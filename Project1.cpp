// Compile this program from the command line using     
//      g++ -std=c++11 ModelSampleProgram.cpp
// Compile this program in an IDE (CodeBlocks, VisualStudio, etc.) by 
//     setting compiler to use C++11 standard

// Each of these include files is required to use some function 
// that is defined in that file. 
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <algorithm>

//--------------------------------------------------------------------------
//Using SDL and standard IO
#include <SDL.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Point {
public:
	// Constructor. Any setup operation you wish for the class.
	Point(int px, int py) {
		x = px; y = py;
		key = INFINITY;
		pi = nullptr;
	} // end constructor
	double key;
	Point* pi;
	int getX() { return x; }
	int getY() { return y; }
	double dist(const Point &p) {
		int c = std::pow(p.x - x, 2) + std::pow(p.y - y, 2);
		return std::sqrt(c);
	}
	friend std::ostream& operator<<(std::ostream& os, const Point& p);
private:
	int x;
	int y;
}; // end class Point
// Point operator stream
std::ostream& operator<<(std::ostream& os, const Point& p) {
	os << '(' << p.x << ',' << p.y << ')';
	return os;
}

class Application {
public:
	Application() {}
	void run() {
		SDLDriver driver;
		// Step F
		waitUntilQuit(&driver);
	}
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	// This is the important stuff, nothing outside of here should change much
private:
	// Step F
	class SDLDriver {
		//The window we'll be rendering to
		SDL_Window* gWindow = NULL;

		//The window renderer
		SDL_Renderer* gRenderer = NULL;

		const int size = 5;
	public:
		SDLDriver() {
			gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
			if (gWindow == NULL)
			{
				printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			}
			else
			{
				//Create renderer for window
				gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
				if (gRenderer == NULL)
				{
					printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				}
				else
				{
					//Initialize renderer color
					SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				}
			}
		}
		void drawEdge(Point* a, Point* b) {
			drawLine(a, b, 0x11, 0x11, 0x11);
		}
		void drawPoint(Point* p) { drawDot(p, 1, 0xFF, 0xFF, 0x00); }
		void drawRoot(Point* p) { drawDot(p, 2, 0xFF, 0x00, 0xFF); }
		void delay(int milliseconds) {
			SDL_Delay(milliseconds);
		}
		void close() {
			//Destroy window	
			SDL_DestroyRenderer(gRenderer);
			SDL_DestroyWindow(gWindow);
			gWindow = NULL;
			gRenderer = NULL;

			//Quit SDL subsystems
			SDL_Quit();
		}
	private:
		void drawDot(Point* p, int radius, Uint8 color_r, Uint8 color_g, Uint8 color_b) {
			//Update screen
			SDL_Rect fillRect = { p->getX() * size - radius * size, p->getY() * size - radius * size, radius * 2 * size, radius * 2 * size };
			SDL_SetRenderDrawColor(gRenderer, color_r, color_g, color_b, 0x33);
			SDL_RenderFillRect(gRenderer, &fillRect);
			SDL_RenderPresent(gRenderer);
		}
		void drawLine(Point* a, Point* b, Uint8 color_r, Uint8 color_g, Uint8 color_b) {
			if (a != nullptr && b != nullptr) {
				SDL_SetRenderDrawColor(gRenderer, color_r, color_g, color_b, 0x33);
				SDL_RenderDrawLine(gRenderer, a->getX() * size, a->getY() * size, b->getX() * size, b->getY() * size);
				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	};
	void waitUntilQuit(SDLDriver *driver) {
		//Main loop flag
		bool quit = false;

		//Event handler
		SDL_Event e;

		//While application is running
		while (!quit)
		{
			//Handle events on queue
			while (SDL_PollEvent(&e) != 0)
			{
				//User requests quit
				if (e.type == SDL_QUIT)
				{
					quit = true;
				}
			}
		}
		driver->close();
	}
};

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------

// Pass arguments or parameters from command-line execution. argc is the count of
// those parameters, including the executable filename. argv[] is an array of the 
// parameters.
int main(int argc, char *argv[])
{
	Application app;
	app.run();
	return 0;
} // end main

