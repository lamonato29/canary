/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

class PropStream;

namespace OTB {
	using Identifier = std::array<char, 4>;

	/**
	 * @brief Represents a node in the OTB file structure.
	 *
	 * Nodes form a tree structure and contain properties.
	 */
	struct Node {
		Node() = default;
		Node(Node &&) = default;
		Node &operator=(Node &&) = default;
		Node(const Node &) = delete;
		Node &operator=(const Node &) = delete;

		std::list<Node> children;
		mio::mmap_source::const_iterator propsBegin {};
		mio::mmap_source::const_iterator propsEnd {};
		uint8_t type {};
		enum NodeChar : uint8_t {
			ESCAPE = 0xFD,
			START = 0xFE,
			END = 0xFF,
		};
	};

	struct LoadError : std::exception {
		const char* what() const noexcept override = 0;
	};

	struct InvalidOTBFormat final : LoadError {
		const char* what() const noexcept override {
			return "Invalid OTBM file format";
		}
	};

	/**
	 * @brief Class responsible for loading OTB files.
	 *
	 * It parses the OTB file structure and builds a tree of Nodes.
	 */
	class Loader {
		mio::mmap_source fileContents;
		Node root;
		std::vector<char> propBuffer;

	public:
		/**
		 * @brief Constructs a Loader and parses the file.
		 *
		 * @param fileName The path to the OTB file.
		 * @param acceptedIdentifier The expected identifier of the OTB file.
		 */
		Loader(const std::string &fileName, const Identifier &acceptedIdentifier);

		/**
		 * @brief Extracts properties from a node into a PropStream.
		 *
		 * @param node The node to extract properties from.
		 * @param props The PropStream to fill with properties.
		 * @return True if successful, false otherwise.
		 */
		bool getProps(const Node &node, PropStream &props);

		/**
		 * @brief Returns the root node of the parsed tree.
		 *
		 * @return Reference to the root Node.
		 */
		const Node &parseTree();
	};
} // namespace OTB

/**
 * @brief Helper class to read properties from a stream of bytes.
 */
class PropStream {
public:
	/**
	 * @brief Initializes the stream with a buffer.
	 *
	 * @param a Pointer to the buffer.
	 * @param size Size of the buffer.
	 */
	void init(const char* a, size_t size) {
		p = a;
		end = a + size;
	}

	/**
	 * @brief Get the remaining size of the stream.
	 *
	 * @return The remaining size in bytes.
	 */
	size_t size() const {
		return end - p;
	}

	/**
	 * @brief Reads a value of type T from the stream.
	 *
	 * @tparam T The type of the value to read.
	 * @param ret Reference to store the read value.
	 * @return True if successful, false if there are not enough bytes.
	 */
	template <typename T>
	bool read(T &ret) {
		static_assert(std::is_trivially_copyable_v<T>, "Type T must be trivially copyable");

		if (size() < sizeof(T)) {
			return false;
		}

		std::span<const char> charSpan { p, sizeof(T) };
		auto byteSpan = std::as_bytes(charSpan);

		std::array<std::byte, sizeof(T)> tempBuffer;
		std::ranges::copy(byteSpan, tempBuffer.begin());

		ret = std::bit_cast<T>(tempBuffer);

		p += sizeof(T);

		return true;
	}

	/**
	 * @brief Reads a string from the stream.
	 *
	 * @param ret Reference to store the read string.
	 * @return True if successful, false if there are not enough bytes.
	 */
	bool readString(std::string &ret) {
		uint16_t strLen;
		if (!read<uint16_t>(strLen)) {
			return false;
		}

		if (size() < strLen) {
			return false;
		}

		std::vector<char> tempBuffer(strLen);
		std::span<const char> sourceSpan(p, strLen);
		std::ranges::copy(sourceSpan, tempBuffer.begin());

		ret.assign(tempBuffer.begin(), tempBuffer.end());

		p += strLen;

		return true;
	}

	/**
	 * @brief Skips n bytes in the stream.
	 *
	 * @param n Number of bytes to skip.
	 * @return True if successful, false if there are not enough bytes.
	 */
	bool skip(size_t n) {
		if (size() < n) {
			return false;
		}

		p += n;
		return true;
	}

private:
	const char* p = nullptr;
	const char* end = nullptr;
};

/**
 * @brief Helper class to write properties to a stream of bytes.
 */
class PropWriteStream {
public:
	PropWriteStream() = default;

	// non-copyable
	PropWriteStream(const PropWriteStream &) = delete;
	PropWriteStream &operator=(const PropWriteStream &) = delete;

	/**
	 * @brief Get the data stream buffer.
	 *
	 * @param size Reference to store the size of the buffer.
	 * @return Pointer to the buffer data.
	 */
	const char* getStream(size_t &size) const {
		size = buffer.size();
		return buffer.data();
	}

	/**
	 * @brief Clears the buffer.
	 */
	void clear() {
		buffer.clear();
	}

	/**
	 * @brief Writes a value of type T to the stream.
	 *
	 * @tparam T The type of the value to write.
	 * @param add The value to write.
	 */
	template <typename T>
	void write(T add) {
		static_assert(std::is_trivially_copyable_v<T>, "Type T must be trivially copyable");

		auto byteArray = std::bit_cast<std::array<char, sizeof(T)>>(add);
		std::span<const char> charSpan(byteArray);
		std::ranges::copy(charSpan, std::back_inserter(buffer));
	}

	/**
	 * @brief Writes a string to the stream.
	 *
	 * @param str The string to write.
	 */
	void writeString(const std::string &str) {
		size_t strLength = str.size();
		if (strLength > std::numeric_limits<uint16_t>::max()) {
			write<uint16_t>(0);
			return;
		}

		write(static_cast<uint16_t>(strLength));
		std::ranges::copy(str, std::back_inserter(buffer));
	}

private:
	std::vector<char> buffer;
};
