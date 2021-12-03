// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_TEST_HPP
#define RULES_PROVIDER_TEST_HPP

#include <verificaC19-sdk/interfaces/ILogger.hpp>
#include <verificaC19-sdk/interfaces/IRulesProvider.hpp>

#ifdef __cplusplus

#include <string>
#include <map>

namespace verificaC19Sdk {

class RulesProviderTest : public IRulesProvider {

public:

	RulesProviderTest(ILogger* logger);
	void refreshRules(IRulesStorage* rulesStorage) const;

private:

	ILogger* m_logger;

};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void* RulesProviderTest_c_create(void* logger);
typedef void* (*pfRulesProviderTest_c_create)(void*);

void RulesProviderTest_c_release(const void* rulesProviderTest);
typedef void (*pfRulesProviderTest_c_release)(const void*);

void RulesProviderTest_c_refreshRules(const void* rulesProviderTest, void* keysStorage);
typedef void (*pfRulesProviderTest_c_refreshRules)(const void*, void*);

#ifdef __cplusplus
}
#endif

#endif // #ifndef RULES_PROVIDER_TEST_HPP
