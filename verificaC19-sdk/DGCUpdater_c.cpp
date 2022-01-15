// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <unistd.h>
#include <verificaC19-sdk/DGCUpdater.hpp>

extern "C" {

#include <verificaC19-sdk/DGCUpdater.h>

}

Updater* DGCUpdater_c_create(int intervalSeconds,
		RulesProvider* rulesProvider, RulesStorage* rulesStorage,
		KeysProvider* keysProvider, KeysStorage* keysStorage,
		CRLProvider* crlProvider, CRLStorage* crlStorage,
		Logger* logger) {
	return (Updater*)new verificaC19Sdk::DGCUpdater(intervalSeconds,
			(verificaC19Sdk::IRulesProvider*)rulesProvider, (verificaC19Sdk::IRulesStorage*)rulesStorage,
			(verificaC19Sdk::IKeysProvider*)keysProvider, (verificaC19Sdk::IKeysStorage*)keysStorage,
			(verificaC19Sdk::ICRLProvider*)crlProvider, (verificaC19Sdk::ICRLStorage*)crlStorage,
			(verificaC19Sdk::ILogger*)logger);
}

Updater* DGCUpdaterAll_c_create(int intervalSeconds,
		AllProvider* allProvider, RulesStorage* rulesStorage,
		KeysStorage* keysStorage, CRLStorage* crlStorage, Logger* logger) {
	return (Updater*)new verificaC19Sdk::DGCUpdater(intervalSeconds,
			(verificaC19Sdk::IAllProvider*)allProvider, (verificaC19Sdk::IRulesStorage*)rulesStorage,
			(verificaC19Sdk::IKeysStorage*)keysStorage, (verificaC19Sdk::ICRLStorage*)crlStorage,
			(verificaC19Sdk::ILogger*)logger);
}

void DGCUpdater_c_release(Updater* dgcUpdater) {
	delete (verificaC19Sdk::DGCUpdater*)dgcUpdater;
}

void DGCUpdater_c_forceUpdateRules(Updater* dgcUpdater) {
	DGCUpdater_forceUpdateRules((verificaC19Sdk::DGCUpdater*)dgcUpdater);
}

void DGCUpdater_c_forceUpdateKeys(Updater* dgcUpdater) {
	DGCUpdater_forceUpdateKeys((verificaC19Sdk::DGCUpdater*)dgcUpdater);
}

void DGCUpdater_c_forceUpdateCRL(Updater* dgcUpdater) {
	DGCUpdater_forceUpdateCRL((verificaC19Sdk::DGCUpdater*)dgcUpdater);
}

void DGCUpdater_c_forceUpdateAll(Updater* dgcUpdater) {
	DGCUpdater_forceUpdateAll((verificaC19Sdk::DGCUpdater*)dgcUpdater);
}

bool DGCUpdater_c_isUpdated(Updater* dgcUpdater) {
	return DGCUpdater_isUpdated((verificaC19Sdk::DGCUpdater*)dgcUpdater);
}
