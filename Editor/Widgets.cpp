#include "Widgets.h"

ModalStringPrompt::ModalStringPrompt(const char* modal_id, const char* label)
	: m_ModalId(modal_id)
	, m_Label(label)
{
}

void ModalStringPrompt::Open()
{
	ImGui::OpenPopup(m_ModalId.c_str());
}

const char* ModalStringPrompt::GetModalId() const
{
	return m_ModalId.c_str();
}

bool ModalStringPrompt::Update(std::string& response)
{
	bool has_accepted = false;

	if (ImGui::BeginPopupModal(m_ModalId.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::InputText(m_Label.c_str(), m_Buffer, s_BufferSize);

		if (ImGui::Button("Add"))
		{
			response = m_Buffer;
			has_accepted = true;

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	return has_accepted;
}
