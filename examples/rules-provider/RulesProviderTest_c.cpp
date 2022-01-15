// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <rules-provider/RulesProviderTest.hpp>

extern "C" {

#include <rules-provider/RulesProviderTest.h>

}

RulesProvider* RulesProviderTest_c_create(Logger* logger) {
	return (RulesProvider*)new verificaC19Sdk::RulesProviderTest((verificaC19Sdk::ILogger*)logger);
}
