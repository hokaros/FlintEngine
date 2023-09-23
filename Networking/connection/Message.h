#pragma once
#include <iostream>
#include <vector>

template<typename T>
struct MessageHeader {
	T id{};
	uint32_t size = 0;
};

template<typename T>
struct Message {
	MessageHeader<T> header{};
	std::vector<uint8_t> body;

	Message() {
		header.size = size();
	}

	size_t size() {
		return sizeof(MessageHeader<T>) + body.size();
	}

	friend std::ostream& operator<<(std::ostream& out, Message& message) {
		out << "Id: " << message.header.id << ", size: " << message.header.size;
		return out;
	}

	template<typename DataType>
	friend Message& operator<<(Message& message, const DataType& newData) {
		// check if the data is proper
		static_assert(std::is_standard_layout<DataType>::value, "The data is too complex");

		// copy data
		size_t s = message.body.size(); // get the starting size
		message.body.resize(s + sizeof(DataType)); // update size
		std::memcpy(message.body.data() + s, &newData, sizeof(DataType));

		// update the header
		message.header.size = message.size();

		return message;
	}


	template<typename DataType>
	friend Message<T>& operator >> (Message<T>& message, DataType& data)
	{
		static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pulled from vector");

		size_t i = message.body.size() - sizeof(DataType);
		std::memcpy(&data, message.body.data() + i, sizeof(DataType));
		message.body.resize(i);

		message.header.size = message.size();

		return message;
	}
};