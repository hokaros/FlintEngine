#include "InputController.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"

#include "ftl.h"

InputController* InputController::s_Main{ nullptr };

InputController* InputController::Main() {
	return s_Main;
}

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

		if (event.type != SDL_KEYDOWN && event.type != SDL_KEYUP)
			continue; //not my business

		SDL_Keycode keycode = event.key.keysym.sym;
		if (event.type == SDL_KEYDOWN)
		{
			OnKeyDown(keycode);
		}
		else if (event.type == SDL_KEYUP)
		{
			OnKeyUp(keycode);
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

void InputController::ClearFrameInfo()
{
	m_PressedThisFrame.clear();
}
