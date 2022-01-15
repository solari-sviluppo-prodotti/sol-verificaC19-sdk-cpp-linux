// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_STORAGE_FILE_H
#define KEYS_STORAGE_FILE_H

#include <verificaC19-sdk/interfaces/KeysStorage.h>

#include <stdbool.h>
#include <time.h>

KeysStorage* KeysStorageFile_c_create();
typedef KeysStorage* (*pfKeysStorageFile_c_create)();

void KeysStorageFile_c_release(const KeysStorage* keysStorage);
typedef void (*pfKeysStorageFile_c_release)(const KeysStorage*);

const char* KeysStorageFile_c_getKey(const KeysStorage* keysStorage, const char* kid);
typedef const char* (*pfKeysStorageFile_c_getKey)(const KeysStorage*, const char*);

void KeysStorageFile_c_beginUpdatingKeys(const KeysStorage* keysStorage);
typedef void (*pfKeysStorageFile_c_beginUpdatingKeys)(const KeysStorage*);

void KeysStorageFile_c_storeKey(const KeysStorage* keysStorage, const char* kid, const char* ecx);
typedef void (*pfKeysStorageFile_c_storeKey)(const KeysStorage*, const char*, const char*);

void KeysStorageFile_c_commitUpdatedKeys(const KeysStorage* keysStorage);
typedef void (*pfKeysStorageFile_c_commitUpdatedKeys)(const KeysStorage*);

void KeysStorageFile_c_rollbackUpdatedKeys(const KeysStorage* keysStorage);
typedef void (*pfKeysStorageFile_c_rollbackUpdatedKeys)(const KeysStorage*);

bool KeysStorageFile_c_isUpdating(const KeysStorage* keysStorage);
typedef bool (*pfKeysStorageFile_c_isUpdating)(const KeysStorage*);

time_t KeysStorageFile_c_lastUpdate(const KeysStorage* keysStorage);
typedef time_t (*pfKeysStorageFile_c_lastUpdate)(const KeysStorage*);

void KeysStorageFile_c_setLastStoredKeyToken(const KeysStorage* keysStorage, const char* token);
typedef void (*pfKeysStorageFile_c_setLastStoredKeyToken)(const KeysStorage*, const char*);

const char* KeysStorageFile_c_getLastStoredKeyToken(const KeysStorage* keysStorage);
typedef const char* (*pfKeysStorageFile_c_getLastStoredKeyToken)(const KeysStorage*);

#endif // #ifndef KEYS_STORAGE_FILE_H
