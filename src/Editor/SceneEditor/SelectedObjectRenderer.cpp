#include "SelectedObjectRenderer.h"

void SelectedObjectRenderer::Render(SelectedObjectManager& selection_manager, SceneRenderer& renderer)
{
	std::shared_ptr<EditorUniversalHandle> selected_object = selection_manager.GetSelectedObject().lock();
	if (selected_object == nullptr)
		return;

	EditorGameObjectHandle* go_handle = selected_object->GetGameObjectHandle();
	if (go_handle == nullptr)
		return;

	RenderGameObject(go_handle->GetGameObject().GetResult(), renderer);
}

void SelectedObjectRenderer::RenderGameObject(GameObject& game_object, SceneRenderer& renderer)
{
	constexpr float axis_length = 5.0f; // TODO: make this viewport-size-reactive

	const Vector object_pos = game_object.GetWorldPosition();

	{
		const Vector object_local_x = game_object.VectorLocalToWorld(Vector(1, 0));
		const Vector x_axis_end = object_pos + object_local_x * axis_length;
		renderer.RenderLine(object_pos, x_axis_end, X_AXIS_COLOR, SELECTION_RENDER_LAYER);
	}

	{
		const Vector object_local_y = game_object.VectorLocalToWorld(Vector(0, 1));
		const Vector y_axis_end = object_pos + object_local_y * axis_length;
		renderer.RenderLine(object_pos, y_axis_end, Y_AXIS_COLOR, SELECTION_RENDER_LAYER);
	}
}
