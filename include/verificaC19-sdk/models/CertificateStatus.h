// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CERTIFICATE_STATUS_H
#define CERTIFICATE_STATUS_H

/**
 *
 * This enum class defines all the possible status of certifications after their verification.
 *
 */
enum CertificateStatus {
	NOT_VALID = 0,
	NOT_VALID_YET = 1,
	VALID = 2,
	NOT_EU_DCC = 4,
	TEST_NEEDED = 5
};

typedef enum CertificateStatus CertificateStatus;

#endif // #ifndef CERTIFICATE_STATUS_H
