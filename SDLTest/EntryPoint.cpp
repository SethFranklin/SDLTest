
#include "backend.h"
#include <Windows.h>

// Linux

/*

int main(int argc, char* argv[])
{

	Backend::StartGame();

	return 0;

}

*/

// Windows

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	Backend::StartGame();

	return 0;

}