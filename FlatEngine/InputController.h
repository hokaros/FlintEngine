#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <SDL.h>
#include "Vector.h"

class InputController
{
public:
	InputController();
	~InputController();

	// Aktualizuje statusy klawiszy. Zwraca false, jeœli zamkniêto okno
	bool Update();
	bool IsKeyDown(SDL_Keycode key) const;
	bool PressedThisFrame(SDL_Keycode key) const;

	Vector GetMousePosition() const;

	static InputController* Main();

private:
	void OnKeyDown(SDL_Keycode key);
	void OnKeyUp(SDL_Keycode key);
	void ClearFrameInfo();

private:
	std::vector<SDL_Keycode> m_PressedThisFrame;
	std::unordered_map<SDL_Keycode, bool> m_KeyDownInfo;

	static InputController* s_Main;
};

