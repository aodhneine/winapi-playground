module;
#define _AMD64_
#include <windef.h>
export module winapi;

export namespace winapi {
	constexpr UINT CP_UTF8 = 65001;

	extern "C" BOOL WINAPI SetConsoleOutputCP(
		UINT wCodePageID
	);

	extern "C" BOOL WINAPI SetConsoleCP(
		UINT wCodePageID
	);

	extern "C" UINT GetACP();

	constexpr DWORD STD_OUTPUT_HANDLE = (DWORD) -11;

	extern "C" BOOL WINAPI WriteConsoleA(
		HANDLE hConsoleOutput,
		const VOID * lpBuffer,
		DWORD nNumberOfCharsToWrite,
		LPDWORD lpNumberofCharsWritten,
		LPVOID lpReserved
	);

	extern "C" HANDLE WINAPI GetStdHandle(
		DWORD nStdHandle
	);
}
