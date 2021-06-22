// We need this header file, as we cannot export macros from header files. T_T
#ifndef _variadic_h_included
#define  _variadic_h_included

#define va_arg __crt_va_arg
#define va_end __crt_va_end
#define va_start(ap, n) ((void)(__va_start(&ap, n)))
#define __crt_va_arg(ap, t) \
	((sizeof(t) > sizeof(__int64) || (sizeof(t) & (sizeof(t) - 1)) != 0) \
		? **(t**)((ap += sizeof(__int64)) - sizeof(__int64)) \
		: *(t* )((ap += sizeof(__int64)) - sizeof(__int64)))
#define __crt_va_end(ap) ((void)(ap = (va_list)0))

typedef char* va_list;
extern "C" void __cdecl __va_start(va_list*, ...);

#endif // _variadic_h_included
