// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef ICRL_PROVIDER_HPP
#define ICRL_PROVIDER_HPP

#include <verificaC19-sdk/interfaces/ICRLStorage.hpp>

#include <string>

namespace verificaC19Sdk {

/**
 * Interface to implement CRL Provider service to retrieve CRL from backend
 */
class ICRLProvider {

public:

	/**
	 * Refresh CRL requesting from backend and store in provided CRL storage
	 *
	 * @param crlStorage Implementation of CRL Storage service, used to store
	 *                   retrieved CRL
	 */
	virtual void refreshCRL(ICRLStorage* crlStorage) const = 0;

};

} // namespace verificaC19Sdk

#endif // #ifndef ICRL_PROVIDER_HPP
