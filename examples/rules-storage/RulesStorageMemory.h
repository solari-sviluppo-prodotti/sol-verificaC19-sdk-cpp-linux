// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_MEMORY_H
#define RULES_STORAGE_MEMORY_H

#include <verificaC19-sdk/interfaces/RulesStorage.h>

#include <stdbool.h>
#include <time.h>

RulesStorage* RulesStorageMemory_c_create();
typedef RulesStorage* (*pfRulesStorageMemory_c_create)(void*);

void RulesStorageMemory_c_release(const RulesStorage* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_release)(const RulesStorage*);

const char* RulesStorageMemory_c_getRule(const RulesStorage* rulesStorageMemory, const char* name, const char* type);
typedef const char* (*pfRulesStorageMemory_c_getRule)(const RulesStorage*, const char*, const char*);

void RulesStorageMemory_c_beginUpdatingRules(const RulesStorage* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_beginUpdatingRules)(const RulesStorage*);

void RulesStorageMemory_c_storeRule(const RulesStorage* rulesStorageMemory, const char* name, const char* type, const char* value);
typedef void (*pfRulesStorageMemory_c_storeRule)(const RulesStorage*, const char*, const char*, const char*);

void RulesStorageMemory_c_commitUpdatedRules(const RulesStorage* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_commitUpdatedRules)(const RulesStorage*);

void RulesStorageMemory_c_rollbackUpdatedRules(const RulesStorage* rulesStorageMemory);
typedef void (*pfRulesStorageMemory_c_rollbackUpdatedRules)(const RulesStorage*);

bool RulesStorageMemory_c_isUpdating(const RulesStorage* rulesStorageMemory);
typedef bool (*pfRulesStorageMemory_c_isUpdating)(const RulesStorage*);

time_t RulesStorageMemory_c_lastUpdate(const RulesStorage* rulesStorageMemory);
typedef time_t (*pfRulesStorageMemory_c_lastUpdate)(const RulesStorage*);

#endif // #ifndef RULES_STORAGE_MEMORY_H
