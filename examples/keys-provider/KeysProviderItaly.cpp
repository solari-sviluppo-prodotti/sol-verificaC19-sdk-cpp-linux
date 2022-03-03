// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <curl/curl.h>

#include <json/json.h>

#include <set>
#include <string>

#include <verificaC19-sdk/DGCVerifier.hpp>
#include <keys-provider/KeysProviderItaly.hpp>

namespace verificaC19Sdk {

#define URL_STATUS "https://get.dgc.gov.it/v1/dgc/signercertificate/status"
#define URL_UPDATE "https://get.dgc.gov.it/v1/dgc/signercertificate/update"

size_t KeysProviderItaly::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

long KeysProviderItaly::curlRequest(std::string url, std::string header, std::string& response, std::string& rxheader) const {

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &rxheader);
	struct curl_slist *hs = NULL;
	hs = curl_slist_append(hs, std::string(std::string("User-Agent: verificac19-sdk-cpp/") + std::string(DGCVerifier_Local_Version)).c_str());
	if (!header.empty()) {
		hs = curl_slist_append(hs, header.c_str());
	}
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	CURLcode res = curl_easy_perform(curl);
	long http_code = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return http_code;
}

long KeysProviderItaly::curlRequest(std::string url, std::string& response) const {
	std::string rxheader;
	return curlRequest(url, "", response, rxheader);
}

class LoggerNull : public ILogger {

public:

	void error(const std::string& message, ...) const { }
	void info(const std::string& message, ...) const { }
	void debug(const std::string& message, ...) const { }

};

KeysProviderItaly::KeysProviderItaly(ILogger* logger) : m_logger(logger) {
	if (m_logger == NULL) {
		m_logger = new LoggerNull();
	}
}

void KeysProviderItaly::refreshKeys(IKeysStorage* keysStorage) const {

	int retries = 0;
	std::set<std::string> keyList;
	while (true) {
		std::string status;
		long httpCode = curlRequest(URL_STATUS, status);
		if (httpCode != 200) {
			m_logger->error("Error %d requesting keys status", httpCode);
			if (++retries > 3) {
				break;
			} else {
				continue;
			}
		}

		Json::Value jsonKeys;
		std::string errors;
		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();
		reader->parse(status.c_str(), status.c_str() + status.length(), &jsonKeys, &errors);
		for (Json::Value::iterator it = jsonKeys.begin(); it != jsonKeys.end(); it++) {
			Json::Value jsonKey = *it;
			keyList.insert(jsonKey.asString());
		}
		delete reader;
		break;
	}
	if (keyList.empty()) {
		m_logger->error("Did not received list of valid keys");
		return;
	}

	m_logger->debug("Valid key list size: %d", keyList.size());

	std::string header = keysStorage->getLastStoredKeyToken();
	keysStorage->beginUpdatingKeys();
	retries = 0;
	std::string token;
	while (true) {
		std::string ecx;
		std::string rxheader;
		m_logger->debug("Requesting key with header: %s", header.c_str());
		long httpCode = curlRequest(URL_UPDATE, header, ecx, rxheader);
		if (httpCode == 204) {
			m_logger->debug("Load key end");
			keysStorage->setLastStoredKeyToken(token);
			keysStorage->commitUpdatedKeys();
			break;
		}
		if (httpCode != 200) {
			m_logger->error("Error %d requesting key", httpCode);
			if (++retries > 3) {
				keysStorage->rollbackUpdatedKeys();
				break;
			} else {
				continue;
			}
		}
		token = header;
		retries = 0;
		if (ecx.empty()) {
			m_logger->error("Received empty ecx");
			keysStorage->rollbackUpdatedKeys();
			break;
		}
		std::string kid;
		if (rxheader.find("X-KID:") != std::string::npos) {
			kid = rxheader.substr(rxheader.find("X-KID:") + 7);
			kid = kid.substr(0, kid.find("\r"));
		} else {
			m_logger->error("Received empty kid");
			keysStorage->rollbackUpdatedKeys();
			break;
		}
		if (rxheader.find("X-RESUME-TOKEN:") != std::string::npos) {
			header = rxheader.substr(rxheader.find("X-RESUME-TOKEN:"));
			header = header.substr(0, header.find("\r"));
		}
		if (keyList.find(kid) == keyList.end()) {
			m_logger->error("Ignoring key: %s (%d)", kid.c_str(), ecx.length());
		} else {
			m_logger->debug("Storing key: %s (%d)", kid.c_str(), ecx.length());
		}
		keysStorage->storeKey(kid, ecx);
	};
}

} // namespace verificaC19Sdk
