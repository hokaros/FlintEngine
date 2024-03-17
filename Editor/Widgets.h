#pragma once
#include "../FlatEngine/imgui/imgui.h"
#include <iostream>

class ModalStringPrompt
{
public:
	ModalStringPrompt(const char* modal_id, const char* label);

	void Open();
	const char* GetModalId() const;
	bool Update(std::string& response);

private:
	static constexpr size_t s_BufferSize = 256;

	char m_Buffer[s_BufferSize];
	std::string m_ModalId;
	std::string m_Label;
};
