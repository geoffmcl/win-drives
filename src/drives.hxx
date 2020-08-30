// drives.hxx
#ifndef _drives_hxx_
#define _drives_hxx_

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif				

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "drives_io.hxx"

#define BUF_MAX     4096
#define MAX_BUFS    128

extern TCHAR * GetNxtBuf(void);
extern TCHAR * get_comma_sep_number( ULARGE_INTEGER uli );
extern TCHAR * get_comma_sep_number_padded( ULARGE_INTEGER uli, size_t min );
extern TCHAR * get_d_num( double byts );
extern TCHAR * get_k_num64( ULARGE_INTEGER uli );

extern int verbose;


#endif // #ifndef _drives_hxx_
// drives.hxx


