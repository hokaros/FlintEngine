#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/Vector.h"

class ViewportController
{
public:
	void UpdateFromMouse(Rect& viewport);
	void UpdateFromKeyboard(Rect& viewport);

	static Vector ScreenToViewportSpace(const Vector& screen_pos, const Rect& viewport);

private:
	void ProcessTranslation(Rect& viewport);
	void ProcessZooming(Rect& viewport);

	Vector GetMoveDirection();
	float GetZoomMultiplier();
};

