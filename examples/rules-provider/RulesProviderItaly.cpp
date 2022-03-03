// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <curl/curl.h>

#include <json/json.h>

#include <string>

#include <verificaC19-sdk/DGCVerifier.hpp>
#include <rules-provider/RulesProviderItaly.hpp>

namespace verificaC19Sdk {

#define URL "https://get.dgc.gov.it/v1/dgc/settings"

size_t RulesProviderItaly::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

long RulesProviderItaly::curlRequest(std::string url, std::string& response) const {

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	struct curl_slist *hs = NULL;
	hs = curl_slist_append(hs, std::string(std::string("User-Agent: verificac19-sdk-cpp/") + std::string(DGCVerifier_Local_Version)).c_str());
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	CURLcode res = curl_easy_perform(curl);
	long http_code = 0;
	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return http_code;
}

class LoggerNull : public ILogger {

public:

	void error(const std::string& message, ...) const { }
	void info(const std::string& message, ...) const { }
	void debug(const std::string& message, ...) const { }

};

RulesProviderItaly::RulesProviderItaly(ILogger* logger) : m_logger(logger) {
	if (m_logger == NULL) {
		m_logger = new LoggerNull();
	}
}

void RulesProviderItaly::refreshRules(IRulesStorage* rulesStorage) const {
	std::string rules;
	m_logger->debug("Requesting rules");
	long httpCode = curlRequest(URL, rules);
	if (httpCode != 200) {
		m_logger->error("Error %d requesting rules", httpCode);
		return;
	}
	if (rules.empty()) {
		m_logger->error("Received empty rules");
		return;
	}
	rulesStorage->beginUpdatingRules();
	Json::Value jsonRules;
	std::string errors;
	Json::CharReaderBuilder builder;
	Json::CharReader* reader = builder.newCharReader();
	reader->parse(rules.c_str(), rules.c_str() + rules.length(), &jsonRules, &errors);
	for (Json::Value::iterator it = jsonRules.begin(); it != jsonRules.end(); it++) {
		Json::Value jsonRule = *it;
		if (jsonRule.isMember("name") &&
			jsonRule.isMember("type") &&
			jsonRule.isMember("value")) {
			std::string name = jsonRule["name"].asString();
			std::string type = jsonRule["type"].asString();
			std::string value = jsonRule["value"].asString();
			m_logger->debug("Storing rule: %s:%s -> %s", name.c_str(), type.c_str(), value.c_str());
			rulesStorage->storeRule(name, type, value);
		}
	}
	rulesStorage->commitUpdatedRules();
	delete reader;
}

} // namespace verificaC19Sdk
