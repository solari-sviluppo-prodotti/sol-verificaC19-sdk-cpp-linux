// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_ITALY_H
#define RULES_PROVIDER_ITALY_H

#include <verificaC19-sdk/interfaces/RulesProvider.h>
#include <verificaC19-sdk/interfaces/RulesStorage.h>
#include <verificaC19-sdk/interfaces/Logger.h>

RulesProvider* RulesProviderItaly_c_create(Logger* logger);
typedef RulesProvider* (*pfRulesProviderItaly_c_create)(Logger*);

void RulesProviderItaly_c_release(const RulesProvider* rulesProviderItaly);
typedef void (*pfRulesProviderItaly_c_release)(const RulesProvider*);

void RulesProviderItaly_c_refreshKeys(const RulesProvider* rulesProviderItaly, RulesStorage* rulesStorage);
typedef void (*pfRulesProviderItaly_c_refreshRules)(const RulesProvider*, RulesStorage*);

#endif // #ifndef RULES_PROVIDER_ITALY_H
