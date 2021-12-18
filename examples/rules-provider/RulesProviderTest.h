// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_TEST_H
#define RULES_PROVIDER_TEST_H

void* RulesProviderTest_c_create(void* logger);
typedef void* (*pfRulesProviderTest_c_create)(void*);

void RulesProviderTest_c_release(const void* rulesProviderTest);
typedef void (*pfRulesProviderTest_c_release)(const void*);

void RulesProviderTest_c_refreshRules(const void* rulesProviderTest, void* keysStorage);
typedef void (*pfRulesProviderTest_c_refreshRules)(const void*, void*);

#endif // #ifndef RULES_PROVIDER_TEST_H
