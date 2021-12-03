// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <dlfcn.h>
#include <unistd.h>

#include <KeysStorageMemory.hpp>
#include <KeysProviderTest.hpp>
#include <KeysProviderItaly.hpp>
#include <RulesStorageMemory.hpp>
#include <RulesProviderTest.hpp>
#include <RulesProviderItaly.hpp>
#include <LoggerStdout.hpp>
#include <verificaC19-sdk/DGCVerifier.hpp>
#include <verificaC19-sdk/DGCRulesKeysUpdater.hpp>

static void logCertificate(const struct CertificateSimple_c* certificate, const void* logger) {
	LoggerStdout_c_info(logger, "---------- Certificate log ----------");
	char* certificateStatus = "";
	switch (certificate->certificateStatus) {
		case NOT_VALID: certificateStatus = "NOT_VALID"; break;
		case NOT_VALID_YET: certificateStatus = "NOT_VALID_YET"; break;
		case VALID: certificateStatus = "VALID"; break;
		case PARTIALLY_VALID: certificateStatus = "PARTIALLY_VALID"; break;
		case NOT_EU_DCC: certificateStatus = "NOT_EU_DCC"; break;
	}
	LoggerStdout_c_info(logger, "Certificate status: %s", certificateStatus);
	LoggerStdout_c_info(logger, "Person standardisedFamilyName: %s", certificate->person.standardisedFamilyName);
	LoggerStdout_c_info(logger, "Person familyName: %s", certificate->person.familyName);
	LoggerStdout_c_info(logger, "Person standardisedGivenName: %s", certificate->person.standardisedGivenName);
	LoggerStdout_c_info(logger, "Person givenName: %s", certificate->person.givenName);
	LoggerStdout_c_info(logger, "Date of birth: %s", certificate->dateOfBirth);
	LoggerStdout_c_info(logger, "Verification timeStamp: %s", certificate->timeStamp);
}

int main (int argc, char** argv) {
	void* logger = LoggerStdout_c_create(DEBUG);

	LoggerStdout_c_info(logger, "---------- Test certificate, IT: Vaccine ----------");
	{
		void* keysStorage = KeysStorageMemory_c_create();
		void* keysProvider = KeysProviderTest_c_create(logger);
		void* rulesStorage = RulesStorageMemory_c_create();
		void* rulesProvider = RulesProviderTest_c_create(logger);
		KeysProviderTest_c_refreshKeys(keysProvider, keysStorage);
		RulesProviderTest_c_refreshRules(rulesProvider, rulesStorage);

		void* verifier = DGCVerifier_c_create(keysStorage, rulesStorage, logger);
		if (!DGCVerifier_c_verifyMinSdkVersion(verifier)) {
			LoggerStdout_c_error(logger, "Minimum SDK version does not match");
		} else {
			struct CertificateSimple_c* certificate = DGCVerifier_c_verify(verifier, "HC1:6BFOXN%TS3DH0YOJ58S S-W5HDC *M0II5XHC9B5G2+$N IOP-IA%NFQGRJPC%OQHIZC4.OI1RM8ZA.A5:S9MKN4NN3F85QNCY0O%0VZ001HOC9JU0D0HT0HB2PL/IB*09B9LW4T*8+DCMH0LDK2%K:XFE70*LP$V25$0Q:J:4MO1P0%0L0HD+9E/HY+4J6TH48S%4K.GJ2PT3QY:GQ3TE2I+-CPHN6D7LLK*2HG%89UV-0LZ 2ZJJ524-LH/CJTK96L6SR9MU9DHGZ%P WUQRENS431T1XCNCF+47AY0-IFO0500TGPN8F5G.41Q2E4T8ALW.INSV$ 07UV5SR+BNQHNML7 /KD3TU 4V*CAT3ZGLQMI/XI%ZJNSBBXK2:UG%UJMI:TU+MMPZ5$/PMX19UE:-PSR3/$NU44CBE6DQ3D7B0FBOFX0DV2DGMB$YPF62I$60/F$Z2I6IFX21XNI-LM%3/DF/U6Z9FEOJVRLVW6K$UG+BKK57:1+D10%4K83F+1VWD1NE");
			logCertificate(certificate, logger);
			CertificateSimple_c_release(certificate);
		}
		DGCVerifier_c_release(verifier);
		RulesProviderTest_c_release(rulesProvider);
		RulesStorageMemory_c_release(rulesStorage);
		KeysProviderTest_c_release(keysProvider);
		KeysStorageMemory_c_release(keysStorage);
	}

	LoggerStdout_c_info(logger, "---------- Test certificate, IT: Recovery ----------");
	{
		void* keysStorage = KeysStorageMemory_c_create();
		void* keysProvider = KeysProviderTest_c_create(logger);
		void* rulesStorage = RulesStorageMemory_c_create();
		void* rulesProvider = RulesProviderTest_c_create(logger);
		KeysProviderTest_c_refreshKeys(keysProvider, keysStorage);
		RulesProviderTest_c_refreshRules(rulesProvider, rulesStorage);

		void* verifier = DGCVerifier_c_create(keysStorage, rulesStorage, logger);
		if (!DGCVerifier_c_verifyMinSdkVersion(verifier)) {
			LoggerStdout_c_error(logger, "Minimum SDK version does not match");
		} else {
			struct CertificateSimple_c* certificate = DGCVerifier_c_verify(verifier, "HC1:6BFOXN%TS3DH0YOJ58S S-W5HDC *MEB2B2JJ59J-9BC6:X9NECX0AKQC:3DCV4*XUA2P-FHT-H4SI/J9WVHWVH+ZEOV1J$HNTICZUBOM*LP$V25$0Q:J40IA3L/*84-5%:C92JN*4CY0*%9F/8J2P4.818T+:IX3M3.96RPVD9J-OZT1-NT0 2$$0$2PZX69B9VCDHI2/T9TU1BPIJKH/T7B-S-*O/Y41FD+X49+5Z-6%.HDD8R6W1FDJGJSFJ/4Q:T0.KJTNP8EFULNC:HA0K5HKRB4TD85LOLF92GF.3O.Z8CC7-2FQYG$%21 2O*4R60NM8JI0EUGP$I/XK$M8ZQE6YB9M66P8N31I.ROSK%IA1Q2N53Q-OQ2VC6E26T11ROSNK5W-*H+MJ%0RGZVGWNURI75RBSQSHLH1JG*CMH2.-S$7VX6N*Z1881J7G.F9I+SV06F+1M*93%D");
			logCertificate(certificate, logger);
			CertificateSimple_c_release(certificate);
		}
		DGCVerifier_c_release(verifier);
		RulesProviderTest_c_release(rulesProvider);
		RulesStorageMemory_c_release(rulesStorage);
		KeysProviderTest_c_release(keysProvider);
		KeysStorageMemory_c_release(keysStorage);
	}

	LoggerStdout_c_info(logger, "---------- Test certificate, DE: RAPID test ----------");
	{
		void* keysStorage = KeysStorageMemory_c_create();
		void* keysProvider = KeysProviderTest_c_create(logger);
		void* rulesStorage = RulesStorageMemory_c_create();
		void* rulesProvider = RulesProviderTest_c_create(logger);
		KeysProviderTest_c_refreshKeys(keysProvider, keysStorage);
		RulesProviderTest_c_refreshRules(rulesProvider, rulesStorage);

		void* verifier = DGCVerifier_c_create(keysStorage, rulesStorage, logger);
		if (!DGCVerifier_c_verifyMinSdkVersion(verifier)) {
			LoggerStdout_c_error(logger, "Minimum SDK version does not match");
		} else {
			struct CertificateSimple_c* certificate = DGCVerifier_c_verify(verifier, "HC1:6BFR%BH:7*I0PS33NUA9HWP5PZ2CLJ*GH7WV-UNA1VZJKZ6HX.A/5R..9*CV6+LJ*F.UN7A2BT8B+6B897S69R48S1.R1VJO9Q1ZZO+CC$A9%T5X7RI25A8S57D JK-PQ+JR*FDTW3+1EC1JXLOQ58+KFL49ZMENAO.YOWR75PAH0HD6AIHCPWHJTF.RJ*JCSKEHL1N31HWEO67KJH8TIX-B3QB-+9*LCU:C:P2QEEQ7KF$V--4CW7JWILDWU%Q%IO0LAK70J$KW2JW56.KO8E2RHPH60ILI8T0N/7OEPD7P3+3IH9VZIVWP.44FX87QH5I97ZK0MK8OIGC3 3CQ6WO+9P9ECRSV%72M4L65 KAVKE*YPRHSIF1 89*4NDZ7FU6:F6NPJ1PHL059BGBB1%/C/J91R75Z5I7CWV0TREWYSY8ULK5HWPGEP$SI5B1$8HDOCH3JEBCL*8SE2AZT9SC+84JVGR39:2V*TR:KBW/4S:FK DOHF-1789MQ.18CV2C3YCN79OR176:1U:0CQVNGDJ0GUPO%CRT+QC/O$:D/WQY$3*5UR2M4YPFXK$DH");
			logCertificate(certificate, logger);
			CertificateSimple_c_release(certificate);
		}
		DGCVerifier_c_release(verifier);
		RulesProviderTest_c_release(rulesProvider);
		RulesStorageMemory_c_release(rulesStorage);
		KeysProviderTest_c_release(keysProvider);
		KeysStorageMemory_c_release(keysStorage);
	}

	LoggerStdout_c_info(logger, "---------- Test certificate, IT: MOLECULAR test ----------");
	{
		void* keysStorage = KeysStorageMemory_c_create();
		void* keysProvider = KeysProviderTest_c_create(logger);
		void* rulesStorage = RulesStorageMemory_c_create();
		void* rulesProvider = RulesProviderTest_c_create(logger);
		KeysProviderTest_c_refreshKeys(keysProvider, keysStorage);
		RulesProviderTest_c_refreshRules(rulesProvider, rulesStorage);

		void* verifier = DGCVerifier_c_create(keysStorage, rulesStorage, logger);
		if (!DGCVerifier_c_verifyMinSdkVersion(verifier)) {
			LoggerStdout_c_error(logger, "Minimum SDK version does not match");
		} else {
			struct CertificateSimple_c* certificate = DGCVerifier_c_verify(verifier, "HC1:6BFOXN%TS3DH0YOJ58S S-W5HDC *M0II*%6C9B5G2+$NEJPP-IA%NGRIRJPC%OQHIZC4.OI:OIG/Q80P2W4VZ0K1H$$05QN*Y0K.G +AG5T01HJCAMKN$71Z95Z11VTO.L8YBJ-B93:GQBGZHHBIH5C99.B4DBF:F0.8ELG:.CC-8LQECKEBLDSH8XAG.6A-JE:GQA KX-SZDG0$JO+SW*PR+PHXF8IQV$K%OKOUFBBQR-S3D1PI0/7Q.H0807-L9CL62/2JJ11K2919GI1X1DDM8RMA0/41:6Z.2:NC-%CN$KJLCLF9+FJE 4Y3LL/II 05B9.Z8M+8:Y001HCY0R%0IGF5JNCPIGSUNG6YS75XJ/J0/V7.UI$RU8ZB.W2FI28LHUZUYZQNI9Y FQQGQ$FP DDVBDVBBX33UQLTU8L20H6/*12SADB9:G9J+9Y 5LJA8JF8JFHJP7NVDEBK3JQ7TI 05QNT+CCZ1ZA2I+T*R9XZ6/:COTJCURIF8CZPCJ4EF5LU5I-Q:.N$P9DX5NAM*PJYD3L2V0GBG.JL4LESU72S1CM%5OC%VSTJ8NC1TGO:QS02V505GJUTH");
			logCertificate(certificate, logger);
			CertificateSimple_c_release(certificate);
		}
		DGCVerifier_c_release(verifier);
		RulesProviderTest_c_release(rulesProvider);
		RulesStorageMemory_c_release(rulesStorage);
		KeysProviderTest_c_release(keysProvider);
		KeysStorageMemory_c_release(keysStorage);
	}

	LoggerStdout_c_info(logger, "---------- Test certificate, IT: MOLECULAR test 2G mode ------");
	{
		void* keysStorage = KeysStorageMemory_c_create();
		void* keysProvider = KeysProviderTest_c_create(logger);
		void* rulesStorage = RulesStorageMemory_c_create();
		void* rulesProvider = RulesProviderTest_c_create(logger);
		KeysProviderTest_c_refreshKeys(keysProvider, keysStorage);
		RulesProviderTest_c_refreshRules(rulesProvider, rulesStorage);

		void* verifier = DGCVerifier_c_create(keysStorage, rulesStorage, logger);
		DGCVerifier_c_setScanMode(verifier, SCAN_MODE_2G);
		if (!DGCVerifier_c_verifyMinSdkVersion(verifier)) {
			LoggerStdout_c_error(logger, "Minimum SDK version does not match");
		} else {
			struct CertificateSimple_c* certificate = DGCVerifier_c_verify(verifier, "HC1:6BFOXN%TS3DH0YOJ58S S-W5HDC *M0II*%6C9B5G2+$NEJPP-IA%NGRIRJPC%OQHIZC4.OI:OIG/Q80P2W4VZ0K1H$$05QN*Y0K.G +AG5T01HJCAMKN$71Z95Z11VTO.L8YBJ-B93:GQBGZHHBIH5C99.B4DBF:F0.8ELG:.CC-8LQECKEBLDSH8XAG.6A-JE:GQA KX-SZDG0$JO+SW*PR+PHXF8IQV$K%OKOUFBBQR-S3D1PI0/7Q.H0807-L9CL62/2JJ11K2919GI1X1DDM8RMA0/41:6Z.2:NC-%CN$KJLCLF9+FJE 4Y3LL/II 05B9.Z8M+8:Y001HCY0R%0IGF5JNCPIGSUNG6YS75XJ/J0/V7.UI$RU8ZB.W2FI28LHUZUYZQNI9Y FQQGQ$FP DDVBDVBBX33UQLTU8L20H6/*12SADB9:G9J+9Y 5LJA8JF8JFHJP7NVDEBK3JQ7TI 05QNT+CCZ1ZA2I+T*R9XZ6/:COTJCURIF8CZPCJ4EF5LU5I-Q:.N$P9DX5NAM*PJYD3L2V0GBG.JL4LESU72S1CM%5OC%VSTJ8NC1TGO:QS02V505GJUTH");
			logCertificate(certificate, logger);
			CertificateSimple_c_release(certificate);
		}
		DGCVerifier_c_release(verifier);
		RulesProviderTest_c_release(rulesProvider);
		RulesStorageMemory_c_release(rulesStorage);
		KeysProviderTest_c_release(keysProvider);
		KeysStorageMemory_c_release(keysStorage);
	}

	LoggerStdout_c_info(logger, "---------- Test with dynamic library loading ----------");
	{
		void* keysStorage = KeysStorageMemory_c_create();
		void* keysProvider = KeysProviderItaly_c_create(NULL);
		void* rulesStorage = RulesStorageMemory_c_create();
		void* rulesProvider = RulesProviderItaly_c_create(NULL);

		void* verificaC19Handle = dlopen("libverificaC19-sdk.so", RTLD_NOW);
		if (verificaC19Handle == NULL) {
			LoggerStdout_c_info(logger, "Error loading SDK library");
		} else {
			pfDGCVerifier_c_create fDGCVerifier_c_create = (pfDGCVerifier_c_create)dlsym(verificaC19Handle, "DGCVerifier_c_create");
			pfDGCVerifier_c_release fDGCVerifier_c_release = (pfDGCVerifier_c_release)dlsym(verificaC19Handle, "DGCVerifier_c_release");
			pfDGCVerifier_c_verifyMinSdkVersion fDGCVerifier_c_verifyMinSdkVersion = (pfDGCVerifier_c_verifyMinSdkVersion)dlsym(verificaC19Handle, "DGCVerifier_c_verifyMinSdkVersion");
			pfDGCVerifier_c_setScanMode fDGCVerifier_c_setScanMode = (pfDGCVerifier_c_setScanMode)dlsym(verificaC19Handle, "DGCVerifier_c_setScanMode");
			pfDGCVerifier_c_verify fDGCVerifier_c_verify = (pfDGCVerifier_c_verify)dlsym(verificaC19Handle, "DGCVerifier_c_verify");

			pfDGCRulesKeysUpdaterRulesAndKeys_c_create fDGCRulesKeysUpdater_c_create = (pfDGCRulesKeysUpdaterRulesAndKeys_c_create)dlsym(verificaC19Handle, "DGCRulesKeysUpdaterRulesAndKeys_c_create");
			pfDGCRulesKeysUpdater_c_release fDGCRulesKeysUpdater_c_release = (pfDGCRulesKeysUpdater_c_release)dlsym(verificaC19Handle, "DGCRulesKeysUpdater_c_release");

			void* rulesKeysUpdater = fDGCRulesKeysUpdater_c_create(86400, rulesProvider, rulesStorage, keysProvider, keysStorage, NULL);
			if (RulesStorageMemory_c_lastUpdate(rulesStorage) > 86400 || KeysStorageMemory_c_lastUpdate(keysStorage) > 86400) {
				LoggerStdout_c_info(logger, "Rules or Keys are updating, waiting");
				while (RulesStorageMemory_c_lastUpdate(rulesStorage) > 86400 || KeysStorageMemory_c_lastUpdate(keysStorage) > 86400) {
					usleep(10000);
				}
			}

			void* verifier = fDGCVerifier_c_create(keysStorage, rulesStorage, logger);

			if (!fDGCVerifier_c_verifyMinSdkVersion(verifier)) {
				LoggerStdout_c_error(logger, "Minimum SDK version does not match");
			} else {
				DGCVerifier_c_setScanMode(verifier, SCAN_MODE_2G);
				void* certificate = fDGCVerifier_c_verify(verifier, "HC1:6BFOXN%TS3DH0YOJ58S S-W5HDC *M0II*%6C9B5G2+$NEJPP-IA%NGRIRJPC%OQHIZC4.OI:OIG/Q80P2W4VZ0K1H$$05QN*Y0K.G +AG5T01HJCAMKN$71Z95Z11VTO.L8YBJ-B93:GQBGZHHBIH5C99.B4DBF:F0.8ELG:.CC-8LQECKEBLDSH8XAG.6A-JE:GQA KX-SZDG0$JO+SW*PR+PHXF8IQV$K%OKOUFBBQR-S3D1PI0/7Q.H0807-L9CL62/2JJ11K2919GI1X1DDM8RMA0/41:6Z.2:NC-%CN$KJLCLF9+FJE 4Y3LL/II 05B9.Z8M+8:Y001HCY0R%0IGF5JNCPIGSUNG6YS75XJ/J0/V7.UI$RU8ZB.W2FI28LHUZUYZQNI9Y FQQGQ$FP DDVBDVBBX33UQLTU8L20H6/*12SADB9:G9J+9Y 5LJA8JF8JFHJP7NVDEBK3JQ7TI 05QNT+CCZ1ZA2I+T*R9XZ6/:COTJCURIF8CZPCJ4EF5LU5I-Q:.N$P9DX5NAM*PJYD3L2V0GBG.JL4LESU72S1CM%5OC%VSTJ8NC1TGO:QS02V505GJUTH");
				fDGCVerifier_c_release(verifier);
				logCertificate(certificate, logger);
				CertificateSimple_c_release(certificate);
			}

			fDGCRulesKeysUpdater_c_release(rulesKeysUpdater);

			dlclose(verificaC19Handle);
		}

		RulesProviderItaly_c_release(rulesProvider);
		RulesStorageMemory_c_release(rulesStorage);
		KeysProviderItaly_c_release(keysProvider);
		KeysStorageMemory_c_release(keysStorage);
	}

	LoggerStdout_c_release(logger);
}
