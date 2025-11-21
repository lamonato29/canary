/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019-2024 OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#pragma once

#include "lib/thread/thread_pool.hpp"

struct WebhookTask {
	std::string payload;
	std::string url;

	WebhookTask(std::string p, std::string u) :
		payload(std::move(p)), url(std::move(u)) { }
};

/**
 * @brief Manages webhook integrations.
 *
 * This singleton class allows sending HTTP payloads (webhooks) to external services,
 * typically used for Discord integration or logging events to an external API.
 */
class Webhook {
public:
	static constexpr size_t DEFAULT_DELAY_MS = 1000;

	explicit Webhook(ThreadPool &threadPool);

	// Singleton - ensures we don't accidentally copy it
	Webhook(const Webhook &) = delete;
	void operator=(const Webhook &) = delete;

	/**
	 * @brief Gets the singleton instance of Webhook.
	 *
	 * @return Reference to the Webhook instance.
	 */
	static Webhook &getInstance();

	/**
	 * @brief Runs the webhook processing loop.
	 *
	 * Checks for pending webhook tasks and dispatches them.
	 */
	void run();

	/**
	 * @brief Sends a raw payload to a specified URL.
	 *
	 * @param payload The JSON payload to send.
	 * @param url The destination URL.
	 */
	void sendPayload(const std::string &payload, const std::string &url);

	/**
	 * @brief Sends a formatted message (typically for Discord).
	 *
	 * @param title The title of the message/embed.
	 * @param message The content of the message.
	 * @param color The color of the embed (if applicable).
	 * @param url The webhook URL (optional, defaults to configured URL).
	 * @param embed Whether to send as a rich embed or plain text.
	 */
	void sendMessage(const std::string &title, const std::string &message, int color, std::string url = "", bool embed = true);

	/**
	 * @brief Sends a simple message.
	 *
	 * @param message The message content.
	 * @param url The webhook URL (optional).
	 */
	void sendMessage(const std::string &message, std::string url = "");

private:
	std::mutex taskLock;
	ThreadPool &threadPool;
	std::deque<std::shared_ptr<WebhookTask>> webhooks;
	curl_slist* headers = nullptr;

	void sendWebhook();

	int sendRequest(const char* url, const char* payload, std::string* response_body) const;
	static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
	std::string getPayload(const std::string &title, const std::string &message, int color, bool embed = true) const;
};

constexpr auto g_webhook = Webhook::getInstance;
