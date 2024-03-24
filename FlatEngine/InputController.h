#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <SDL.h>
#include "Vector.h"

class IInputController
{
public:
	virtual bool Update() = 0; // Aktualizuje statusy klawiszy. Zwraca false, jeœli zamkniêto okno

	virtual bool IsKeyDown(SDL_Keycode key) const = 0;
	virtual bool PressedThisFrame(SDL_Keycode key) const = 0;
	virtual Vector GetMousePosition() const = 0;

	virtual ~IInputController() = default;

	static const IInputController* Main();

protected:
	static IInputController* s_Main;
};

class InputController
	: public IInputController
{
public:
	InputController();
	~InputController();

	virtual bool Update() override;

	virtual bool IsKeyDown(SDL_Keycode key) const override;
	virtual bool PressedThisFrame(SDL_Keycode key) const override;
	virtual Vector GetMousePosition() const override;

private:
	void OnKeyDown(SDL_Keycode key);
	void OnKeyUp(SDL_Keycode key);
	void ClearFrameInfo();

private:
	std::vector<SDL_Keycode> m_PressedThisFrame;
	std::unordered_map<SDL_Keycode, bool> m_KeyDownInfo;
};

