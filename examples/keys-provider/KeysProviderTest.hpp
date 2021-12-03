// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef KEYS_PROVIDER_TEST_HPP
#define KEYS_PROVIDER_TEST_HPP

#include <verificaC19-sdk/interfaces/IKeysProvider.hpp>
#include <verificaC19-sdk/interfaces/ILogger.hpp>

#ifdef __cplusplus

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

#endif // #ifdef __cplusplus

#ifdef __cplusplus
extern "C" {
#endif

void* KeysProviderTest_c_create(void* logger);
typedef void* (*pfKeysProviderTest_c_create)(void*);

void KeysProviderTest_c_release(const void* keysProviderTest);
typedef void (*pfKeysProviderTest_c_release)(const void*);

void KeysProviderTest_c_refreshKeys(const void* keysProviderTest, void* keysStorage);
typedef void (*pfKeysProviderTest_c_refreshKeys)(const void*, void*);

#ifdef __cplusplus
}
#endif

#endif // #ifndef KEYS_PROVIDER_TEST_HPP
