// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_H
#define KEYS_STORAGE_H

#include <time.h>
#include <stdbool.h>

typedef struct {} KeysStorage;

void KeysStorage_c_release(const KeysStorage* keysStorage);
typedef void (*pfKeysStorage_c_release)(const KeysStorage*);

const char* KeysStorage_c_getKey(const KeysStorage* keysStorage, const char* kid);
typedef const char* (*pfKeysStorage_c_getKey)(const KeysStorage*, const char*);

void KeysStorage_c_beginUpdatingKeys(const KeysStorage* keysStorage);
typedef void (*pfKeysStorage_c_beginUpdatingKeys)(const KeysStorage*);

void KeysStorage_c_storeKey(const KeysStorage* keysStorage, const char* kid, const char* ecx);
typedef void (*pfKeysStorage_c_storeKey)(const KeysStorage*, const char*, const char*);

void KeysStorage_c_commitUpdatedKeys(const KeysStorage* keysStorage);
typedef void (*pfKeysStorage_c_commitUpdatedKeys)(const KeysStorage*);

void KeysStorage_c_rollbackUpdatedKeys(const KeysStorage* keysStorage);
typedef void (*pfKeysStorage_c_rollbackUpdatedKeys)(const KeysStorage*);

bool KeysStorage_c_isUpdating(const void* keysStorage);
typedef bool (*pfKeysStorage_c_isUpdating)(const void*);

time_t KeysStorage_c_lastUpdate(const KeysStorage* keysStorage);
typedef time_t (*pfKeysStorage_c_lastUpdate)(const KeysStorage*);

void KeysStorage_c_setLastStoredKeyToken(const KeysStorage* keysStorage, const char* token);
typedef void (*pfKeysStorage_c_setLastStoredKeyToken)(const KeysStorage*, const char*);

const char* KeysStorage_c_getLastStoredKeyToken(const KeysStorage* keysStorage);
typedef const char* (*pfKeysStorage_c_getLastStoredKeyToken)(const KeysStorage*);

#endif // #ifndef KEYS_STORAGE_H
