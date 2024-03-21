#include "Widgets.h"

void ModalPrompt::Open()
{
	m_ShouldOpenOnUpdate = true;
}

void ModalPrompt::Update()
{
	if (m_ShouldOpenOnUpdate)
	{
		PreOpen();
		ImGui::OpenPopup(GetModalId());
		m_ShouldOpenOnUpdate = false;
	}

	if (ImGui::BeginPopupModal(GetModalId(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		FillModal();

		ImGui::EndPopup();
	}
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

void ModalStringPrompt::SetAcceptCallback(const char* accept_btn_name, std::function<void(std::string)> on_accepted)
{
	m_AcceptBtnName = accept_btn_name;
	m_OnAccepted = on_accepted;
}

void ModalStringPrompt::SetLabel(const std::string& label)
{
	m_Label = label;
}

void ModalStringPrompt::PreOpen()
{
	// Clear
	m_Buffer[0] = '\0';
}

void ModalStringPrompt::FillModal()
{
	ImGui::InputText(m_Label.c_str(), m_Buffer, s_BufferSize);

	if (ImGui::Button(m_AcceptBtnName.c_str()))
	{
		ImGui::CloseCurrentPopup();
		m_OnAccepted(m_Buffer);
	}

	if (ImGui::Button("Cancel"))
	{
		ImGui::CloseCurrentPopup();
	}
}

ModalChoicePrompt::ModalChoicePrompt(const char* modal_id)
	: ModalPrompt(modal_id)
{
}

void ModalChoicePrompt::AddChoice(const char* name, std::function<void()> on_selected)
{
	m_Choices.emplace_back(name, on_selected);
}

void ModalChoicePrompt::AddChoice(const std::string& name, std::function<void()> on_selected)
{
	m_Choices.emplace_back(name, on_selected);
}

void ModalChoicePrompt::FillModal()
{
	for (const Choice& choice : m_Choices)
	{
		if (ImGui::Button(choice.m_Name.c_str(), ImVec2(200, 30)))
		{
			ImGui::CloseCurrentPopup();
			choice.m_OnSelected();
		}
	}

	if (ImGui::Button("Cancel"))
	{
		ImGui::CloseCurrentPopup();
	}
}

ModalChoicePrompt::Choice::Choice(const std::string& name, std::function<void()> on_selected)
	: m_Name(name)
	, m_OnSelected(on_selected)
{
}