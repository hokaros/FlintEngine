#include "DebugRenderer.h"
#include "Window.h"
#include "utility.h"
#include <Rendering/SceneRenderer.h>

DebugRenderer* DebugRenderer::s_Instance = nullptr;

DebugRenderer::DebugRenderer(SceneRenderer& renderer)
	: m_Renderer(renderer)
{
	s_Instance = this;
}

void DebugRenderer::DrawLine(const Vector& start, const Vector& end, const Rgb8& color)
{
	s_Instance->DrawLineImpl(start, end, color);
}

void DebugRenderer::DrawRect(const Rect& rect, const Rgb8& color)
{
	s_Instance->DrawRectImpl(rect, color);
}

void DebugRenderer::DrawWireRect(const Rect& rect, const Rgb8& color)
{
	s_Instance->DrawWireRectImpl(rect, color);
}

void DebugRenderer::DrawLineImpl(const Vector& start, const Vector& end, const Rgb8& color)
{
	m_Renderer.RenderLine(start, end, color, rendering::LayerId::DEBUG_FIRST_PLAN);
}

void DebugRenderer::DrawRectImpl(const Rect& rect, const Rgb8& color)
{
	m_Renderer.RenderRect(rect, color, rendering::LayerId::DEBUG_FIRST_PLAN);
}

void DebugRenderer::DrawWireRectImpl(const Rect& rect, const Rgb8& color)
{
	m_Renderer.RenderWireRect(rect, color, rendering::LayerId::DEBUG_FIRST_PLAN);
}
