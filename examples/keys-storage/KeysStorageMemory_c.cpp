// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <keys-storage/KeysStorageMemory.hpp>

extern "C" {

#include <keys-storage/KeysStorageMemory.h>

}

KeysStorage* KeysStorageMemory_c_create() {
	return (KeysStorage*)new verificaC19Sdk::KeysStorageMemory();
}
