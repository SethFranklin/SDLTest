
#pragma once

#include <SDL.h>

namespace Input
{

	typedef int KeyCode;

	enum ButtonCode
	{

		ButtonA = 0,
		ButtonB = 1,
		ButtonX = 2,
		ButtonY = 3,
		ButtonLBumper = 4,
		ButtonRBumper = 5,
		ButtonSelect = 6,
		ButtonStart = 7,
		ButtonLeftClick = 8,
		ButtonRightClick = 9,

	};

	void ClearInputs(); // Only used by backend
	void DownEventK(KeyCode);
	void UpEventK(KeyCode);
	void DownEventJ(KeyCode);
	void UpEventJ(KeyCode);

	bool KeyDown(KeyCode);
	bool KeyPressed(KeyCode);
	bool KeyReleased(KeyCode);

	bool ButtonDown(KeyCode);
	bool ButtonPressed(KeyCode);
	bool ButtonReleased(KeyCode);

}
