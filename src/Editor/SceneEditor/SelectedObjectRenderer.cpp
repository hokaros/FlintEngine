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
	constexpr float viewport_size_to_axis_length = 1 / 50.0f;

	const Vector viewport_size = renderer.GetViewport().size;
	const float x_axis_length = viewport_size.x * viewport_size_to_axis_length;
	const float y_axis_length = viewport_size.y * viewport_size_to_axis_length;

	ITransformable& transformable = game_object.GetTransformable();

	const Vector object_pos = transformable.GetWorldPosition();

	{
		const Vector object_local_x = transformable.VectorLocalToWorld(Vector(1, 0)).Normalize();
		const Vector x_axis_end = object_pos + object_local_x * x_axis_length;
		renderer.RenderLine(object_pos, x_axis_end, X_AXIS_COLOR, rendering::LayerId::EDITOR_SELECTION);
	}

	{
		const Vector object_local_y = transformable.VectorLocalToWorld(Vector(0, 1)).Normalize();
		const Vector y_axis_end = object_pos + object_local_y * y_axis_length;
		renderer.RenderLine(object_pos, y_axis_end, Y_AXIS_COLOR, rendering::LayerId::EDITOR_SELECTION);
	}
}
