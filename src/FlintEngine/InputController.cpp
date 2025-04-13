#include "InputController.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"

#include <FTL/VectorOperations.h>

IInputController* IInputController::s_Main{ nullptr };

InputController::InputController()
{
	if (s_Main == nullptr) 
	{
		s_Main = this;
	}
}

InputController::~InputController() 
{
	if (s_Main == this) 
	{
		s_Main = nullptr;
	}
}

bool InputController::Update() 
{
	ClearFrameInfo();

	SDL_Event event;
	bool quit = false;

	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		{
			SDL_Keycode keycode = event.key.keysym.sym;
			if (event.type == SDL_KEYDOWN)
			{
				OnKeyDown(keycode);
			}
			else if (event.type == SDL_KEYUP)
			{
				OnKeyUp(keycode);
			}
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP)
		{
			const MouseButton button = MouseButtonIdToEnum(event.button.button);
			if (button == MouseButton::Unknown)
				continue;

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				OnMouseButtonDown(button);
			}
			else if (event.type == SDL_MOUSEBUTTONUP)
			{
				OnMouseButtonUp(button);
			}
		}
	};

	return !quit;
}

bool InputController::IsKeyDown(SDL_Keycode key) const 
{
	auto it = m_KeyDownInfo.find(key);
	if (it == m_KeyDownInfo.end())
		return false;

	return it->second;
}

bool InputController::PressedThisFrame(SDL_Keycode key) const 
{
	return ftl::vector_contains(m_PressedThisFrame, key);
}

bool InputController::MouseButtonPressedThisFrame(MouseButton button) const
{
	return ftl::vector_contains(m_MousePressedThisFrame, button);
}

Vector InputController::GetMousePosition() const 
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	return Vector(x, y);
}

void InputController::OnKeyDown(SDL_Keycode key)
{
	m_PressedThisFrame.push_back(key);
	
	// Update keydown status
	auto it = m_KeyDownInfo.find(key);
	if (it == m_KeyDownInfo.end())
	{
		m_KeyDownInfo.insert({ key, true });
	}
	else
	{
		it->second = true;
	}
}

void InputController::OnKeyUp(SDL_Keycode key)
{
	auto it = m_KeyDownInfo.find(key);
	if (it == m_KeyDownInfo.end())
		return; // No entry

	it->second = false;
}

void InputController::OnMouseButtonDown(MouseButton button)
{
	m_MousePressedThisFrame.push_back(button);
}

void InputController::OnMouseButtonUp(MouseButton button)
{
}

void InputController::ClearFrameInfo()
{
	m_PressedThisFrame.clear();
	m_MousePressedThisFrame.clear();
}

MouseButton InputController::MouseButtonIdToEnum(uint8_t button_id)
{
	if (button_id == 1)
		return MouseButton::Left;
	if (button_id == 2)
		return MouseButton::Middle;
	if (button_id == 3)
		return MouseButton::Right;

	return MouseButton::Unknown;
}

const IInputController* IInputController::Main()
{
	return s_Main;
}
