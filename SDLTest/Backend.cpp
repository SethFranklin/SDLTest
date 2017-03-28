
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>

#include <SDL.h>
#include <glew.h>

#include "Backend.h"
#include "Input.h"

const char* ProjectName = "SDLTest";
const bool Debug = true;

bool Running = true;

SDL_Window* Window;
SDL_GLContext GLContext;

GLuint FrameBuffer;
GLuint GameTexture;
GLuint DepthBuffer;

SDL_Joystick* Joystick;

int FrameRate = 60;

const int GameWidth = 256;
const int GameHeight = 224;

double GameAspectRatio = double(GameWidth) / double(GameHeight);

int MonitorWidth = 1280;
int MonitorHeight = 720;

void Run();
void InitializeSDL();
void Update(float DeltaTime);
void PollEvent(SDL_Event Event);
void IntiializeFrameBuffer();

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

	if (Debug) freopen("output.txt", "w", stdout);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0)
	{

		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;

		Running = false;

	}

	Window = SDL_CreateWindow(ProjectName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, MonitorWidth, MonitorHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (Window == nullptr)
	{

		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;

		Running = false;

	}

	GLContext = SDL_GL_CreateContext(Window);

	glewExperimental = GL_TRUE;
	glewInit();

	glEnable(GL_DEPTH_TEST);

	Joystick = SDL_JoystickOpen(0);

}

void Run()
{

	InitializeSDL();
	IntiializeFrameBuffer();

	auto LastFrameTime = std::chrono::high_resolution_clock::now();

	double TenToTheNine = 1000000000;

	std::chrono::duration<double> FrameTime = std::chrono::nanoseconds(long(TenToTheNine / float(FrameRate)));

	while (Running)
	{

		auto CurrentFrameTime = std::chrono::high_resolution_clock::now();

		auto DeltaTime = CurrentFrameTime - LastFrameTime;

		if (DeltaTime >= FrameTime)
		{

			Update(float(DeltaTime.count()) / float(TenToTheNine));

			LastFrameTime = std::chrono::high_resolution_clock::now();

		}

	}

	SDL_GL_DeleteContext(GLContext);
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

	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	glViewport(0, 0, GameWidth, GameHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, GameWidth, GameHeight, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Begin Draw

	glBegin(GL_QUADS);

	glColor3f(0.7f, 0.f, 0.f);

	glVertex2i(0, 0);
	glVertex2i(GameWidth, 0);
	if (Input::KeyDown('e')) glVertex2i(GameWidth, GameHeight / 2);
	else glVertex2i(GameWidth, GameHeight);
	glColor3f(0.7f, 0.7f, 0.7f);

	glVertex2i(0, GameHeight);

	glEnd();


	// End Draw

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	double MonitorAspectRatio = double(MonitorWidth) / double(MonitorHeight);

	if (std::abs(GameAspectRatio - MonitorAspectRatio) <= 0.000000001) // If the aspect ratios are the same, render without stretching
	{

		glViewport(0, 0, MonitorWidth, MonitorHeight);

	}
	else if (MonitorAspectRatio > GameAspectRatio) // Vertical Black Bars
	{

		int Width = int((double(MonitorHeight) * GameAspectRatio));
		glViewport((MonitorWidth / 2) - (Width / 2), 0, Width, MonitorHeight);

	}
	else // Horizontal Black Bars
	{

		int Height = int((double(MonitorWidth) / GameAspectRatio));
		glViewport(0, (MonitorHeight / 2) - (Height / 2), MonitorWidth, Height);

	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, GameTexture);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex2i(-1, -1);
	glTexCoord2i(0, 1); glVertex2i(-1, 1);
	glTexCoord2i(1, 1); glVertex2i(1, 1);
	glTexCoord2i(1, 0); glVertex2i(1, -1);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	SDL_GL_SwapWindow(Window);

}

void PollEvent(SDL_Event Event)
{

	switch (Event.type)
	{

		case SDL_KEYDOWN :

			Input::DownEventK(Event.key.keysym.sym);

			break;

		case SDL_KEYUP :

			Input::UpEventK(Event.key.keysym.sym);

			break;

		case SDL_JOYBUTTONDOWN :

			Input::DownEventJ(Event.jbutton.button);

			break;

		case SDL_JOYBUTTONUP :

			Input::UpEventJ(Event.jbutton.button);

			break;

		case SDL_JOYAXISMOTION :



			break;

		case SDL_JOYHATMOTION :



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

void IntiializeFrameBuffer()
{

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	FrameBuffer = 0;
	glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	// The texture we're going to render to
	glGenTextures(1, &GameTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, GameTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, GameWidth, GameHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenRenderbuffers(1, &DepthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, GameWidth, GameHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthBuffer);

	// Set "GameTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GameTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{

		std::cout << "Frame Buffer Initialization Failed" << std::endl;

		Running = false;

	}

}