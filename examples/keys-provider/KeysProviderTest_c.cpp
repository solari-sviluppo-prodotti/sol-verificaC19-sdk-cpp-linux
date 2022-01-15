// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <keys-provider/KeysProviderTest.hpp>

extern "C" {

#include <keys-provider/KeysProviderTest.h>

}

KeysProvider* KeysProviderTest_c_create(Logger* logger) {
	return (KeysProvider*)new verificaC19Sdk::KeysProviderTest((verificaC19Sdk::ILogger*)logger);
}
