#define _AMD64_
#include <libloaderapi.h>

typedef void* HANDLE;
typedef HANDLE HWND;
typedef HANDLE HMENU;

constexpr DWORD WS_OVERLAPPED = 0x00000000L;
constexpr DWORD WS_CAPTION = 0x00C00000L;
constexpr DWORD WS_SYSMENU = 0x00080000L;
constexpr DWORD WS_THICKFRAME = 0x00040000L;
constexpr DWORD WS_MINIMIZEBOX = 0x00020000L;
constexpr DWORD WS_MAXIMIZEBOX = 0x00010000L;
constexpr DWORD WS_OVERLAPPEDWINDOW = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;

constexpr int CW_USEDEFAULT = 0x80000000;

extern "C" HWND CreateWindowExA(
	DWORD dwExStyle,
	LPCSTR lpClassName,
	LPCSTR lpWindowName,
	DWORD dwStyle,
	int X,
	int Y,
	int nWidth,
	int nHeight,
	HWND hWndParent,
	HMENU hMenu,
	HINSTANCE hInstance,
	LPVOID lpParam
);

extern "C" BOOL ShowWindow(
	HWND hWnd,
	int nCmdShow
);

constexpr DWORD STD_OUTPUT_HANDLE = (DWORD)-11;

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

typedef LRESULT(CALLBACK* WNDPROC)(
	HWND hwnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
);

typedef HANDLE HICON;
typedef HANDLE HCURSOR;
typedef HANDLE HBRUSH;

extern "C" struct WNDCLASS {
	UINT  style;
	WNDPROC lpfnWndProc;
	int cbClsExtra;
	int cbWndExtra;
	HINSTANCE hInstance;
	HICON hIcon;
	HCURSOR hCursor;
	HBRUSH hbrBackground;
	LPCSTR lpszMenuName;
	LPCSTR lpszClassName;
};

extern "C" ATOM RegisterClassA(
	const WNDCLASS * lpWndClass
);

constexpr UINT WM_DESTROY = 0x0002;
constexpr UINT WM_PAINT = 0x000f;
constexpr UINT WM_QUIT = 0x0012;

extern "C" void PostQuitMessage(
	int nExitCode
);

extern "C" LRESULT DefWindowProcA(
	HWND hWnd,
	UINT Msg,
	WPARAM wParam,
	LPARAM lParam
);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProcA(hwnd, uMsg, wParam, lParam);
}

extern "C" struct POINT {
	LONG x;
	LONG y;
};

extern "C" struct MSG {
	HWND hwnd;
	UINT message;
	WPARAM wParam;
	LPARAM lParam;
	DWORD time;
	POINT pt;
	DWORD lPrivate;
};

typedef MSG* LPMSG;

constexpr UINT PM_REMOVE = 0x0001;

extern "C" BOOL PeekMessageA(
	LPMSG lpMsg,
	HWND hWnd,
	UINT wMsgFilterMin,
	UINT wMsgFilterMax,
	UINT wRemoveMsg
);

extern "C" BOOL TranslateMessage(
	const MSG * lpMsg
);

extern "C" LRESULT DispatchMessage(
	const MSG * lpMsg
);

extern "C" UINT GetACP();

int main() {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	if (hStdout == nullptr) {
		return 1;
	}

	WriteConsoleA(hStdout, "test\n", 5, nullptr, nullptr);

	HINSTANCE hInstance = GetModuleHandle(nullptr);

	if (hInstance == nullptr) {
		WriteConsoleA(hStdout, "failed to get module handle\n", 28, nullptr, nullptr);
		return 1;
	}

	constexpr const char* CLASS_NAME = "vulkan-fun";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClassA(&wc);

	HWND hWindow = CreateWindowExA(0, CLASS_NAME, "Having fun with Vulkan API", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

	if (hWindow == nullptr) {
		WriteConsoleA(hStdout, "failed to create window handle\n", 31, nullptr, nullptr);
		return 1;
	}

	ShowWindow(hWindow, 0);

	while (1) {
		MSG msg;

		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				return 0;
			}
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
