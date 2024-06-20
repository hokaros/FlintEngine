#include "IEditableGameObject.h"

void IEditableGameObject::CopyChildrenToRuntimeObject(const IEditableGameObject& src, GameObject& dest)
{
	for (const std::unique_ptr<IGameObject>& child : src.GetChildren())
	{
		const IEditableGameObject* editable_child = dynamic_cast<const IEditableGameObject*>(child.get());
		// TODO: don't require IEditableGameObject here
		std::unique_ptr<GameObject> runtime_child = std::make_unique<GameObject>(editable_child->GetResult_Depr());

		CopyChildrenToRuntimeObject(*editable_child, *runtime_child);

		dest.AddChild(std::move(runtime_child));
	}
}