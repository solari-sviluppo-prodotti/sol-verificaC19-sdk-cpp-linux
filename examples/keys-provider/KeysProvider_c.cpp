// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <verificaC19-sdk/interfaces/IKeysProvider.hpp>

extern "C" {

#include <verificaC19-sdk/interfaces/KeysProvider.h>

}

void KeysProvider_c_release(const KeysProvider* keysProvider) {
	verificaC19Sdk::IKeysProvider* this_ = (verificaC19Sdk::IKeysProvider*)keysProvider;
	delete this_;
}

void KeysProvider_c_refreshKeys(const KeysProvider* keysProvider, KeysStorage* keysStorage) {
	verificaC19Sdk::IKeysProvider* this_ = (verificaC19Sdk::IKeysProvider*)keysProvider;
	this_->refreshKeys((verificaC19Sdk::IKeysStorage*)keysStorage);
}
