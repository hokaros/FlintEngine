#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include <iostream>
#include <functional>


class ModalPrompt
{
public:
	void Open(); // TODO: open context
	const char* GetModalId() const;
	void SetModalId(const std::string& new_id);

protected:
	ModalPrompt(const char* modal_id);

	bool m_ShouldOpenOnUpdate = false;

private:
	std::string m_ModalId;
};

class ModalStringPrompt
	: public ModalPrompt
{
public:
	ModalStringPrompt(const char* modal_id, const char* label);

	void SetAcceptCallback(const char* accept_btn_name, std::function<void(std::string)> on_accepted);

	void Update();

	void SetLabel(const std::string& label);

private:
	static constexpr size_t s_BufferSize = 256;

	char m_Buffer[s_BufferSize];
	std::string m_Label;

	std::string m_AcceptBtnName = "Accept";
	std::function<void(std::string)> m_OnAccepted;
};




class ModalChoicePrompt
	: public ModalPrompt
{
public:
	ModalChoicePrompt(const char* modal_id);

	void AddChoice(const char* name, std::function<void()> on_selected);
	void AddChoice(const std::string& name, std::function<void()> on_selected);

	void Update();

private:
	struct Choice
	{
		Choice(const std::string& name, std::function<void()> on_selected);

		std::string m_Name;
		std::function<void()> m_OnSelected;
	};

private:
	std::vector<Choice> m_Choices;
};
