// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <rules-provider/RulesProviderItaly.hpp>

extern "C" {

#include <rules-provider/RulesProviderItaly.h>

}

RulesProvider* RulesProviderItaly_c_create(Logger* logger) {
	return (RulesProvider*)new verificaC19Sdk::RulesProviderItaly((verificaC19Sdk::ILogger*)logger);
}
