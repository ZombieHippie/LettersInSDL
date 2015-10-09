// Compile this program from the command line using     
//      g++ -std=c++11 ModelSampleProgram.cpp
// Compile this program in an IDE (CodeBlocks, VisualStudio, etc.) by 
//     setting compiler to use C++11 standard

// Each of these include files is required to use some function 
// that is defined in that file. 
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>

//--------------------------------------------------------------------------
//Using SDL and standard IO
#include <SDL2/SDL.h>

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Point {
public:
	// Constructor. Any setup operation you wish for the class.
	Point() {
		//std::cout << "Making new empty point..." << std::endl;
	}
	Point(float px, float py) : x(px), y(py){
		//std::cout << "x(" << x << ") y(" << y << ")" << std::endl;
	} // end constructor
	float getX() const { return x; }
	float getY() const { return y; }
	// return new Point scaled version of this Point
	Point scale(float scale) const {
		Point res(x * scale, y * scale);
		return res;
	}
	Point move(float dx, float dy) const {
		Point res(x + dx, y + dy);
		return res;
	}
	std::string toString() const {
		std::string res = "(";
		res += std::to_string(x);
		res += ",";
		res += std::to_string(y);
		res += ")";
		return res;
	}
	friend std::ostream& operator<<(std::ostream& os, const Point& p);
/*	Point& operator=(Point& p) {
		this->x = p.getX();
		this->y = p.getY();
	}*/
private:
	float x;
	float y;
}; // end class Point
// Point operator stream
std::ostream& operator<<(std::ostream& os, const Point& p) {
	os << p.toString();
	return os;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// This is the important stuff, nothing outside of here should be terribly unique

typedef std::vector<Point> Line;

class SymbolData {
private:
	std::vector<Line> lines;
public:
	// called when accessing map char that does not exist
	SymbolData() {};
	SymbolData(std::vector<Line> givenLines): lines(givenLines) {};
	// returns a vector of a series of points that will draw the symbol
	// at the correct scale
	std::vector<Line> getDrawLines(float scale) {
		std::vector<Line> res;
		for (std::vector<Line>::iterator itLn = lines.begin(); itLn < lines.end(); itLn++) {
			Line newDrawLine;
			for (Line::iterator itP = itLn->begin(); itP < itLn->end(); itP++) {
				Point p = *itP;
				// add scaled version of line to DrawLine
				newDrawLine.push_back(p.scale(scale));
			}
			// add draw line to results
			res.push_back(newDrawLine);
		}
		return res;
	}
};

// This is the basic helper driver that we can use for drawing lines onto		
class SDLDriver {
	//The window we'll be rendering to
	SDL_Window* gWindow = NULL;

	//The window renderer
	SDL_Renderer* gRenderer = NULL;
public:
	SDLDriver() {
		gWindow = SDL_CreateWindow("Project 1", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		 SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	void drawEdge(const Point a, const Point b) {
		drawLine(a, b, 0xaa, 0xaa, 0xaa);
	}
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
	void drawLine(const Point a, const Point b, Uint8 color_r, Uint8 color_g, Uint8 color_b) {
		SDL_SetRenderDrawColor(gRenderer, color_r, color_g, color_b, 0xFF);
		SDL_RenderDrawLine(gRenderer, a.getX(), a.getY(),
			b.getX(), b.getY());
		//Update screen
		SDL_RenderPresent(gRenderer);
	}
};


class SymbolManager {
	std::vector<SymbolData> charsToSymbolData;
public:
	static constexpr float CHAR_WIDTH = 9.0; // width of symbols
	static constexpr float TYPE_KERNING = 1.0; // distance between symbols
	static constexpr float CHAR_HEIGHT = 16.0; // height of symbols
	SymbolManager() {
		Point dummyPoint;
		std::vector<Point> dummyCharacter;
		std::vector<Line> dummyFont;
		#include "./CharDefs.h"		
		std::vector<Line> lines;

		for (int i = 0; i < dummyFont.size(); i++) {
			std::vector<Line> charLines;
			charLines.push_back(dummyFont[i]);
			SymbolData data(charLines);
			charsToSymbolData.push_back(data);
		}
	}
	SymbolData *getSymbolFromChar (char c) {
		return &charsToSymbolData[c - 32];
	}
};

class Application {
	// these objects instantiated with the object
	SDLDriver driver;
	// Make a map of the letters to points
	SymbolManager manager;
public:
	Application() {}
	void run() {
		
		// Set output of program to file
		freopen( "project1.log", "wt", stdout ); 
		freopen( "project1.log", "wt", stderr );


		// draw sample stuff
		drawChar('J', 1, 1, 8);
		
		float font_size = 20;
		for (float i = 0; i < 3; i++) {
			drawString("Hello World", 50.0, 20.0 + (i * font_size * 1.8), font_size);
		}
		drawString("The quick brown fox jumped over the lazy dog.", 20, 140, 12.0);
		drawString("THE QUICK BROWN FOX JUMPED OVER THE LAZY DOG.", 20, 170, 12.0);
		drawString("Cole Lawrence", 20, 210, 12.0);
		drawString("Josiah McGurty", 40, 240, 12.0);
		drawString("Andrew Rowell", 60, 270, 12.0);
		drawString("Other Chars: ;,.!?#$&\"'()*%+-/\\", 20, 300, 12.0);
		drawString("1234567890", 20, 330, 12.0);

		waitUntilQuit();
	}
private:
	void drawString(std::string str, float x, float y, float font_size) {
		char tab2[1024];
		strncpy(tab2, str.c_str(), sizeof(tab2));
		tab2[sizeof(tab2) - 1] = 0;
		drawChars(tab2, str.size(), x, y, font_size / SymbolManager::CHAR_WIDTH);
	}
	// Use the symbol Manager to interact with holding the symbols
	// map of letters to point maps

	// looks up symbol, and draws
	void drawChar(char c, float x, float y, float scale) {
		SymbolData *symbol_to_be_drawn = manager.getSymbolFromChar(c);
		std::vector<Line> lns = symbol_to_be_drawn->getDrawLines(scale);
		std::cout << "Drawing character: '" << c << "' (" << lns.size() << " lines)" << std::endl;
		for (std::vector<Line>::iterator itLn = lns.begin(); itLn < lns.end(); itLn++) {
			Line points = *itLn;
			// begin drawing lines with SDL
			std::cout << "Start drawing line\n";
			Point lastPoint;
			for (Line::iterator itP = points.begin(); itP < points.end(); itP++) {
				Point pointToDraw = *itP;
				pointToDraw = pointToDraw.move(x, y);
				std::cout << pointToDraw;
				if (itP != points.begin()) {
					driver.drawEdge(pointToDraw, lastPoint);
				}
				lastPoint = pointToDraw;
			}
			// end drawing lines with SDL
			std::cout << "\nEnd drawing line\n";
		}
	}
	// looks up each symbol and draws
	void drawChars(char chars[], int n, float x, float y, float scale) {
		float char_relative_x;
		int length_of_chars = n;
		for (int char_index = 0; char_index < length_of_chars; char_index++) {
			char_relative_x = scale * float(char_index) * (SymbolManager::TYPE_KERNING + SymbolManager::CHAR_WIDTH);
			drawChar(chars[char_index], x + char_relative_x, y, scale);
		}
	}

	// This is the important stuff, nothing outside of here should be terribly unique
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	void waitUntilQuit() {
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
		driver.close();
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
