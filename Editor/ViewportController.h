#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/Vector.h"

class ViewportController
{
public:
	void Update(Rect& viewport);

private:
	void ProcessTranslation(Rect& viewport);
	void ProcessZooming(Rect& viewport);

	Vector GetMoveDirection();
	float GetZoomMultiplier();
};

