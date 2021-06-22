import winapi;
import stdio;

int main() {
	winapi::SetConsoleOutputCP(winapi::CP_UTF8);
	winapi::SetConsoleCP(winapi::CP_UTF8);

	unsigned int codePage = winapi::GetACP();
	
	stdio::printf("Code page identified as: %d\n", codePage);
	stdio::printf("UTF-8 encoding test: %s\n", "こんにちは");
	stdio::printf("Unknown specifier: %l followed by a string: %s\n", "nyaaan");

	return 0;
}
