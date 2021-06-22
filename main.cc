#define _AMD64_
#include <windef.h>

constexpr DWORD STD_OUTPUT_HANDLE = (DWORD) -11;

extern "C" HANDLE WINAPI GetStdHandle(
	DWORD nStdHandle
);

extern "C" BOOL WINAPI WriteConsoleA(
	HANDLE hConsoleOutput,
	const VOID * lpBuffer,
	DWORD nNumberOfCharsToWrite,
	LPDWORD lpNumberofCharsWritten,
	LPVOID lpReserved
);

constexpr UINT CP_UTF8 = 65001;

extern "C" BOOL WINAPI SetConsoleOutputCP(
	UINT wCodePageID
);

extern "C" BOOL WINAPI SetConsoleCP(
	UINT wCodePageID
);

extern "C" UINT GetACP();

typedef char* va_list;
#define va_arg __crt_va_arg
#define va_end __crt_va_end
#define va_start(ap, n) ((void)(__va_start(&ap, n)))
#define __crt_va_arg(ap, t) \
	((sizeof(t) > sizeof(__int64) || (sizeof(t) & (sizeof(t) - 1)) != 0) \
		? **(t**)((ap += sizeof(__int64)) - sizeof(__int64)) \
			:  *(t* )((ap += sizeof(__int64)) - sizeof(__int64)))
#define __crt_va_end(ap) ((void)(ap = (va_list)0))
extern "C" void __cdecl __va_start(va_list*, ...);

void __printf_dump_buffer_into(HANDLE file, const char *const buffer, DWORD bufferLength) {
	WriteConsoleA(file, buffer, bufferLength, nullptr, nullptr);
}

void __printf_format_integer(UINT number, char* buffer, DWORD* buffer_length) {
	DWORD i = 0;
	
	while (number != 0) {
		UINT digit = number % 10;
		number = number / 10;
		buffer[15 - i] = '0' + (char) digit;
		++i;
	}

	*buffer_length = i;
}

void printf(const char *const fmt, ...) {
	HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);
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
				WriteConsoleA(stdout, integer_repr_buffer + 16 - integer_repr_buffer_length, integer_repr_buffer_length, nullptr, nullptr);
			} break;
			case 's': {
				char* string = va_arg(args, char*);
				WriteConsoleA(stdout, string, (DWORD) strlen(string), nullptr, nullptr);
			} break;
			default:
				// Unknown specifier, silently ignore the next argument.
				(void) va_arg(args, void*);
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

int main() {
	HANDLE stdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (stdout == nullptr) {
		return 1;
	}

	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);

	UINT codePage = GetACP();
	
	printf("Code page identified as: %d\n", codePage);
	printf("UTF-8 encoding test: %s\n", "こんにちは");

	return 0;
}
