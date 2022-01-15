// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <string.h>
#include <rules-storage/RulesStorageFile.hpp>

extern "C" {

#include <rules-storage/RulesStorageFile.h>

}

RulesStorage* RulesStorageFile_c_create() {
	return (RulesStorage*)new verificaC19Sdk::RulesStorageFile();
}
