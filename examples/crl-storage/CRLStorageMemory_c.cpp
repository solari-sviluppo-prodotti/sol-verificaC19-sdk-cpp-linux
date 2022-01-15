// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <crl-storage/CRLStorageMemory.hpp>

extern "C" {

#include <crl-storage/CRLStorageMemory.h>

}

CRLStorage* CRLStorageMemory_c_create() {
	return (CRLStorage*)new verificaC19Sdk::CRLStorageMemory();
}
