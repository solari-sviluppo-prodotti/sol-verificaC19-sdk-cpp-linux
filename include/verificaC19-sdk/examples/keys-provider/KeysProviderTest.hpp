// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_TEST_HPP
#define KEYS_PROVIDER_TEST_HPP

#include <verificaC19-sdk/interfaces/IKeysProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

#include <string>
#include <map>

namespace verificaC19Sdk {

/**
 * Implements provider to store test keys
 */
class KeysProviderTest : public IKeysProvider {

public:

	/**
	 * @param logger Optional implementation of Logger, if NULL no log will be done
	 */
	KeysProviderTest(ILogger* logger);
	void refreshKeys(IKeysStorage* keysStorage) const;

private:

	ILogger* m_logger;

};

} // namespace verificaC19Sdk

#endif // #ifndef KEYS_PROVIDER_TEST_HPP
