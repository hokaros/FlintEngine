#pragma once
#include <imgui/imgui.h>
#include <Math/Rect.h>

Vector ImVecToVec(ImVec2 imVec); // TODO: move to helpers

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

