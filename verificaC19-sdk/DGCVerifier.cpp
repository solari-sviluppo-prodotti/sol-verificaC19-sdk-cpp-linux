// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <ctype.h>
#include <string.h>

#include <ctime>
#include <locale>
#include <string>
#include <set>
#include <vector>
#include <sstream>

#include <zlib.h>

#include <cn-cbor/cn-cbor.h>

#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/err.h>
#include <openssl/bn.h>
#include <openssl/ecdsa.h>

#if OPENSSL_VERSION_NUMBER < 0x10100000L
#define ECDSA_SIG_set0(signature, r, s) signature->r = r; signature->s = s;
#define X509_get_extension_flags(x) (x->ex_flags)
#define X509_get_extended_key_usage(x)  (x->ex_xkusage)
#endif

#include <verificaC19-sdk/models/CertificateModel.hpp>
#include <verificaC19-sdk/DGCVerifier.hpp>

namespace verificaC19Sdk {

#define COUNTRY_SAN_MARINO        "SM"
#define COUNTRY_ITALY             "IT"

#define WORK_VACCINE_MANDATORY_AGE  50

// BASE45
static std::string decodeBase45(const std::string& src) {
	// map used to get real value for every input character and also to validate
	// the input character
	static signed char _C2I[256] = {
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		36, -1,-1,-1,  37, 38,-1,-1, -1,-1, 39, 40, -1, 41, 42, 43,
		0,   1,  2,  3,   4,  5,  6,  7,   8,  9, 44,-1, -1,-1,-1,-1,

		-1, 10, 11, 12,  13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24,
		25, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 35, -1,-1,-1,-1,
		-1, 10, 11, 12,  13, 14, 15, 16,  17, 18, 19, 20,  21, 22, 23, 24,
		25, 26, 27, 28,  29, 30, 31, 32,  33, 34, 35, 35, -1,-1,-1,-1,

		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,

		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
		-1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1, -1,-1,-1,-1,
	};

	std::stringstream decoded;

	// loop for each group of three characters (last group can have only two characters)
	for(int i = 0; i < src.length(); i += 3) {
		long x;
		int a,b;

		// to decode base45 data we must have at least two characters
		if (src.length() - i < 2)
			return "";

		// validate the two charatres read, must be in base45 charset interval
		if ((-1 == (a = _C2I[src[i]])) || (-1 == (b = _C2I[src[i+1]])))
			return "";

		// calculate output value
		x = a + 45 * b;

		// if we have at least three characters, get also the third character
		if (src.length() - i >= 3) {
			// validate the third character read, must be in base45 charset interval
			if (-1 == (a = _C2I[src[i+2]]))
				return "";

			// calculate output value
			x += a * 45 * 45;

			// check output value range
			if (x > 0xFFFF)
				return "";

			// write high part of output value
			decoded << (char)(x / 256);
			x %= 256;
		} else {
			// check output value range
			if (x > 0xFF)
				return "";
		}

		// write low part of output value
		decoded << (char)x;
	};

	return decoded.str();
}

// BASE64
static std::string encodeBase64Block(const unsigned char* in, size_t len) {
	static const char cb64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	unsigned char lin[3];
	memset(lin, 0, sizeof(lin));
	memcpy(lin, in, len);
	std::stringstream encoded;
	encoded << cb64[lin[0] >> 2];
	encoded << cb64[((lin[0] & 0x03) << 4) | ((lin[1] & 0xf0) >> 4)];
	encoded << (len > 1 ? cb64[((lin[1] & 0x0f) << 2) | ((lin[2] & 0xc0) >> 6)] : '=');
	encoded << (len > 2 ? cb64[lin[2] & 0x3f] : '=');
	return encoded.str();
}

static std::string encodeBase64(const unsigned char* in, size_t len) {
	std::stringstream encoded;

	for (int i = 0; i < len; i += 3) {
		if (len - i > 3) {
			encoded << encodeBase64Block(in + i, 3);
		} else {
			encoded << encodeBase64Block(in + i, len - i);
		}
	}

	return encoded.str();
}

static size_t decodeBase64(const std::string& in, unsigned char* out) {
	static const int cd64[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
	56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
	7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
	0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

	size_t len = 0;
	for (size_t i = 0; i < in.length(); i += 4)
	{
		int n = cd64[in[i]] << 18 | cd64[in[i + 1]] << 12 | cd64[in[i + 2]] << 6 | cd64[in[i + 3]];
		out[len++] = n >> 16;
		out[len++] = n >> 8 & 0xFF;
		out[len++] = n & 0xFF;
	}
	return len;
}

// UTILS
static std::vector<std::string> splitToVector(const std::string& s, char delimiter) {
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}
	if (token.empty()) { tokens.push_back(token); }
	return tokens;
}

static std::set<std::string> splitToSet(const std::string& s, char delimiter) {
	std::set<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter)) {
		tokens.insert(token);
	}
	if (token.empty()) { tokens.insert(token); }
	return tokens;
}

class LoggerNull : public ILogger {

public:

	void error(const std::string& message, ...) const { }
	void info(const std::string& message, ...) const { }
	void debug(const std::string& message, ...) const { }

};

DGCVerifier::DGCVerifier(IKeysStorage* keysStorage, IRulesStorage* rulesStorage, ICRLStorage* crlStorage, ILogger* logger) :
		m_keysStorage(keysStorage), m_rulesStorage(rulesStorage), m_crlStorage(crlStorage), m_logger(logger) {
	if (m_logger == NULL) {
		m_logger = new LoggerNull();
	}
}

bool DGCVerifier::verifyMinSdkVersion() const {
	const std::string sdkMinVersion = m_rulesStorage->getRule(RULE_NAME_sdk, RULE_TYPE_SDK_MIN_VERSION);
	if (!sdkMinVersion.empty()) {
		std::vector<std::string> sdkMinVersionVector = splitToVector(sdkMinVersion, '.');
		std::vector<std::string> sdkVersionVector = splitToVector(DGCVerifier_SDK_Version, '.');
		if (sdkMinVersionVector.size() == sdkVersionVector.size()) {
			for (int i = 0; i < sdkVersionVector.size(); i++) {
				if (atoi(sdkVersionVector[i].c_str()) < atoi(sdkMinVersionVector[i].c_str())) {
					// stop loop, this SDK version is lower than minimum SDK version
					return false;
				} else if (atoi(sdkVersionVector[i].c_str()) > atoi(sdkMinVersionVector[i].c_str())) {
					// stop loop, this SDK version is bigger than minimum SDK version
					break;
				}
			}
		}
	}
	return true;
}

CertificateSimple DGCVerifier::verify(const std::string& dgcQr, const std::string& scanMode) const {

	CertificateSimple certificateSimple;

	do {
		if (dgcQr.substr(0, 4) != "HC1:") {
			m_logger->error("No Green Pass QR code detected");
			break;
		}

		try {
			CertificateModel certificate;

			unsigned char* decbuf = NULL;
			cn_cbor* cbordata = NULL;

			do {
				// Data decode -------------------------------------------------
				std::string comprbuf = decodeBase45(dgcQr.substr(4));
				if (comprbuf.empty()) {
					m_logger->error("Error decoding Green Pass base45");
					break;
				}
				m_logger->info("Decoded Green Pass to %d bytes", comprbuf.length());

				unsigned long declen = comprbuf.length() * 2;
				unsigned char* decbuf = (unsigned char*)malloc(declen);

				/* ZLIB */
				z_stream infstream;
				infstream.zalloc = Z_NULL;
				infstream.zfree = Z_NULL;
				infstream.opaque = Z_NULL;
				infstream.avail_in = comprbuf.length();
				infstream.next_in = (Bytef*)comprbuf.c_str();
				infstream.avail_out = declen;
				infstream.next_out = decbuf;

				if (inflateInit(&infstream) != Z_OK) {
					m_logger->error("Error inflateInit");
					break;
				}

				if (inflate(&infstream, Z_NO_FLUSH) != Z_STREAM_END) {
					m_logger->error("Error inflate");
					break;
				}

				if (inflateEnd(&infstream) != Z_OK) {
					m_logger->error("Error inflateEnd");
					break;
				}

				declen = declen - infstream.avail_out;

				m_logger->debug("Decompressed Green Pass in %d bytes", declen);

				/* CBOR */
				struct cn_cbor_errback back;
				cbordata = cn_cbor_decode(decbuf, declen, &back);
				if (cbordata == NULL) {
					m_logger->error("Error cbordata");
					break;
				}

				cn_cbor* cborarray = cn_cbor_index(cbordata, 0);
				if (cborarray == NULL) {
					m_logger->error("Error cborarray");
					break;
				}

				if (cborarray->type == CN_CBOR_BYTES) {
					cborarray = cbordata;
				}
				cn_cbor* cborprotected = cn_cbor_index(cborarray, 0);
				if (cborprotected == NULL) {
					m_logger->error("Error cborprotected");
					break;
				}

				cn_cbor* cborunprotected = cn_cbor_index(cborarray, 1);
				if (cborunprotected == NULL) {
					m_logger->error("Error cborunprotected");
					break;
				}

				cn_cbor* cborsign = cn_cbor_index(cborarray, 3);
				if (cborsign == NULL) {
					m_logger->error("Error cborsign");
					break;
				}

				cn_cbor* cborprotecteddec = cn_cbor_decode(cborprotected->v.bytes, cborprotected->length, &back);
				cn_cbor* cboralgo = NULL;
				cn_cbor* cborkid = NULL;
				if (cborprotecteddec != NULL) {
					cborkid = cn_cbor_mapget_int(cborprotecteddec, 4);
					cboralgo = cn_cbor_mapget_int(cborprotecteddec, 1);
				}

				if (cborkid == NULL) {
					cborkid = cn_cbor_mapget_int(cborunprotected, 4);
				}
				if (cborkid == NULL) {
					m_logger->error("Error cborkid");
					break;
				}

				if (cboralgo == NULL) {
					cboralgo = cn_cbor_mapget_int(cborunprotected, 1);
				}
				if (cboralgo == NULL) {
					m_logger->error("Error cboralgo");
					break;
				}

				cn_cbor* cborpayload = cn_cbor_index(cborarray, 2);
				if (cborpayload == NULL) {
					m_logger->error("Error cborpayload");
					break;
				}

				cn_cbor* cborpayloaddec = cn_cbor_decode(cborpayload->v.bytes, cborpayload->length, &back);
				if (cborpayloaddec == NULL) {
					m_logger->error("Error cborpayloaddec");
					break;
				}

				cn_cbor* gpissuer = cn_cbor_mapget_int(cborpayloaddec, 1);
				if (gpissuer == NULL) {
					m_logger->error("Error gpissuer");
					break;
				}

				cn_cbor* gpexpiry = cn_cbor_mapget_int(cborpayloaddec, 4);
				if (gpexpiry == NULL) {
					m_logger->error("Error gpexpiry");
					break;
				}

				cn_cbor* gpgenerated = cn_cbor_mapget_int(cborpayloaddec, 6);
				if (gpgenerated == NULL) {
					m_logger->error("Error gpgenerated");
					break;
				}

				cn_cbor* gpdata = cn_cbor_mapget_int(cborpayloaddec, -260);
				if (gpdata == NULL) {
					m_logger->error("Error gpdata");
					break;
				}

				cn_cbor* gpdata2 = cn_cbor_mapget_int(gpdata, 1);
				if (gpdata2 == NULL) {
					m_logger->error("Error gpdata2");
					break;
				}

				cn_cbor* gpnam = cn_cbor_mapget_string(gpdata2, "nam");
				if (gpnam == NULL) {
					m_logger->error("Error gpnam");
					break;
				}

				cn_cbor* gpv = cn_cbor_mapget_string(gpdata2, "v"); // vaccined
				cn_cbor* gpr = cn_cbor_mapget_string(gpdata2, "r"); // recovered
				cn_cbor* gpt = cn_cbor_mapget_string(gpdata2, "t"); // tested
				cn_cbor* gpe = cn_cbor_mapget_string(gpdata2, "e"); // exemption

				cn_cbor* gpvtr = NULL;
				std::string typevtr;
				if (gpv != NULL) {
					typevtr = "v";
					gpvtr = gpv->last_child;
				}
				if (gpr != NULL) {
					typevtr = "r";
					gpvtr = gpr->last_child;
				}
				if (gpt != NULL) {
					typevtr = "t";
					gpvtr = gpt->last_child;
				}
				if (gpe != NULL) {
					typevtr = "e";
					gpvtr = gpe->last_child;
				}
				if (gpvtr == NULL) {
					m_logger->error("Error gpvtr");
					break;
				}

				cn_cbor* gpver = cn_cbor_mapget_string(gpdata2, "ver");
				if (gpver == NULL) {
					m_logger->error("Error gpver");
					break;
				}

				cn_cbor* gpdob = cn_cbor_mapget_string(gpdata2, "dob");
				if (gpdob == NULL) {
					m_logger->error("Error gpdob");
					break;
				}

				for (cn_cbor* cp = gpnam->first_child; cp; cp = cp->next) {
					std::string name;
					if (cp->v.str != NULL && cp->length > 0) {
						name = std::string(cp->v.str, cp->length);
					} else {
						name = "";
					}
					std::string value;
					if (cp->next) {
						cp = cp->next;
						if (cp->v.str != NULL && cp->length > 0) {
							value = std::string(cp->v.str, cp->length);
						} else {
							value = "";
						}
					}
					if (name == "fn") {
						certificate.person.familyName = value; // Replace(value, '\'', '`');
					} else if (name == "fnt") {
						certificate.person.standardisedFamilyName = value; // Replace(value, '\'', '`');
					} else if (name == "gn") {
						certificate.person.givenName = value; // Replace(value, '\'', '`');
					} else if (name == "gnt") {
						certificate.person.standardisedGivenName = value; // Replace(value, '\'', '`');
					}
				}
				certificateSimple.person.familyName = certificate.person.familyName;
				certificateSimple.person.standardisedFamilyName = certificate.person.standardisedFamilyName;
				certificateSimple.person.givenName = certificate.person.givenName;
				certificateSimple.person.standardisedGivenName = certificate.person.standardisedGivenName;
				for (cn_cbor* cp = gpvtr->first_child; cp; cp = cp->next) {
					std::string name;
					if (cp->v.str != NULL && cp->length > 0) {
						name = std::string(cp->v.str, cp->length);
					} else {
						name = "";
					}
					std::string value;
					if (cp->next) {
						cp = cp->next;
						if (cp->v.str != NULL && cp->length > 0) {
							value = std::string(cp->v.str, cp->length);
						} else {
							value = "";
						}
					}
					if (typevtr == "v") {
						if (name == "ci") {
							certificate.identifier = value;
							certificateSimple.identifier = value;
						} else if (name == "co") {
							certificate.country = value;
						} else if (name == "dn") {
							certificate.vaccination.doseNumber = cp->v.uint;
						} else if (name == "dt") {
							certificate.vaccination.dateOfVaccination = value;
						} else if (name == "is") {
							certificate.issuer = value;
						} else if (name == "ma") {
							certificate.vaccination.manufacturer = value;
						} else if (name == "mp") {
							certificate.vaccination.medicinalProduct = value;
						} else if (name == "sd") {
							certificate.vaccination.totalSeriesOfDoses = cp->v.uint;
						} else if (name == "tg") {
							certificate.disease = value;
						} else if (name == "vp") {
							certificate.vaccination.vaccine = value;
						}
					} else if (typevtr == "r") {
						if (name == "ci") {
							certificate.identifier = value;
							certificateSimple.identifier = value;
						} else if (name == "co") {
							certificate.country = value;
						} else if (name == "df") {
							certificate.recoveryStatement.certificateValidFrom = value;
						} else if (name == "du") {
							certificate.recoveryStatement.certificateValidUntil = value;
						} else if (name == "fr") {
							certificate.recoveryStatement.dateOfFirstPositiveTest = value;
						} else if (name == "is") {
							certificate.issuer = value;
						} else if (name == "tg") {
							certificate.disease = value;
						}
					} else if (typevtr == "t") {
						if (name == "ci") {
							certificate.identifier = value;
							certificateSimple.identifier = value;
						} else if (name == "co") {
							certificate.country = value;
						} else if (name == "is") {
							certificate.issuer = value;
						} else if (name == "nm") {
							certificate.test.testName = value;
						} else if (name == "ma") {
							certificate.test.testManufacturer = value;
						} else if (name == "sc") {
							certificate.test.dateTimeOfCollection = value;
						} else if (name == "dr") {
							certificate.test.dateTimeOfTestResult = value;
						} else if (name == "tc") {
							certificate.test.testingCentre = value;
						} else if (name == "tg") {
							certificate.disease = value;
						} else if (name == "tr") {
							certificate.test.testResult = value;
						} else if (name == "tt") {
							certificate.test.typeOfTest = value;
						}
					} else if (typevtr == "e") {
						if (name == "ci") {
							certificate.identifier = value;
							certificateSimple.identifier = value;
						} else if (name == "co") {
							certificate.country = value;
						} else if (name == "is") {
							certificate.issuer = value;
						} else if (name == "tg") {
							certificate.disease = value;
						} else if (name == "df") {
							certificate.exemption.certificateValidFrom = value;
						} else if (name == "du") {
							certificate.exemption.certificateValidUntil = value;
						}
					}
				}
				certificate.dateOfBirth = std::string(gpdob->v.str, gpdob->length);
				certificateSimple.dateOfBirth = certificate.dateOfBirth;

				certificate.version = std::string(gpver->v.str, gpver->length);
				certificate.countryIssuer = std::string(gpissuer->v.str, gpissuer->length);

				// Certificate validity dates ---------------------------------
				std::time_t expiry = gpexpiry->v.uint;
				std::time_t generated = gpgenerated->v.uint;

				{
					std::tm* lt = std::localtime(&expiry);
					if (lt != NULL) {
						char mbstr[100];
						std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%dT%H:%M:%S%z", lt);
						certificate.dateTimeOfExpiration = std::string(mbstr);
					} else {
						// set for invalid value
						expiry = 0;
					}
				}

				{
					std::tm* lt = std::localtime(&generated);
					if (lt != NULL) {
						char mbstr[100];
						std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%dT%H:%M:%S%z", lt);
						certificate.dateTimeOfGeneration = std::string(mbstr);
					} else {
						// set for invalid value
						generated = 0;
					}
				}

				// Certificate validity dates ---------------------------------
				std::time_t now = time(NULL);
				if (generated > 0 && now < generated) {
					m_logger->info("Certificate of %s not valid yet",
							certificate.dateTimeOfGeneration.c_str());
					certificateSimple.certificateStatus = NOT_VALID_YET;
					break;
				}

				if (expiry > 0 && now > expiry) {
					m_logger->info("Certificate expired at %s",
							certificate.dateTimeOfExpiration.c_str());
					certificateSimple.certificateStatus = NOT_VALID;
					break;
				}

				// Black list validation----------------------------------------
				// check if Green Pass is in blackList
				const std::string& blackList = m_rulesStorage->getRule(RULE_NAME_black_list_uvci, RULE_TYPE_black_list_uvci);
				std::set<std::string> blackListed = splitToSet(blackList, ';');
				if (blackListed.find(certificate.identifier) != blackListed.end()) {
					m_logger->info("Certificate %s in black list", certificate.identifier.c_str());
					certificateSimple.certificateStatus = NOT_VALID;
					break;
				}

				// CRL validation----------------------------------------
				// check if Green Pass is in CRL
				if (m_crlStorage != NULL) {
					unsigned char hash[SHA256_DIGEST_LENGTH];
					SHA256_CTX sha256;
					SHA256_Init(&sha256);
					SHA256_Update(&sha256, certificate.identifier.c_str(), certificate.identifier.length());
					SHA256_Final(hash, &sha256);
					std::string certificateHash = encodeBase64(hash, SHA256_DIGEST_LENGTH);
					if (m_crlStorage->hasCertificateHash(certificateHash)) {
						m_logger->info("Certificate %s in CRL", certificate.identifier.c_str());
						certificateSimple.certificateStatus = NOT_VALID;
						break;
					}
				}

				// Signature verification --------------------------------------
				std::string base64Kid = encodeBase64(cborkid->v.bytes, cborkid->length);

				cn_cbor* cbverify = cn_cbor_array_create(&back);
				cn_cbor_array_append(cbverify, cn_cbor_string_create("Signature1", &back), &back);
				cn_cbor_array_append(cbverify, cborprotected, &back);
				cn_cbor_array_append(cbverify, cn_cbor_data_create(NULL, 0, &back), &back);
				cn_cbor_array_append(cbverify, cborpayload, &back);

				unsigned char dataToBeVerified[2048];
				ssize_t dataToBeVerifiedLen = cn_cbor_encoder_write(dataToBeVerified, 0, sizeof(dataToBeVerified), cbverify);

				// SDK 1.1.1 apply different rules to italian recovery
				bool keyUsageForRecovery = false;
				const std::string& base64Ecx = m_keysStorage->getKey(base64Kid);
				if (!base64Ecx.empty()) {
					m_logger->debug("Loaded key %s", base64Kid.c_str());
					unsigned char* x509buf = (unsigned char*)malloc(base64Ecx.length());
					int x509len = decodeBase64(base64Ecx, x509buf);
					const unsigned char* pk = x509buf;
					X509* cert = d2i_X509(NULL, &pk, x509len);
					free(x509buf);

					if (cert == NULL) {
						certificateSimple.certificateStatus = NOT_VALID;
						m_logger->error("Error loading sign verify certificate");
						break;
					}

					// find the extendedKeyUsage
					int extIndex = X509_get_ext_by_NID(cert, NID_ext_key_usage, -1);
					if (extIndex > 0) {
						// get the correct X.509 extension
						X509_EXTENSION *ext = X509_get_ext(cert, extIndex);
						if (ext != NULL) {
							// get the extendedKeyUsage
							EXTENDED_KEY_USAGE *eku = static_cast<EXTENDED_KEY_USAGE*>(X509V3_EXT_d2i(ext));
							if (eku != NULL) {
								// print all OIDs
								for (int i = 0; i < sk_ASN1_OBJECT_num(eku); i++) {
									char buffer[100];
									OBJ_obj2txt(buffer, sizeof(buffer), sk_ASN1_OBJECT_value(eku, i), 1); // get OID
									// Private OID
									// OID_RECOVERY("1.3.6.1.4.1.1847.2021.1.3")
									// OID_ALT_RECOVERY("1.3.6.1.4.1.0.1847.2021.1.3")
									if ((strcmp("1.3.6.1.4.1.1847.2021.1.3", buffer) == 0) ||
											(strcmp("1.3.6.1.4.1.0.1847.2021.1.3", buffer) == 0)) {
										keyUsageForRecovery = true;
									}
								}
								EXTENDED_KEY_USAGE_free(eku);
							}
						}
					}

					EVP_PKEY* pkey = X509_get_pubkey(cert);
					if (pkey == NULL) {
						certificateSimple.certificateStatus = NOT_VALID;
						X509_free(cert);
						m_logger->error("Error get public key");
						break;
					}

					if (cboralgo->v.sint == -7) {
						// ECDSA_256
						EC_KEY* eckey = EVP_PKEY_get1_EC_KEY(pkey);
						if (eckey == NULL) {
							certificateSimple.certificateStatus = NOT_VALID;
							X509_free(cert);
							m_logger->info("Error get elliptic curve key");
							break;
						}

						unsigned char hash[SHA256_DIGEST_LENGTH];
						SHA256(dataToBeVerified, dataToBeVerifiedLen, hash);

						ECDSA_SIG * signature = ECDSA_SIG_new();
						BIGNUM* r = BN_bin2bn(cborsign->v.bytes, cborsign->length / 2, NULL);
						BIGNUM* s = BN_bin2bn(cborsign->v.bytes + cborsign->length / 2, cborsign->length / 2, NULL);
						ECDSA_SIG_set0(signature, r, s);

						int verify = ECDSA_do_verify(hash, SHA256_DIGEST_LENGTH, signature, eckey);
						ECDSA_SIG_free(signature);
						m_logger->info("Elliptic curve verify result: %d", verify);
						X509_free(cert);
						if (verify != 1) {
							certificateSimple.certificateStatus = NOT_VALID;
							break;
						}
					} else {
						// RSA_PSS_256
						EVP_MD_CTX* ctx = EVP_MD_CTX_create();
						EVP_PKEY_CTX *pkey_ctx;
						if (EVP_DigestVerifyInit(ctx, &pkey_ctx, EVP_sha256(), NULL, pkey) > 0 ) {
							if (EVP_PKEY_CTX_set_rsa_padding(pkey_ctx, RSA_PKCS1_PSS_PADDING) > 0) {
								if (EVP_DigestVerifyUpdate(ctx, dataToBeVerified, dataToBeVerifiedLen) > 0) {
									int verify = EVP_DigestVerifyFinal(ctx, (unsigned char*)cborsign->v.bytes, cborsign->length);
									m_logger->info("EVP verify result: %d", verify);
									X509_free(cert);
									EVP_MD_CTX_destroy(ctx);
									if (verify != 1) {
										certificateSimple.certificateStatus = NOT_VALID;
										break;
									}
								} else {
									certificateSimple.certificateStatus = NOT_VALID;
									X509_free(cert);
									EVP_MD_CTX_destroy(ctx);
									m_logger->debug("EVP_DigestVerifyUpdate error");
									break;
								}
							} else {
								certificateSimple.certificateStatus = NOT_VALID;
								X509_free(cert);
								EVP_MD_CTX_destroy(ctx);
								m_logger->debug("EVP_PKEY_CTX_set_rsa_padding error");
								break;
							}
						} else {
							certificateSimple.certificateStatus = NOT_VALID;
							X509_free(cert);
							EVP_MD_CTX_destroy(ctx);
							m_logger->debug("EVP_DigestVerifyInit error");
							break;
						}
					}
				} else {
					certificateSimple.certificateStatus = NOT_VALID;
					m_logger->info("Key %s not found by certificates provider", base64Kid.c_str());
					break;
				}

				// Rules validation--------------------------------------------
				if (typevtr == "v") {
					// Check if vaccine is present in setting list; otherwise, return not valid
					std::string checkDays = m_rulesStorage->getRule(RULE_NAME_vaccine_end_day_complete, certificate.vaccination.medicinalProduct);
					if (checkDays.empty()) {
						m_logger->info("Unknown or not managed vaccine %s ",
								certificate.vaccination.medicinalProduct.c_str());
						certificateSimple.certificateStatus = NOT_VALID;
						break;
					}

					// In Italy, Sputnik is accepted only for San Marino republic
					if (certificate.vaccination.medicinalProduct == RULE_TYPE_Sputnik_V) {
						if (certificate.country != COUNTRY_SAN_MARINO) {
							m_logger->info("Vaccine %s done in not accepted nation %s",
									certificate.vaccination.medicinalProduct.c_str(),
									certificate.country.c_str());
							certificateSimple.certificateStatus = NOT_VALID;
							break;
						}
					}

					// check for partial or complete vaccine
					int startDay = -1;
					int endDay = -1;
					if (certificate.vaccination.doseNumber < certificate.vaccination.totalSeriesOfDoses) {
						// partial vaccine
						std::string startDays = m_rulesStorage->getRule(RULE_NAME_vaccine_start_day_not_complete, certificate.vaccination.medicinalProduct);
						std::string endDays = m_rulesStorage->getRule(RULE_NAME_vaccine_end_day_not_complete, certificate.vaccination.medicinalProduct);
						m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_start_day_not_complete, certificate.vaccination.medicinalProduct.c_str(), startDays.c_str());
						m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_end_day_not_complete, certificate.vaccination.medicinalProduct.c_str(), endDays.c_str());
						if (startDays.empty() || endDays.empty()) {
							m_logger->info("Partial vaccine %s validity days not found (%s - %s)",
									certificate.vaccination.medicinalProduct.c_str(),
									startDays.c_str(), endDays.c_str());
							certificateSimple.certificateStatus = NOT_VALID;
							break;
						}
						// SDK version 1.1.1 booster mode (and school)
						if (scanMode == SCAN_MODE_BOOSTER) {
							m_logger->info("Partial vaccine %s not valid for selected scan mode",
									certificate.vaccination.medicinalProduct.c_str());
							certificateSimple.certificateStatus = NOT_VALID;
							break;
						}
						startDay = atoi(startDays.c_str());
						endDay = atoi(endDays.c_str());
					} else if (certificate.vaccination.doseNumber >= certificate.vaccination.totalSeriesOfDoses) {
						// complete vaccine
						std::string startDays;
						std::string endDays;
						std::string  countryCode;
						if (scanMode == SCAN_MODE_STANDARD) {
							countryCode = certificate.country;
						} else {
							countryCode = COUNTRY_ITALY;
						}

						if (
							(certificate.vaccination.medicinalProduct == RULE_TYPE_EU_1_20_1525 && certificate.vaccination.doseNumber >= 2)
							||
							(certificate.vaccination.medicinalProduct != RULE_TYPE_EU_1_20_1525 && (certificate.vaccination.doseNumber >= 3 || certificate.vaccination.doseNumber > certificate.vaccination.totalSeriesOfDoses))
						) {
							// booster
							if (countryCode == COUNTRY_ITALY) {
								startDays = m_rulesStorage->getRule(RULE_NAME_vaccine_start_day_booster_IT, RULE_TYPE_GENERIC);
								m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_start_day_booster_IT, RULE_TYPE_GENERIC, startDays.c_str());
								if (startDays.empty()) startDays = "0";
								endDays = m_rulesStorage->getRule(RULE_NAME_vaccine_end_day_booster_IT, RULE_TYPE_GENERIC);
								m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_end_day_booster_IT, RULE_TYPE_GENERIC, endDays.c_str());
								if (endDays.empty()) endDays = "180";
							} else {
								startDays = m_rulesStorage->getRule(RULE_NAME_vaccine_start_day_booster_NOT_IT, RULE_TYPE_GENERIC);
								m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_start_day_booster_NOT_IT, RULE_TYPE_GENERIC, startDays.c_str());
								if (startDays.empty()) startDays = "0";
								endDays = m_rulesStorage->getRule(RULE_NAME_vaccine_end_day_booster_NOT_IT, RULE_TYPE_GENERIC);
								m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_end_day_booster_NOT_IT, RULE_TYPE_GENERIC, endDays.c_str());
								if (endDays.empty()) endDays = "270";
							}
						} else {
							if (countryCode == COUNTRY_ITALY) {
								startDays = m_rulesStorage->getRule(RULE_NAME_vaccine_start_day_complete_IT, RULE_TYPE_GENERIC);
								m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_start_day_complete_IT, RULE_TYPE_GENERIC, startDays.c_str());
								if (startDays.empty()) startDays = "0";
								endDays = m_rulesStorage->getRule(RULE_NAME_vaccine_end_day_complete_IT, RULE_TYPE_GENERIC);
								m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_end_day_complete_IT, RULE_TYPE_GENERIC, endDays.c_str());
								if (endDays.empty()) endDays = "180";
							} else {
								startDays = m_rulesStorage->getRule(RULE_NAME_vaccine_start_day_complete_NOT_IT, RULE_TYPE_GENERIC);
								m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_start_day_complete_NOT_IT, RULE_TYPE_GENERIC, startDays.c_str());
								if (startDays.empty()) startDays = "0";
								endDays = m_rulesStorage->getRule(RULE_NAME_vaccine_end_day_complete_NOT_IT, RULE_TYPE_GENERIC);
								m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_vaccine_end_day_complete_NOT_IT, RULE_TYPE_GENERIC, endDays.c_str());
								if (endDays.empty()) endDays = "270";
							}
						}

						if (startDays.empty() || endDays.empty()) {
							m_logger->info("Complete vaccine %s validity days not found  (%s - %s)",
									certificate.vaccination.medicinalProduct.c_str(),
									startDays.c_str(), endDays.c_str());
							certificateSimple.certificateStatus = NOT_VALID;
							break;
						}
						startDay = atoi(startDays.c_str());
						endDay = atoi(endDays.c_str());
						// - Janssen TYPE_EU_1_20_1525 with doseNumber greater than totalSeriesOfDoses (second dose or more)
						//   is valid from now, skip start validity check because Janssen does not have partially condition (single dose vaccine)
						//   and rule of completed vaccine has validity of 15 days after dose injection (but this is not true for recall)
						if (certificate.vaccination.medicinalProduct == RULE_TYPE_EU_1_20_1525 &&
								certificate.vaccination.doseNumber > certificate.vaccination.totalSeriesOfDoses) {
							m_logger->debug("Janssen complete or booster: %d/%d", certificate.vaccination.doseNumber, certificate.vaccination.totalSeriesOfDoses);
							startDay = 0;
						}
						// SDK version 1.1.1 start validity immediate for Janssen complete vaccination with at least 2 doses
						if (certificate.vaccination.medicinalProduct == RULE_TYPE_EU_1_20_1525 &&
								certificate.vaccination.doseNumber == certificate.vaccination.totalSeriesOfDoses &&
								certificate.vaccination.totalSeriesOfDoses >= 2) {
							m_logger->debug("Janssen complete or booster: %d/%d", certificate.vaccination.doseNumber, certificate.vaccination.totalSeriesOfDoses);
							startDay = 0;
						}
					}

					// get current date
					time_t t = time(NULL);
					struct tm currentDate;
					localtime_r(&t, &currentDate);

					// get current day
					time_t currentDay = (t + currentDate.tm_gmtoff) / 3600 / 24;

					// get vaccine date
					struct tm vaccineDate;
					memset(&vaccineDate, 0, sizeof(vaccineDate));
					strptime(certificate.vaccination.dateOfVaccination.c_str(), "%Y-%m-%d", &vaccineDate);

					// get vaccine day
					time_t vaccineDay = (mktime(&vaccineDate) + 43200) / 3600 / 24;

					long days = currentDay - vaccineDay;
					if (days < startDay) {
						// digital certificate not valid
						m_logger->info("Vaccine certificate of %s not valid yet (%d: %d - %d)",
								certificate.vaccination.dateOfVaccination.c_str(),
								days, startDay, endDay);
						certificateSimple.certificateStatus = NOT_VALID_YET;
						break;
					}
					if (days > endDay) {
						// digital certificate not valid
						m_logger->info("Vaccine certificate of %s not valid (%d: %d - %d)",
								certificate.vaccination.dateOfVaccination.c_str(),
								days, startDay, endDay);
						certificateSimple.certificateStatus = NOT_VALID;
						break;
					}
					if (certificate.vaccination.doseNumber < certificate.vaccination.totalSeriesOfDoses) {
						// digital certificate partially valid (only in Italy)
						m_logger->info("Vaccine certificate of %s partially valid (%d: %d - %d)",
								certificate.vaccination.dateOfVaccination.c_str(),
								days, startDay, endDay);
						certificateSimple.certificateStatus = VALID;
						break;
					}
					// SDK version 1.1.1 booster mode
					if (scanMode == SCAN_MODE_BOOSTER) {
						// - Janssen TYPE_EU_1_20_1525 complete with doseNumber lesser than two
						//   respond that test is needed
						if (certificate.vaccination.medicinalProduct == RULE_TYPE_EU_1_20_1525 &&
								certificate.vaccination.doseNumber == certificate.vaccination.totalSeriesOfDoses &&
								certificate.vaccination.doseNumber < 2) {
							m_logger->info("Vaccine certificate of %s valid (%d: %d - %d) but test needed for selected scan mode",
									certificate.vaccination.dateOfVaccination.c_str(),
									days, startDay, endDay);
							certificateSimple.certificateStatus = TEST_NEEDED;
							break;
						}
						// - Not Janssen TYPE_EU_1_20_1525 complete with doseNumber lesser than three
						//   respond that test is needed
						if (certificate.vaccination.medicinalProduct != RULE_TYPE_EU_1_20_1525 &&
								certificate.vaccination.doseNumber == certificate.vaccination.totalSeriesOfDoses &&
								certificate.vaccination.doseNumber < 3) {
							m_logger->info("Vaccine certificate of %s valid (%d: %d - %d) but test needed for selected scan mode",
									certificate.vaccination.dateOfVaccination.c_str(),
									days, startDay, endDay);
							certificateSimple.certificateStatus = TEST_NEEDED;
							break;
						}
					}
					certificateSimple.certificateStatus = VALID;
					m_logger->info("Vaccine certificate of %s valid (%d: %d - %d)",
							certificate.vaccination.dateOfVaccination.c_str(),
							days, startDay, endDay);
				}

				if (typevtr == "r") {
					std::string startDays;
					std::string endDays;
					std::string  countryCode;
					if (scanMode == SCAN_MODE_STANDARD) {
						countryCode = certificate.country;
					} else {
						countryCode = COUNTRY_ITALY;
					}

					if (certificate.country == COUNTRY_ITALY && keyUsageForRecovery) {
						// recovery bis
						startDays = m_rulesStorage->getRule(RULE_NAME_recovery_pv_cert_start_day, RULE_TYPE_GENERIC);
						m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_recovery_pv_cert_start_day, RULE_TYPE_GENERIC, startDays.c_str());
						if (startDays.empty()) endDays = "0";
						endDays = m_rulesStorage->getRule(RULE_NAME_recovery_pv_cert_end_day, RULE_TYPE_GENERIC);
						m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_recovery_pv_cert_end_day, RULE_TYPE_GENERIC, endDays.c_str());
						if (endDays.empty()) endDays = "270";
					} else {
						if (countryCode == COUNTRY_ITALY) {
							startDays = m_rulesStorage->getRule(RULE_NAME_recovery_cert_start_day_IT, RULE_TYPE_GENERIC);
							m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_recovery_cert_start_day_IT, RULE_TYPE_GENERIC, startDays.c_str());
							if (startDays.empty()) endDays = "0";
							endDays = m_rulesStorage->getRule(RULE_NAME_recovery_cert_end_day_IT, RULE_TYPE_GENERIC);
							m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_recovery_cert_end_day_IT, RULE_TYPE_GENERIC, endDays.c_str());
							if (endDays.empty()) endDays = "180";
						} else {
							startDays = m_rulesStorage->getRule(RULE_NAME_recovery_cert_start_day_NOT_IT, RULE_TYPE_GENERIC);
							m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_recovery_cert_start_day_NOT_IT, RULE_TYPE_GENERIC, startDays.c_str());
							if (startDays.empty()) startDays = "0";
							endDays = m_rulesStorage->getRule(RULE_NAME_recovery_cert_end_day_NOT_IT, RULE_TYPE_GENERIC);
							m_logger->debug("Loaded rule %s, %s: %s", RULE_NAME_recovery_cert_end_day_NOT_IT, RULE_TYPE_GENERIC, endDays.c_str());
							if (endDays.empty()) endDays = "270";
						}
					}

					if (startDays.empty() || endDays.empty()) {
						m_logger->info("Recovery validity days not found (%s - %s)",
								startDays.c_str(), endDays.c_str());
						certificateSimple.certificateStatus = NOT_VALID;
						break;
					}
					int startDay = atoi(startDays.c_str());
					int endDay = atoi(endDays.c_str());

					// get current date
					time_t t = time(NULL);
					struct tm currentDate;
					localtime_r(&t, &currentDate);

					// get current day
					time_t currentDay = (t + currentDate.tm_gmtoff) / 3600 / 24;

					// get recovery from date
					struct tm recoveryFromDate;
					memset(&recoveryFromDate, 0, sizeof(recoveryFromDate));
					strptime(certificate.recoveryStatement.certificateValidFrom.c_str(), "%Y-%m-%d", &recoveryFromDate);

					// get recovery from day
					time_t recoveryFromDay = (mktime(&recoveryFromDate) + 43200) / 3600 / 24;

					// get recovery until date
					struct tm recoveryUntilDate;
					memset(&recoveryUntilDate, 0, sizeof(recoveryUntilDate));
					strptime(certificate.recoveryStatement.certificateValidUntil.c_str(), "%Y-%m-%d", &recoveryUntilDate);

					// get recovery to day
					time_t recoveryUntilDay = (mktime(&recoveryUntilDate) + 43200) / 3600 / 24;

					// get recovery to day
					time_t recoveryToDay = recoveryFromDay + endDay;

					if (currentDay < recoveryFromDay) {
						// certificate not valid yet
						m_logger->info("Recovery certificate of %s (+%d) - %s (+%d) not valid yet",
								certificate.recoveryStatement.certificateValidFrom.c_str(), startDay,
								certificate.recoveryStatement.certificateValidUntil.c_str(), endDay);
						certificateSimple.certificateStatus = NOT_VALID_YET;
						break;
					}
					if (currentDay > recoveryToDay) {
						// certificate not valid
						m_logger->info("Recovery certificate of %s (+%d) - %s (+%d) not valid",
								certificate.recoveryStatement.certificateValidFrom.c_str(), startDay,
								certificate.recoveryStatement.certificateValidUntil.c_str(), endDay);
						certificateSimple.certificateStatus = NOT_VALID;
						break;
					}
					// SDK version 1.1.1 booster mode
					if (scanMode == SCAN_MODE_BOOSTER) {
						m_logger->info("Recovery certificate of %s (+%d) - %s (+%d) valid but test needed for selected scan mode",
								certificate.recoveryStatement.certificateValidFrom.c_str(), startDay,
								certificate.recoveryStatement.certificateValidUntil.c_str(), endDay);
						certificateSimple.certificateStatus = TEST_NEEDED;
						break;
					}
					certificateSimple.certificateStatus = VALID;
					m_logger->info("Recovery certificate of %s (+%d) - %s (+%d) valid",
							certificate.recoveryStatement.certificateValidFrom.c_str(), startDay,
							certificate.recoveryStatement.certificateValidUntil.c_str(), endDay);
				}

				if (typevtr == "t") {
					if (scanMode == SCAN_MODE_2G || scanMode == SCAN_MODE_BOOSTER) {
						certificateSimple.certificateStatus = NOT_VALID;
						m_logger->debug("Test certificate of %s not valid for selected scan mode",
								certificate.test.dateTimeOfCollection.c_str());
					} else {
						int startHour = -1;
						int endHour = -1;

						if (certificate.test.typeOfTest == MOLECULAR) {
							std::string startHours = m_rulesStorage->getRule(RULE_NAME_molecular_test_start_hours, RULE_TYPE_GENERIC);
							std::string endHours = m_rulesStorage->getRule(RULE_NAME_molecular_test_end_hours, RULE_TYPE_GENERIC);
							if (startHours.empty() || endHours.empty()) {
								m_logger->info("Molecular test validity hours not found (%s - %s)",
										startHours.c_str(), endHours.c_str());
								certificateSimple.certificateStatus = NOT_VALID;
								break;
							}
							startHour = atoi(startHours.c_str());
							endHour = atoi(endHours.c_str()) - 1; // extreme excluded
						}

						else if (certificate.test.typeOfTest == RAPID) {
							std::string startHours = m_rulesStorage->getRule(RULE_NAME_rapid_test_start_hours, RULE_TYPE_GENERIC);
							std::string endHours = m_rulesStorage->getRule(RULE_NAME_rapid_test_end_hours, RULE_TYPE_GENERIC);
							if (startHours.empty() || endHours.empty()) {
								m_logger->info("Rapid test validity hours not found (%s - %s)",
										startHours.c_str(), endHours.c_str());
								certificateSimple.certificateStatus = NOT_VALID;
								break;
							}
							startHour = atoi(startHours.c_str());
							endHour = atoi(endHours.c_str()) - 1; // extreme excluded
						}

						else {
							m_logger->info("Unknown test type %s",
									certificate.test.typeOfTest.c_str());
							certificateSimple.certificateStatus = NOT_VALID;
							break;
						}

						if (certificate.test.testResult != NOT_DETECTED) {
							m_logger->info("Test result is not 'not detected' %s",
									certificate.test.testResult.c_str());
							certificateSimple.certificateStatus = NOT_VALID;
							break;
						}

						// get current datetime
						time_t t = time(NULL);
						struct tm currentDateTime;
						localtime_r(&t, &currentDateTime);

						// get current time
						time_t currentTime = t + currentDateTime.tm_gmtoff;

						// get test datetime
						struct tm testDateTime;
						memset(&testDateTime, 0, sizeof(testDateTime));
						strptime(certificate.test.dateTimeOfCollection.c_str(), "%Y-%m-%dT%H:%M:%S%z", &testDateTime);

						// get recovery from day
						time_t recoveryTime = mktime(&testDateTime);

						long hours = (currentTime - recoveryTime) / 3600;
						if (currentTime < recoveryTime) hours--; // -1 .. -3599 / 3600 = 0, we want = -1
						hours--; // for daylinght saving
						if (hours < startHour) {
							// digital certificate not valid
							m_logger->info("Test certificate of %s not valid yet (%d: %d - %d)",
									certificate.test.dateTimeOfCollection.c_str(),
									hours, startHour, endHour);
							certificateSimple.certificateStatus = NOT_VALID_YET;
							break;
						}
						if (hours > endHour) {
							// digital certificate not valid
							m_logger->info("Test certificate of %s not valid (%d: %d - %d)",
									certificate.test.dateTimeOfCollection.c_str(),
									hours, startHour, endHour);
							certificateSimple.certificateStatus = NOT_VALID;
							break;
						}
						if (scanMode == SCAN_MODE_WORK) {
							// get limit date
							time_t t = time(NULL);
							struct tm limitDate;
							localtime_r(&t, &limitDate);
							limitDate.tm_year = limitDate.tm_year - WORK_VACCINE_MANDATORY_AGE;

							// get limit day
							time_t limitDay = (mktime(&limitDate) + limitDate.tm_gmtoff) / 3600 / 24;

							// get birth date
							struct tm birthDate;
							memset(&birthDate, 0, sizeof(birthDate));
							strptime(certificate.dateOfBirth.c_str(), "%Y-%m-%d", &birthDate);

							// get birth day
							time_t birthDay = (mktime(&birthDate) + 43200) / 3600 / 24;

							if (birthDay <= limitDay) {
								// digital certificate not valid
								m_logger->info("Test certificate of %s not valid (%d: %d - %d) for selected scan mode",
										certificate.test.dateTimeOfCollection.c_str(),
										hours, startHour, endHour);
								certificateSimple.certificateStatus = NOT_VALID;
								break;
							}
						}
						certificateSimple.certificateStatus = VALID;
						m_logger->debug("Test certificate of %s valid (%d: %d - %d)",
								certificate.test.dateTimeOfCollection.c_str(),
								hours, startHour, endHour);
					}
				}

				if (typevtr == "e") {
					/*
					 * Exemption certificate valid for school
					if (scanMode == SCAN_MODE_SCHOOL) {
						certificateSimple.certificateStatus = NOT_VALID;
						m_logger->debug("Exemption certificate of %s - %s not valid for selected scan mode",
								certificate.exemption.certificateValidFrom.c_str(),
								certificate.exemption.certificateValidUntil.c_str());
					} else {
					*/
						// get current date
						time_t t = time(NULL);
						struct tm currentDate;
						localtime_r(&t, &currentDate);

						// get current day
						time_t currentDay = (t + currentDate.tm_gmtoff) / 3600 / 24;

						// get exemption from date
						struct tm exemptionFromDate;
						memset(&exemptionFromDate, 0, sizeof(exemptionFromDate));
						strptime(certificate.exemption.certificateValidFrom.c_str(), "%Y-%m-%d", &exemptionFromDate);

						// get exemption from day
						time_t exemptionFromDay = (mktime(&exemptionFromDate) + 43200) / 3600 / 24;

						// get exemption until date
						struct tm exemptionUntilDate;
						memset(&exemptionUntilDate, 0, sizeof(exemptionUntilDate));
						strptime(certificate.exemption.certificateValidUntil.c_str(), "%Y-%m-%d", &exemptionUntilDate);

						// get exemption to day
						time_t exemptionUntilDay = (mktime(&exemptionUntilDate) + 43200) / 3600 / 24;

						if (currentDay < exemptionFromDay) {
							// certificate not valid yet
							m_logger->info("Exemption certificate of %s - %s not valid yet",
									certificate.exemption.certificateValidFrom.c_str(),
									certificate.exemption.certificateValidUntil.c_str());
							certificateSimple.certificateStatus = NOT_VALID_YET;
							break;
						}
						if (currentDay > exemptionUntilDay) {
							// certificate not valid
							m_logger->info("Exemption certificate of %s - %s not valid",
									certificate.exemption.certificateValidFrom.c_str(),
									certificate.exemption.certificateValidUntil.c_str());
							certificateSimple.certificateStatus = NOT_VALID;
							break;
						}
						// SDK version 1.1.1 booster mode
						if (scanMode == SCAN_MODE_BOOSTER) {
							m_logger->info("Exemption certificate of %s - %s valid but test needed for selected scan mode",
									certificate.exemption.certificateValidFrom.c_str(),
									certificate.exemption.certificateValidUntil.c_str());
							certificateSimple.certificateStatus = TEST_NEEDED;
							break;
						}
						certificateSimple.certificateStatus = VALID;
						m_logger->info("Exemption certificate of %s - %s valid",
								certificate.exemption.certificateValidFrom.c_str(),
								certificate.exemption.certificateValidUntil.c_str());
					//}
				}
			} while (false);

			if (cbordata != NULL) cn_cbor_free(cbordata);
			if (decbuf != NULL) free(decbuf);
		} catch(std::exception& ex) {
			m_logger->error("'%s'", ex.what());
		}
	} while (false);

	// set verification timestamp
	char mbstr[100];
	std::time_t t = std::time(NULL);
	std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%dT%H:%M:%S%z", std::localtime(&t));
	certificateSimple.timeStamp = std::string(mbstr);

	return certificateSimple;
}

DGCVerifier* DGCVerifier_create(IKeysStorage* keysStorage, IRulesStorage* rulesStorage, ICRLStorage* crlStorage, ILogger* logger) {
	return new DGCVerifier(keysStorage, rulesStorage, crlStorage, logger);
}

void DGCVerifier_release(DGCVerifier* dgcVerifier) {
	delete dgcVerifier;
}

bool DGCVerifier_verifyMinSdkVersion(DGCVerifier* dgcVerifier) {
	return dgcVerifier->verifyMinSdkVersion();
}

CertificateSimple DGCVerifier_verify(DGCVerifier* dgcVerifier, const std::string& dgcQr, const std::string& scanMode) {
	return dgcVerifier->verify(dgcQr, scanMode);
}

} // namespace verificaC19Sdk
