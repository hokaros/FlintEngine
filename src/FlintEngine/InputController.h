#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <SDL.h>
#include <Math/Vector.h>

enum class MouseButton
{
	Left,
	Right,
	Middle,
	Unknown
};

class IInputController
{
public:
	virtual void ProcessEvent(const SDL_Event& event) = 0;
	virtual void ClearFrameData() = 0;

	virtual bool IsKeyDown(SDL_Keycode key) const = 0;
	virtual bool PressedThisFrame(SDL_Keycode key) const = 0;
	virtual bool MouseButtonPressedThisFrame(MouseButton button) const = 0;
	virtual bool IsWindowClosedThisFrame() const = 0;
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

	virtual void ProcessEvent(const SDL_Event& event) override;
	virtual void ClearFrameData() override;

	virtual bool IsKeyDown(SDL_Keycode key) const override;
	virtual bool PressedThisFrame(SDL_Keycode key) const override;
	virtual bool MouseButtonPressedThisFrame(MouseButton button) const override;
	virtual bool IsWindowClosedThisFrame() const override;

	// Returns mouse position in ViewportSpace
	virtual Vector GetMousePosition() const override;

private:
	void OnKeyDown(SDL_Keycode key);
	void OnKeyUp(SDL_Keycode key);
	void OnMouseButtonDown(MouseButton button);
	void OnMouseButtonUp(MouseButton button);

	static [[nodiscard]] MouseButton MouseButtonIdToEnum(uint8_t button_id);

private:
	std::vector<SDL_Keycode> m_PressedThisFrame;
	std::vector<MouseButton> m_MousePressedThisFrame;
	std::unordered_map<SDL_Keycode, bool> m_KeyDownInfo;
	bool m_IsWindowClosedThisFrame = false;
};

