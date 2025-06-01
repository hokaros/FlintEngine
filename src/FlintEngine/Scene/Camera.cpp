#include "Camera.h"

#include <Core/GameObject.h>
#include <Scene/Scene.h>

DEFINE_COMPONENT(Camera);

DEFINE_FIELD(Camera, m_Size);

Rect Camera::CalculateViewport() const
{
	return Rect::FromCenterAndSize(GetOwner().GetWorldPosition(), m_Size);
}

void Camera::Awake()
{
	GetOwner().GetScene()->GetCameraManager().RegisterCamera(*this);
}

void Camera::OnDestroy()
{
	GetOwner().GetScene()->GetCameraManager().UnregisterCamera(*this);
}

void Camera::RenderEditorSelected(SceneRenderer& renderer)
{
	const Rgb8 yellow = Rgb8(0xf5, 0xec, 0x42);
	const Rect rect = CalculateViewport();
	renderer.RenderWireRect(rect, yellow, rendering::LayerId::DEBUG_SCREENSPACE);
}
