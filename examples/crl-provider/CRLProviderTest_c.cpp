// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <crl-provider/CRLProviderTest.hpp>

extern "C" {

#include <crl-provider/CRLProviderTest.h>

}

CRLProvider* CRLProviderTest_c_create(Logger* logger) {
	return (CRLProvider*)new verificaC19Sdk::CRLProviderTest((verificaC19Sdk::ILogger*)logger);
}
