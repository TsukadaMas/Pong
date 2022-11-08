#pragma once
#include "SDL.h"
#include <SDL_ttf.h>
#include <iostream>
#include <string>
using namespace std;

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

class GameEngine {
private:
	bool isRunning = true;
	SDL_Renderer* renderer;
	SDL_Event event;

	SDL_Rect playerPaddle;
	SDL_Rect aiPaddle;
	SDL_Rect ball;
	SDL_Rect divider;
	SDL_Rect background = { 0,0, WINDOW_WIDTH,WINDOW_HEIGHT };

	SDL_Window* window;

	TTF_Font* font;
	SDL_Texture* fontTexture;
	SDL_Rect fontRectScore;
	void RenderFont(const char* text, int x, int y, bool isRefreshText);

	int mouseX, mouseY; //mouse cords

	int speed_x, speed_y;
	int direction[2] = { -1,1 };
	int playerScore, aiScore;


public:
	static GameEngine* Instance();
	bool IsRunning() { return isRunning; }
	bool initGameEngine();
	void initGameWorld();
	bool CheckCollision(SDL_Rect A, SDL_Rect B);
	void Input();
	void Update();
	void Render();
	void Quit();
};