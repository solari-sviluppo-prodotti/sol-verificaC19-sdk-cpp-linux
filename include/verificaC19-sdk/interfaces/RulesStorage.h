// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_H
#define RULES_STORAGE_H

typedef struct {} RulesStorage;

void RulesStorage_c_release(const RulesStorage* rulesStorage);
typedef void (*pfRulesStorage_c_release)(const RulesStorage*);

const char* RulesStorage_c_getRule(const RulesStorage* rulesStorage, const char* name, const char* type);
typedef const char* (*pfRulesStorage_c_getRule)(const RulesStorage*, const char*, const char*);

void RulesStorage_c_beginUpdatingRules(const RulesStorage* rulesStorage);
typedef void (*pfRulesStorage_c_beginUpdatingRules)(const RulesStorage*);

void RulesStorage_c_storeRule(const RulesStorage* rulesStorage, const char* name, const char* type, const char* value);
typedef void (*pfRulesStorage_c_storeRule)(const RulesStorage*, const char*, const char*, const char*);

void RulesStorage_c_commitUpdatedRules(const RulesStorage* rulesStorage);
typedef void (*pfRulesStorage_c_commitUpdatedRules)(const RulesStorage*);

void RulesStorage_c_rollbackUpdatedRules(const RulesStorage* rulesStorage);
typedef void (*pfRulesStorage_c_rollbackUpdatedRules)(const RulesStorage*);

bool RulesStorage_c_isUpdating(const RulesStorage* rulesStorage);
typedef bool (*pfRulesStorage_c_isUpdating)(const RulesStorage*);

time_t RulesStorage_c_lastUpdate(const RulesStorage* rulesStorage);
typedef time_t (*pfRulesStorage_c_lastUpdate)(const RulesStorage*);

#endif // #ifndef RULES_STORAGE_H
