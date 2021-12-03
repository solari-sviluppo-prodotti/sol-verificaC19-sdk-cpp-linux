// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#ifndef CERTIFICATE_STATUS_HPP
#define CERTIFICATE_STATUS_HPP

/**
 *
 * This enum class defines all the possible status of certifications after their verification.
 *
 */
enum CertificateStatus {
	NOT_VALID,
	NOT_VALID_YET,
	VALID,
	PARTIALLY_VALID,
	NOT_EU_DCC
};

typedef enum CertificateStatus CertificateStatus;

#ifdef __cplusplus
namespace verificaC19Sdk {

typedef enum ::CertificateStatus CertificateStatus;

} // namespace verificaC19Sdk
#endif

#endif // #ifndef CERTIFICATE_STATUS_HPP
