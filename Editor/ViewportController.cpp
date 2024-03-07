#include "ViewportController.h"

void ViewportController::Update(Rect& viewport)
{
    constexpr float normalSpeed = 0.01f;
    constexpr float boostedSpeed = 0.1f;
    float speed = normalSpeed;
    
    if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
    {
        speed = boostedSpeed;
    }
    
    viewport.pos += GetMoveDirection() *speed;
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
