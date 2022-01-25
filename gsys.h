
#ifndef GSYSH_INCLUDED
#define GSYSH_INCLUDED

#ifdef _WIN32
#define TGOSWIN32
#ifndef _WIN64
#define _WINSOCKAPI_	// don't suck in old winsock---- mingw64 does this
#endif
#else
#define TGOSLINUX
typedef int SOCKET;
#define INVALID_SOCKET -1
#endif

#define LOG_ON
#define byt unsigned char
#define lwrd unsigned int
#define swrd unsigned short int
#define PACKED4 __attribute__ ((aligned (4), packed))


#endif