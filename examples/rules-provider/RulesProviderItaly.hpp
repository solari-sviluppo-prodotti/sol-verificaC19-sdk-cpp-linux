// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_ITALY_HPP
#define RULES_PROVIDER_ITALY_HPP

#include <verificaC19-sdk/interfaces/IRulesProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

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

extern "C" {

#include <examples/rules-provider/RulesProviderItaly.h>

} // extern "C"

#endif // #ifndef RULES_PROVIDER_ITALY_HPP
