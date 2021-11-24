// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_TEST_HPP
#define RULES_PROVIDER_TEST_HPP

#include <string>
#include <map>
#include <verificaC19-sdk/interfaces/ILogger.hpp>
#include <verificaC19-sdk/interfaces/IRulesProvider.hpp>

namespace verificaC19Sdk {

class RulesProviderTest : public IRulesProvider {

public:

	RulesProviderTest(ILogger* logger);
	void refreshRules(IRulesStorage* rulesStorage) const;

private:

	ILogger* m_logger;

};

} // namespace verificaC19Sdk

#endif // #ifndef RULES_PROVIDER_TEST_HPP
