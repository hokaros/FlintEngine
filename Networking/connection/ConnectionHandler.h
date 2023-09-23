#pragma once
#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include <iostream>
#include "Message.h"
#include "TSQueue.h"
#define BUFFER_SIZE 10240

template<typename T>
class ConnectionHandler
{
public:
	// Zdarzenia
	std::function<void(int connectionId, const Message<T>& message)> onMessageReceived;
	std::function<void(int connectionId)> onDisconnected;

	ConnectionHandler(int id, asio::io_context& context, asio::ip::tcp::socket socket)
		: id(id), context(context), socket(std::move(socket)), incomingBuffer(BUFFER_SIZE), createBuffer(0), currentHeader(NULL) {

	}

	~ConnectionHandler() {
		socket.close();
		delete currentHeader;
	}

	void OpenInput() {
		if (!socket.is_open()) {
			if (onDisconnected)
				onDisconnected(id);
			return;
		}

		socket.async_read_some(asio::buffer(incomingBuffer.data(), incomingBuffer.size()),
			[&](asio::error_code ec, size_t length) {
				if (!ec) {
					OnRead(length);
					OpenInput(); // czytaj dalej
				}
			});
	}

	void WriteMessage(const Message<T>& msg) {
		asio::post(context,
			[this, msg]() {
				outMessages.PushBack(msg);
				if (!isSending)
					SendMessages();
			});
	}

	bool IsConnected() const {
		return socket.is_open();
	}

	int GetId() const {
		return id;
	}
private:
	int id;

	asio::io_context& context;
	asio::ip::tcp::socket socket;
	std::vector<char> incomingBuffer;

	std::vector<char> createBuffer;
	bool onGoingMessage = false;
	size_t createdSize = 0;
	MessageHeader<T>* currentHeader;

	TSQueue<Message<T>> outMessages;
	bool isSending = false;
private:
	MessageHeader<T>* ReadHeader() {
		// Przekopiowanie nag³ówka
		size_t headerSize = sizeof(MessageHeader<T>);
		MessageHeader<T>* header = (MessageHeader<T>*) new char[headerSize];
		std::memcpy(header, incomingBuffer.data(), headerSize);

		// Przygotowanie miejsca na wiadomoœæ
		createBuffer.resize(header->size);

		onGoingMessage = true;

		return header;
	}

	// Events
	void OnRead(size_t length) {
		if (!onGoingMessage) {
			currentHeader = ReadHeader();
		}

		// Dostawienie czêœci wiadomoœci
		std::memcpy(createBuffer.data() + createdSize, incomingBuffer.data(), length);
		createdSize += length;

		if (createdSize == createBuffer.size()) {
			OnMessageGot();
		}
	}

	void OnMessageGot() {
		// Odczytanie cia³a wiadomoœci
		size_t bodySize = createBuffer.size() - sizeof(MessageHeader<T>);
		std::vector<char> tempBody(bodySize);
		std::memcpy(tempBody.data(), createBuffer.data() + sizeof(MessageHeader<T>), bodySize);
		// Z³o¿enie w ca³oœæ
		Message<T> message;
		message.header.id = currentHeader->id;
		message.header.size = currentHeader->size;

		message.body.resize(bodySize);
		std::memmove(message.body.data(), tempBody.data(), bodySize);

		// Reset
		createBuffer.resize(0);
		createdSize = 0;
		onGoingMessage = false;

		std::cout << "Message successfully received" << std::endl;
		// Callback
		if (onMessageReceived)
			onMessageReceived(id, message);

		delete currentHeader;
		currentHeader = NULL;
	}

	void SendMessages() {
		if (outMessages.IsEmpty()) {
			isSending = false;
			return;
		}

		isSending = true;

		Message<T> msg = outMessages.PopFront();
		asio::async_write(socket, asio::buffer(&msg.header, sizeof(msg.header)),
			[this, msg](std::error_code ec, size_t length) {
				if (!ec) {
					if (msg.body.size() > 0) {
						// Wys³anie cia³a wiadomoœci
						asio::async_write(socket, asio::buffer(msg.body.data(), msg.body.size()),
							[this, msg](std::error_code ec, size_t length) {
								SendMessages();
							});
					}
					else {
						SendMessages();
					}
				}
				else {
					SendMessages();
				}
			});
	}
};




enum class WildMessage {
	KEYS,
	LOCK_MOUSE,
	CLIPBOARD_CHANGE,
	WRITE,
	PLAYER_DEATH,
	WALL_DESTRUCTION,
	LABIRYNTH_CHANGE,
	END_OF_GAME,
	POSITION,
	NEW_DIRECTION,
	WEAPON_CHANGE,
	CHANGE_OF_AIMING_DIRECTION,
	SHOT,
	JOIN_REQUEST,
	JOIN_ACCEPT,
	JOIN_DENIED,
	PLAYER_JOINED,
	PLAYER_LEFT,
	GAME_STARTED
};
