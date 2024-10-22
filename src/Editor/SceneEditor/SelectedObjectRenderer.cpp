#include "SelectedObjectRenderer.h"

void SelectedObjectRenderer::Render(SelectedObjectManager& selection_manager, SceneRenderer& renderer)
{
	std::shared_ptr<EditorUniversalHandle> selected_object = selection_manager.GetSelectedObject().lock();
	if (selected_object == nullptr)
		return;

	EditorGameObjectHandle* go_handle = selected_object->GetGameObjectHandle();
	if (go_handle == nullptr)
		return;

	RenderGameObject(go_handle->GetGameObject(), renderer);
}

void SelectedObjectRenderer::RenderGameObject(GameObject& game_object, SceneRenderer& renderer)
{
	game_object.RenderEditorSelected(renderer);
	RenderAxes(game_object, renderer);
}

void SelectedObjectRenderer::RenderAxes(GameObject& game_object, SceneRenderer& renderer)
{
	constexpr float axis_length = 5.0f; // TODO: make this viewport-size-reactive

	ITransformable& transformable = game_object.GetTransformable();

	const Vector object_pos = transformable.GetWorldPosition();

	{
		const Vector object_local_x = transformable.VectorLocalToWorld(Vector(1, 0)).Normalize();
		const Vector x_axis_end = object_pos + object_local_x * axis_length;
		renderer.RenderLine(object_pos, x_axis_end, X_AXIS_COLOR, SELECTION_RENDER_LAYER);
	}

	{
		const Vector object_local_y = transformable.VectorLocalToWorld(Vector(0, 1)).Normalize();
		const Vector y_axis_end = object_pos + object_local_y * axis_length;
		renderer.RenderLine(object_pos, y_axis_end, Y_AXIS_COLOR, SELECTION_RENDER_LAYER);
	}
}
