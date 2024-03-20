#include "Widgets.h"

void ModalPrompt::Open()
{
	m_ShouldOpenOnUpdate = true;
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
	if (m_ShouldOpenOnUpdate)
	{
		ImGui::OpenPopup(GetModalId());
		m_ShouldOpenOnUpdate = false;
	}

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

void ModalChoicePrompt::Update()
{
	if (m_ShouldOpenOnUpdate)
	{
		ImGui::OpenPopup(GetModalId());
		m_ShouldOpenOnUpdate = false;
	}

	// TODO: after we simplify ModalStringPrompt, we can unify this
	if (ImGui::BeginPopupModal(GetModalId(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
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

		ImGui::EndPopup();
	}
}

ModalChoicePrompt::Choice::Choice(const std::string& name, std::function<void()> on_selected)
	: m_Name(name)
	, m_OnSelected(on_selected)
{
}