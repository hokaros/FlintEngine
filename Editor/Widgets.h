#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include <iostream>
#include <functional>


class ModalPrompt
{
public:
	void Open();
	const char* GetModalId() const;
	void SetModalId(const std::string& new_id);

protected:
	ModalPrompt(const char* modal_id);

private:
	std::string m_ModalId;
};

class ModalStringPrompt
	: public ModalPrompt
{
public:
	ModalStringPrompt(const char* modal_id, const char* label);

	bool Update(std::string& response);

	void SetLabel(const std::string& label);

private:
	static constexpr size_t s_BufferSize = 256;

	char m_Buffer[s_BufferSize];
	std::string m_Label;
};




class ModalChoicePrompt
{
public:
	void AddChoice(const std::string& name, std::function<void()> on_selected);

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
