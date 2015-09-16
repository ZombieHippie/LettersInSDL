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
#include <map>

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

class SymbolData {
private:
	std::vector<std::vector<*Point>> points;
public:
	// returns a vector of a series of points that will draw the symbol
	// at the correct scale
	std::vector<std::vector<Point>> getDrawLines(float scale);
};

class SymbolManager {
	std::map<char, SymbolData> charsToSymbolData;
public:
	static constexpr float CHAR_WIDTH = 9.0; // width of symbols
	static constexpr float TYPE_KERNING = .5; // distance between symbols
	static constexpr float CHAR_HEIGHT = 16.0; // height of symbols
	SymbolManager() {
		charsToSymbolData = new std::map<char, *SymbolData>();
	}
	~SymbolManager() {
		delete charsToSymbolData;
	}
	SymbolData *getSymbolFromChar (char c);
};

class Application {
public:
	Application() {}
	void run() {
		//--------------------------------------------------------------------------
		//--------------------------------------------------------------------------
		//--------------------------------------------------------------------------
		// This is the important stuff, nothing outside of here should change much
		SDLDriver driver;

		// Make a map of the letters to points
		SymbolManager manager;

		// draw sample stuff
		drawChar(&manager, 'a', 10, 10, 1.0)
		// drawChars(char[]{'C','s','c',' ','3','2','5'}, x, y)		



		waitUntilQuit(&driver);
	}
private:
	// Use the symbol Manager to interact with holding the symbols
	// map of letters to point maps

	// looks up symbol, and draws
	void drawChar(SymbolManager *manager, char c, float x, float y, float scale) {
		SymbolManager::Symbol *symbol_to_be_drawn = manager->getSymbolFromChar(c);
		std::vector<std::vector<Point>> lns = symbol_to_be_drawn->getDrawLines(scale);
		for (std::vector<std::vector<Point>>::iterator itLn = lns.begin(); itLn < itLn.end(); itLn++) {
			std::vector<Point> points = *itLn;
			// begin drawing lines with SDL
			std::cout << "Start drawing line" << std::endl;
			for (std::vector<Point>::iterator itP = points.begin(); itP < points.end(); itP++) {
				Point pointToDraw = *itP;
				std::cout << pointToDraw << std::endl;
			}
			// end drawing lines with SDL
			std::cout << "End drawing line" << std::endl;
		}
	}
	// looks up each symbol and draws
	void drawChars(SymbolManager *manager, char[] chars, float x, float y, float scale) {
		float dx = SymbolManager.CHAR_WIDTH * scale;
		float char_relative_x;
		for (int char_index = 0; char_index < len(chars); char_index++) {
			char_relative_x = scale * float(char_index) * SymbolManager.TYPE_KERNING;
			drawChar(manager, chars[char_index], x + char_relative_x, y, scale);
		}
	}

	// This is the important stuff, nothing outside of here should change much
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	// This is the basic helper driver that we can use for drawing lines onto		
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

