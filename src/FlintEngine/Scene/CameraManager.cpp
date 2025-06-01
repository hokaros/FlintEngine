#include "CameraManager.h"

namespace scene
{
	Camera* CameraManager::GetCurrentCamera()
	{
		return m_CurrentCamera;
	}

	void CameraManager::RegisterCamera(Camera& camera)
	{
		if (m_CurrentCamera == nullptr)
		{
			m_CurrentCamera = &camera;
		}
	}

	void CameraManager::UnregisterCamera(Camera& camera)
	{
		if (m_CurrentCamera == &camera)
		{
			m_CurrentCamera = nullptr;
		}
	}
}