// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef IALL_PROVIDER_HPP
#define IALL_PROVIDER_HPP

#include <verificaC19-sdk/interfaces/IRulesProvider.hpp>
#include <verificaC19-sdk/interfaces/IKeysProvider.hpp>
#include <verificaC19-sdk/interfaces/ICRLProvider.hpp>

#include <string>

namespace verificaC19Sdk {

/**
 * Interface to implement all (Rules, Keys and CRL) Provider service to retrieve rules, keys and CRL from backend
 */
class IAllProvider : public IRulesProvider, IKeysProvider {

public:

	/**
	 * Refresh Rules requesting from backend and store in provided Rules storage
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to store
	 *                     retrieved rules
	 */
	virtual void refreshRules(IRulesStorage* rulesStorage) const { refreshAll(rulesStorage, NULL, NULL); }

	/**
	 * Refresh Keys requesting from backend and store in provided Keys storage
	 *
	 * @param keysStorage Implementation of Keys Storage service, used to store
	 *                    retrieved keys
	 */
	virtual void refreshKeys(IKeysStorage* keysStorage) const { refreshAll(NULL, keysStorage, NULL); }


	/**
	 * Refresh Keys requesting from backend and store in provided Keys storage
	 *
	 * @param crlStorage Implementation of CRL Storage service, used to store
	 *                   retrieved CRL
	 */
	virtual void refreshCRL(ICRLStorage* crlStorage) const { refreshAll(NULL, NULL, crlStorage); }

	/**
	 * Refresh Rules and Keys requesting from backend and store in provided Rules and Keys storage
	 *
	 * @param rulesStorage Implementation of Rules Storage service, used to store
	 *                     retrieved rules
	 *
	 * @param keysStorage Implementation of Keys Storage service, used to store
	 *                    retrieved keys
	 *
	 * @param crlStorage Implementation of CRL Storage service, used to store
	 *                   retrieved CRL
	 */
	virtual void refreshAll(IRulesStorage* rulesStorage, IKeysStorage* keysStorage, ICRLStorage* crlStorage) const = 0;

};

} // namespace verificaC19Sdk

#endif // #ifndef IALL_PROVIDER_HPP
