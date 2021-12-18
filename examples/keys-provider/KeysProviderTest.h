// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_TEST_H
#define KEYS_PROVIDER_TEST_H

void* KeysProviderTest_c_create(void* logger);
typedef void* (*pfKeysProviderTest_c_create)(void*);

void KeysProviderTest_c_release(const void* keysProviderTest);
typedef void (*pfKeysProviderTest_c_release)(const void*);

void KeysProviderTest_c_refreshKeys(const void* keysProviderTest, void* keysStorage);
typedef void (*pfKeysProviderTest_c_refreshKeys)(const void*, void*);

#endif // #ifndef KEYS_PROVIDER_TEST_H
