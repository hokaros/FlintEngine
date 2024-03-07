#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include "../FlatEngine/Vector.h"

class ViewportController
{
public:
	void Update(Rect& viewport);

private:
	Vector GetMoveDirection();
};

