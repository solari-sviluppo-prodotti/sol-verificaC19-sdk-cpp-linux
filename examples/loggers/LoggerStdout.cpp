// Copyright (c) 2021 Solari di Udine S.p.A
// Licensed under the Apache License, Version 2.0

#include <sys/time.h>

#include <stdarg.h>
#include <iostream>
#include <LoggerStdout.hpp>

namespace verificaC19Sdk {

void LoggerStdout::error(const std::string& format, ...) const {
	if (m_level < ERROR) return;
	va_list args1;
	va_start(args1, format);
	log(format, args1);
	va_end(args1);
}

void LoggerStdout::info(const std::string& format, ...) const {
	if (m_level < INFO) return;
	va_list args1;
	va_start(args1, format);
	log(format, args1);
	va_end(args1);
}

void LoggerStdout::debug(const std::string& format, ...) const {
	if (m_level < DEBUG) return;
	va_list args1;
	va_start(args1, format);
	log(format, args1);
	va_end(args1);
}

void LoggerStdout::log(const std::string& format, va_list ap) const {
	//Time
	struct timeval rawtime;
	gettimeofday(&rawtime, NULL);
	struct tm* info=localtime(&rawtime.tv_sec);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%d/%m %H:%M:%S", info);
	std::cout << std::string(buffer);
	std::printf(".%03ld: ", rawtime.tv_usec / 1000);
	std::vprintf(format.c_str(), ap);
	std::cout << std::endl;
}

} // namespace verificaC19Sdk
