/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#include "server/server_definitions.hpp"

class OutputMessage;
using OutputMessage_ptr = std::shared_ptr<OutputMessage>;
class Connection;
using Connection_ptr = std::shared_ptr<Connection>;
using ConnectionWeak_ptr = std::weak_ptr<Connection>;

class NetworkMessage;

/**
 * @brief Abstract base class for network protocols.
 *
 * This class defines the common interface and functionality for all protocols
 * handled by the server (e.g., Game, Login, Status). It manages encryption (XTEA, RSA),
 * checksums, compression, and message parsing/sending.
 */
class Protocol : public std::enable_shared_from_this<Protocol> {
public:
	explicit Protocol(const Connection_ptr &initConnection);

	virtual ~Protocol() = default;

	// non-copyable
	Protocol(const Protocol &) = delete;
	Protocol &operator=(const Protocol &) = delete;

	/**
	 * @brief Parses an incoming packet.
	 *
	 * @param msg The received NetworkMessage.
	 */
	virtual void parsePacket(NetworkMessage &) { }

	/**
	 * @brief Callback executed when a message is about to be sent.
	 *
	 * Handles encryption, checksumming, and compression before transmission.
	 *
	 * @param msg The message to send.
	 */
	virtual void onSendMessage(const OutputMessage_ptr &msg);

	/**
	 * @brief Callback executed when a message is received.
	 *
	 * Decrypts and validates the message.
	 *
	 * @param msg The received message.
	 * @return True if the message is valid and processed, false otherwise.
	 */
	bool onRecvMessage(NetworkMessage &msg);

	bool sendRecvMessageCallback(NetworkMessage &msg);

	/**
	 * @brief Handler for the first message received after connection.
	 *
	 * @param msg The received message.
	 */
	virtual void onRecvFirstMessage(NetworkMessage &msg) = 0;

	virtual void sendLoginChallenge() { }

	/**
	 * @brief Checks if the connection associated with this protocol has expired.
	 *
	 * @return True if expired, false otherwise.
	 */
	bool isConnectionExpired() const;

	/**
	 * @brief Gets the associated connection.
	 *
	 * @return Shared pointer to the Connection.
	 */
	Connection_ptr getConnection() const;

	/**
	 * @brief Gets the client's IP address.
	 *
	 * @return The IP address.
	 */
	uint32_t getIP() const;

	/**
	 * @brief Gets a buffer from the output pool for sending messages.
	 *
	 * @param size Size of the buffer needed.
	 * @return Shared pointer to an OutputMessage.
	 */
	OutputMessage_ptr getOutputBuffer(int32_t size);

	OutputMessage_ptr &getCurrentBuffer() {
		return outputBuffer;
	}

	/**
	 * @brief Sends an output message through the connection.
	 *
	 * @param msg The message to send.
	 */
	void send(OutputMessage_ptr msg) const;

protected:
	/**
	 * @brief Disconnects the client.
	 */
	void disconnect() const;

	/**
	 * @brief Enables XTEA encryption for this protocol session.
	 */
	void enableXTEAEncryption() {
		encryptionEnabled = true;
	}

	/**
	 * @brief Sets the XTEA encryption key.
	 *
	 * @param newKey Pointer to the 4-integer key array.
	 */
	void setXTEAKey(const uint32_t* newKey) {
		std::ranges::copy(newKey, newKey + 4, this->key.begin());
	}

	/**
	 * @brief Sets the checksum method used by the protocol.
	 *
	 * @param method The checksum method (e.g., Adler32, Sequence).
	 */
	void setChecksumMethod(ChecksumMethods_t method) {
		checksumMethod = method;
	}

	/**
	 * @brief Decrypts RSA block in the message.
	 *
	 * @param msg The message containing encrypted data.
	 * @return True if decryption was successful, false otherwise.
	 */
	static bool RSA_decrypt(NetworkMessage &msg);

	void setRawMessages(bool value) {
		rawMessages = value;
	}

	virtual void release() { }

private:
	struct ZStream {
		ZStream() noexcept;

		~ZStream() {
			deflateEnd(stream.get());
		}

		std::unique_ptr<z_stream> stream;
		std::array<char, NETWORKMESSAGE_MAXSIZE> buffer {};
	};

	void XTEA_transform(uint8_t* buffer, size_t messageLength, bool encrypt) const;
	void XTEA_encrypt(OutputMessage &msg) const;
	bool XTEA_decrypt(NetworkMessage &msg) const;
	bool compression(OutputMessage &msg) const;

	OutputMessage_ptr outputBuffer;

	const ConnectionWeak_ptr connectionPtr;
	std::array<uint32_t, 4> key = {};
	uint32_t serverSequenceNumber = 0;
	uint32_t clientSequenceNumber = 0;
	std::underlying_type_t<ChecksumMethods_t> checksumMethod = CHECKSUM_METHOD_NONE;
	bool encryptionEnabled = false;
	bool rawMessages = false;

	friend class Connection;
};
