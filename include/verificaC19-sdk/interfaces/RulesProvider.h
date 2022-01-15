// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_PROVIDER_H
#define RULES_PROVIDER_H

#include <verificaC19-sdk/interfaces/RulesStorage.h>

typedef struct {} RulesProvider;

void RulesProvider_c_release(const RulesProvider* rulesProvider);
typedef void (*pfRulesProvider_c_release)(const RulesProvider*);

void RulesProvider_c_refreshRules(const RulesProvider* rulesProvider, RulesStorage* keysStorage);
typedef void (*pfRulesProvider_c_refreshRules)(const RulesProvider*, RulesStorage*);

#endif // #ifndef RULES_PROVIDER_H
