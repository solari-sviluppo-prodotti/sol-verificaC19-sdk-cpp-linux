# sol-verificaC19-cpp-linux


### C++ library for decoding and validate the European Digital Green Certificate

#### Summary
The library allows to decode and validate any EU Digital Green Certificate,
providing interfaces to easily implement specific providers for every country's backend.

#### Dependencies

The following dependencies are used in the project by the verifier SDK:
- **[zlib](https://github.com/madler/zlib) 1.2.11.** Zlib is a general-purpose, patent-free, lossless data compression library which is used by many different programs.
- **[cn-cbor](https://github.com/jimsch/cn-cbor) 1.0.0.** A constrained node implementation of CBOR in C.
- **[libcrypto](https://github.com/openssl/openssl) at least 1.0.2j.** OpenSSL is a robust, commercial-grade, full-featured Open Source Toolkit for the Transport Layer Security (TLS) protocol formerly known as the Secure Sockets Layer (SSL) protocol.

The following dependencies are used in the project by the examples:
- **[libcurl](https://github.com/curl/curl) at least 7.61.0.** Library for transferring data specified with URL syntax.
- **[jsoncpp](https://github.com/open-source-parsers/jsoncpp) at least 1.8.4.** JSON is a lightweight data-interchange format. It can represent numbers, strings, ordered sequences of values, and collections of name/value pairs.

#### Build

Once cloned, to build on your system you can use this command, will compile and install libverificaC19-sdk.so:

```sh
cmake .
make
make install # as root or sudoer
```

If you want to build also with examples and create also examples library you can use this command,
will compile and install libverificaC19-sdk.so and libverificaC19-common-examples.so:

```sh
cmake . -DBUILD_EXAMPLES=ON
make
make install # as root or sudoer
```


If you want to build static libraries (with or without examples) you can use this command,
will compile and install libverificaC19-sdk.a and (if enabled) libverificaC19-common-examples.a:

```sh
cmake . -DBUILD_STATIC=ON
make
make install # as root or sudoer
```

#### Tested architectures

This SDK has been tested in these architectures:
- x86-64
- ARM v4T
- ARM v7

#### Usage

The main entry point of the library is the `DGCVerifier` class.

This class needs a implementation of IKeysStorage, IRulesStorage and ICRLStorage interfaces to access to storage.

Optionally you can use a implementation of ILogger interface to log operations.

You can simply instantiate and configure it:

```cpp
#include <verificaC19-sdk/examples/keys-storage/KeysStorageFile.hpp>
#include <verificaC19-sdk/examples/rules-storage/RulesStorageFile.hpp>
#include <verificaC19-sdk/examples/crl-storage/CRLStorageFile.hpp>
#include <verificaC19-sdk/examples/loggers/LoggerStdout.hpp>
#include <verificaC19-sdk/DGCVerifier.hpp>

LoggerStdout logger(DEBUG);
KeysStorageFile keysStorage;
RulesStorageFile rulesStorage;
CRLStorageFile crlStorage;

DGCVerifier verifier(&keysStorage, &rulesStorage, &crlStorage, &logger);
```

Once instantiated and configured with storage services, you can simply use it
calling the following methods:

```cpp
...
if (!verifier.verifyMinSdkVersion()) {
  logger.error("Minimum SDK version does not match");
}

// Setup scan mode
verifier.setScanMode(SCAN_MODE_2G);

std::string qrCode = "Raw qr code data starting with HC1:";

// Decode and validate the qr code data.
// The result will contain all the details of the validated object
CertificateSimple result = verifier.verify(qrCode);

enum CertificateStatus status = result.certificateStatus;
...

```

The rules, keys and CRL updater process is managed by th `DGCUpdater` class.

This class needs a implementation of IKeysStorage, IRulesStorage and ICRLStorage interfaces to access to storage
and IKeysProvider, IRulesProvider and ICRLProvider interfaces to access to providers.

Optionally you can use a implementation of ILogger interface to log operations.

You can simply instantiate and configure it:

```cpp
#include <verificaC19-sdk/examples/keys-storage/KeysStorageFile.hpp>
#include <verificaC19-sdk/examples/keys-provider/KeysProviderItaly.hpp>
#include <verificaC19-sdk/examples/rules-storage/RulesStorageFile.hpp>
#include <verificaC19-sdk/examples/rules-provider/RulesProviderItaly.hpp>
#include <verificaC19-sdk/examples/crl-storage/CRLStorageFile.hpp>
#include <verificaC19-sdk/examples/crl-provider/CRLProviderItaly.hpp>
#include <verificaC19-sdk/examples/loggers/LoggerStdout.hpp>
#include <verificaC19-sdk/DGCUpdater.hpp>

LoggerStdout logger(DEBUG);
KeysStorageFile keysStorage;
RulesStorageFile rulesStorage;
CRLStorageFile crlStorage;
KeysProviderItaly keysProvider(&logger);
RulesProviderItaly rulesProvider(&logger);
CRLProviderItaly crlProvider(&logger);

DGCUpdater updater(86400, &rulesProvider, &rulesStorage,
		&keysProvider, &keysStorage, &crlProvider, &crlStorage, &logger);
```

so the complete example with rules and keys update and DGC verify is:

```cpp
#include <verificaC19-sdk/examples/keys-storage/KeysStorageFile.hpp>
#include <verificaC19-sdk/examples/keys-provider/KeysProviderItaly.hpp>
#include <verificaC19-sdk/examples/rules-storage/RulesStorageFile.hpp>
#include <verificaC19-sdk/examples/rules-provider/RulesProviderItaly.hpp>
#include <verificaC19-sdk/examples/crl-storage/CRLStorageFile.hpp>
#include <verificaC19-sdk/examples/crl-provider/CRLProviderItaly.hpp>
#include <verificaC19-sdk/examples/loggers/LoggerStdout.hpp>
#include <verificaC19-sdk/DGCVerifier.hpp>
#include <verificaC19-sdk/DGCRulesKeysUpdater.hpp>

// Initialization (at startup)

LoggerStdout logger(DEBUG);
KeysStorageFile keysStorage;
RulesStorageFile rulesStorage;
CRLStorageFile crlStorage;
KeysProviderItaly keysProvider(&logger);
RulesProviderItaly rulesProvider(&logger);
CRLProviderItaly crlProvider(&logger);

// This automatically updates rules, keys and CRL every 86400 seconds
DGCUpdater updater(86400, &rulesProvider, &rulesStorage,
		&keysProvider, &keysStorage, &crlProvider, &crlStorage, &logger);

DGCVerifier verifier(&keysStorage, &rulesStorage, &crlStorage, &logger);

// At startup wait to have a valid set of rules and keys, if this is
// first application launch the storage cuold be empty
if (!updater.isUpdated()) {
	// Signal to user that rules and keys is updating
	while (!updater.isUpdated()) {
		usleep(10000);
	}
	// Signal to user that rules, keys and CRL was updated
}


// Usage (for every read Digital Certificate)

if (!verifier.verifyMinSdkVersion()) {
  logger.error("Minimum SDK version does not match");
}

// Setup scan mode
verifier.setScanMode(SCAN_MODE_2G);

std::string qrCode = "Raw qr code data starting with HC1:";

// Decode and validate the qr code data.
// The result will contain all the details of the validated object
CertificateSimple result = verifier.verify(qrCode);

enum CertificateStatus status = result.certificateStatus;
```

#### Rules, Keys and CRL storage

Rules, Keys and CRL storage are mandatory services and must be implemented using
IRulesStorage, IKeysStorage and ICRLStorage interfaces.

SDK provide:
- full working example implementations to store Rules, Keys and CRL in memory,
(`examples/rules-storage/RulesStorageMemory.hpp /.cpp` and
 `examples/keys-storage/KeysStorageMemory.hpp /.cpp` and
 `examples/crl-storage/CRLStorageMemory.hpp /.cpp`)
- full working example implementations to store Rules, Keys and CRL in json File,
(`examples/rules-storage/RulesStorageFile.hpp /.cpp` and
 `examples/keys-storage/KeysStorageFile.hpp /.cpp` and
 `examples/crl-storage/CRLStorageFile.hpp /.cpp`)

but you can implement your storage in File or in Database.

Interfaces declare methods to:
- store Rule, Key or CRL without transaction
- store Rules, Keys or CRL in transaction, to use "old" Rules, Keys or CRL until update is done
- get info about Rules, Keys or CRL update in progress
- get info about Rules, Keys or CRL last update time
- get info about last Key token stored

#### Rules, Keys and CRL provider

Rules, Keys and CRL update provider are an optional services used to update Rules, Keys and CRL
and can be implemented using IRulesProvider, IKeysProbider, ICRLProvider or IAllProvider interfaces.

SDK provide full working example implementations to update Rules, Keys and CRL from Italian provider,
(`examples/rules-provider/RulesProviderItaly.hpp /.cpp` and
 `examples/keys-provider/KeysProviderItaly.hpp /.cpp` and
 `examples/crl-provider/CRLProviderItaly.hpp /.cpp`)
but you can implement provider to get Rules, Keys or CRL from other states or by your custom provider.

Interfaces declare methods to refresh Rules, Keys, CRL or All (IAllProvider).

Interface methods request instance of Rules, Keys or CRL storage interface as described above.

#### Logger

SDK contemplate use of logger with interface ILogger, but you can easily set
a NULL pointer to this to not have log or implement you logger.

SDK provide example implementation of standard output logger (`examples/loggers/LoggerStdout.hpp / .cpp`).

### Application examples

SDK provides also example of C++ (`examples/verificaC19-sdk-example.cpp`)
and C (`examples/c/verificaC19-sdk-example.c`) applications.

SDK provides also simple client of C++ (`examples/verificaC19-sdk-client.cpp`)
and C (`examples/c/verificaC19-sdk-client.c`) applications.

#### Disclaimer
This library is **not** an official implementation, therefore its use may be subject to restrictions by some countries regulations.
The author assumes no responsibility for any unauthorized use of the library and no warranties about the correctness of the implementation, as better stated in the License.


#### Licensing

Licensed under the **Apache License, Version 2.0** (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at https://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the [LICENSE](./LICENSE) for the specific language governing permissions and limitations under the License.
