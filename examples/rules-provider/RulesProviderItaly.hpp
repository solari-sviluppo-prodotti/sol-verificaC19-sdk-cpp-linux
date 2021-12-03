// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_ITALY_HPP
#define RULES_PROVIDER_ITALY_HPP

#include <verificaC19-sdk/interfaces/IRulesProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

#ifdef __cplusplus

#include <string>
#include <map>

namespace verificaC19Sdk {

class RulesProviderItaly : public IRulesProvider {

public:

	RulesProviderItaly(ILogger* logger);
	void refreshRules(IRulesStorage* rulesStorage) const;

private:

	static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);
	long curlRequest(std::string url, std::string& response) const;

	ILogger* m_logger;

};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void* RulesProviderItaly_c_create(void* logger);
typedef void* (*pfRulesProviderItaly_c_create)(void*);

void RulesProviderItaly_c_release(const void* rulesProviderItaly);
typedef void (*pfRulesProviderItaly_c_release)(const void*);

void RulesProviderItaly_c_refreshKeys(const void* rulesProviderItaly, void* rulesStorage);
typedef void (*pfRulesProviderItaly_c_refreshRules)(const void*, void*);

#ifdef __cplusplus
}
#endif

#endif // #ifndef RULES_PROVIDER_ITALY_HPP
