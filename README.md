# sol-verificaC19-cpp-linux


### C++ library for decoding and validate the European Digital Green Certificate

#### Summary
The library allows to decode and validate any EU Digital Green Certificate,
providing interfaces to easily implement specific providers for every country's backend.

#### Dependencies

The following dependencies are used in the project by the verifier SDK:
- **[zlib](https://github.com/madler/zlib).** Zlib is a general-purpose, patent-free, lossless data compression library which is used by many different programs.
- **[cn-cbor](https://github.com/jimsch/cn-cbor).** A constrained node implementation of CBOR in C.
- **[libcrypto](https://github.com/openssl/openssl).** OpenSSL is a robust, commercial-grade, full-featured Open Source Toolkit for the Transport Layer Security (TLS) protocol formerly known as the Secure Sockets Layer (SSL) protocol.

The following dependencies are used in the project by the examples:
- **[libcurl](https://github.com/curl/curl).** Library for transferring data specified with URL syntax.
- **[jsoncpp](https://github.com/open-source-parsers/jsoncpp).** JSON is a lightweight data-interchange format. It can represent numbers, strings, ordered sequences of values, and collections of name/value pairs.

#### Build

Once cloned, to build on your system you can use this command:

```
cmake .
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

This class needs a implementation of IKeysStorage and IRulesStorage interfaces to access to storage.

Optionally you can use a implementation of ILogger interface to log operations.

You can simply instantiate and configure it:

```
#include <verificaC19-sdk/DGCVerifier.hpp>

LoggerStdout logger(DEBUG);
KeysStorageMemory keysStorage;
RulesStorageMemory rulesStorage;

DGCVerifier verifier(&keysStorage, &rulesStorage, &logger);
```

Once instantiated and configured with storage services, you can simply use it
calling the following methods:

```
...
if (!verifier.verifyMinSdkVersion()) {
  logger.error("Minimum SDK version does not match");
}

std::string qrCode = "Raw qr code data starting with HC1:";

// Decode and validate the qr code data.
// The result will contain all the details of the validated object
CertificateSimple result = verifier.verify(qrCode);

enum CertificateStatus status = result.certificateStatus;
...

```

#### Rules and Keys storage

Rules and Keys storage are mandatory services and must be implemented using
IRulesStorage and IKeysStorage interfaces.

SDK provide example implementations to store Rules and Keys in memory,
(RulesStorageMemory and KeysStorageMemory) but you can implement storage in File or in Database.

Interfaces declare methods to:
- store Rule or Key without transaction
- store Rules or Keys in transaction, to use "old" Rules or Keys until update is done
- get info about Rules or Keys update in progress
- get info about Rules or Keys last update time
- get info about last Key token stored

#### Rules and Keys provider

Rules and Keys update provider are an optional services used to update Keys and Rules
and can be implemented using IRulesProvider, IKeysProbider or IRulesKeysProvider interfaces.

SDK provide example implementations to update Rules and Keys from Italian provider,
(RulesProviderItaly and KeysProvuderItaly) but you can implement provider
to get Rules or Keys from other states or by your custom provider.

Interfaces declare methods to refresh Rules or Keys or All (IRulesKeysProvider).

Interface methods request instance of Rules or Keys storage interface as described above.

#### Logger

SDK contemplate use of logger with interface ILogger, but you can easily set
a NULL pointer to this to not have log or implement you logger.

We provide example implementation of standard output logger.

#### Disclaimer
This library is **not** an official implementation, therefore its use may be subject to restrictions by some countries regulations.
The author assumes no responsibility for any unauthorized use of the library and no warranties about the correctness of the implementation, as better stated in the License.


#### Licensing

Licensed under the **Apache License, Version 2.0** (the "License"); you may not use this file except in compliance with the License.

You may obtain a copy of the License at https://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the [LICENSE](./LICENSE) for the specific language governing permissions and limitations under the License.
