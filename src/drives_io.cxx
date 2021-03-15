// drive_io.cxx

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "drives.hxx"
#include <WinIOCtl.h>

#ifndef EndBuf
#define EndBuf(a) ( a + strlen(a) )
#endif

TCHAR * Get_Hex_Stg( BYTE * bp, DWORD len )
{
    TCHAR * cp = GetNxtBuf();
    DWORD dwi;
    *cp = 0;
    if (bp) {
        for (dwi = 0; dwi < len; dwi++) {
            sprintf(EndBuf(cp),"%02X", (bp[dwi] & 0x0FF));
        }
    }
    return cp;
}

/* -----------------------------------------
BOOL WINAPI DeviceIoControl(
  __in         HANDLE hDevice,
  __in         DWORD dwIoControlCode,
  __in_opt     LPVOID lpInBuffer,
  __in         DWORD nInBufferSize,
  __out_opt    LPVOID lpOutBuffer,
  __in         DWORD nOutBufferSize,
  __out_opt    LPDWORD lpBytesReturned,
  __inout_opt  LPOVERLAPPED lpOverlapped
);
hDevice [in] 
A handle to the device on which the operation is to be performed. 
The device is typically a volume, directory, file, or stream. To 
retrieve a device handle, use the CreateFile function. For more 
information, see Remarks

Remarks
To retrieve a handle to the device, you must call the CreateFile 
function with either the name of a device or the name of the driver 
associated with a device. To specify a device name, use the 
following format:

\\.\DeviceName

DeviceIoControl can accept a handle to a specific device. For example, 
to open a handle to the logical drive A: with CreateFile, specify \\.\a:. 
Alternatively, you can use the names \\.\PhysicalDrive0, \\.\PhysicalDrive1, 
and so on, to open handles to the physical drives on a system.

You should specify the FILE_SHARE_READ and FILE_SHARE_WRITE access flags
when calling CreateFile to open a handle to a device driver. However, 
when you open a communications resource, such as a serial port, you must 
specify exclusive access. Use the other CreateFile parameters as follows 
when opening a device handle:

The fdwCreate parameter must specify OPEN_EXISTING. 
The hTemplateFile parameter must be NULL. 
The fdwAttrsAndFlags parameter can specify FILE_FLAG_OVERLAPPED to indicate 
that the returned handle can be used in overlapped (asynchronous) I/O operations. 
For lists of supported control codes, see the following topics:

Communications Control Codes 
Device Management Control Codes 
Directory Management Control Codes 
Disk Management Control Codes 
File Management Control Codes 
Power Management Control Codes 
Volume Management Control Codes 

Device Management Control Codes
The following control codes are used with changer devices.
Value Meaning 
IOCTL_CHANGER_EXCHANGE_MEDIUM  Moves a piece of media from a source element to one destination, and the piece of media originally in the first destination to a second destination. 
IOCTL_CHANGER_GET_ELEMENT_STATUS  Retrieves the status of all elements or a specified number of elements of a particular type. 
IOCTL_CHANGER_GET_PARAMETERS  Retrieves the parameters of the specified device. 
IOCTL_CHANGER_GET_PRODUCT_DATA  Retrieves the product data for the specified device. 
IOCTL_CHANGER_GET_STATUS  Retrieves the current status of the specified device. 
IOCTL_CHANGER_INITIALIZE_ELEMENT_STATUS  Initializes the status of all elements or the specified elements of a particular type. 
IOCTL_CHANGER_MOVE_MEDIUM  Moves a piece of media to a destination. 
IOCTL_CHANGER_QUERY_VOLUME_TAGS  Retrieves the volume tag information for the specified elements. 
IOCTL_CHANGER_REINITIALIZE_TRANSPORT  Physically recalibrates a transport element. 
IOCTL_CHANGER_SET_ACCESS  Sets the state of the device's insert/eject port, door, or keypad. 
IOCTL_CHANGER_SET_POSITION  Sets the changer's robotic transport mechanism to the specified element address. 

Directory Management Control Codes
The following control codes are used with file compression and decompression.
Value Meaning 
FSCTL_GET_COMPRESSION  Obtains the compression state of a file or directory 
FSCTL_SET_COMPRESSION  Sets the compression state of a file or directory. 
The following control codes are used with reparse points.
Value Meaning 
FSCTL_DELETE_REPARSE_POINT  Deletes a reparse point for a file or directory. 
FSCTL_GET_REPARSE_POINT  Returns reparse point data for a file or directory. 
FSCTL_SET_REPARSE_POINT  Sets a reparse point on a file or directory. 

Disk Management Control Codes
The following table identifies the control codes that are used in disk management.

Control code Operation 
IOCTL_DISK_CREATE_DISK  Initializes the specified disk and disk partition table by using the specified information. 
IOCTL_DISK_DELETE_DRIVE_LAYOUT  Removes the boot signature from the master boot record. 
IOCTL_DISK_FORMAT_TRACKS  Formats a contiguous set of floppy disk tracks. 
IOCTL_DISK_FORMAT_TRACKS_EX  Formats a contiguous set of floppy disk tracks with an extended set of track specification parameters. 
IOCTL_DISK_GET_CACHE_INFORMATION  Retrieves the disk cache configuration data. 
IOCTL_DISK_GET_DRIVE_GEOMETRY_EX  Retrieves information about the physical disk's geometry. 
IOCTL_DISK_GET_DRIVE_LAYOUT_EX  Retrieves information about the number of partitions on a disk and the features of each partition. 
IOCTL_DISK_GET_LENGTH_INFO  Retrieves the length of the specified disk, volume, or partition. 
IOCTL_DISK_GET_PARTITION_INFO_EX  Retrieves partition information for AT and EFI (Extensible Firmware Interface) partitions. 
IOCTL_DISK_GROW_PARTITION  Enlarges the specified partition. 
IOCTL_DISK_IS_WRITABLE  Determines whether the specified disk is writable. 
IOCTL_DISK_PERFORMANCE  Provides disk performance information. 
IOCTL_DISK_PERFORMANCE_OFF  Disables disk performance information. 
IOCTL_DISK_REASSIGN_BLOCKS  Maps disk blocks to spare-block pool. 
IOCTL_DISK_RESET_SNAPSHOT_INFO  Clears all Volume Shadow Copy Service (VSS) hardware-based shadow copy (also called "snapshot") information from the disk. 
IOCTL_DISK_SET_CACHE_INFORMATION  Sets the disk cache configuration data. 
IOCTL_DISK_SET_DRIVE_LAYOUT_EX  Partitions a disk. 
IOCTL_DISK_SET_PARTITION_INFO_EX  Sets the disk partition type. 
IOCTL_DISK_UPDATE_PROPERTIES  Invalidates the cached partition table of the specified disk and re-enumerates the disk. 
IOCTL_DISK_VERIFY  Performs logical format of a disk extent. 

The following list identifies the obsolete control codes:
IOCTL_DISK_CONTROLLER_NUMBER 
IOCTL_DISK_GET_DRIVE_GEOMETRY - Note  IOCTL_DISK_GET_DRIVE_GEOMETRY has been superseded 
by IOCTL_DISK_GET_DRIVE_GEOMETRY_EX, which retrieves additional information.
typedef struct _DISK_GEOMETRY {
  LARGE_INTEGER Cylinders;
  MEDIA_TYPE    MediaType;
  DWORD         TracksPerCylinder;
  DWORD         SectorsPerTrack;
  DWORD         BytesPerSector;
} DISK_GEOMETRY;

typedef struct _DISK_GEOMETRY_EX {
  DISK_GEOMETRY Geometry;
  LARGE_INTEGER DiskSize;
  BYTE          Data[1];
} DISK_GEOMETRY_EX, *PDISK_GEOMETRY_EX;
Remarks
DISK_GEOMETRY_EX is a variable-length structure composed of 
a DISK_GEOMETRY structure followed by a DISK_PARTITION_INFO structure 
and a DISK_DETECTION_INFO structure. Because the detection information 
is not at a fixed location within the DISK_GEOMETRY_EX structure, use the 
following macro to access the DISK_DETECTION_INFO structure.
typedef struct _DISK_PARTITION_INFO {
  DWORD           SizeOfPartitionInfo;
  PARTITION_STYLE PartitionStyle;
  union {
    struct {
      DWORD Signature;
    } Mbr;
    struct {
      GUID DiskId;
    } Gpt;
  } ;
} DISK_PARTITION_INFO, *PDISK_PARTITION_INFO;
typedef enum _PARTITION_STYLE {
  PARTITION_STYLE_MBR   = 0,
  PARTITION_STYLE_GPT   = 1,
  PARTITION_STYLE_RAW   = 2 
} PARTITION_STYLE;
IOCTL_DISK_GET_DRIVE_LAYOUT - Note  IOCTL_DISK_GET_DRIVE_LAYOUT has been superseded
by IOCTL_DISK_GET_DRIVE_LAYOUT_EX, which retrieves layout information for AT and EFI (Extensible Firmware Interface) partitions.
typedef struct _DRIVE_LAYOUT_INFORMATION_EX {
  DWORD                    PartitionStyle;
  DWORD                    PartitionCount;
  union {
    DRIVE_LAYOUT_INFORMATION_MBR Mbr;
    DRIVE_LAYOUT_INFORMATION_GPT Gpt;
  } ;
  PARTITION_INFORMATION_EX PartitionEntry[1];
} DRIVE_LAYOUT_INFORMATION_EX, *PDRIVE_LAYOUT_INFORMATION_EX;
typedef struct {
  PARTITION_STYLE PartitionStyle;
  LARGE_INTEGER   StartingOffset;
  LARGE_INTEGER   PartitionLength;
  DWORD           PartitionNumber;
  BOOLEAN         RewritePartition;
  union {
    PARTITION_INFORMATION_MBR Mbr;
    PARTITION_INFORMATION_GPT Gpt;
  } ;
} PARTITION_INFORMATION_EX;

typedef struct _DRIVE_LAYOUT_INFORMATION_MBR {
  ULONG Signature;
} DRIVE_LAYOUT_INFORMATION_MBR, *PDRIVE_LAYOUT_INFORMATION_MBR;

typedef struct _DRIVE_LAYOUT_INFORMATION_GPT {
  GUID          DiskId;
  LARGE_INTEGER StartingUsableOffset;
  LARGE_INTEGER UsableLength;
  ULONG         MaxPartitionCount;
} DRIVE_LAYOUT_INFORMATION_GPT, *PDRIVE_LAYOUT_INFORMATION_GPT;

IOCTL_DISK_GET_PARTITION_INFO - Note  IOCTL_DISK_GET_PARTITION_INFO is superseded 
by IOCTL_DISK_GET_PARTITION_INFO_EX, which retrieves partition information for AT and Extensible Firmware Interface (EFI) partitions.
IOCTL_DISK_HISTOGRAM_DATA 
IOCTL_DISK_HISTOGRAM_RESET 
IOCTL_DISK_HISTOGRAM_STRUCTURE 
IOCTL_DISK_LOGGING 
IOCTL_DISK_REQUEST_DATA 
IOCTL_DISK_REQUEST_STRUCTURE 
IOCTL_DISK_SET_DRIVE_LAYOUT 
IOCTL_DISK_SET_PARTITION_INFO 

typedef enum _MEDIA_TYPE {
  Unknown,
  F5_1Pt2_512,
  F3_1Pt44_512,
  F3_2Pt88_512,
  F3_20Pt8_512,
  F3_720_512,
  F5_360_512,
  F5_320_512,
  F5_320_1024,
  F5_180_512,
  F5_160_512,
  RemovableMedia,
  FixedMedia,
  F3_120M_512,
  F3_640_512,
  F5_640_512,
  F5_720_512,
  F3_1Pt2_512,
  F3_1Pt23_1024,
  F5_1Pt23_1024,
  F3_128Mb_512,
  F3_230Mb_512,
  F8_256_128,
  F3_200Mb_512,
  F3_240M_512,
  F3_32M_512 
} MEDIA_TYPE;

   ----------------------------------------- */

typedef struct tagMT2STG {
    MEDIA_TYPE mt;
    LPCSTR text;
    LPCSTR sdesc;
}MT2STG, * PMT2STG;

MT2STG sMt2Stg[] = {
    { Unknown, "Format is unknown", "Unknown" },
    { F5_1Pt2_512, "A 5.25\" floppy, with 1.2MB and 512 bytes/sector.", "5.25\" 1.2MB" },
    { F3_1Pt44_512, "A 3.5\" floppy, with 1.44MB and 512 bytes/sector.", "3.5\" 1.44MB" },
    { F3_2Pt88_512, "A 3.5\" floppy, with 2.88MB and 512 bytes/sector.", "3.5\" 2.88MB" },
    { F3_20Pt8_512, "A 3.5\" floppy, with 20.8MB and 512 bytes/sector.", "3.5\" 20.8MB" },
    { F3_720_512, "A 3.5\" floppy, with 720KB and 512 bytes/sector.", "3.5\" 720KB" },
    { F5_360_512, "A 5.25\" floppy, with 360KB and 512 bytes/sector.", "5.25\" 360KB" },
    { F5_320_512, "A 5.25\" floppy, with 320KB and 512 bytes/sector.", "5.25\" 320KB" },
    { F5_320_1024, "A 5.25\" floppy, with 320KB and 1024 bytes/sector.", "5.25\" 320KB" },
    { F5_180_512, "A 5.25\" floppy, with 180KB and 512 bytes/sector.", "5.25\" 180KB" },
    { F5_160_512, "A 5.25\" floppy, with 160KB and 512 bytes/sector.", "5.25\" 160KB" },
    { RemovableMedia, "Removable media other than floppy.", "Removable" },
    { FixedMedia, "Fixed hard disk media.", "HDD" },
    { F3_120M_512, "A 3.5\" floppy, with 120MB and 512 bytes/sector.", "3.5\" 120MB" },
    { F3_640_512, "A 3.5\" floppy, with 640KB and 512 bytes/sector.", "3.5\" 640KB" },
    { F5_640_512, "A 5.25\" floppy, with 640KB and 512 bytes/sector.", "5.25\" 640KB" },
    { F5_720_512, "A 5.25\" floppy, with 720KB and 512 bytes/sector.", "5.25\" 720KB" },
    { F3_1Pt2_512, "A 3.5\" floppy, with 1.2MB and 512 bytes/sector.", "3.5\" 1.2MB" },
    { F3_1Pt23_1024, "A 3.5\" floppy, with 1.23MB and 1024 bytes/sector.", "3.5\" 1.23MB" },
    { F5_1Pt23_1024, "A 5.25\" floppy, with 1.23MB and 1024 bytes/sector.", "5.25\" 1.23MB" },
    { F3_128Mb_512, "A 3.5\" floppy, with 128MB and 512 bytes/sector.", "3.5\" 128MB" },
    { F3_230Mb_512, "A 3.5\" floppy, with 230MB and 512 bytes/sector.", "3.5\" 230MB" },
    { F8_256_128, "An 8\" floppy, with 256KB and 128 bytes/sector.", "8\" 256KB" },
    { F3_200Mb_512, "A 3.5\" floppy, with 200MB and 512 bytes/sector. (HiFD).", "3.5\" 200MB" },
    { F3_240M_512, "A 3.5\" floppy, with 240MB and 512 bytes/sector. (HiFD).", "3.5\" 240MB" },
    { F3_32M_512, "A 3.5\" floppy, with 32MB and 512 bytes/sector.", "3.5\" 32MB" },
    { Unknown,   0, 0 }
};

LPCSTR Media_Type_2_Stg( MEDIA_TYPE mt, int full )
{
    PMT2STG pmt = &sMt2Stg[0];
    while (pmt->text) {
        if (pmt->mt == mt) {
            if (full)
                return pmt->text;
            else
                return pmt->sdesc;
        }
        pmt++;
    }
    PTSTR cp = GetNxtBuf();
    sprintf(cp, "Value %u (%s), NOT IN LIST", mt, Get_Hex_Stg((BYTE*)&mt, sizeof(MEDIA_TYPE)));
    return cp;
}

typedef struct tagPS2STG {
    PARTITION_STYLE ps;
    LPCSTR text;
    LPCSTR sdesc;
} PS2STG, * PPS2STG;

PS2STG sPs2Stg[] = {
    { PARTITION_STYLE_MBR, "Master boot record (MBR) - standard AT-style", "MBR" },
    { PARTITION_STYLE_GPT, "GUID Partition Table (GPT) format.", "GPT" },
    { PARTITION_STYLE_RAW, "Partition not recognized - not MBR or GPT.", "UNK" },
    { (PARTITION_STYLE)0, NULL, NULL }
};

LPCSTR Partition_Style_2_Stg( PARTITION_STYLE ps, int full )
{
    PPS2STG pps = &sPs2Stg[0];
    while (pps->text) {
        if (pps->ps == ps) {
            if (full)
                return pps->text;
            else
                return pps->sdesc;
        }
        pps++;
    }
    PTSTR cp = GetNxtBuf();
    sprintf(cp, "Value %u (0x%s), NOT IN LIST", ps, Get_Hex_Stg((BYTE*)&ps, sizeof(PARTITION_STYLE)));
    return cp;
}

HANDLE Open_Drive_Device( LPCSTR drive )
{
    HANDLE hDevice;
    hDevice = CreateFile(drive, // TEXT("\\\\.\\PhysicalDrive0"),  // drive to open
                    0,                // no access to the drive
                    FILE_SHARE_READ | // share mode
                    FILE_SHARE_WRITE, 
                    NULL,             // default security attributes
                    OPEN_EXISTING,    // disposition
                    0,                // file attributes
                    NULL);            // do not copy file attributes
    return hDevice;
}

#ifndef VFH
#define VFH(a)  ( a && ( a != INVALID_HANDLE_VALUE ) )
#endif

BOOL DoDeviceIO( HANDLE hDevice, DWORD code, LPVOID lpVoid, DWORD vLen, PDWORD pLen )
{
    BOOL bResult = DeviceIoControl(
        hDevice,        // device to be queried
        code,           // operation to perform
        NULL, 0,        // no input buffer
        lpVoid, vLen,   // output buffer, and length
        pLen,           // # bytes returned
        (LPOVERLAPPED) NULL);  // synchronous I/O
    return bResult;
}

BOOL GetDriveGeometry(LPCSTR drive, DISK_GEOMETRY *pdg, PDWORD pLen)
{
    HANDLE hDevice;               // handle to the drive to be examined 
    BOOL bResult;                 // results flag
    hDevice = Open_Drive_Device(drive);
    if (!VFH(hDevice)) {    // cannot open the drive
        return (FALSE);
    }
    //bResult = DeviceIoControl(hDevice,  // device to be queried
    //    IOCTL_DISK_GET_DRIVE_GEOMETRY,  // operation to perform
    //                         NULL, 0, // no input buffer
    //                        pdg, sizeof(*pdg),     // output buffer
    //                        pLen,                 // # bytes returned
    //                        (LPOVERLAPPED) NULL);  // synchronous I/O
    bResult = DoDeviceIO(hDevice,  // device to be queried
        IOCTL_DISK_GET_DRIVE_GEOMETRY,  // operation to perform
         pdg, sizeof(*pdg),     // output buffer
         pLen );                // # bytes returned
    CloseHandle(hDevice);
    return (bResult);
}

BOOL GetDriveGeometryEx(LPCSTR drive, LPVOID pVoid, DWORD vLen, PDWORD pLen)
{
    HANDLE hDevice;               // handle to the drive to be examined 
    BOOL bResult;                 // results flag
    hDevice = Open_Drive_Device(drive);
    if (!VFH(hDevice)) {    // cannot open the drive
        return (FALSE);
    }
    bResult = DoDeviceIO(hDevice,  // device to be queried
        IOCTL_DISK_GET_DRIVE_GEOMETRY_EX,  // operation to perform
        pVoid, vLen,     // output buffer
        pLen );                // # bytes returned
    CloseHandle(hDevice);
    return (bResult);
}

// IOCTL_DISK_GET_DRIVE_LAYOUT_EX
BOOL GetDriveLayoutEx(LPCSTR drive, LPVOID pVoid, DWORD vLen, PDWORD pLen)
{
    HANDLE hDevice;               // handle to the drive to be examined 
    BOOL bResult;                 // results flag
    hDevice = Open_Drive_Device(drive);
    if (!VFH(hDevice)) {    // cannot open the drive
        return (FALSE);
    }
    bResult = DoDeviceIO(hDevice,  // device to be queried
        IOCTL_DISK_GET_DRIVE_LAYOUT_EX,  // operation to perform
        pVoid, vLen,     // output buffer
        pLen );                // # bytes returned
    CloseHandle(hDevice);
    return (bResult);
}


DWORD GetLastErrorMsg( PTSTR lpm, DWORD dwLen, DWORD dwErr )
{
   PVOID lpMsgBuf = 0;
   DWORD    dwr;
   dwr = FormatMessage( 
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErr,   //	GetLastError(),
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
      (LPTSTR) &lpMsgBuf,
		0,
		NULL );
   
   //dwr = strlen(lpMsgBuf);
   if( ( dwr == 0 ) || ( dwr >= dwLen ) )
      dwr = (DWORD)-1;
   else
      strcat(lpm, (const char *)lpMsgBuf);

   //	SPRTF("%s:%s\n",lpm,(LPCTSTR)lpMsgBuf);
   // Free the buffer.
   if( lpMsgBuf )
      LocalFree( lpMsgBuf );
   if (dwr != -1) {
        dwr = (DWORD)strlen(lpm);
        while (dwr) {
            dwr--;
            if (lpm[dwr] > ' ') {
                dwr++;
                break;
            }
            lpm[dwr] = 0;
        }
   }
   return dwr;
}


// NOT USED in this app - See 'GetDriveGeometriesEx'...
void GetDriveGeometries(void)
{
    DISK_GEOMETRY pdg;            // disk drive geometry structure
    BOOL bResult;                 // generic results flag
    ULONGLONG DiskSize;           // size of the drive, in bytes
    int i, failed;
    //LPCSTR drive = TEXT("\\\\.\\PhysicalDrive0");
    PTSTR cp, errb; // = GetNxtBuf();
    DWORD dwLen;

    failed = 0;
    for (i = 0; i < 20; i++) {
        cp = GetNxtBuf();
        sprintf(cp,TEXT("\\\\.\\PhysicalDrive%u"),i);
        bResult = GetDriveGeometry (cp, &pdg, &dwLen);
        if (bResult) {
            SPRTF("\nDrive: [%s]: %s ", cp, Media_Type_2_Stg(pdg.MediaType,0));
            SPRTF("Cylinders = %I64d, ", pdg.Cylinders);
            SPRTF("Tracks/cylinder = %ld, ", (ULONG) pdg.TracksPerCylinder);
            SPRTF("Sectors/track = %ld, ", (ULONG) pdg.SectorsPerTrack);
            SPRTF("Bytes/sector = %ld, ", (ULONG) pdg.BytesPerSector);
            DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *
                (ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;
            SPRTF("Size = %I64d (Bytes) = %I64d (Gb)\n", DiskSize,
                DiskSize / (1024 * 1024 * 1024));
        } else {
            DWORD dwErr = GetLastError();
            errb = GetNxtBuf();
            dwLen = GetLastErrorMsg(errb,BUF_MAX-2,dwErr);
            if (dwLen == -1)
                SPRTF("\nDrive: [%s] FAILED. Error %ld.\n", cp, dwErr);
            else
                SPRTF("\nDrive: [%s] FAILED. Error [%s](%ld).\n", cp, errb, dwErr);

            failed++;
            if (failed > 1) {
                break;
            }
        }
    }
}

void GetDriveGeometriesEx(void)
{
    BOOL bResult;                 // generic results flag
    ULONGLONG DiskSize;           // size of the drive, in bytes
    int i, failed;
    //LPCSTR drive = TEXT("\\\\.\\PhysicalDrive0");
    PTSTR cp, errb; // = GetNxtBuf();
    DWORD dwLen, dwi;
    DISK_GEOMETRY * pdg;
    DWORD dwErr;
    ULARGE_INTEGER ul1, ul2;

    failed = 0;
    SPRTF("\nUsing IOCTL to get raw drive information...(%d)\n", verbose);
    for (i = 0; i < 20; i++) {
        cp = GetNxtBuf();
        sprintf(cp,TEXT("\\\\.\\PhysicalDrive%u"),i);
        pdg = (DISK_GEOMETRY *)GetNxtBuf();
        ZeroMemory(pdg, BUF_MAX);
        bResult = GetDriveGeometryEx(cp, pdg, BUF_MAX, &dwLen);
        if (bResult) {
            PDISK_GEOMETRY_EX pdgx = (PDISK_GEOMETRY_EX)pdg;
            PDISK_PARTITION_INFO pdpi = DiskGeometryGetPartition(pdgx);
            SPRTF("\nDrive: [%s]: %s ", cp, Media_Type_2_Stg(pdg->MediaType,0));
            //DiskSize = pdg->Cylinders.QuadPart * (ULONG)pdg->TracksPerCylinder *
            //    (ULONG)pdg->SectorsPerTrack * (ULONG)pdg->BytesPerSector;
            DiskSize = pdgx->DiskSize.QuadPart;
            //SPRTF("Size = %I64d (Bytes) = %I64d (Gb)\n", DiskSize,
            //    DiskSize / (1024 * 1024 * 1024));
            double sz = (double)pdgx->DiskSize.QuadPart;
            ul1.HighPart = pdgx->DiskSize.HighPart;
            ul1.LowPart  = pdgx->DiskSize.LowPart;
            SPRTF("Size = %s Bytes, %s\n",
                get_comma_sep_number_padded(ul1, 20),
                get_k_num64(ul1) );
            //    DiskSize / (1024 * 1024 * 1024));
            SPRTF("Cylinders = %I64d, ", pdg->Cylinders);
            SPRTF("Tracks/cylinder = %ld, ", (ULONG) pdg->TracksPerCylinder);
            SPRTF("Sectors/track = %ld, ", (ULONG) pdg->SectorsPerTrack);
            SPRTF("Bytes/sector = %ld\n", (ULONG) pdg->BytesPerSector);

            SPRTF("Petition Style: [%s]\n", Partition_Style_2_Stg(pdpi->PartitionStyle,1));
            // IOCTL_DISK_GET_DRIVE_LAYOUT_EX
            PDRIVE_LAYOUT_INFORMATION_EX pdlox = (PDRIVE_LAYOUT_INFORMATION_EX)GetNxtBuf();
            ZeroMemory(pdlox, BUF_MAX);
            dwLen = 0;
            bResult = GetDriveLayoutEx(cp, pdlox, BUF_MAX, &dwLen);
            if (bResult) {
                BYTE * pb = NULL;
                DWORD blen = 0;
                PARTITION_INFORMATION_EX * pix = &pdlox->PartitionEntry[0];
                if (pix->PartitionStyle == PARTITION_STYLE_MBR) {
                   DRIVE_LAYOUT_INFORMATION_MBR * pmbr = &pdlox->Mbr;
                   pb = (BYTE *)&pmbr->Signature;
                   blen = sizeof(ULONG);
                } else if (pix->PartitionStyle == PARTITION_STYLE_GPT) {
                   DRIVE_LAYOUT_INFORMATION_GPT * pgpt = &pdlox->Gpt;
                   //ul = (ULONG)pgpt->DiskId;
                   pb = (BYTE *)&pgpt->DiskId;
                   blen = sizeof(GUID);
                }
                SPRTF("Sig [%s], with up to %d partitions...\n", Get_Hex_Stg(pb,blen), pdlox->PartitionCount );
                for (dwi = 0; dwi < pdlox->PartitionCount; dwi++)
                {
                    pix = &pdlox->PartitionEntry[dwi];
                    ul1.HighPart = pix->StartingOffset.HighPart;
                    ul1.LowPart  = pix->StartingOffset.LowPart;
                    ul2.HighPart = pix->PartitionLength.HighPart;
                    ul2.LowPart  = pix->PartitionLength.LowPart;
                    if (ul2.QuadPart > 0) {
                        SPRTF("%u:%s: %s %s Bytes, Beg=%s (%u)\n", (dwi + 1),
                            Partition_Style_2_Stg(pix->PartitionStyle,0),
                            get_k_num64(ul2),
                            get_comma_sep_number_padded(ul2, 20),
                            get_comma_sep_number_padded(ul1, 20),
                            pix->PartitionNumber );
                    }
                }
            } else {
                dwErr = GetLastError();
                errb = GetNxtBuf();
                dwLen = GetLastErrorMsg(errb,BUF_MAX-2,dwErr);
                if (dwLen == -1)
                    SPRTF("GetDriveLayoutEx: FAILED. Error %ld.\n", dwErr);
                else
                    SPRTF("GetDriveLayoutEx: FAILED. Error [%s](%ld).\n", errb, dwErr);
            }
        } else {
            dwErr = GetLastError();
            errb = GetNxtBuf();
            dwLen = GetLastErrorMsg(errb,BUF_MAX-2,dwErr);
            if (dwLen == -1)
                SPRTF("\nDrive: [%s] FAILED. Error %ld.\n", cp, dwErr);
            else
                SPRTF("\nDrive: [%s] FAILED. Error [%s](%ld).\n", cp, errb, dwErr);

            failed++;
            if (failed > 2) {
                SPRTF("loop %d of 20: Aborting on %d failures...\n", (i + 1), failed);
                break;
            }
        }
    }
}

// eof - drives_io.cxx
