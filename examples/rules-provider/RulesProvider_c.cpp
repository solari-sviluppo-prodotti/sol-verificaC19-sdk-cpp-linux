// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <verificaC19-sdk/interfaces/IRulesProvider.hpp>

extern "C" {

#include <verificaC19-sdk/interfaces/RulesProvider.h>

}

void RulesProvider_c_release(const RulesProvider* rulesProvider) {
	verificaC19Sdk::IRulesProvider* this_ = (verificaC19Sdk::IRulesProvider*)rulesProvider;
	delete this_;
}

void RulesProvider_c_refreshRules(const RulesProvider* rulesProvider, RulesStorage* rulesStorage) {
	verificaC19Sdk::IRulesProvider* this_ = (verificaC19Sdk::IRulesProvider*)rulesProvider;
	this_->refreshRules((verificaC19Sdk::IRulesStorage*)rulesStorage);
}
