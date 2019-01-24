//
// Created by hdc on 1/22/19.
//

#ifndef CPLUS_A_H
#define CPLUS_A_H

typedef unsigned int    uc_uint32;  /**< Unsigned 32-bit integer */
typedef unsigned short  uc_uint16;  /**< Unsigned 16-bit integer */
typedef short           uc_int16;  /**< Signed 16-bit integer */
typedef unsigned char   uc_byte;    /**< Unsigned 8-bit byte */
typedef char *          uc_string;  /**< Null-terminated string */
typedef char            uc_char;    /**< Single character */
typedef int             uc_sint32;  /**< Signed 32-bit integer */
typedef unsigned char   uc_uint8;   /**< Unsigned 8-bit integer */
typedef signed char     uc_int8;    /**< Signed 8-bit integer */

typedef int             uc_intptr;  /**< Signed int of the same size as a pointer */
typedef unsigned int    uc_uintptr; /**< Unsigned int of the same size as a pointer */

#define UC_PVR_RECORDCONFIG_FILE_NAME       "PVRRecordConfig.bin"
#define UC_CHECKPRODUCT_LIST_FILE_NAME      "CheckProductList.bin"
#define UC_IFCP_DEVICE_FILE_NAME                        "IFCPDeviceData.bin"
#define UC_IFCP_DEVICE_KEY_FILE_NAME                "IFCPDeviceKeyData.bin"
#define UC_PS_PROPERTY_FILE_NAME            "PSProperty.bin"
#define UC_CSSN_SIZE (4)
#define UC_CSUK_SIZE (16)
#define UC_CPUK_SIZE (16)
#define UC_TAUK_SIZE (16)
#define UC_CSSK_SIZE (16)
#define UC_OTTSK_SIZE (16)
#define UC_CPSK_SIZE (16)
#define UC_PVRMSK_SIZE (16)
#define UC_PVRRK_SIZE (16)
#define UC_PRIVATE_DATA_SIZE (16)
#define UC_PRIVATE_DATA_MAXSIZE (1024)
#define UC_SERVICE_ID_SIZE (2)
#define UC_TRANSPORT_ID_SIZE (2)
#define UC_NETWORK_ID_SIZE (2)
#define UC_PRE_ENABLEMENT_PESK_SIZE (256)
#define UC_PRE_ENABLEMENT_PERSONALIZED_DATA_MAXSIZE (1024)
#define UC_IFCP_APP_CONTROL_FEATURE_BITMAP_SIZE (4)
#define UC_IFCP_CONTENT_USAGE_RULE_SIZE   (4)

typedef struct _HostDeviceDataPersonalizedData
{
    uc_uint32 vLength;
    uc_uint8 vData[UC_PRE_ENABLEMENT_PERSONALIZED_DATA_MAXSIZE];
}HostDeviceDataPersonalizedData;

typedef struct _HostDeviceDataPrivateData
{
    uc_uint32 vLength;
    uc_uint8 vData[UC_PRIVATE_DATA_MAXSIZE];
}HostDeviceDataPrivateData;

typedef struct _HostDeviceData
{
    uc_uint8 vCssn[UC_CSSN_SIZE];
    uc_uint8 vCsuk[UC_CSUK_SIZE];
    uc_uint8 vCpuk[UC_CPUK_SIZE];
#if defined(INCL_TRANSFORMATION)
    uc_uint8 vTauk[UC_TAUK_SIZE];
#endif
    uc_bool  vIs3LevelKeyLadder;
#ifdef INCL_PULLEMM
    uc_uint8 vPINCode[UC_PIN_CODE_SIZE];
    uc_uint8 vMAC[UC_MAC_SIZE];
#endif
    HostDeviceDataPrivateData vPrivateData;  /* modify */
    HostDeviceDataPersonalizedData vPersonalizedData;
    uc_uint8 vOTPIndexCnt;
#ifdef INCL_PULLEMM
    uc_bool  vValidPrivateData;
    uc_bool  vValidCSSN;
#endif
}HostDeviceData;

extern "C" uc_result UT_GetDeviceData(HostDeviceData *vHostDeviceData)
{
    uc_result vResult = UC_ERROR_SUCCESS;
    FILE *vFile = NULL;

    memset((MW_UCHAR *)vHostDeviceData, 0x0, sizeof(HostDeviceData));

    if(UC_ERROR_SUCCESS == vResult)
    {
        CString vDeviceFileName;
        LPCTSTR vDeviceFileNameStr = NULL;

        vDeviceFileName.Format("%s%s", UT_GetExePath(), UT_DEVICE_FILE_NAME);
        vDeviceFileNameStr = vDeviceFileName;
        
        /*Try to open device file.*/
        if( 0 != fopen_s( &vFile, (const char *)vDeviceFileNameStr, "rb" )) 
        {
            /*Failed to open a file.*/
            vResult = UC_ERROR_MISSING_DATA;
            vFile = NULL;
        }
    }
    if(UC_ERROR_SUCCESS == vResult)
    {
        size_t vBytesRead = fread( vHostDeviceData, 1, sizeof(HostDeviceData), vFile );
        if(sizeof(HostDeviceData) != vBytesRead)
        {
            vResult = UC_ERROR_MISSING_DATA;
        }
    }

    if(vFile)
        fclose(vFile);

    return vResult;
}

void UT_SetExePath()
{
    TCHAR vPath[MAX_PATH];
    memset(vPath, 0x0, sizeof(vPath));
    GetModuleFileName( NULL, vPath, MAX_PATH );
    int i = 0, j = 0;
    while( vPath[i]!=0 )
    {
        if( vPath[i]=='\\' )
            j = i;
        i++;
    }
    vPath[j+1] = '\0';
    gUtExePath.Format( "%s", vPath );
}

/*Get current exe path.*/
CString UT_GetExePath()
{
    if(gUtExePath.IsEmpty())
    {
        UT_SetExePath();
        UT_SetDeviceDefault();
    }
    return gUtExePath;
}

#endif //CPLUS_A_H
