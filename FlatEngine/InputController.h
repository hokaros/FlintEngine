#pragma once
#include <iostream>
#include <SDL.h>
#include "Vector.h"

class InputController
{
public:
	InputController( SDL_Keycode* managedKeys, size_t keysCount);
	~InputController();
	// Aktualizuje statusy klawiszy. Zwraca false, jeœli zamkniêto okno
	bool Update();
	bool IsKeyDown(SDL_Keycode key) const;
	bool PressedThisFrame(SDL_Keycode key) const;

	Vector GetMousePosition() const;

	static InputController* Main();
private:
	size_t keysCount;
	SDL_Keycode* managedKeys;
	bool* keyDownInfo;
	bool* pressedThisFrame;

	static InputController* main;

private:
	int KeyIndex(SDL_Keycode key) const;
	void ClearFrameInfo();
};

