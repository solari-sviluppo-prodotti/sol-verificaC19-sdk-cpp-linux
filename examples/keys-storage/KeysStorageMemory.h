// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_MEMORY_H
#define KEYS_STORAGE_MEMORY_H

#include <verificaC19-sdk/interfaces/KeysStorage.h>

#include <stdbool.h>
#include <time.h>

KeysStorage* KeysStorageMemory_c_create();
typedef KeysStorage* (*pfKeysStorageMemory_c_create)();

void KeysStorageMemory_c_release(const KeysStorage* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_release)(const KeysStorage*);

const char* KeysStorageMemory_c_getKey(const KeysStorage* keysStorageMemory, const char* kid);
typedef const char* (*pfKeysStorageMemory_c_getKey)(const KeysStorage*, const char*);

void KeysStorageMemory_c_beginUpdatingKeys(const KeysStorage* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_beginUpdatingKeys)(const KeysStorage*);

void KeysStorageMemory_c_storeKey(const KeysStorage* keysStorageMemory, const char* kid, const char* ecx);
typedef void (*pfKeysStorageMemory_c_storeKey)(const KeysStorage*, const char*, const char*);

void KeysStorageMemory_c_commitUpdatedKeys(const KeysStorage* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_commitUpdatedKeys)(const KeysStorage*);

void KeysStorageMemory_c_rollbackUpdatedKeys(const KeysStorage* keysStorageMemory);
typedef void (*pfKeysStorageMemory_c_rollbackUpdatedKeys)(const KeysStorage*);

bool KeysStorageMemory_c_isUpdating(const void* keysStorageMemory);
typedef bool (*pfKeysStorageMemory_c_isUpdating)(const void*);

time_t KeysStorageMemory_c_lastUpdate(const KeysStorage* keysStorageMemory);
typedef time_t (*pfKeysStorageMemory_c_lastUpdate)(const KeysStorage*);

void KeysStorageMemory_c_setLastStoredKeyToken(const KeysStorage* keysStorageMemory, const char* token);
typedef void (*pfKeysStorageMemory_c_setLastStoredKeyToken)(const KeysStorage*, const char*);

const char* KeysStorageMemory_c_getLastStoredKeyToken(const KeysStorage* keysStorageMemory);
typedef const char* (*pfKeysStorageMemory_c_getLastStoredKeyToken)(const KeysStorage*);

#endif // #ifndef KEYS_STORAGE_MEMORY_H
