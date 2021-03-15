// drives.cpp
// Defines the entry point for the console application.
// Enumerate the DRIVES found, total and free for each.
// ====================================================

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "drives.hxx" // includes "drives_io.hxx"
#define MEOR    "\n"
// #define sprtf printf
#define StringCbPrintf sprintf_s

int verbose = 0;

DWORD drvmask;
DWORD SectorsPerCluster, BytesPerSector, NumberOfFreeClusters, TotalNumberOfClusters;
ULARGE_INTEGER FreeBytesAvailable, TotalNumberOfBytes, TotalNumberOfFreeBytes;

static double TotalBytes = 0.0;
static double TotalFree = 0.0;
static int TotalCount = 0;

#ifndef _SPRTF_HXX_
//#define BUF_MAX     4096  // was 264
//#define MAX_BUFS    32
static int iNextBuf = 0;
static TCHAR _s_buffer[BUF_MAX * MAX_BUFS];
TCHAR * GetNxtBuf(void)
{
    iNextBuf++;
    if (iNextBuf >= MAX_BUFS)
        iNextBuf = 0;
    return &_s_buffer[BUF_MAX * iNextBuf];
}
#endif // #ifndef _SPRTF_HXX_

// get nice number (double)
TCHAR * get_d_num( double byts )
{
   TCHAR * pb = GetNxtBuf();    // _s_kbuf64;
   if( byts < 1024 ) {
      StringCbPrintf(pb, 256, _T("%f bytes"), byts);
   } else if( byts < 1024*1024 ) {
      double ks = ((double)byts / 1024.0);
      StringCbPrintf(pb, 256, _T("%0.2f KB"), ks);
   } else if( byts < 1024*1024*1024 ) {
      double ms = ((double)byts / (1024.0*1024.0));
      StringCbPrintf(pb, 256, _T("%0.2f MB"), ms);
   } else {
      double gs = ((double)byts / (1024.0*1024.0*1024.0));
      StringCbPrintf(pb, 256, _T("%0.2f GB"), gs);
   }
   return pb;
}

TCHAR * get_k_num64( ULARGE_INTEGER uli )
{
   TCHAR * pb = GetNxtBuf();
   TCHAR * cp;
   size_t len;
   double byts = ((double)uli.HighPart * 4294967296.0);
   byts += uli.LowPart;
   cp = get_d_num( byts );
   len = strlen(cp);
   sprintf(pb,"%12s",cp);
   return pb;
}

TCHAR * comma_sep_number_string( TCHAR * pb1 )
{
   TCHAR * pb2 = GetNxtBuf();
   size_t len, i, rem, off;
   len = lstrlen(pb1);
   rem = (len % 3);
   off = 0;
   if(rem)
      rem++;
   for(i = 0; i < len; i++) {
      if( rem ) {
         rem--;
         if( i && ( rem == 0 ))
            pb2[off++] = ',';
      }
      if( rem == 0 )
         rem = 3;
      pb2[off++] = pb1[i];
   }
   pb2[off] = 0;
   return pb2;
}


TCHAR * get_comma_sep_number( ULARGE_INTEGER uli )
{
   TCHAR * pb1 = GetNxtBuf();   // _s_kbuf64_1;
   StringCbPrintf(pb1, 256, _T("%I64u"), uli.QuadPart);
   return( comma_sep_number_string(pb1) );
}

TCHAR * get_comma_sep_number_padded( ULARGE_INTEGER uli, size_t min )
{
   TCHAR * pb1 = GetNxtBuf();   // _s_kbuf64_1;
   StringCbPrintf(pb1, 256, _T("%I64u"), uli.QuadPart);
   TCHAR * pb2 = comma_sep_number_string(pb1);
   size_t len = strlen(pb2);
   if (len < min) {
       size_t diff = min - len;
       *pb1 = 0;
       while(diff--)
           strcat(pb1," ");
       strcat(pb1,pb2);
       pb2 = pb1;
   }
   return pb2;
}


DWORDLONG DoMult3_ints( DWORD dw1, DWORD dw2, DWORD dw3 )
{
	DWORDLONG	dwlr, dwl1, dwl2, dwl3;
	dwl1 = dw1;
	dwl2 = dw2;
	dwl3 = dw3;
	dwlr = dwl1 * dwl2;
	dwlr = dwlr * dwl3;
	return dwlr;
}

DWORDLONG DoMult3( DWORD dw1, DWORD dw2, DWORD dw3 )
{
   double dwl1, dwl2, dwl3, dwrd;
	DWORDLONG	dwlr;
	dwl1 = (double)dw1;
	dwl2 = (double)dw2;
	dwl3 = (double)dw3;
	dwrd = dwl1 * dwl2;
	dwrd = dwrd * dwl3;
   dwlr = (DWORDLONG)dwrd;
	return dwlr;
}

double dDoMult3( DWORD dw1, DWORD dw2, DWORD dw3 )
{
   double dwl1, dwl2, dwl3, dwrd;
	dwl1 = (double)dw1;
	dwl2 = (double)dw2;
	dwl3 = (double)dw3;
	dwrd = dwl1 * dwl2;
	dwrd = dwrd * dwl3;
	return dwrd;
}

TCHAR * get_trim_float( double db )
{
   TCHAR * pb1 = GetNxtBuf();   // _s_cbbuf1;
   size_t len, i;
   size_t dot = 0;
   int allzero = 1;
   StringCbPrintf(pb1, 256, _T("%f"), db );
   len = lstrlen(pb1);
   for(i = 0; i < len; i++)
   {
      if( pb1[i] == '.' )
      {
         dot = i;
         i++;
         for(; i < len; i++)
         {
            if( pb1[i] != '0' )
            {
               allzero = 0;
               break;
            }
         }
      }
   }
   if(dot && allzero) {
      pb1[dot] = 0;
   }
   return pb1;
}


// If the function succeeds, the return value is a bitmask representing 
// the currently available disk drives. 
// Bit position 0 (the least-significant bit) is drive A, 
// bit position 1 is drive B, bit position 2 is drive C, and so on.
DWORD show_logical_drives(void)
{
    DWORD test = 1;
    int letter = 'A';
    int count = 0;
    DWORD   curr;

    drvmask = GetLogicalDrives();
    curr = drvmask;
    while (test && curr) {
        if (curr & test) {
            if (count == 0)
                SPRTF("Drives: ");
            SPRTF("%c: ", letter);
            curr &= ~test;
            count++;
        }
        test = test << 1;
        letter++;
    }
    if (count) {
        SPRTF(" - %d drives...\n", count);
    } else {
        SPRTF("ERROR: GetLogicalDrives FAILED!\n");
    }
    if (verbose) {
        TCHAR * nb = GetNxtBuf();
        curr = GetLogicalDriveStrings(BUF_MAX, nb);
        if (curr) {
            curr = 0;
            SPRTF("DriveStrings: ");
            while (*nb) {
                SPRTF("%s ", nb);
                curr++;
                while (*nb) {
                    nb++;   // get to first null
                }
                if (nb[1])
                    nb++;
            }
            SPRTF("- %ld drives.\n", curr);
        }
        else {
            SPRTF("ERROR: GetLogicalDriveStrings(BUF_MAX, nb); FAILED\n");
        }
    }
    return drvmask;
}

static TCHAR VolumeNameBuffer[264];
static DWORD VolumeSerialNumber;
static DWORD MaximumComponentLength, FileSystemFlags;
static TCHAR FileSystemNameBuffer[264];
static TCHAR buffer[264];
TCHAR * Get_Volume_Info( TCHAR * drv )
{
    size_t len;
    TCHAR * lpRootPathName = buffer;
    LPTSTR lpVolumeNameBuffer = VolumeNameBuffer;
    LPDWORD lpVolumeSerialNumber = &VolumeSerialNumber;
    LPDWORD lpMaximumComponentLength = &MaximumComponentLength;
    LPDWORD lpFileSystemFlags = &FileSystemFlags;
    LPTSTR lpFileSystemNameBuffer = FileSystemNameBuffer;
    strcpy(lpRootPathName,drv);
    len = strlen(lpRootPathName);
    if (len && (lpRootPathName[len-1] == '.')) {
        len--;
        lpRootPathName[len] = 0;
    }
    if ( GetVolumeInformation( lpRootPathName,
        lpVolumeNameBuffer, 256,    // DWORD nVolumeNameSize,
        lpVolumeSerialNumber,
        lpMaximumComponentLength,
        lpFileSystemFlags,
        lpFileSystemNameBuffer, 256 ) )
    {
        sprintf( lpRootPathName, "Volume: [%s], Type [%s]", lpVolumeNameBuffer,
            lpFileSystemNameBuffer );
        return lpRootPathName;
    }
    return "No volume info!";
}

void GetDiskSpace( _TCHAR * ps )
{
   int shwn = 0;
   TCHAR * pb1 = GetNxtBuf();   //_s_cbbuf1;
   TCHAR * pb2 = GetNxtBuf();   //_s_cbbuf2;
   // on some system a horrid dialog pops up, so
   UINT ui = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOALIGNMENTFAULTEXCEPT |
       SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);
   // ======================================================================
   if( GetDiskFreeSpaceEx( ps, //  LPCTSTR lpDirectoryName,
      &FreeBytesAvailable,
      &TotalNumberOfBytes,
      &TotalNumberOfFreeBytes ) )
   {
       TotalBytes += TotalNumberOfBytes.QuadPart;
       TotalFree  += TotalNumberOfFreeBytes.QuadPart;
       TotalCount++;
       if (verbose) {
           SPRTF( _T("Disk %s: %s") MEOR, ps, Get_Volume_Info(ps) );
          StringCbPrintf(pb1, 256, _T("Total %s (%s bytes),") MEOR,
                get_k_num64( TotalNumberOfBytes ),
                get_comma_sep_number_padded(TotalNumberOfBytes,20) );
          SPRTF(pb1);
          SPRTF( _T("Free  %s (%s bytes).") MEOR,
                get_k_num64( TotalNumberOfFreeBytes ),
                get_comma_sep_number_padded(TotalNumberOfFreeBytes,20) );
       } else {
          SPRTF( _T("Disk %s: "), ps );
          SPRTF( _T("Total %s, "), get_k_num64( TotalNumberOfBytes ));
          SPRTF( _T("Free  %s.") MEOR, get_k_num64( TotalNumberOfFreeBytes ) );
       }
      shwn++;
   }

   if( !shwn && GetDiskFreeSpace( ps, &SectorsPerCluster, &BytesPerSector,
         &NumberOfFreeClusters, &TotalNumberOfClusters) )
   {
      //DWORDLONG dwlfree = DoMult3( BytesPerSector, SectorsPerCluster, NumberOfFreeClusters );
      //DWORDLONG dwltot = DoMult3( BytesPerSector, SectorsPerCluster, TotalNumberOfClusters );
      double dwlfree = dDoMult3( BytesPerSector, SectorsPerCluster, NumberOfFreeClusters );
      double dwltot = dDoMult3( BytesPerSector, SectorsPerCluster, TotalNumberOfClusters );
       TotalBytes += dwltot;
       TotalFree  += dwlfree;
       TotalCount++;
      if (verbose) {
          SPRTF( _T("Disk %s:") MEOR, ps );
          //SPRTF( _T("Total %lu bytes,") MEOR, dwlfree );
          //SPRTF( _T("Free  %lu bytes.") MEOR, dwltot );
          //SPRTF( _T("BPS:%u, SPC:%u, TOT:%u, FREE:%u.") MEOR,
          //   BytesPerSector, SectorsPerCluster, TotalNumberOfClusters, NumberOfFreeClusters );
          SPRTF( _T("Total %s (%s bytes),") MEOR,
             get_d_num( dwltot ),
             comma_sep_number_string(get_trim_float(dwltot)) );
          SPRTF( _T("Free  %s (%s bytes).") MEOR,
             get_d_num( dwlfree ),
             comma_sep_number_string(get_trim_float(dwlfree)) );
      } else {
          SPRTF( _T("Disk %s: "), ps );
          SPRTF( _T("Total %s, "), get_d_num( dwltot ));
          SPRTF( _T("Free  %s.") MEOR,
             get_d_num( dwlfree ) );
      }
      shwn++;
   }
   
   if( !shwn && verbose ) {
      SPRTF( _T("Disk %s: FAILED TO GET VALUES") MEOR, ps );
   }
   SetErrorMode(ui);
}

void show_version( char * name )
{
    size_t off = 0;
    size_t len = strlen(name);
    size_t i;
    char * nb;
    for (i = 0; i < len; i++) {
        int ch = name[i];
        if ((ch == '/') || (ch == '\\'))
            off = i + 1;
    }
    nb = GetNxtBuf();
    strcpy(nb,&name[off]);
    SPRTF("%s: Compiled on [%s], at [%s]\n",
        nb, __DATE__, __TIME__ );
}

#define ISNUM(a)    (( a >= '0') && (a <= '9'))

int is_all_nums( char * arg )
{
    size_t len = strlen(arg);
    size_t i;
    for (i = 0; i < len; i++) {
        if ( !ISNUM(arg[i]) )
            return 0;
    }
    return 1;
}

void show_help()
{
    char* log = get_log_file();
    printf("drives: Show valid logical drives... A...Z\n");
    printf("Only option is -v[n] to show more information.\n");
    printf("Output written to '%s' log.\n", log ? log : "Not Available");
}

int main(int argc, char * argv[])
{
    if (argc > 1) {
        int i;
        for (i = 1; i < argc; i++) {
            char * arg = argv[i];
            if ((strcmp(arg, "--version") == 0) || (strcmp(arg, "--help") == 0))
                goto help;
            if (*arg == '-') {
                arg++;
                if (*arg == 'v') {
                    arg++;
                    if (*arg) {
                        if (is_all_nums(arg)) {
                            verbose = atoi(arg);
                        }
                        else {
                            goto Bad_Param;
                        }
                    }
                    else {
                        verbose++;
                    }
                } else if ((*arg == '?') || (*arg == 'h')) {
help:
                    show_version(argv[0]);
                    show_help();
                    return 0;

                } else {
                    goto Bad_Param;
                }
            } else {
Bad_Param:
                printf("ERROR: Unknown parameter [%s]! Aborting...\n", argv[i]);
                return 1;
            }
        }
    }
    //if (verbose)
    //    show_version( argv[0] );

    if (show_logical_drives()) {
        DWORD mask = drvmask;
        DWORD test = 1;
        int letter = 'A';
        char buffer[32];
        char * cp = buffer;
        while ( test && mask )
        {
            if ( test & mask ) {
                sprintf(cp,"%c:\\.", letter);
                GetDiskSpace( cp );
            }
            mask &= ~test;
            test = test << 1;
            letter++;
        }
        if (verbose) {
            SPRTF( "Drives %d: Total %s, Free %s\n", TotalCount,
                get_d_num( TotalBytes ),
                get_d_num( TotalFree ) ); 
        }

    }

    if (verbose > 4)
        GetDriveGeometriesEx();

	return 0;
}

/* -----------------------------
Drive: [\\.\PhysicalDrive0]: HDD Cylinders = 38913, Tracks/cylinder = 255, Sectors/track = 63, Bytes/sector = 512, Size = 320070320640 (Bytes) = 298 (Gb)
Drive: [\\.\PhysicalDrive1]: HDD Cylinders = 14593, Tracks/cylinder = 255, Sectors/track = 63, Bytes/sector = 512, Size = 120031511040 (Bytes) = 111 (Gb)
Drive: [\\.\PhysicalDrive2]: HDD Cylinders = 60801, Tracks/cylinder = 255, Sectors/track = 63, Bytes/sector = 512, Size = 500105249280 (Bytes) = 465 (Gb)
Drive: [\\.\PhysicalDrive3] FAILED. Error [The device is not ready.](21).
Drive: [\\.\PhysicalDrive4] FAILED. Error [The device is not ready.](21).
Drives: C: D: E: F: G: H: I: J:  - 8 drives...
Disk C:\.: Total    218.89 GB, Free      96.54 GB.
Disk D:\.: Total     74.50 GB, Free       9.19 GB.
Disk E:\.: Total    111.78 GB, Free      36.72 GB.
Disk J:\.: Total    465.76 GB, Free     113.27 GB.

Using EX function
Drive: [\\.\PhysicalDrive0]: HDD Cylinders = 38913, Tracks/cylinder = 255, Sectors/track = 63, Bytes/sector = 512, Size = 320072933376 (Bytes) = 298 (Gb)
Petition Style: [Master boot record (MBR) - standard AT-style]
Drive: [\\.\PhysicalDrive1]: HDD Cylinders = 14593, Tracks/cylinder = 255, Sectors/track = 63, Bytes/sector = 512, Size = 120034123776 (Bytes) = 111 (Gb)
Petition Style: [Master boot record (MBR) - standard AT-style]
Drive: [\\.\PhysicalDrive2]: HDD Cylinders = 60801, Tracks/cylinder = 255, Sectors/track = 63, Bytes/sector = 512, Size = 500107862016 (Bytes) = 465 (Gb)
Petition Style: [Master boot record (MBR) - standard AT-style]
   ----------------------------- */
// drives.cxx

