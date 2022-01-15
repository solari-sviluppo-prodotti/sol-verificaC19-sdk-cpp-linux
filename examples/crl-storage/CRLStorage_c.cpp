// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <string.h>
#include <stdlib.h>
#include <verificaC19-sdk/interfaces/ICRLStorage.hpp>

extern "C" {

#include <verificaC19-sdk/interfaces/CRLStorage.h>

}

void CRLStorage_c_release(const CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	delete this_;
}

bool CRLStorage_c_hasCertificateHash(const CRLStorage* crlStorage, const char* certificateHash) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	return this_->hasCertificateHash(std::string(certificateHash));
}

void CRLStorage_c_beginUpdatingCRL(const CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	this_->beginUpdatingCRL();
}

void CRLStorage_c_storeChunk(const CRLStorage* crlStorage, const char* chunk) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	return this_->storeChunk(std::string(chunk));
}

void CRLStorage_c_clearCRL(const CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	this_->clearCRL();
}

void CRLStorage_c_commitUpdatedCRL(const CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	this_->commitUpdatedCRL();
}

void CRLStorage_c_rollbackUpdatedCRL(const CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	this_->rollbackUpdatedCRL();
}

bool CRLStorage_c_isUpdating(const CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	return this_->isUpdating();
}

int CRLStorage_c_version(const CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	return this_->version();
}

const char* CRLStorage_c_downloadInfo(const CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	std::string downloadInfo = this_->downloadInfo();
	char* p = (char*)malloc(downloadInfo.length() + 1);
	strcpy(p, downloadInfo.c_str());
	return p;
}

time_t CRLStorage_c_lastUpdate(const CRLStorage* crlStorage) {
	verificaC19Sdk::ICRLStorage* this_ = (verificaC19Sdk::ICRLStorage*)crlStorage;
	return this_->lastUpdate();
}

