#include "ViewportController.h"

Vector ImVecToVec(ImVec2 imVec)
{
    return Vector(imVec.x, imVec.y);
}


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
    const float multip = GetZoomMultiplier();

    Vector mousePosScreenSpace = ImVecToVec(ImGui::GetIO().MousePos);
    Vector mousePosOutputSpace = mousePosScreenSpace - ImVecToVec(ImGui::GetCursorScreenPos());

    Vector outputSize = ImVecToVec(ImGui::GetContentRegionAvail());
    Vector viewportSizeToOutputSize = Vector::Divide( viewport.size, outputSize );

    Vector toMouseViewport = Vector::Scale(viewportSizeToOutputSize, mousePosOutputSpace);
    viewport.pos = viewport.pos - (toMouseViewport * multip - toMouseViewport);

    viewport.size *= multip;
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

float ViewportController::GetZoomMultiplier()
{
    constexpr float normalSpeed = 1.2f;
    constexpr float boostedSpeed = 2.0f;
    float speed = normalSpeed;

    if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
    {
        speed = boostedSpeed;
    }

    float wheel = ImGui::GetIO().MouseWheel;
    float multip = 0.0f;
    if (wheel < 0)
    {
        multip = speed;
    }
    else if (wheel > 0)
    {
        multip = 1 / speed;
    }
    else
    {
        multip = 1.0f;
    }

    return multip;
}
