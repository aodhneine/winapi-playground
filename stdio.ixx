module;
#define _AMD64_
#include <windef.h>
#include "variadic.h"
export module stdio;

import winapi;

namespace stdio {
	void __printf_dump_buffer_into(HANDLE file, const char* const buffer, DWORD bufferLength) {
		winapi::WriteConsoleA(file, buffer, bufferLength, nullptr, nullptr);
	}

	void __printf_format_integer(UINT number, char* buffer, DWORD* buffer_length) {
		DWORD i = 0;

		while (number != 0) {
			UINT digit = number % 10;
			number = number / 10;
			buffer[15 - i] = '0' + (char)digit;
			++i;
		}

		*buffer_length = i;
	}

	export void printf(const char* const fmt, ...) {
		HANDLE stdout = winapi::GetStdHandle(winapi::STD_OUTPUT_HANDLE);
		if (stdout == nullptr) {
			return;
		}

		char buffer[64];
		DWORD buffer_length = 0;

		va_list args;
		va_start(args, fmt);

		size_t fmt_length = strlen(fmt);

		for (size_t i = 0; i < fmt_length; ++i) {
			if (fmt[i] == '%') {
				char specifier = fmt[i + 1];

				__printf_dump_buffer_into(stdout, buffer, buffer_length);
				buffer_length = 0;

				switch (specifier) {
				case 'd': {
					char integer_repr_buffer[16];
					DWORD integer_repr_buffer_length = 0;
					UINT number = va_arg(args, UINT);
					__printf_format_integer(number, integer_repr_buffer, &integer_repr_buffer_length);
					winapi::WriteConsoleA(stdout, integer_repr_buffer + 16 - integer_repr_buffer_length, integer_repr_buffer_length, nullptr, nullptr);
				} break;
				case 's': {
					char* string = va_arg(args, char*);
					winapi::WriteConsoleA(stdout, string, (DWORD)strlen(string), nullptr, nullptr);
				} break;
				default:
					// Unknown specifier, we print it and do nothing with the argument.
					winapi::WriteConsoleA(stdout, &fmt[i], 2, nullptr, nullptr);
					break;
				}

				++i;
				continue;
			}

			buffer[buffer_length] = fmt[i];
			buffer_length += 1;
		}

		if (buffer_length != 0) {
			__printf_dump_buffer_into(stdout, buffer, buffer_length);
		}

		va_end(args);
	}
}
