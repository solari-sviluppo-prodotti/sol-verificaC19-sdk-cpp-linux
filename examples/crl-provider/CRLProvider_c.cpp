// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <verificaC19-sdk/interfaces/ICRLProvider.hpp>

extern "C" {

#include <verificaC19-sdk/interfaces/CRLProvider.h>

}

void CRLProvider_c_release(const CRLProvider* crlProvider) {
	verificaC19Sdk::ICRLProvider* this_ = (verificaC19Sdk::ICRLProvider*)crlProvider;
	delete this_;
}

void CRLProvider_c_refreshCRL(const CRLProvider* crlProvider, CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLProvider* this_ = (verificaC19Sdk::ICRLProvider*)crlProvider;
	this_->refreshCRL((verificaC19Sdk::ICRLStorage*)crlStorage);
}
