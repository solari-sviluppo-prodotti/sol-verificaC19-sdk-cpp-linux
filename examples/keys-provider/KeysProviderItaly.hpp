// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_ITALY_HPP
#define KEYS_PROVIDER_ITALY_HPP

#include <verificaC19-sdk/interfaces/IKeysProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

#ifdef __cplusplus

#include <string>
#include <map>

namespace verificaC19Sdk {

class KeysProviderItaly : public IKeysProvider {

public:

	KeysProviderItaly(ILogger* logger);
	void refreshKeys(IKeysStorage* keysStorage) const;

private:

	static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
	long curlRequest(std::string url, std::string header, std::string& response, std::string& rxheader) const;
	long curlRequest(std::string url, std::string& response) const;

	ILogger* m_logger;

};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void* KeysProviderItaly_c_create(void* logger);
typedef void* (*pfKeysProviderItaly_c_create)(void*);

void KeysProviderItaly_c_release(const void* keysProviderItaly);
typedef void (*pfKeysProviderItaly_c_release)(const void*);

void KeysProviderItaly_c_refreshKeys(const void* keysProviderItaly, void* keysStorage);
typedef void (*pfKeysProviderItaly_c_refreshKeys)(const void*, void*);

#ifdef __cplusplus
}
#endif

#endif // #ifndef KEYS_PROVIDER_ITALY_HPP
