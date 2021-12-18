// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef IRULES_PROVIDER_HPP
#define IRULES_PROVIDER_HPP

#include <verificaC19-sdk/interfaces/IRulesStorage.hpp>

#include <string>

namespace verificaC19Sdk {

/**
 * Interface to implement Rules Provider service to retreive rules from backend
 */
class IRulesProvider {

public:

	/**
	 * Refresh Rules requesting from backend and store in provided Rules storage
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to store
	 *                     retreived rules
	 */
	virtual void refreshRules(IRulesStorage* rulesStorage) const = 0;

};

} // namespace verificaC19Sdk

#endif // #ifndef IRULES_PROVIDER_HPP
