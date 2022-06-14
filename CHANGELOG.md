# Changelog

## [1.1.11] - 14/07/2022

- Removed IT entry mode

## [1.1.10] - 07/04/2022

- New rules for IT entry for Under 18

## [1.1.9] - 31/03/2022

- Removed work scan mode

## [1.1.8] - 03/01/2022

- Added User-Agent:  verificac19-sdk-cpp/<version> to requests to API

## [1.1.7] - 16/02/2022

- Align to official Android SDK 1.1.5

### Added

- Added entry to italy scan mode
- Align with new medical rules for italian DL 4 february

### Removed

- Removed school scan mode

## [1.1.6] - 09/02/2022

- Fix checks for certificate with multiple records

## [1.1.5] - 07/02/2022

- Fix recovery certificate end validity for school mode

## [1.1.4] - 31/01/2022

- Align to official Android SDK 1.1.4

### Added
- New medical rules for vaccine, booster and recovery certificates

## [1.1.3] - 26/01/2022

- Align to official Android SDK 1.1.2 for exemption certificates

See https://github.com/rawmain/it-dgc-documentation/blob/feat/scanmode-work-school-exemption/SCANMODE.md
or https://github.com/rawmain/it-dgc-documentation/blob/master/SCANMODE.md for detailed info

### Added
- Support fo exemption certificate
- Work mode, not valid test for over 50
- School mode

## [1.1.2] - 03/01/2022

- Align to official Android SDK 1.1.1

This version has uncompatible constructors and methods with previous version for DRL implementation.

### Added
- DRL (DGC Revocation List)

## [1.1.1] - 03/01/2022

- Align to official Android SDK 1.1.0

### Added
- Booster mode, valid only booster vaccine, for complete vaccine or recovery will
request additional test

## [1.1.0] - 21/12/2021

### Removed
- Removed `PARTIALLY_VALID` status

## [1.0.4] - 10/12/2021

- Aligned to official Android SDK 1.0.4

### Added
- Super Green Pass mode (does not accept tests)
