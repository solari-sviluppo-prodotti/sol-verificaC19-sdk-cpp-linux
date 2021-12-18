// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_ITALY_HPP
#define KEYS_PROVIDER_ITALY_HPP

#include <verificaC19-sdk/interfaces/IKeysProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

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

extern "C" {

#include <examples/keys-provider/KeysProviderItaly.h>

} // extern "C"

#endif // #ifndef KEYS_PROVIDER_ITALY_HPP
