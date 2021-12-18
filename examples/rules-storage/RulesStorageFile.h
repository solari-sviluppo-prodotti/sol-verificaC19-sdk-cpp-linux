// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef RULES_STORAGE_FILE_H
#define RULES_STORAGE_FILE_H

#include <stdbool.h>

void* RulesStorageFile_c_create();
typedef void* (*pfRulesStorageFile_c_create)(void*);

void RulesStorageFile_c_release(const void* rulesStorageMemory);
typedef void (*pfRulesStorageFile_c_release)(const void*);

const char* RulesStorageFile_c_getRule(const void* rulesStorageMemory, const char* name, const char* type);
typedef const char* (*pfRulesStorageFile_c_getRule)(const void*, const char*, const char*);

void RulesStorageFile_c_beginUpdatingRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageFile_c_beginUpdatingRules)(const void*);

void RulesStorageFile_c_storeRule(const void* rulesStorageMemory, const char* name, const char* type, const char* value);
typedef void (*pfRulesStorageFile_c_storeRule)(const void*, const char*, const char*, const char*);

void RulesStorageFile_c_commitUpdatedRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageFile_c_commitUpdatedRules)(const void*);

void RulesStorageFile_c_rollbackUpdatedRules(const void* rulesStorageMemory);
typedef void (*pfRulesStorageFile_c_rollbackUpdatedRules)(const void*);

bool RulesStorageFile_c_isUpdating(const void* rulesStorageMemory);
typedef bool (*pfRulesStorageFile_c_isUpdating)(const void*);

time_t RulesStorageFile_c_lastUpdate(const void* rulesStorageMemory);
typedef time_t (*pfRulesStorageFile_c_lastUpdate)(const void*);

#endif // #ifndef RULES_STORAGE_FILE_H
