#include "IEditableGameObject.h"

void IEditableGameObject::CopyChildrenToRuntimeObject(const IEditableGameObject& src, GameObject& dest)
{
	for (const std::unique_ptr<IEditableGameObject>& editable_child : src.GetChildren())
	{
		std::unique_ptr<GameObject> runtime_child = std::make_unique<GameObject>(editable_child->GetResult());

		CopyChildrenToRuntimeObject(*editable_child, *runtime_child);

		dest.AddChild(std::move(runtime_child));
	}
}