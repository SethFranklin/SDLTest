
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include <SDL.h>

#include "Backend.h"
#include "Input.h"

const char* ProjectName = "SDLTest";

bool Running = true;

SDL_Window* Window;
SDL_Renderer* Renderer;

int FrameRate = 60;

const int GameWidth = 400;
const int GameHeight = 225;

double GameAspectRatio = double(GameWidth) / double(GameHeight);

int MonitorWidth = 1280;
int MonitorHeight = 720;

void Run();
void InitializeSDL();
void Update(float DeltaTime);
void PollEvent(SDL_Event Event);

void Backend::StartGame()
{

	std::thread MainThread(Run);
	MainThread.join();

}

void Backend::CloseGame()
{

	Running = false;

}

void InitializeSDL()
{

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{

		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;

		Running = false;

	}

	Window = SDL_CreateWindow(ProjectName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MonitorWidth, MonitorHeight, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE);

	if (Window == nullptr)
	{

		SDL_Quit();

		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;

		Running = false;

	}

	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);

	if (Renderer == nullptr)
	{

		SDL_DestroyWindow(Window);
		SDL_Quit();

		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;

		Running = false;

	}

}

void Run()
{

	InitializeSDL();

	auto LastFrameTime = std::chrono::high_resolution_clock::now();

	double TenToTheNine = 1000000000;

	std::chrono::duration<double> FrameTime = std::chrono::nanoseconds(long(TenToTheNine / float(FrameRate)));

	while (Running)
	{

		auto CurrentFrameTime = std::chrono::high_resolution_clock::now();

		auto DeltaTime = CurrentFrameTime - LastFrameTime;

		if (DeltaTime >= FrameTime)
		{

			LastFrameTime = CurrentFrameTime;

			Update(float(DeltaTime.count()) / float(TenToTheNine));

		}

	}

	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();

}

void Update(float DeltaTime)
{

	Input::ClearInputs();

	SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{

		PollEvent(Event);

	}

	SDL_Texture* GameTexture = SDL_CreateTexture(Renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, GameWidth, GameHeight);
	SDL_SetRenderTarget(Renderer, GameTexture);

	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	// Begin Draw

	SDL_Rect Rectangle = {0, 0, GameWidth, GameHeight - 1};
	if (Input::KeyDown('e')) Rectangle = { 0, 0, GameWidth, GameHeight / 2};

	SDL_SetRenderDrawColor(Renderer, 0, 255, 255, 255);

	SDL_RenderFillRect(Renderer, &Rectangle);

	// End Draw

	SDL_RenderCopy(Renderer, GameTexture, NULL, NULL);
	SDL_SetRenderTarget(Renderer, NULL);

	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	// Resize Texture to Monitor

	double MonitorAspectRatio = double(MonitorWidth) / double(MonitorHeight);

	if (std::abs(GameAspectRatio - MonitorAspectRatio) <= 0.000000001) // If the aspect ratios are the same, render without stretching
	{

		SDL_RenderCopy(Renderer, GameTexture, NULL, NULL);

	}
	else if (MonitorAspectRatio > GameAspectRatio) // Vertical Black Bars
	{

		int Width = int((double(MonitorHeight) * GameAspectRatio));
		SDL_Rect RenderRectangle = {(MonitorWidth / 2) - (Width / 2), 0, Width, MonitorHeight};
		SDL_RenderCopy(Renderer, GameTexture, NULL, &RenderRectangle);

	}
	else // Horizontal Black Bars
	{

		int Height = int((double(MonitorWidth) / GameAspectRatio));
		SDL_Rect RenderRectangle = {0, (MonitorHeight / 2) - (Height / 2), MonitorWidth, Height};
		SDL_RenderCopy(Renderer, GameTexture, NULL, &RenderRectangle);

	}

	SDL_RenderPresent(Renderer);

}

void PollEvent(SDL_Event Event)
{

	switch (Event.type)
	{

		case SDL_KEYDOWN :

			Input::DownEvent(Event.key.keysym.sym);

			break;

		case SDL_KEYUP :

			Input::UpEvent(Event.key.keysym.sym);

			break;

		case SDL_WINDOWEVENT :

			switch (Event.window.event)
			{

				case SDL_WINDOWEVENT_RESIZED :

					MonitorWidth = Event.window.data1;
					MonitorHeight = Event.window.data2;

					break;

				case SDL_WINDOWEVENT_CLOSE :

					Backend::CloseGame();

					break;

			}

			break;

	}

}