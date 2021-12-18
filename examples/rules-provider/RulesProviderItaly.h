// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_ITALY_H
#define RULES_PROVIDER_ITALY_H

void* RulesProviderItaly_c_create(void* logger);
typedef void* (*pfRulesProviderItaly_c_create)(void*);

void RulesProviderItaly_c_release(const void* rulesProviderItaly);
typedef void (*pfRulesProviderItaly_c_release)(const void*);

void RulesProviderItaly_c_refreshKeys(const void* rulesProviderItaly, void* rulesStorage);
typedef void (*pfRulesProviderItaly_c_refreshRules)(const void*, void*);

#endif // #ifndef RULES_PROVIDER_ITALY_H
