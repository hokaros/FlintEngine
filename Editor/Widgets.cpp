#include "Widgets.h"

void ModalPrompt::Open()
{
	ImGui::OpenPopup(m_ModalId.c_str());
}

const char* ModalPrompt::GetModalId() const
{
	return m_ModalId.c_str();
}

void ModalPrompt::SetModalId(const std::string& new_id)
{
	m_ModalId = new_id;
}

ModalPrompt::ModalPrompt(const char* modal_id)
	: m_ModalId(modal_id)
{
}


ModalStringPrompt::ModalStringPrompt(const char* modal_id, const char* label)
	: ModalPrompt(modal_id)
	, m_Label(label)
{
}

bool ModalStringPrompt::Update(std::string& response)
{
	bool has_accepted = false;

	if (ImGui::BeginPopupModal(GetModalId(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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

void ModalStringPrompt::SetLabel(const std::string& label)
{
	m_Label = label;
}

void ModalChoicePrompt::AddChoice(const std::string& name, std::function<void()> on_selected)
{
	m_Choices.emplace_back(name, on_selected);
}

ModalChoicePrompt::Choice::Choice(const std::string& name, std::function<void()> on_selected)
	: m_Name(name)
	, m_OnSelected(on_selected)
{
}