
#include <SDL.h>
#include <vector>

#include "Input.h"

std::vector<Input::KeyCode> PressedKeys;
std::vector<Input::KeyCode> HeldKeys;
std::vector<Input::KeyCode> ReleasedKeys;

bool StringInVector(std::vector<Input::KeyCode> VectorToSearch, Input::KeyCode StringToFind);

void Input::ClearInputs()
{

	PressedKeys.clear();
	ReleasedKeys.clear();

}

void Input::DownEvent(Input::KeyCode Key)
{

	if (!StringInVector(HeldKeys, Key))
	{

		PressedKeys.push_back(Key);
		HeldKeys.push_back(Key);

	}

}

void Input::UpEvent(Input::KeyCode Key)
{

	ReleasedKeys.push_back(Key);
	HeldKeys.erase(std::find(HeldKeys.begin(), HeldKeys.end(), Key));

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

bool StringInVector(std::vector<Input::KeyCode> VectorToSearch, Input::KeyCode StringToFind)
{

	return (std::find(VectorToSearch.begin(), VectorToSearch.end(), StringToFind) != VectorToSearch.end());

}