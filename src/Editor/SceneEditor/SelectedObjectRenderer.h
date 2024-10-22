#pragma once
#include <SelectedObjectManager.h>

class SelectedObjectRenderer
{
public:
	static void Render(SelectedObjectManager& selection_manager, SceneRenderer& renderer);

private:
	static void RenderGameObject(GameObject& game_object, SceneRenderer& renderer);
	static void RenderAxes(GameObject& game_object, SceneRenderer& renderer);

private:
	static constexpr uint SELECTION_RENDER_LAYER = UINT32_MAX;
	static constexpr Rgb8 X_AXIS_COLOR = Rgb8(0xFF, 0x00, 0x00);
	static constexpr Rgb8 Y_AXIS_COLOR = Rgb8(0x00, 0xFF, 0x00);
};