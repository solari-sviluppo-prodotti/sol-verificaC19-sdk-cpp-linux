// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef IKEYS_PROVIDER_HPP
#define IKEYS_PROVIDER_HPP

#include <verificaC19-sdk/interfaces/IKeysStorage.hpp>

#include <string>

namespace verificaC19Sdk {

/**
 * Interface to implement Key Provider service to retrieve keys from backend
 */
class IKeysProvider {

public:

	/**
	 * Refresh Keys requesting from backend and store in provided Keys storage
	 *
	 * @param keysStorage Implementation of Keys Storage service, used to store
	 *                    retrieved keys
	 */
	virtual void refreshKeys(IKeysStorage* keysStorage) const = 0;

};

} // namespace verificaC19Sdk

#endif // #ifndef IKEYS_PROVIDER_HPP
