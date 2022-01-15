cmake_minimum_required(VERSION 2.8)
set(CMAKE_PROJECT_VERSION 1.1.2)

option(BUILD_EXAMPLES "Build example library and applications" OFF)
option(BUILD_STATIC "Build static library" OFF)

# Directory defines
set (DIR_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set (DIR_VERIFICAC19_SDK "${CMAKE_CURRENT_SOURCE_DIR}/verificaC19-sdk")
set (DIR_EXAMPLES "${CMAKE_CURRENT_SOURCE_DIR}/examples")
set (DIR_EXAMPLES_C "${CMAKE_CURRENT_SOURCE_DIR}/examples/c")
set (DIR_EXAMPLES_CLIENT "${CMAKE_CURRENT_SOURCE_DIR}/examples/client")
set (DIR_EXAMPLES_KEYS_PROVIDER "${CMAKE_CURRENT_SOURCE_DIR}/examples/keys-provider")
set (DIR_EXAMPLES_KEYS_STORAGE "${CMAKE_CURRENT_SOURCE_DIR}/examples/keys-storage")
set (DIR_EXAMPLES_RULES_PROVIDER "${CMAKE_CURRENT_SOURCE_DIR}/examples/rules-provider")
set (DIR_EXAMPLES_RULES_STORAGE "${CMAKE_CURRENT_SOURCE_DIR}/examples/rules-storage")
set (DIR_EXAMPLES_CRL_PROVIDER "${CMAKE_CURRENT_SOURCE_DIR}/examples/crl-provider")
set (DIR_EXAMPLES_CRL_STORAGE "${CMAKE_CURRENT_SOURCE_DIR}/examples/crl-storage")
set (DIR_EXAMPLES_LOGGERS "${CMAKE_CURRENT_SOURCE_DIR}/examples/loggers")

set (CXX_FLAGS_COMMON "$ENV{CXXFLAGS} -fPIC -O0")
set (CMAKE_CXX_FLAGS_RELEASE "${CXX_FLAGS_COMMON} -s")
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CXX_FLAGS_COMMON} -g")

set (C_FLAGS_COMMON "$ENV{CFLAGS} -O0")
set (CMAKE_C_FLAGS_RELEASE "${C_FLAGS_COMMON} -s")
set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${C_FLAGS_COMMON} -g")

set (CMAKE_BUILD_TYPE DEBUG)

include_directories(
	"${CMAKE_CURRENT_SOURCE_DIR}/${ROOT_SOURCE_DIR}"
	"${DIR_INCLUDE}"
	"${DIR_INCLUDE}/verificaC19-sdk/examples"
	"${DIR_EXAMPLES}"
	"${DIR_EXAMPLES_C}"
	"${DIR_EXAMPLES_KEYS_PROVIDER}"
	"${DIR_EXAMPLES_KEYS_STORAGE}"
	"${DIR_EXAMPLES_RULES_PROVIDER}"
	"${DIR_EXAMPLES_RULES_STORAGE}"
	"${DIR_EXAMPLES_CRL_PROVIDER}"
	"${DIR_EXAMPLES_CRL_STORAGE}"
	"${DIR_EXAMPLES_LOGGERS}"
)

aux_source_directory(${DIR_VERIFICAC19_SDK} SRC_VERIFICAC19_SDK)
set(SRC_CPP_EXAMPLE ${DIR_EXAMPLES}/verificaC19-sdk-example.cpp)
set(SRC_C_EXAMPLE ${DIR_EXAMPLES}/c/verificaC19-sdk-example.c)
set(SRC_CPP_CLIENT_EXAMPLE ${DIR_EXAMPLES}/verificaC19-sdk-client.cpp)
set(SRC_C_CLIENT_EXAMPLE ${DIR_EXAMPLES}/c/verificaC19-sdk-client.c)
aux_source_directory(${DIR_EXAMPLES_C} SRC_EXAMPLES_C)
aux_source_directory(${DIR_EXAMPLES_CLIENT} SRC_EXAMPLES_CLIENT)
aux_source_directory(${DIR_EXAMPLES_KEYS_PROVIDER} SRC_EXAMPLES_KEYS_PROVIDER)
aux_source_directory(${DIR_EXAMPLES_KEYS_STORAGE} SRC_EXAMPLES_KEYS_STORAGE)
aux_source_directory(${DIR_EXAMPLES_RULES_PROVIDER} SRC_EXAMPLES_RULES_PROVIDER)
aux_source_directory(${DIR_EXAMPLES_RULES_STORAGE} SRC_EXAMPLES_RULES_STORAGE)
aux_source_directory(${DIR_EXAMPLES_CRL_PROVIDER} SRC_EXAMPLES_CRL_PROVIDER)
aux_source_directory(${DIR_EXAMPLES_CRL_STORAGE} SRC_EXAMPLES_CRL_STORAGE)
aux_source_directory(${DIR_EXAMPLES_LOGGERS} SRC_EXAMPLES_LOGGERS)

if(BUILD_STATIC)

add_library(verificaC19-sdk STATIC ${SRC_VERIFICAC19_SDK})
set_target_properties(verificaC19-sdk PROPERTIES OUTPUT_NAME verificaC19-sdk)

else(BUILD_STATIC)

add_library(verificaC19-sdk SHARED ${SRC_VERIFICAC19_SDK})
set_target_properties(verificaC19-sdk PROPERTIES OUTPUT_NAME verificaC19-sdk)
set_target_properties(verificaC19-sdk PROPERTIES VERSION ${CMAKE_PROJECT_VERSION})
target_link_libraries(verificaC19-sdk -lcrypto -lz -lcn-cbor)
install(TARGETS verificaC19-sdk LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include)
INSTALL(DIRECTORY ${DIR_INCLUDE}/verificaC19-sdk DESTINATION include)

endif(BUILD_STATIC)

if(BUILD_EXAMPLES)

if(BUILD_STATIC)

add_library(verificaC19-common-examples STATIC ${SRC_EXAMPLES_KEYS_PROVIDER} ${SRC_EXAMPLES_KEYS_STORAGE} ${SRC_EXAMPLES_RULES_PROVIDER} ${SRC_EXAMPLES_RULES_STORAGE} ${SRC_EXAMPLES_CRL_PROVIDER} ${SRC_EXAMPLES_CRL_STORAGE} ${SRC_EXAMPLES_LOGGERS})
set_target_properties(verificaC19-common-examples PROPERTIES OUTPUT_NAME verificaC19-common-examples)

else(BUILD_STATIC)

add_library(verificaC19-common-examples SHARED ${SRC_EXAMPLES_KEYS_PROVIDER} ${SRC_EXAMPLES_KEYS_STORAGE} ${SRC_EXAMPLES_RULES_PROVIDER} ${SRC_EXAMPLES_RULES_STORAGE} ${SRC_EXAMPLES_CRL_PROVIDER} ${SRC_EXAMPLES_CRL_STORAGE} ${SRC_EXAMPLES_LOGGERS})
set_target_properties(verificaC19-common-examples PROPERTIES OUTPUT_NAME verificaC19-common-examples)
set_target_properties(verificaC19-common-examples PROPERTIES VERSION ${CMAKE_PROJECT_VERSION})
target_link_libraries(verificaC19-common-examples -lcurl -ljsoncpp -lpthread)
install(TARGETS verificaC19-common-examples LIBRARY DESTINATION lib PUBLIC_HEADER DESTINATION include)

endif(BUILD_STATIC)

add_executable(verificaC19-example ${SRC_CPP_EXAMPLE})
target_link_libraries (verificaC19-example verificaC19-sdk verificaC19-common-examples -lcurl -ljsoncpp -lcrypto -lz -lcn-cbor -lpthread -ldl)
install(TARGETS verificaC19-example ARCHIVE DESTINATION bin RUNTIME DESTINATION bin)

add_executable(verificaC19-c-example ${SRC_C_EXAMPLE})
target_link_libraries (verificaC19-c-example verificaC19-sdk verificaC19-common-examples -lcurl -ljsoncpp -lcrypto -lz -lcn-cbor -lpthread -ldl)
install(TARGETS verificaC19-c-example ARCHIVE DESTINATION bin RUNTIME DESTINATION bin)

add_executable(verificaC19-client ${SRC_CPP_CLIENT_EXAMPLE})
target_link_libraries (verificaC19-client verificaC19-sdk verificaC19-common-examples -lcurl -ljsoncpp -lcrypto -lz -lcn-cbor -lpthread)
install(TARGETS verificaC19-client ARCHIVE DESTINATION bin RUNTIME DESTINATION bin)

add_executable(verificaC19-c-client ${SRC_C_CLIENT_EXAMPLE})
target_link_libraries (verificaC19-c-client verificaC19-sdk verificaC19-common-examples -lcurl -ljsoncpp -lcrypto -lz -lcn-cbor -lpthread)
install(TARGETS verificaC19-c-client ARCHIVE DESTINATION bin RUNTIME DESTINATION bin)

endif(BUILD_EXAMPLES)
