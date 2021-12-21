// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_TEST_H
#define RULES_PROVIDER_TEST_H

#include <verificaC19-sdk/interfaces/RulesProvider.h>
#include <verificaC19-sdk/interfaces/RulesStorage.h>
#include <verificaC19-sdk/interfaces/Logger.h>

RulesProvider* RulesProviderTest_c_create(Logger* logger);
typedef RulesProvider* (*pfRulesProviderTest_c_create)(Logger*);

void RulesProviderTest_c_release(const RulesProvider* rulesProviderTest);
typedef void (*pfRulesProviderTest_c_release)(const RulesProvider*);

void RulesProviderTest_c_refreshRules(const RulesProvider* rulesProviderTest, RulesStorage* keysStorage);
typedef void (*pfRulesProviderTest_c_refreshRules)(const RulesProvider*, RulesStorage*);

#endif // #ifndef RULES_PROVIDER_TEST_H
