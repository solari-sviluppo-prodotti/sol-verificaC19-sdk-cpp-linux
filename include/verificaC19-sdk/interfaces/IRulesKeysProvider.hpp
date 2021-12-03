// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef IRULES_KEYS_PROVIDER_HPP
#define IRULES_KEYS_PROVIDER_HPP

#include <verificaC19-sdk/interfaces/IRulesProvider.hpp>
#include <verificaC19-sdk/interfaces/IKeysProvider.hpp>

#ifdef __cplusplus

#include <string>

namespace verificaC19Sdk {

/**
 * Interface to implement Rules and Key Provider service to retreive rules and keys from backend
 */
class IRulesKeysProvider : public IRulesProvider, IKeysProvider {

public:

	/**
	 * Refresh Rules requesting from backend and store in provided Rules storage
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to store
	 *                     retreived rules
	 */
	virtual void refreshRules(IRulesStorage* rulesStorage) const { refreshAll(rulesStorage, NULL); }

	/**
	 * Refresh Keys requesting from backend and store in provided Keys storage
	 *
	 * @param keysStorage Implementation of Keys Storage service, used to store
	 *                    retreived keys
	 */
	virtual void refreshKeys(IKeysStorage* keysStorage) const { refreshAll(NULL, keysStorage); }

	/**
	 * Refresh Rules and Keys requesting from backend and store in provided Rules and Keys storage
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to store
	 *                     retreived rules
	 *
	 * @param keysStorage Implementation of Keys Storage service, used to store
	 *                    retreived keys
	 */
	virtual void refreshAll(IRulesStorage* rulesStorage, IKeysStorage* keysStorage) const = 0;

};

} // namespace verificaC19Sdk

#endif // #ifdef __cplusplus

#endif // #ifndef IRULES_KEYS_PROVIDER_HPP
