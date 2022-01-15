// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_ITALY_H
#define RULES_PROVIDER_ITALY_H

#include <verificaC19-sdk/interfaces/RulesProvider.h>
#include <verificaC19-sdk/interfaces/RulesStorage.h>
#include <verificaC19-sdk/interfaces/Logger.h>

RulesProvider* RulesProviderItaly_c_create(Logger* logger);
typedef RulesProvider* (*pfRulesProviderItaly_c_create)(Logger*);

#endif // #ifndef RULES_PROVIDER_ITALY_H
