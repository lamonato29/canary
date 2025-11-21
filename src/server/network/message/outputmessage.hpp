/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#include "server/network/message/networkmessage.hpp"
#include "server/network/connection/connection.hpp"

class Protocol;

/**
 * @brief Represents an outgoing network message.
 *
 * Inherits from NetworkMessage and adds functionality for writing headers,
 * managing output buffers, and appending messages.
 */
class OutputMessage : public NetworkMessage {
public:
	OutputMessage() = default;
	virtual ~OutputMessage() = default;

	// non-copyable
	OutputMessage(const OutputMessage &) = delete;
	OutputMessage &operator=(const OutputMessage &) = delete;

	/**
	 * @brief Gets the pointer to the output buffer.
	 *
	 * @return Pointer to the start of the output buffer.
	 */
	uint8_t* getOutputBuffer() {
		return buffer.data() + outputBufferStart;
	}

	/**
	 * @brief Writes padding bytes and header.
	 */
	void writePaddingAmount() {
		uint8_t paddingAmount = 8 - (info.length % 8) - 1;
		addPaddingBytes(paddingAmount);
		add_header(paddingAmount);
	}

	/**
	 * @brief Writes the message length header.
	 */
	void writeMessageLength() {
		add_header(static_cast<uint16_t>((info.length - 4) / 8));
	}

	/**
	 * @brief Adds cryptographic headers to the message.
	 *
	 * @param addChecksum Whether to add a checksum.
	 * @param checksum The checksum value.
	 */
	void addCryptoHeader(bool addChecksum, uint32_t checksum) {
		if (addChecksum) {
			add_header(checksum);
		}

		writeMessageLength();
	}

	/**
	 * @brief Appends another NetworkMessage to this one.
	 *
	 * @param msg The message to append.
	 */
	void append(const NetworkMessage &msg) {
		auto msgLen = msg.getLength();
		std::span<const unsigned char> sourceSpan(msg.getBuffer() + INITIAL_BUFFER_POSITION, msgLen);
		std::span<unsigned char> destSpan(buffer.data() + info.position, msgLen);
		std::ranges::copy(sourceSpan, destSpan.begin());
		info.length += msgLen;
		info.position += msgLen;
	}

	/**
	 * @brief Appends another OutputMessage (via pointer) to this one.
	 *
	 * @param msg Pointer to the message to append.
	 */
	void append(const OutputMessage_ptr &msg) {
		auto msgLen = msg->getLength();
		std::span<const unsigned char> sourceSpan(msg->getBuffer() + INITIAL_BUFFER_POSITION, msgLen);
		std::span<unsigned char> destSpan(buffer.data() + info.position, msgLen);
		std::ranges::copy(sourceSpan, destSpan.begin());
		info.length += msgLen;
		info.position += msgLen;
	}

private:
	template <typename T>
	void add_header(T addHeader) {
		if (outputBufferStart < sizeof(T)) {
			g_logger().error("[{}]: Insufficient buffer space for header!", __FUNCTION__);
			return;
		}

		// Ensure at runtime that outputBufferStart >= sizeof(T)
		assert(outputBufferStart >= sizeof(T));
		// Move the buffer position back to make space for the header
		outputBufferStart -= sizeof(T);

		static_assert(std::is_trivially_copyable_v<T>, "Type T must be trivially copyable");

		// Convert the header to an array of unsigned char using std::bit_cast
		auto byteArray = std::bit_cast<std::array<unsigned char, sizeof(T)>>(addHeader);

		std::span<const unsigned char> byteSpan(byteArray);
		// Copy the bytes into the buffer
		std::ranges::copy(byteSpan, buffer.begin() + outputBufferStart);
		// Update the message size
		info.length += sizeof(T);
	}

	MsgSize_t outputBufferStart = INITIAL_BUFFER_POSITION;
};

/**
 * @brief Manages a pool of OutputMessages and auto-sending protocols.
 *
 * This singleton class handles the buffering and batched sending of output messages.
 */
class OutputMessagePool {
public:
	OutputMessagePool() = default;

	// non-copyable
	OutputMessagePool(const OutputMessagePool &) = delete;
	OutputMessagePool &operator=(const OutputMessagePool &) = delete;

	/**
	 * @brief Get the singleton instance of OutputMessagePool.
	 *
	 * @return Reference to the OutputMessagePool instance.
	 */
	static OutputMessagePool &getInstance();

	/**
	 * @brief Sends all buffered messages for registered protocols.
	 */
	void sendAll();

	/**
	 * @brief Schedules `sendAll` to be executed later (e.g., next IO cycle).
	 */
	void scheduleSendAll();

	/**
	 * @brief Obtains a fresh OutputMessage from the pool (or creates new).
	 *
	 * @return Shared pointer to an OutputMessage.
	 */
	static OutputMessage_ptr getOutputMessage();

	/**
	 * @brief Adds a protocol to the auto-send list.
	 *
	 * Protocols in this list will have their buffered messages flushed periodically.
	 *
	 * @param protocol The protocol to add.
	 */
	void addProtocolToAutosend(const Protocol_ptr &protocol);

	/**
	 * @brief Removes a protocol from the auto-send list.
	 *
	 * @param protocol The protocol to remove.
	 */
	void removeProtocolFromAutosend(const Protocol_ptr &protocol);

private:
	// NOTE: A vector is used here because this container is mostly read
	// and relatively rarely modified (only when a client connects/disconnects)
	std::vector<Protocol_ptr> bufferedProtocols;
};
