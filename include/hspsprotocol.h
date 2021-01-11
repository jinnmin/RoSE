#ifndef HSPS_PROTOCOL_H_
#define HSPS_PROTOCOL_H_

#define DF_UI
#define DF_PROTOCOL

#define KZ_UI
#if defined(KZ_UI)
#define KZ_UI_DEBUG
#define KZ_UI_WRITE_N_READ
#define KZ_UI_GET_HP  // Get HP State
#define KZ_DB
#undef KZ_UI_RM_TEST // RM Test
#endif

#include <cstdint>

const uint8_t KEEP_ALIVE_CMD              = 0x01;
const uint8_t MAIN_FW_CMD                 = 0x02;
const uint8_t SUB_FW_CMD                  = 0x03;
const uint8_t DATE_CMD                    = 0x04;

const uint8_t CART_STATUS_CMD             = 0x11;
const uint8_t INSPECTION_STATUS_CMD       = 0x12;
const uint8_t SYSTEM_STATE_CMD            = 0x13;
const uint8_t SYS_SET_STATE_CMD           = 0x14;
const uint8_t ANGLE_DATA_CMD              = 0x15;

const uint8_t ERROR_CMD                   = 0xFC;


//===================================================================

/* --- DEVICE ID --- */
const uint16_t DEVICE_ID_SLIMUS			        = 0x1010;
const uint16_t DEVICE_ID_DB2			        = 0x1020;
const uint16_t DEVICE_ID_TRIPLO			        = 0x1030;
const uint16_t DEVICE_ID_PLASONIC		        = 0x1040;
const uint16_t DEVICE_ID_GENTLO 		        = 0x4754; //GT
const uint16_t DEVICE_ID_DFIT    		        = 0x4446; //DF

/* --- ALRAM ERROR CODE --- */

// SKW 2019-05-03 
const uint16_t NORMAL_OPERATION                                             = 0x0000;
const uint16_t CHECK_THE_CONNECTOR                                          = 0x0001;
const uint16_t INSERT_THE_TIP                                               = 0x0002;
const uint16_t THE_TIME_IS_EXCEEDED_REPLACE_THE_TIP                         = 0x0003;
const uint16_t UNREGISTERED_TIP_HAS_USED                                    = 0x0004;
const uint16_t REUSE_TIP_HAS_USED                                           = 0x0005;
const uint16_t COUNTRY_CODE_ERROR                                           = 0x0006;
const uint16_t COMMUNICATION_ERROR                                          = 0x0007;
const uint16_t FAILED_TO_READ_CARTRIDGE_INFORMATION                         = 0x0008;
const uint16_t MOTOR_ERROR_DURING_SHOT                                      = 0x0009;
const uint16_t FW_COMMUNICATION_ERROR                                       = 0x0020;


/* --- F/W MODULE --- */
const uint16_t MAIN				                = 0x0;
const uint16_t ULTRASOUND			            = 0x1;

/* --- UI PAGE INFO --- */
const uint16_t PAGE_HOME				        = 0x00;
const uint16_t PAGE_ALEX			            = 0x01;
const uint16_t PAGE_NDYAG   			        = 0x02;
const uint16_t PAGERMAUTO                       = 0x03;
const uint16_t PAGERMMANUAL                     = 0x04;
const uint16_t PAGERN                           = 0x05;
const uint16_t PAGERC                           = 0x06;
const uint16_t PAGERV                           = 0x07;
const uint16_t PAGESETTINGS                     = 0x08;
const uint16_t PAGEPATIENTINFO                  = 0x09;
const uint16_t PAGEHPSETTINGS                   = 0x0A;
const uint16_t PAGEADMINSETTING                 = 0x0B;
const uint16_t PAGETIMESETTING                  = 0x0C;
const uint16_t PAGEWIFISETTING                  = 0x0D;
const uint16_t PAGEADMINFWUPDATE                = 0x0E;
const uint16_t PAGESEARCHPATIENT                = 0x0F;
const uint16_t PAGEFLTREATMENTHISTORY           = 0x10;
const uint16_t PAGEFLAUTO                       = 0x11;
const uint16_t PAGEFLMANUAL                     = 0x12;

const uint16_t PAGERMAUTO_RETURN                = 0x30;


const uint16_t PAGE_ADMIN_MAIN			        = 0x03;	/* No page of input passwd */
const uint16_t PAGE_ADMIN_UPGRADE_FW            = 0x04;
const uint16_t PAGE_ADMIN_SET_DATE_TIME	        = 0x05;
const uint16_t PAGE_ADMIN_SET_TIP		        = 0x06;


/* --- SELECT HP INFO --- */
const uint16_t HP_ALEX				        = 0x00;
const uint16_t HP_NDYAG 			        = 0x01;
#if defined(KZ_UI)
const uint16_t HP_RM    				        = 0x02;
const uint16_t HP_RV         			        = 0x03;
const uint16_t HP_RN    				        = 0x04;
const uint16_t HP_RC         			        = 0x05;
#endif


/* --- OPERATION MODE INFO --- */
const uint16_t OPERATION_CONTINUE		        = 0x00;
const uint16_t OPERATION_D1			            = 0x01;
const uint16_t OPERATION_D2			            = 0x02;
const uint16_t OPERATION_D3			            = 0x03;
const uint16_t OPERATION_MIX			        = 0x04;

/* --- OPERATION LEVEL INFO --- */
const uint16_t OPERATION_LV1			        = 0x00;
const uint16_t OPERATION_LV2			        = 0x01;
const uint16_t OPERATION_LV3			        = 0x02;
const uint16_t OPERATION_LV4			        = 0x03;
const uint16_t OPERATION_LV5			        = 0x04;

/* --- OPERATION READY INFO --- */
const uint16_t OPERATION_STANDBY			    = 0x00;
const uint16_t OPERATION_READY			        = 0x01;

/* --- OPERATION READY INFO --- */
const uint16_t BUZZER_SND_ON			        = 0x00;
const uint16_t BUZZER_SND_OFF			        = 0x01;		/* MUTE */

//===================================================================

/*
     * PARAMETER DATA SIZE (BYTE)
     * +---------------------------------------------------------------------------------------------------------+
     * | START_BIT || (HEADER)DeviceID  || (HEADER)CMD        || (HEADER) LEN     || (HEADER) INSTRUCTION ||(DATA) CODE  || (DATA) CRC_BIT | STOP_BIT |
     * +---------------------------------------------------------------------------------------------------------+
     * | 2BYTE     || 1BYTE             || 1BYTE              || 1BYTE            || 1BYTE                ||6BYTE        || 2BYTE          | 2BYTE    |
     * +---------------------------------------------------------------------------------------------------------+
     * DATA_BYTE = 2 + 1 + 1 + 1 + 1 + 6 + 2 + 2 => 16 Bytes
 */

#define MSG_DEFAULT_SIZE                        6       // DATA CODE
#define MSG_HEADER_SIZE                         4       // HEADER CMD + HEADER LEN + HEADER INSTRUCTION
#define MSG_PROTOCOL_SIZE                       16      // START_BIT + HEADER LEN + HEADER CODE + HEADER ID + DATA CRC + STOP_BIT


const int PARAM_RES_DATE                        = 6;	// 6BYTE
const int PARAM_RES_DEV_ID                      = 8;	// 8BYTE
const int PARAM_ALRAM                           = 2;	// 2BYTE
const int PARAM_RES_FW_UPDATE                   = 2;	// 2BYTE
const int PARAM_REQ_FW_SET_DATE                 = 6;	// 6BYTE
const int PARAM_REQ_UI_PAGE_INFO                = 1;	// 1BYTE
const int PARAM_REQ_SELECT_HP_INFO              = 1;	// 1BYTE
const int PARAM_RES_SELECT_HP_INFO              = 8;	// 8BYTE
const int PARAM_RES_PS_TIP_INFO                 = 11;	// 11BYTE
const int PARAM_REQ_PS_TIP_SETTING              = 11;	// 11BYTE
const int PARAM_REQ_SET_OPERATION_MODE          = 2;	// 2BYTE
const int PARAM_REQ_SET_LEVEL                   = 2;	// 2BYTE
const int PARAM_REQ_SET_STANDBY_READY           = 2;	// 2BYTE
const int PARAM_REQ_SET_BUZZER_MODE             = 2;	// 2BYTE
const int PARAM_REQ_SEND_OPERATION_TIME         = 6;	// 6BYTE
const int PARAM_REQ_TOTAL_OPERATION_TIME        = 1;	// 1BYTE
const int PARAM_RES_TOTAL_OPERATION_TIME        = 6;	// 6BYTE
const int PARAM_REQ_RESET_TOTAL_OPERATION_TIME  = 1;	// 1BYTE
const int PARAM_RES_RESET_TOTAL_OPERATION_TIME  = 1;	// 1BYTE

//===================================================================

const uint16_t START_BIT         = 0xFFFD;
const uint16_t CRC_BIT          = 0xCC33;
const uint16_t STOP_BIT         = 0xC33C;
const uint16_t USER_STOP_BIT    = 0xC33E;


const int CONNECT       = 0x0;
const int DISCONNECT    = 0x1;

#if defined(KZ_UI)// JJHWANG for KZ
const uint16_t GT_MAIN_BD   = 0x4754;
const uint16_t KZ_MAIN_BD   = 0x4B5A;
const uint16_t DF_MAIN_BD   = 0x4446;
const uint16_t HL_BD        = 0x484C;
const uint16_t HR_BD        = 0x4852;
const uint16_t SD_BD        = 0x5344;
const uint16_t RM_BD        = 0x524D;
const uint16_t SL_BD        = 0x534C;
const uint16_t SR_BD        = 0x5352;

const uint16_t DF_BD        = 0x4446;
const uint16_t PS_BD        = 0x5053;
const uint16_t RC_BD        = 0x5243;
const uint16_t RN_BD        = 0x524E;
const uint16_t RV_BD        = 0x5256;

const uint16_t FACE_HEAD        = 1;
const uint16_t FACE_NOSE        = 2;
const uint16_t FACE_NECK        = 3;
const uint16_t FACE_JAWLINE     = 4;
const uint16_t FACE_CHEEK       = 5;
const uint16_t FACE_NASOLABIAL  = 6;
const uint16_t FACE_EYES        = 7;

#endif

#define APPLICATION     100
#define FW_MAIN         0
#define FW_SONO         1
#define FW_RMHP         2
#define FW_RVHP         3
#define FW_RNHP         4
#define FW_RCHP         5

//===================================================================

#endif // HSPS_PROTOCOL_H_
