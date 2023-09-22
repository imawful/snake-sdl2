/*
	Simple snake game using WASD
	Author: AWFUL
*/
#include <stdbool.h>
#include <SDL2/SDL.h>

int calcNearest(int, int);  // snaps coords to grid
bool collides(SDL_Rect, SDL_Rect);
int getNumBetween(int , int , Uint32 );

int main(int argc, char *argv[])
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		SDL_LogError(0,"failed to initialize video");
	
	//create window 
	//SDL_CreateWindow(const char *title, int x, int y, int w, int h, Uint32 flags);
	int WINWIDTH = 640;
	int WINHEIGHT = 480;
	SDL_Window* w = NULL;
	w = SDL_CreateWindow("i <3 daisy", 
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							WINWIDTH,
							WINHEIGHT,
							SDL_WINDOW_SHOWN);
	//create renderer 
	//SDL_CreateRenderer(SDL_Window * window, int index, Uint32 flags);
	SDL_Renderer* r = NULL; 
	r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
	
	if(w == NULL || r == NULL)
		SDL_LogError(1,"failed to create window and renderer");
	
	//game variables  
	//*** recommended cellsizes and FPS ***//
	// cellSize = 8; FPS = 30; //
	// cellSize = 16; FPS = 20; //
	// cellSize = 32; FPS = 10; // 
	//*** higher fps = higher speed of snake ***//
	int cellSize = 16;
	int MAXSNAKESIZE = 111;
	SDL_Rect snake[MAXSNAKESIZE];
	int length = 0;
	snake[length].x = snake[length].y = snake[length].w = snake[length].h = cellSize;
	length++;
	SDL_Rect candy;
	candy.x = snake[0].x+cellSize*4;
	candy.y = snake[0].y+cellSize*4;
	candy.w = candy.h = cellSize;
	int xChange = 0;
	int yChange = 0;
	int hitIndex = -1;
	bool movePossible = true;
	bool gameOver = false;
	//game loop variables
	bool isRunning = true;
	SDL_Event e;
	int FPS = 20; // target fps
	Uint32 elapsed = 0;
	Uint32 snakeTime = 0;
	int frameTime = 0;
	while(isRunning)
	{
		elapsed = SDL_GetTicks();
		//events 
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					isRunning = false;
					break;
				case SDL_KEYDOWN:
					if(!gameOver)
						switch(e.key.keysym.sym)
						{
							case 27: // ESC 
								isRunning = false;
								break;
							case 119: // W 
								if((yChange != 1) && !gameOver && movePossible)
									{xChange = 0;yChange = -1;movePossible=false;}
								break;
							case 115: // S
								if((yChange != -1) && !gameOver && movePossible)
									{xChange = 0;yChange = 1;movePossible=false;}
								break;
							case 97: // A
								if((xChange !=1) && !gameOver && movePossible)
									{xChange = -1;yChange = 0;movePossible=false;}
								break;
							case 100: // D
								if((xChange !=-1) && !gameOver && movePossible)
									{xChange = 1;yChange = 0;movePossible=false;}
								break;
							default:
								break;
						}
					else
						switch(e.key.keysym.sym)
						{
							case 27:
								isRunning = false;
								break;
							case 114: // R
								length = 0;
								snake[length].x = snake[length].y = snake[length].w = snake[length].h = cellSize;
								length++;
								candy.x = snake[0].x+cellSize*4;
								candy.y = snake[0].y+cellSize*4;
								candy.w = candy.h = cellSize;
								xChange = 0;
								yChange = 0;
								hitIndex = -1;
								snakeTime = SDL_GetTicks();
								gameOver = false;
								break;
							default:
								SDL_Log("press R to restart or ESC to quit");
								break;
						}
				default:
					break;
			}
		}
		// win condtion
		if(length == MAXSNAKESIZE && !gameOver)
			{SDL_Log("YOU WON..."); xChange = 0; yChange = 0;gameOver = true;}
		//movement
		if(!gameOver)
		{
			if(length == 1)
				{snake[0].x += xChange*(cellSize);snake[0].y += yChange*(cellSize);}
			else
			{
				for(int i = length; i > 0; i--)
					{snake[i].x = snake[i-1].x;snake[i].y = snake[i-1].y;}
				snake[0].x += xChange*(cellSize);
				snake[0].y += yChange*(cellSize);
			}
			movePossible = true;
		}		
		//collision
		if(!gameOver)
		{
			// * snake out of bounds *
			if((snake[0].x < 0)||(snake[0].x >= WINWIDTH)||(snake[0].y < 0)||(snake[0].y >= WINHEIGHT))
				{SDL_Log("YOU LOSE... you scored %d in %d seconds", length, (SDL_GetTicks()-snakeTime)/1000);xChange=0;yChange=0; gameOver = true;}
			// * snake on snake action *
			for(int i = 1; i < length; i++)
				if(collides(snake[0], snake[i]))
					{SDL_Log("YOU LOSE... you scored %d in %d seconds", length, (SDL_GetTicks()-snakeTime)/1000);hitIndex=i;xChange=0;yChange=0; gameOver = true;}
			// * snake eats candy *
			if(collides(snake[0], candy))
			{
				//add to snake & set new candy position 
				candy.x = calcNearest(getNumBetween(cellSize, WINWIDTH-cellSize, SDL_GetTicks()), cellSize);
				candy.y = calcNearest(getNumBetween(cellSize, WINHEIGHT-cellSize, SDL_GetTicks()), cellSize);
				snake[length].x = snake[length].y = 0-cellSize;
				snake[length].w = snake[length].h = cellSize;
				length++;
			}
		}
		//rendering
		SDL_RenderClear(r);
		for(int i = 0; i < length; i++)
		{
			if(i == 0)
				SDL_SetRenderDrawColor(r,0,255,0,255); // snake head color 
			else if(i == hitIndex)
				SDL_SetRenderDrawColor(r,255,0,0,255);
			else 
				SDL_SetRenderDrawColor(r,0,0,0,255);   // snake body color
			SDL_RenderFillRect(r, &snake[i]);
			SDL_SetRenderDrawColor(r,255,255,255,255); // snake body border color 
			if(i != 0)
				SDL_RenderDrawRect(r, &snake[i]);
		}
		{SDL_SetRenderDrawColor(r,0,0,255,255);SDL_RenderDrawRect(r, &candy);SDL_RenderFillRect(r, &candy);}
		SDL_SetRenderDrawColor(r,0,0,0,255);
		SDL_RenderPresent(r);
		
		//frame cap
		frameTime = SDL_GetTicks() - elapsed; 
		if(1000/FPS > frameTime)
			SDL_Delay((1000/FPS)-frameTime);
	}

	SDL_DestroyWindow(w);
	SDL_DestroyRenderer(r);
	SDL_Quit();
	return 0;
}
int calcNearest(int pos, int sz)
{
	int rem = pos % sz;
	int dif1, dif2;
	dif1 = pos + (sz-rem);
	dif2 = pos - rem;
	if(pos - dif2 > dif1 - pos)
		return dif1;
	else
		return dif2;
}
bool collides(SDL_Rect obj1, SDL_Rect obj2) // only works with objs of same w and h
{
	return((obj1.x == obj2.x)&&(obj1.y == obj2.y));
}
int getNumBetween(int a, int b, Uint32 ticks)
{
	//simple random number generator
    int t1 = 0;
    t1 = (ticks + 69 * 47) % b;
    if(t1 < a)
        t1 += a;
    if(t1 <= b)
        return t1;
    else 
    {
        for(int i = 0; i < 1000; i++)
        {
            t1 -= i;
            if(t1 <= b)
                break;
        }
    }
    return t1;
}