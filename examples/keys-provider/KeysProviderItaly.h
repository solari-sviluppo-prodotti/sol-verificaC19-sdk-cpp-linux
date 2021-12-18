// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_ITALY_H
#define KEYS_PROVIDER_ITALY_H

void* KeysProviderItaly_c_create(void* logger);
typedef void* (*pfKeysProviderItaly_c_create)(void*);

void KeysProviderItaly_c_release(const void* keysProviderItaly);
typedef void (*pfKeysProviderItaly_c_release)(const void*);

void KeysProviderItaly_c_refreshKeys(const void* keysProviderItaly, void* keysStorage);
typedef void (*pfKeysProviderItaly_c_refreshKeys)(const void*, void*);

#endif // #ifndef KEYS_PROVIDER_ITALY_H
