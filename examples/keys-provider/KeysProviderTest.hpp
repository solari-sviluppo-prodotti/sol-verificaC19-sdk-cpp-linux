// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_TEST_HPP
#define KEYS_PROVIDER_TEST_HPP

#include <verificaC19-sdk/interfaces/IKeysProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

#include <string>
#include <map>

namespace verificaC19Sdk {

class KeysProviderTest : public IKeysProvider {

public:

	KeysProviderTest(ILogger* logger);
	void refreshKeys(IKeysStorage* keysStorage) const;

private:

	ILogger* m_logger;

};

} // namespace verificaC19Sdk

extern "C" {

#include <examples/keys-provider/KeysProviderTest.h>

} // extern "C"

#endif // #ifndef KEYS_PROVIDER_TEST_HPP
