#include "ViewportController.h"

void ViewportController::Update(Rect& viewport)
{
    ProcessTranslation(viewport);
    ProcessZooming(viewport);
}

void ViewportController::ProcessTranslation(Rect& viewport)
{
    constexpr float normalSpeed = 0.01f;
    constexpr float boostedSpeed = 0.1f;
    float speed = normalSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
    {
        speed = boostedSpeed;
    }

    viewport.pos += GetMoveDirection() * speed;
}

void ViewportController::ProcessZooming(Rect& viewport)
{
    // TODO: zooming relative to the mouse pos
    constexpr float normalSpeed = 1.2f;
    constexpr float boostedSpeed = 2.0f;
    float speed = normalSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
    {
        speed = boostedSpeed;
    }

    float wheel = ImGui::GetIO().MouseWheel;

    if (wheel < 0)
    {
        viewport.size = viewport.size * speed;
    }
    else if (wheel > 0)
    {
        viewport.size = viewport.size / speed;
    }
}

Vector ViewportController::GetMoveDirection()
{
    Vector direction = Vector::ZERO;

    if (ImGui::IsKeyDown(ImGuiKey_A) || ImGui::IsKeyDown(ImGuiKey_LeftArrow))
    {
        direction += Vector(Direction::WEST);
    }
    if (ImGui::IsKeyDown(ImGuiKey_D) || ImGui::IsKeyDown(ImGuiKey_RightArrow))
    {
        direction += Vector(Direction::EAST);
    }
    if (ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_UpArrow))
    {
        direction += Vector(Direction::NORTH);
    }
    if (ImGui::IsKeyDown(ImGuiKey_S) || ImGui::IsKeyDown(ImGuiKey_DownArrow))
    {
        direction += Vector(Direction::SOUTH);
    }

    return direction;
}
