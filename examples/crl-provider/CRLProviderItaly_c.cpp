// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <crl-provider/CRLProviderItaly.hpp>

extern "C" {

#include <crl-provider/CRLProviderItaly.h>

}

CRLProvider* CRLProviderItaly_c_create(Logger* logger) {
	return (CRLProvider*)new verificaC19Sdk::CRLProviderItaly((verificaC19Sdk::ILogger*)logger);
}
