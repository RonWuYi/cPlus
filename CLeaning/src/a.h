//
// Created by hdc on 1/22/19.
//

#ifndef CPLUS_A_H
#define CPLUS_A_H

typedef enum {
    /** (0x00000000) The operation was successful. */
    UC_ERROR_SUCCESS                        = 0x00000000,

    /** (0x00000001) One or more required parameters were NULL. */
    UC_ERROR_NULL_PARAM                     = 0x00000001, 

    /** (0x00000002) Memory could not be allocated. */
    UC_ERROR_OUT_OF_MEMORY                  = 0x00000002, 

    /** (0x00000003) Unable to create requested resource. */
    UC_ERROR_UNABLE_TO_CREATE               = 0x00000003,

    /** (0x00000004) Generic OS failure. */
    UC_ERROR_OS_FAILURE                     = 0x00000004,

    /** (0x00000005) The timeout expired before the object was ready. */
    UC_ERROR_WAIT_ABORTED                   = 0x00000005, 

    /** (0x00000006) The buffer passed in is too small. */
    UC_ERROR_INSUFFICIENT_BUFFER            = 0x00000006, 

    /** (0x00000007) The specified resource could not be found. */
    UC_ERROR_RESOURCE_NOT_FOUND             = 0x00000007,

    /** (0x00000008) The resource name specified is invalid. */
    UC_ERROR_BAD_RESOURCE_NAME              = 0x00000008, 

    /** (0x00000009) The requested feature is not implemented. */
    UC_ERROR_NOT_IMPLEMENTED                = 0x00000009, 

    /** (0x0000000A) A connection is still open; stream cannot be closed. */
    UC_ERROR_CONNECTION_STILL_OPEN          = 0x0000000A, 

    /** (0x0000000B) The handle is not valid. */
    UC_ERROR_INVALID_HANDLE                 = 0x0000000B,

    /** (0x0000000C) The handle is valid but is not the correct type. */
    UC_ERROR_WRONG_HANDLE_TYPE              = 0x0000000C, 

    /** (0x0000000D) Too many handles are opened already. */
    UC_ERROR_TOO_MANY_HANDLES               = 0x0000000D,

    /** (0x0000000E) An attempt to shut down was made while a handle is still open. */
    UC_ERROR_HANDLE_STILL_OPEN              = 0x0000000E,

    /** (0x0000000F) This operation cannot be completed because the Cloaked CA Agent was already started. */
    UC_ERROR_ALREADY_STARTED                = 0x0000000F,

    /** (0x00000010) This operation cannot be completed because the Cloaked CA Agent was already stopped. */
    UC_ERROR_ALREADY_STOPPED                = 0x00000010,

    /** (0x00000011) An internal message queue is full and cannot accept more messages. */
    UC_ERROR_QUEUE_FULL                     = 0x00000011,

    /** (0x00000012) A required implementation of an SPI method is missing. */
    UC_ERROR_MISSING_SPI_METHOD             = 0x00000012,

    /** (0x00000013) The version of the SPI implementation is not compatible with the client. */
    UC_ERROR_INCOMPATIBLE_SPI_VERSION       = 0x00000013,

    /** (0x00000014) An invalid message was received. */
    UC_ERROR_MESSAGE_INVALID                = 0x00000014,

    /** (0x00000015) The specified length was not valid. */
    UC_ERROR_INVALID_LENGTH                 = 0x00000015,

    /** (0x00000016) Some required internal data is missing. */
    UC_ERROR_MISSING_DATA                   = 0x00000016,

    /** (0x00000017) Page is not what we want. */
    UC_ERROR_NOT_WANTED_ECMPAGE             = 0x00000017,

    /** (0x00000018) Generic driver failure or mismatched pipe selection. On the Cloaked CA Agent 4.10.1, this error will be returned by ::UniversalClientSPI_IFCP_Communicate or ::UniversalClientSPI_Stream_SetDescramblingKey when setting key on unexpected pipe*/
    UC_ERROR_DRIVER_FAILURE                 = 0x00000018,

    /** (0x00000019) Not entitled. */
    UC_ERROR_NOT_ENTITLED                   = 0x00000019,

    /** (0x0000001A) Invalid PK. */
    UC_ERROR_INVALID_PK                     = 0x0000001A,

    /** (0x0000001B) Invalid GK. */
    UC_ERROR_INVALID_GK                     = 0x0000001B,

    /** (0x0000001C) Invalid TG. */
    UC_ERROR_INVALID_TG                     = 0x0000001C,

    /** (0x0000001D) Missing CWDK. */
    UC_ERROR_INVALID_CWDK                   = 0x0000001D,

    /** (0x0000001E) No CA regional info. */
    UC_ERROR_NO_CA_REGIONAL_INFO            = 0x0000001E,

    /** (0x0000001F) Regional blocking. */
    UC_ERROR_REGIONAL_BLOCKING              = 0x0000001F,

    /** (0x00000020) Restricted opcode. */
    UC_ERROR_RESTRICTED_OPCODE              = 0x00000020,

    /** (0x00000021) Timestamp filter failed. */
    UC_ERROR_TIMESTAMP_FILTER               = 0x00000021,

    /** (0x00000022) Type filter failed. */
    UC_ERROR_TYPE_FILTER                    = 0x00000022,

    /** (0x00000023) Signature verification failed. */
    UC_ERROR_SIGNATURE_VERIFICATION         = 0x00000023,

    /** (0x00000024) TMS failed. */
    UC_ERROR_TMS_FAILED                     = 0x00000024,

    /** (0x00000025) Not PVR entitled. */
    UC_ERROR_NOT_PVR_ENTITLED               = 0x00000025,

    /** (0x00000026) This operation cannot be completed because PMT is not notified. */
    UC_ERROR_UNABLE_TO_CONFIG_PVR_RECORD    = 0x00000026,

    /** (0x00000027) This operation cannot be completed while a request for PVR record  is still being processed. */
    UC_ERROR_CONFIG_PVR_RECORD_STILL_OPEN   = 0x00000027,

    /** (0x00000028) This operation cannot be completed because PVR MSK is missing. */
    UC_ERROR_MISSING_PVR_MSK                = 0x00000028,

    /** (0x00000029) The recorded content expired */
    UC_ERROR_PVR_CONTENT_EXPIRED            = 0x00000029,

    /** (0x0000002A) Failed to generate PVR session key */
    UC_ERROR_FAILED_TO_GENERATE_SESSION_KEY = 0x0000002A,

    /** (0x0000002B) The PVR metadata is invalid */
    UC_ERROR_INVALID_PVR_METADATA          = 0x0000002B,

    /** (0x0000002C) The client type is wrong, a feature only supports a certain client type
    (For example, PVR works only on clients with secure chipset). 
    */
    UC_ERROR_CLIENT_TYPE_ERROR              = 0x0000002C,   

    /** (0x0000002D) Invalid sector. */
    UC_ERROR_INVALID_SECTOR                 = 0x0000002D,
    
    /** (0x0000002E) Client type error, PPV VOD feature should base on Secure Chipset mode. */
    UC_ERROR_VOD_CLIENT_TYPE_ERROR          = 0x0000002E,
    
    /** (0x0000002F) No serial number. */
    UC_ERROR_VOD_NO_SERIAL_NUMBER           = 0x0000002F,
    
    /** (0x00000030) Invalid nonce. */
    UC_ERROR_VOD_INVALID_NONCE              = 0x00000030,

    /** (0x00000031) Pkey Hash mismatch */
    UC_ERROR_PKEY_HASH_MISMATCH             = 0x00000031,

    /** (0x00000032) The variant of the device identifiers is not compatible with the client. */
    UC_ERROR_INCOMPATIBLE_VARIANT           = 0x00000032,

    /** (0x00000033) No Nationality. */
    UC_ERROR_NO_NATIONALITY                 = 0x00000033,

    /** (0x00000034) The uniqueaddress is invalid with the client. */
    UC_ERROR_INVALID_UNIQUE_ADDRESS         = 0x00000034,

    /** (0x00000035) This result code is not used now. */
    UC_ERROR_DIGITAL_COPY_NOMORE            = 0x00000035,
    
    /** (0x00000036) Black out. */
    UC_ERROR_BLACK_OUT                      = 0x00000036,

    /** (0x00000037) Homing Channel Failed. */
    UC_ERROR_HOMING_CHANNEL_FAILED          = 0x00000037,

    /** (0x00000038) FlexiFlash Failed. */
    UC_ERROR_FLEXIFLASH_FAILED          = 0x00000038,

    /** (0x00000039) Middleware User Data Failed */
    UC_ERROR_MIDDLEWARE_USER_DATA_FAILED      = 0x00000039,

    /** (0x0000003A) Pre-Enable product expired */
    UC_ERROR_PRE_ENABLE_PRODUCT_EXPIRED     = 0x0000003A,

    /** (0x0000003B) Missing Pre-Enable Session Key */
    UC_ERROR_MISSING_PESK           = 0x0000003B,

    /** (0x0000003C) The OTP data is invalid */
    UC_ERROR_INVALID_OTP_DATA           = 0x0000003C,

    /** (0x0000003D) The personalized data is invalid */
    UC_ERROR_INVALID_PERSONALIZED_DATA           = 0x0000003D,
    
    /** (0x0000003E) New CG for FSU product received */
    UC_ERROR_NEW_CG_FOR_FSU_RECEIVED           = 0x0000003E,

    /** (0x0000003F) New CG for Push VOD product received */
    UC_ERROR_NEW_CG_FOR_PUSHVOD_RECEIVED           = 0x0000003F,

    /** (0x00000040) CG Mismatch */
    UC_ERROR_CG_MISMATCH           = 0x00000040,

    /** (0x00000041) SG Mismatch */
    UC_ERROR_SG_MISMATCH           = 0x00000041,

    /** (0x00000042) Invalid PK Index */
    UC_ERROR_INVALID_PK_INDEX           = 0x00000042,

    /** (0x00000043) Invalid GK Index */
    UC_ERROR_INVALID_GK_INDEX           = 0x00000043,
    
    /** (0x00000044) Macrovision Failed. */
    UC_ERROR_MACROVISION_FAILED           = 0x00000044,

    /** (0x00000045) SN RANGE Failed. */
    UC_ERROR_SN_RANGE_FAILED           = 0x00000045,

    /** (0x00000046) Unified Client Error. */
    UC_ERROR_CONVERGENT_CLIENT_GENERIC_ERROR      = 0x00000046,

    /** (0x00000047) Smart Card is out. */
    UC_ERROR_SMARTCARD_OUT      = 0x00000047,

    /** (0x00000048) Unknown Card (non-Irdeto), or the smart card is upside down. */
    UC_ERROR_SMARTCARD_UNKNOWN    = 0x00000048,

    /** (0x00000049) Card Error, communications with Smart Card have failed. */
    UC_ERROR_SMARTCARD_ERROR      = 0x00000049,

    /** (0x0000004A) Corrupted Data, the data is corrupted */
    UC_ERROR_IO_DATA_CORRUPTED      = 0x0000004A,    

    /** (0x0000004B) Invalid change verion. */
    UC_ERROR_INVALID_CHANGE_VERSION      = 0x0000004B,

    /** (0x0000004C) This Block has been downloaded. */
    UC_ERROR_FLEXIFLASH_BLOCK_DOWNLOAD_DUPLICATION      = 0x0000004C,

    /** (0x0000004D) The Global SC EMM has been processed by Client. */
    UC_ERROR_GLOBAL_SC_EMM_DUPLICATION      = 0x0000004D,

    /** (0x0000004E) Stop EMM processing. */
    UC_ERROR_STOP_EMM_PROCESSING      = 0x0000004E,
    
    /** (0x0000004F) Load Cloaked CA package failed. */
    UC_ERROR_LOAD_CCAPACKAGE_FAILED      = 0x0000004F,

    /** (0x00000050) Invalid CFG data. */
    UC_ERROR_INVALID_CFG_DATA      = 0x00000050, 

    /** (0x00000051) Package data is invalid. */
    UC_ERROR_INVALID_PACKAGE_DATA      = 0x00000051,

    /** (0x00000052) The VM failed. */
    UC_ERROR_VM_FAILURE      = 0x00000052,  
    
    /** (0x00000053) The securecore is not loaded. */
    UC_ERROR_SECURECORE_NOT_LOADED      = 0x00000053,

    /** (0x00000054) Invalid area index. */
    UC_ERROR_INVALID_AREA_INDEX      = 0x00000054,

    /** (0x00000055) An character is not expected when parse a CFG file */
    UC_ERROR_UNEXPECTED_CHARACTER = 0x00000055,

    /** (0x00000056) Get section Data without parse a CFG file. */
    UC_ERROR_NO_CFG_PARSED = 0x00000056,

    /** (0x00000057) Parsing a CFG file before release the former parsing result. */
    UC_ERROR_CFG_PARSED_ALREADY = 0x00000057,

    /** (0x00000058) No specified section found in the CFG file. */
    UC_ERROR_SECTION_NOT_FOUND = 0x00000058,

    /** (0x00000059) The VM with special id has already defined. */
    UC_ERROR_VM_ALREADY_DEFINED = 0x00000059,

    /** (0x0000005A) Error happen during create the "vm config" object. */
    UC_ERROR_VM_CREATE_VMCONFIG = 0x0000005A,

    /** (0x0000005B) Error happen during create the vm instance. */
    UC_ERROR_VM_CREATE = 0x0000005B,

    /** (0x0000005C) Too many vm created, vm number reach top limit. */
    UC_ERROR_VM_TOP_LIMIT =  0x0000005C,

    /** (0x0000005D) Error ocurr during loading bytecode image to vm. */
    UC_ERROR_VM_LOAD_IMAGE = 0x0000005D,

    /** (0x0000005E) Error ocurr during memory map from native memory space to vm memory space. */
    UC_ERROR_VM_MEMMAP = 0x0000005E,

    /** (0x0000005F) Error ocurr during execute vm. */
    UC_ERROR_VM_EXECUTE = 0x0000005F,

    /** (0x00000060) Error ocurr VM IO. */
    UC_ERROR_VM_IO = 0x00000060,

    /** (0x00000061) Error ocurr VM RESET. */
    UC_ERROR_VM_RESET = 0x00000061,

    /** (0x00000062) The root key hash check failed. */
    UC_ERROR_ROOT_KEY_HASH_CHECK_FAILED = 0x00000062,

    /** (0x00000063) Unsupported package compress algorithm. */
    UC_ERROR_COMPRESS_ALGORITHM_NOT_SUPPORT = 0x00000063,
    
    /** (0x00000064) The SYS ID is invalid. */
    UC_ERROR_INVALID_SYS_ID = 0x00000064,

    /** (0x00000065) The version of the client is too low to support Cloaked CA package download. */
    UC_ERROR_LOW_CLIENT_VERSION = 0x00000065,

    /** (0x00000066) The CA System ID is invalid */
    UC_ERROR_INVALID_CA_SYSTEM_ID = 0x00000066,

    /** (0x00000067) Anchor Failed(illegal device). */
    UC_ERROR_DEVICE_INVALID                 = 0x00000067,

    /** (0x00000068) Request entitlementkeys failed. */
    UC_ERROR_REQUEST_ENTITLEMENT_FAILED     = 0x00000068,

    /** (0x00000069) No response for request for a certain time interval. */
    UC_ERROR_REQUEST_ENTITLEMENT_RESPONSE_TIME_OUT   = 0x00000069,

    /** (0x0000006A) CCIS internal error. */
    UC_ERROR_CCIS_INTERNAL_ERROR            = 0x0000006A,

    /** (0x0000006B) Anchor failed (DeviceID/UA mismatch: for IP only STBs, the application should register again). */
    UC_ERROR_DEVICEID_UA_MISMATCH           = 0x0000006B,

    /** (0x0000006C) VOD not entitled. */
    UC_ERROR_VOD_NOT_ENTITLED               = 0x0000006C,

    /** (0x0000006D) Missing Secure PVR CPSK. */
    UC_ERROR_MISSING_CPSK               = 0x0000006D,

    /** (0x0000006E) Asset ID mismatch. */
    UC_ERROR_ASSET_ID_MISMATCH = 0x0000006E,

    /** (x00000006F) PVR Recording is prohibited due to copy control setting */
    UC_ERROR_PVR_COPY_CONTROL_PROHIBITED = 0x0000006F,


    /** (x000000070) PVR sharing is prohibited due to is shareable setting */
    UC_ERROR_HN_PVR_NOT_SHAREABLE = 0x00000070,

    /** (x000000071) PVR sharing is prohibited due to domain id mismatch */
    UC_ERROR_HN_PVR_NOT_IN_DOMAIN = 0x00000071,

    /** (0x00000072) Fail to process Shared PVRMSK EMM. */
    UC_ERROR_SHARED_PVRMSK_PROCESS_FAIL = 0x00000072,

    /** (0x00000073) White Box Algorithm is invalid. */
    UC_ERROR_INVALID_WB_ALGORITHM = 0x00000073,

    /** (0x00000074) White Box Algorithm is invalid for Pairing EMM. */
    UC_ERROR_INVALID_WB_ALGORITHM_FOR_PAIRING_EMM = 0x00000074,

    /** (0x00000075) White Box Algorithm is invalid for Product Overwrite EMM. */
    UC_ERROR_INVALID_WB_ALGORITHM_FOR_PO_EMM = 0x00000075,

    /** (0x00000076) White Box Algorithm is invalid for Advertisement EMM. */
    UC_ERROR_INVALID_WB_ALGORITHM_FOR_AD_EMM = 0x00000076,

    /** (0x00000077) The chip configuration check is not supported. */
    UC_ERROR_CCC_NOT_SUPPORT = 0x00000077,

    /** (0x00000078) Client registry Failed. */
    UC_ERROR_INITIALIZATION_FAILED = 0x00000078,

    /** (0x00000079) no response for request for a certain time interval. */
    UC_ERROR_CLIENT_REGISTRATION_RESPONSE_TIME_OUT = 0x00000079,

    /** (0x0000007A) PIN CODE Limitation Failed. */
    UC_ERROR_PIN_CODE_LIMIT_FAILED = 0x0000007A,

    /** (0x0000007B) Cannot find the current CG's secret private data. */
    UC_ERROR_CG_PRIVATE_DATA_NOT_PAIRING = 0x0000007B,

    /** (0x0000007C) Proximity Detection is disabled by the Head-end. */
    UC_ERROR_PROXIMITY_DETECTION_DISABLED = 0x0000007C,

    /** (0x0000007D) The data provided to be encrypted/decrypted is invalid. */
    UC_ERROR_PROXIMITY_INVALID_DATA = 0x0000007D,

    /** (0x0000007E) The peer ID does not represent a valid peer or session.  */
    UC_ERROR_PROXIMITY_PEER_UNKNOWN = 0x0000007E,

    /** (0x0000007F) The key being used to encrypt/decrypt data is not ready.  */
    UC_ERROR_PROXIMITY_KEY_NOT_READY = 0x0000007F,

    /** (0x00000080) The proximity service is not configured yet.  */
    UC_ERROR_PROXIMITY_SERVICE_NOT_CONFIGURED = 0x00000080,

    /** (0x00000081) The PVR key ladder level in Pairing EMM does not match with that in secure chipset. */
    UC_ERROR_PVR_KEY_LADDER_LEVEL_NOT_MATCH = 0x00000081,

    /** (0x00000082) Pairing EMM is rejected as the secure chipset does not support IFCP mode. */
    UC_ERROR_PAIRING_EMM_REJECTED_IFCP_NOT_SUPPORTED = 0x00000082,

    /** (0x00000083) The IFCP RAM image is not loaded to the secure chipset successfully. */
    UC_ERROR_IFCP_IMAGE_NOT_LOADED      = 0x00000083,

    /** (0x00000084) SKE failed (This is not used on Cloaked CA Agent 4.2.0). */
    UC_ERROR_SKE_FAILED = 0x00000084,

    /** (0x00000085) The Secure Pre-enablement feature is not supported in IFCP. */
    UC_ERROR_PE_NOT_SUPPORTED      = 0x00000085,

    /** (0x00000086) The arbitrary value does not match feature bitmap set.*/
    UC_ERROR_IFCP_INVALID_ARBITRARY_LENGTH      = 0x00000086,

    /** (0x00000087) The arbitrary value check failed in IFCP.*/
    UC_ERROR_IFCP_AUTHENTICATION_FAILED      = 0x00000087, 

    /** (0x00000088) ECM is rejected as the secure chipset does not support IFCP mode. */
    UC_ERROR_ECM_REJECTED_IFCP_NOT_SUPPORTED = 0x00000088,

    /** (0x00000089) Unknown App Response from IFCP. */
    UC_ERROR_IFCP_UNKNOWN_APP_RESPONSE = 0x00000089,
    
    /** (0x0000008A) Fail when compute HASH value. */
    UC_ERROR_HASH_COMPUTE = 0x0000008A,
    
    /** (0x0000008B) Signal Announcement EMM is rejected as the FSU stream has been received successfully. */
    UC_ERROR_SIGNAL_ANNOUNCEMENT_EMM_NOT_NEEDED = 0x0000008B,

    /** (0x0000008C) Field Trial ECM is rejected as FSU stream receiving failed. */
    UC_ERROR_ECM_REJECTED_FSU_STREAM_FAILED = 0x0000008C,

    /** (0x0000008D) FSU stream was not successfully received. */
    UC_ERROR_FSU_STREAM_FAILED = 0x0000008D,

    /** (0x0000008E) The FSU stream CA System ID was not found in CAT. */
    UC_ERROR_MISSING_FSU_STREAM_PID_IN_CAT = 0x0000008E,

    /** (0x0000008F) The CAT was not notified to Cloaked CA Agent for FSU. */
    UC_ERROR_NO_CAT_NOTIFIED_FOR_FSU = 0x0000008F,

    /** (0x00000090) Not enough heap memory to load the new secure core package during FSU. */
    UC_ERROR_NO_ENOUGH_MEMORY_TO_LOAD_NEW_SECURE_CORE_DURING_FSU = 0x00000090,

    /** (0x00000091) The operation cannot be cancelled by user. */
    UC_ERROR_OPERATION_CANNOT_BE_CANCELED = 0x00000091,    

    /** (0x00000092) This operation cannot be completed because PVR MSK and CPSK are missing. */
    UC_ERROR_MISSING_PVR_MSK_CPSK = 0x00000092,

    /** (0x00000093) Invalid HGPC timestamp */
    UC_ERROR_HGPC_INVALID_TIMESTAMP = 0x00000093,

    /** (0x00000094) Invalid HGPC secure client */
    UC_ERROR_HGPC_INVALID_CLIENT = 0x00000094,

    /** (0x00000095) HGPC HNA message timeout */
    UC_ERROR_HGPC_HNA_MSG_TIMEOUT = 0x00000095,

    /** (0x00000096) TT not support in MSR mode */
    UC_ERROR_TT_NOT_SUPPORT_FOR_MSR = 0x00000096,

    /** (0x00000097) TT application data version mismatch */
    UC_ERROR_TT_APP_DATA_VERSION_MISMATCH = 0x00000097,

    /** (0x00000098) Traitor Tracing feature is not supported in IFCP */
    UC_ERROR_TT_NOT_SUPPORT_IN_IFCP = 0x00000098,

    /** (0x00000099) Incorrect number of CWs in the Traitor Tracing application data */
    UC_ERROR_TT_INCORRECT_CW_NUMBER = 0x00000099,

    /** (0x0000009A) Transformation IFCP TDC load failed */
    UC_ERROR_IFCP_TDC_LOAD_FAILED = 0x0000009A,

    /** (0x0000009B) Transformation IFCP TDC load unfinished */
    UC_ERROR_IFCP_TDC_LOAD_UNFINISHED = 0x0000009B,

    /** (0x0000009C) The secure core does not match the transformation mode */
    UC_ERROR_SCOT_SECURECORE_NOT_MATCHED = 0x0000009C,

    /** (0x0000009D) Either the TDC SPI was not implemented or an error occurred when Cloaked CA loaded TDC via this SPI */
    UC_ERROR_SCOT_TDC_NOT_LOAD = 0x0000009D,

    /** (0x0000009E)  the AD mode in the pairing EMM mismatchs what in the ECM */
    UC_ERROR_IFCP_AD_MODE_MISMATCH = 0x0000009E,

    /** (0x000000A0) General IFCP Auth Ctrl Error, means there is an unexpected Auth Ctrl value received */
    UC_ERROR_IFCP_AUTH_CTRL_ERR = 0x000000A0,

    /** (0x000000A1) SMP General CUR Failure from IFCP, may include Display and Record Errors */
    UC_ERROR_SMP_CUR_NOT_MEET = 0x000000A1,

    /** (0x000000A2) There is a Gereral App Response issue in the IFCP , can be about CUR mismatch or Auth Ctrl check */
    UC_ERROR_SMP_APP_RESPONSE_FAILURE = 0x000000A2,

    /** (0x000000A3) SMP not support in MSR mode */
    UC_ERROR_SMP_NOT_SUPPORTED_FOR_MSR = 0x000000A3

} uc_result;

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
#define uc_bool int
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
//
//extern "C" uc_result UT_GetDeviceData(HostDeviceData *vHostDeviceData)
//{
//    uc_result vResult = UC_ERROR_SUCCESS;
//    FILE *vFile = NULL;
//
//    memset((MW_UCHAR *)vHostDeviceData, 0x0, sizeof(HostDeviceData));
//
//    if(UC_ERROR_SUCCESS == vResult)
//    {
//        CString vDeviceFileName;
//        LPCTSTR vDeviceFileNameStr = NULL;
//
//        vDeviceFileName.Format("%s%s", UT_GetExePath(), UT_DEVICE_FILE_NAME);
//        vDeviceFileNameStr = vDeviceFileName;
//
//        /*Try to open device file.*/
//        if( 0 != fopen_s( &vFile, (const char *)vDeviceFileNameStr, "rb" ))
//        {
//            /*Failed to open a file.*/
//            vResult = UC_ERROR_MISSING_DATA;
//            vFile = NULL;
//        }
//    }
//    if(UC_ERROR_SUCCESS == vResult)
//    {
//        size_t vBytesRead = fread( vHostDeviceData, 1, sizeof(HostDeviceData), vFile );
//        if(sizeof(HostDeviceData) != vBytesRead)
//        {
//            vResult = UC_ERROR_MISSING_DATA;
//        }
//    }
//
//    if(vFile)
//        fclose(vFile);
//
//    return vResult;
//}
//
//void UT_SetExePath()
//{
//    TCHAR vPath[MAX_PATH];
//    memset(vPath, 0x0, sizeof(vPath));
//    GetModuleFileName( NULL, vPath, MAX_PATH );
//    int i = 0, j = 0;
//    while( vPath[i]!=0 )
//    {
//        if( vPath[i]=='\\' )
//            j = i;
//        i++;
//    }
//    vPath[j+1] = '\0';
//    gUtExePath.Format( "%s", vPath );
//}
//
///*Get current exe path.*/
//CString UT_GetExePath()
//{
//    if(gUtExePath.IsEmpty())
//    {
//        UT_SetExePath();
//        UT_SetDeviceDefault();
//    }
//    return gUtExePath;
//}

#endif //CPLUS_A_H
