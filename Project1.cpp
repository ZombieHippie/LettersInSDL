// Adapted from http://headerphile.blogspot.com/2014/04/part-3-game-programming-in-sdl2-drawing.html
// SDL 2.0 API by Name https://wiki.libsdl.org/CategoryAPI
// SDL 2.0 API by Category https://wiki.libsdl.org/APIByCategory

	// Common graphic operations in SDL2
	// SDL_RenderDrawLine  Draw a line, one pixel wide. Wider: use a thin rectangle
	// SDL_RenderFillRect   draw a rectangle
	// ???    write text to graphics (SDL_RWwrite?)
	// ???	  draw a general polygon using vertices
	// ???    draw an oval or circle (SDL_RenderDrawPoints?)
	
/* REMOVE TO USE GRAPHICS
#include <SDL.h>
END OF COMMENT TO USE GRAPHICS
*/
	
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>

// This line allows commonly-used functions to be used without specifying the 
// library in which that function is defined. For instance, this line allows
// the use of "cout" rather than the full specification "cout"
using namespace std;




//--------------------------------------------------------------------------
class Point
{

    public:
    
        // Constructor. Any setup operation you wish for the class.
        Point()
        {
            x = 0; y = 0;  
        } // end constructor
        Point(int a, int b)
        {
            x = a; y = b;  
        } // end constructor

        int getX() { return x; }
        int getY() { return y; }
    
    private:
        int x = 0; 
        int y = 0;
        
}; // end class Point


// NOTE: DO NOT declare with empty parentheses, as vector<Point> myPointvector();
vector<Point> myPointvector;  // vector will expand as needed

// Graphics window sizes
int posX = 100;
int posY = 200;
int sizeX = 300;
int sizeY = 400;
int numberEdgesToDraw = 0;


/* REMOVE TO USE GRAPHICS
SDL_Window* window;
SDL_Renderer* renderer;

bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();

void Render();
void RunGame();

SDL_Rect playerPos;
END OF COMMENT TO USE GRAPHICS
*/
	

// Pass arguments or parameters from command-line execution. argc is the count of
// those parameters, including the executable filename. argv[] is an array of the 
// parameters.
int main (int argc, char *argv[])
{
    string token;
    int xValue, yValue;
    ifstream fin;


	cout << "\nHello World\n";

    // Check the number of arguments. Expected: filename of a file
    if (argc != 2)  // This check is often hardcoded
    {   // If failure in parameters, offer advice for correction
        cout << "\nThis program uses command-line argument.\n";
        cout << "Usage: a.exe <filename>\n";
        exit(0);
    }


    try  // All lines within this block are part of the same exception handler
    {
        fin.open(argv[1]);
    } 
    catch (exception& ex) 
    {
        cout << ex.what();  // display standard explanation of the exception
        exit(0);  // exit the program
    }
            
   
    // Read from the file, one token at a time. If the type of token is known, it
    // can be read into a corresponding variable type, such as 
    //          in >> x;    // Read the first item into an integer variable x.
    //          in >> str;  // Read the next item into a string variable str.
            
    while (fin >> xValue)
    {
        // Do something with the element read from the file
		if (xValue == -9999)  // end point list sentinel value
		{
			Point dummyPoint(-9999, -9999);
			myPointvector.push_back(dummyPoint);  // vector will expand as needed
			break;  // stop reading from file
		}
        cout << "xValue is " << xValue << endl;
		
		fin >> yValue;


		// Transform the (x,y) point to the graphic coordinate system:
			//   Shift all x values by +20
			//   Shift all y values by +20
			//   Scale x and y values by 8
			//   Invert y axis values by subtracting from 400

			
		cout << "Now myPointvector has size " << myPointvector.size() << endl;

		
		
    } // end while
                
    fin.close();


	/* REMOVE TO USE GRAPHICS
	
	if ( !InitEverything() )   // Call to initialize graphics
		return -1;

	RunGame();
	
	END OF COMMENT TO USE GRAPHICS
	*/
	
} // end main


/* REMOVE TO USE GRAPHICS

void RunGame()
{
	bool loop = true;

	while ( loop )
	{

		Render();

		// Add a 16msec delay to make our game run at ~60 fps
		SDL_Delay( 200 );
		
		// increase the number of lines to draw
		if (numberEdgesToDraw < myPointvector.size()) 
			numberEdgesToDraw++; 
		
	}
}


// -----------------------------------------------------------------------
// Evidently a function named "Render()" is essential
void Render()
{
	
	// Common graphic operations in SDL2
	// SDL_RenderDrawLine  Draw a line, one pixel wide. Wider: use a thin rectangle
	// SDL_RenderFillRect   draw a rectangle
	// ???    write text to graphics (SDL_RWwrite?)
	// ???	  draw a general polygon using vertices
	// ???    draw an oval or circle (SDL_RenderDrawPoints?)
	

	// Clear the window 
	SDL_RenderClear( renderer );

	// Change color to blue!  Parameters are Red, Green, Blue, Alpha
	SDL_SetRenderDrawColor( renderer, 51, 51, 204, 255 );

	// Render our "player"
	//SDL_RenderFillRect( renderer, &playerPos );

	

	
	// Draw an increasing number of edges
	for (int i = 0; i < numberEdgesToDraw; i++) 
	{
		if (myPointvector.at(i).getX() == -9999) break;  // no more edges
		
		if (myPointvector.at(i).getX() == -999) continue; // don't draw from this sentinel value
		
		if (myPointvector.at(i+1).getX() == -999) continue; // don't draw to this sentinel value
		
		
		// SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
		
		
		// ???????????? Make a call to SDL_RenderDrawLine(renderer, x1, y1, x2, y2) to draw an edge
	}

	// Change color to green!  Parameters are Red, Green, Blue, Alpha
	SDL_SetRenderDrawColor( renderer, 102, 204, 0, 255 );

	// Render the changes above
	SDL_RenderPresent( renderer);
}


bool InitEverything()
{
	if ( !InitSDL() )
		return false;

	if ( !CreateWindow() )
		return false;

	if ( !CreateRenderer() )
		return false;

	SetupRenderer();

	return true;
}
bool InitSDL()
{
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
	{
		cout << " Failed to initialize SDL : " << SDL_GetError() << endl;
		return false;
	}

	return true;
}
bool CreateWindow()
{
	window = SDL_CreateWindow( "Server", posX, posY, sizeX, sizeY, 0 );

	if ( window == NULL) //  8/19/15 KV  nullptr )
	{
		cout << "Failed to create window : " << SDL_GetError();
		return false;
	}

	return true;
}
bool CreateRenderer()
{
	renderer = SDL_CreateRenderer( window, -1, 0 );

	if ( renderer == NULL) //  8/19/15 KV  nullptr )
	{
		cout << "Failed to create renderer : " << SDL_GetError();
		return false;
	}

	return true;
}
void SetupRenderer()
{
	// Set size of renderer to the same as window
	SDL_RenderSetLogicalSize( renderer, sizeX, sizeY );

	// Set color of renderer background
	SDL_SetRenderDrawColor( renderer, 102, 204, 0, 255 );
}

END OF COMMENT TO USE GRAPHICS
*/
	
