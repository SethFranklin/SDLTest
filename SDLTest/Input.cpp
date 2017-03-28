
#include <SDL.h>
#include <vector>

#include "Input.h"

std::vector<Input::KeyCode> PressedKeys;
std::vector<Input::KeyCode> HeldKeys;
std::vector<Input::KeyCode> ReleasedKeys;

std::vector<Input::KeyCode> PressedButtons;
std::vector<Input::KeyCode> HeldButtons;
std::vector<Input::KeyCode> ReleasedButtons;

bool StringInVector(std::vector<Input::KeyCode>, Input::KeyCode);

void Input::ClearInputs()
{

	PressedKeys.clear();
	ReleasedKeys.clear();
	PressedButtons.clear();
	ReleasedButtons.clear();

}

void Input::DownEventK(Input::KeyCode Key)
{

	if (!StringInVector(HeldKeys, Key))
	{

		PressedKeys.push_back(Key);
		HeldKeys.push_back(Key);

	}

}

void Input::UpEventK(Input::KeyCode Key)
{

	ReleasedKeys.push_back(Key);
	HeldKeys.erase(std::find(HeldKeys.begin(), HeldKeys.end(), Key));

}

void Input::DownEventJ(Input::KeyCode Button)
{

	if (!StringInVector(HeldButtons, Button))
	{

		PressedButtons.push_back(Button);
		HeldButtons.push_back(Button);

	}

}

void Input::UpEventJ(Input::KeyCode Button)
{

	ReleasedButtons.push_back(Button);
	HeldButtons.erase(std::find(HeldButtons.begin(), HeldButtons.end(), Button));

}

bool Input::KeyDown(Input::KeyCode Key)
{

	return (StringInVector(HeldKeys, Key));

}

bool Input::KeyPressed(Input::KeyCode Key)
{

	return (StringInVector(PressedKeys, Key));

}

bool Input::KeyReleased(Input::KeyCode Key)
{

	return (StringInVector(ReleasedKeys, Key));

}

bool Input::ButtonDown(Input::KeyCode Button)
{

	return (StringInVector(HeldButtons, Button));

}

bool Input::ButtonPressed(Input::KeyCode Button)
{

	return (StringInVector(PressedButtons, Button));

}

bool Input::ButtonReleased(Input::KeyCode Button)
{

	return (StringInVector(ReleasedButtons, Button));

}

bool StringInVector(std::vector<Input::KeyCode> VectorToSearch, Input::KeyCode StringToFind)
{

	return (std::find(VectorToSearch.begin(), VectorToSearch.end(), StringToFind) != VectorToSearch.end());

}