// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <keys-provider/KeysProviderItaly.hpp>

extern "C" {

#include <keys-provider/KeysProviderItaly.h>

}

KeysProvider* KeysProviderItaly_c_create(Logger* logger) {
	return (KeysProvider*)new verificaC19Sdk::KeysProviderItaly((verificaC19Sdk::ILogger*)logger);
}
