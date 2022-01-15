// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <curl/curl.h>

#include <json/json.h>

#include <string>

#include <verificaC19-sdk/DGCVerifier.hpp>
#include <crl-provider/CRLProviderItaly.hpp>

namespace verificaC19Sdk {

#define URL_CHECK "https://get.dgc.gov.it/v1/dgc/drl/check"
#define URL_DOWNLOAD "https://get.dgc.gov.it/v1/dgc/drl"

size_t CRLProviderItaly::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

long CRLProviderItaly::curlRequest(std::string url, std::string& response) const {

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	struct curl_slist *hs = NULL;
	hs = curl_slist_append(hs, std::string(std::string("SDK-VERSION: ") + std::string(DGCVerifier_SDK_Version)).c_str());
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

CRLProviderItaly::CRLProviderItaly(ILogger* logger) : m_logger(logger) {
	if (m_logger == NULL) {
		m_logger = new LoggerNull();
	}
}

void CRLProviderItaly::refreshCRL(ICRLStorage* crlStorage) const {

	int retries = 0;
	Json::Value jCRLStatus;
	Json::Value jDownloadInfo;
	{
		std::string downloadInfo = crlStorage->downloadInfo();
		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();
		std::string errors;
		reader->parse(downloadInfo.c_str(), downloadInfo.c_str() + downloadInfo.length(), &jDownloadInfo, &errors);
		delete reader;
	}
	// If no download info start with current version 0
	if (!jDownloadInfo.isMember("version")) {
		jDownloadInfo["version"] = crlStorage->version();
	}
	if (!jDownloadInfo.isMember("chunk")) {
		jDownloadInfo["chunk"] = 1;
	}
	while (true) {
		std::string url = URL_CHECK + std::string("?version=") + jDownloadInfo["version"].asString();
		std::string status;
		long httpCode = curlRequest(url, status);
		if (httpCode != 200) {
			m_logger->error("Error %d requesting CRL status", httpCode);
			if (++retries > 3) {
				break;
			} else {
				continue;
			}
		}

		std::string errors;
		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();
		reader->parse(status.c_str(), status.c_str() + status.length(), &jCRLStatus, &errors);
		delete reader;
		break;
	}

	// check if need to update
	if (jCRLStatus["version"].asInt() == jDownloadInfo["version"].asInt()) {
		// permit to update time of last update
		crlStorage->commitUpdatedCRL();
		m_logger->info("No need to update CRL, same version: %d", jCRLStatus["version"].asInt());
		return;
	}

	// check if missing chunk info
	if (!jCRLStatus.isMember("chunk") || !jCRLStatus.isMember("totalChunk")) {
		m_logger->error("No chunk info in status");
		return;
	}

	// check if wrong chunk info
	if (jCRLStatus["chunk"].asInt() > jCRLStatus["totalChunk"].asInt()) {
		m_logger->error("Wrong chunk info in status");
		return;
	}

	// update starting version from backend (could not be 0)
	jDownloadInfo["version"] = jCRLStatus["fromVersion"];

	m_logger->debug("Our CRL version %d, backend CRL version: %d",
			jDownloadInfo["version"].asInt(), jCRLStatus["version"].asInt());

	retries = 0;
	crlStorage->beginUpdatingCRL();
	while (true) {
		std::string data;
		m_logger->debug("Requesting CRL with chunk: %d", jDownloadInfo["chunk"].asInt());
		std::string url = URL_DOWNLOAD + std::string("?version=") + jDownloadInfo["version"].asString() + std::string("&chunk=") + jDownloadInfo["chunk"].asString();
		long httpCode = curlRequest(url, data);
		if (httpCode != 200) {
			m_logger->error("Error %d requesting CRL", httpCode);
			if (++retries > 3) {
				crlStorage->rollbackUpdatedCRL();
				break;
			} else {
				continue;
			}
		}

		Json::CharReaderBuilder builder;
		Json::CharReader* reader = builder.newCharReader();
		Json::Value jChunk;
		std::string errors;
		reader->parse(data.c_str(), data.c_str() + data.length(), &jChunk, &errors);
		delete reader;

		// check if complete list starting
		if (jChunk.isMember("revokedUcvi") && jChunk.isMember("chunk") && jChunk["chunk"].asInt() == 1) {
			m_logger->debug("First chunk of complete list, remove all previous data");
			crlStorage->clearCRL();
		}

		// check if missing chunk info
		if (!jChunk.isMember("chunk") || !jChunk.isMember("lastChunk")) {
			m_logger->error("No chunk info in data");
			return;
		}

		// check if wrong chunk info
		if (jChunk["chunk"].asInt() > jChunk["lastChunk"].asInt()) {
			m_logger->error("Wrong chunk info in data");
			return;
		}

		// check if end downloading
		if (jChunk["chunk"].asInt() == jChunk["lastChunk"].asInt()) {
			// prepare for future spelling correction
			int total = 0;
			if (jChunk.isMember("totalNumberUCVI")) {
				total = jChunk["totalNumberUCVI"].asInt();
			} else if (jChunk.isMember("totalNumberUVCI")) {
				total = jChunk["totalNumberUVCI"].asInt();
			}
			m_logger->debug("Load CRL end, list size: %d", total);
			crlStorage->storeChunk(data);
			crlStorage->commitUpdatedCRL();
			break;
		}

		// check if error downloading:
		// - changed backend version during download
		// - changed chunk size in bytes during download
		if (jChunk.isMember("version") && jChunk["version"].asInt() == jDownloadInfo["version"].asInt()) {
			m_logger->debug("Changed version during download");
			crlStorage->rollbackUpdatedCRL();
			crlStorage->commitUpdatedCRL();
			break;
		}
		if (jChunk.isMember("sizeSingleChunkInByte") && jDownloadInfo.isMember("sizeSingleChunkInByte") &&
				jChunk["sizeSingleChunkInByte"].asInt() != jDownloadInfo["sizeSingleChunkInByte"].asInt()) {
			m_logger->debug("Changed chunk size during download");
			crlStorage->rollbackUpdatedCRL();
			crlStorage->commitUpdatedCRL();
			break;
		}
		jDownloadInfo["sizeSingleChunkInByte"] = jChunk["sizeSingleChunkInByte"];

		retries = 0;
		if (data.empty()) {
			m_logger->error("Received empty chunk");
			crlStorage->rollbackUpdatedCRL();
			break;
		}

		m_logger->debug("Store CRL chunk: %d of %d", jDownloadInfo["chunk"].asInt(), jChunk["lastChunk"].asInt());
		crlStorage->storeChunk(data);

		jDownloadInfo["chunk"] = jDownloadInfo["chunk"].asInt() + 1;
	};
}

} // namespace verificaC19Sdk

