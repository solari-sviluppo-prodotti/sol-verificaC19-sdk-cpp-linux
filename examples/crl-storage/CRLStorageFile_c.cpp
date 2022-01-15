// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <crl-storage/CRLStorageFile.hpp>

extern "C" {

#include <crl-storage/CRLStorageFile.h>

}


CRLStorage* CRLStorageFile_c_create() {
	return (CRLStorage*)new verificaC19Sdk::CRLStorageFile();
}
