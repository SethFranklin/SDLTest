
#pragma once

#include <SDL.h>

namespace Input
{

	typedef int KeyCode;

	void ClearInputs(); // Only used by backend
	void DownEvent(KeyCode);
	void UpEvent(KeyCode);

	bool KeyDown(KeyCode);
	bool KeyPressed(KeyCode);
	bool KeyReleased(KeyCode);

}
