// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_H
#define KEYS_PROVIDER_H

#include <verificaC19-sdk/interfaces/KeysStorage.h>

typedef struct {} KeysProvider;

void KeysProvider_c_release(const KeysProvider* keysProvider);
typedef void (*pfKeysProvider_c_release)(const KeysProvider*);

void KeysProvider_c_refreshKeys(const KeysProvider* keysProvider, KeysStorage* keysStorage);
typedef void (*pfKeysProvider_c_refreshKeys)(const KeysProvider*, KeysStorage*);


#endif // #ifndef KEYS_PROVIDER_H
