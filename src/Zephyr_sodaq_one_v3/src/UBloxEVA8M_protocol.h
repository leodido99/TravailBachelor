/** 
 * @file UBloxEVA8M_protocol.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jul 9, 2018
 */
#ifndef SRC_UBLOXEVA8M_PROTOCOL_H_
#define SRC_UBLOXEVA8M_PROTOCOL_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * UBX Class IDs
 */
#define UBLOXEVA8M_CLASS_NAV 0x01
#define UBLOXEVA8M_CLASS_RXM 0x02
#define UBLOXEVA8M_CLASS_INF 0x04
#define UBLOXEVA8M_CLASS_ACK 0x05
#define UBLOXEVA8M_CLASS_CFG 0x06
#define UBLOXEVA8M_CLASS_UPD 0x09
#define UBLOXEVA8M_CLASS_MON 0x0A
#define UBLOXEVA8M_CLASS_AID 0x0B
#define UBLOXEVA8M_CLASS_TIM 0x0D
#define UBLOXEVA8M_CLASS_ESF 0x10
#define UBLOXEVA8M_CLASS_MGA 0x13
#define UBLOXEVA8M_CLASS_LOG 0x21
#define UBLOXEVA8M_CLASS_SEC 0x27
#define UBLOXEVA8M_CLASS_HNR 0x28

/**
 * UBX Message IDs
 */
/* UBLOXEVA8M_CLASS_NAV */
#define UBLOXEVA8M_MSG_NAV_PVT 0x07

/**
 * UBX-NAV-PVT
 */
typedef struct __attribute__((packed,aligned(1))) {
    uint32_t    iTOW;       // 00 GPS time of week of the navigation epoch.
    uint16_t    year;       // 04 Year UTC
    uint8_t     month;      // 06 Month, range 1..12 (UTC)
    uint8_t     day;        // 07 Day of month, range 1..31 (UTC)
    uint8_t     hour;       // 08 Hour of day, range 0..23 (UTC)
    uint8_t     minute;     // 09 Minute of hour, range 0..59 (UTC)
    uint8_t     seconds;    // 10 Seconds of minute, range 0..60 (UTC)
    uint8_t     valid;      // 11 Validity Flags (see graphic below)
    uint32_t    tAcc;       // 12 Time accuracy estimate (UTC)
    int32_t     nano;       // 16 Fraction of second, range -1e9 .. 1e9 (UTC)
    uint8_t     fixType;    // 20 GNSSfix Type, range 0..5
    uint8_t     flags;      // 21 Fix Status Flags
    uint8_t     flags2;     // 22 Reserved
    uint8_t     numSV;      // 23 Number of satellites used in Nav Solution
    int32_t     lon;        // 24 Longitude
    int32_t     lat;        // 28 Latitude
    int32_t     height;     // 32 Height above Ellipsoid
    int32_t     hMSL;       // 36 Height above mean sea level
    uint32_t    hAcc;       // 40 Horizontal Accuracy Estimate
    uint32_t    vAcc;       // 44 Vertical Accuracy Estimate
    int32_t     velN;       // 48 NED north velocity
    int32_t     velE;       // 52 NED east velocity
    int32_t     velD;       // 56 NED down velocity
    int32_t     gSpeed;     // 60 Ground Speed (2-D)
    int32_t     heading;    // 64 Heading of motion 2-D
    uint32_t    sAcc;       // 68 Speed Accuracy Estimate
    uint32_t    headingAcc; // 72 Heading Accuracy Estimate
    uint16_t    pDOP;       // 76 Position DOP
    uint8_t   reserved1[6]; // 78 Reserved
    int32_t     headVeh;    // 84 Heading of vehicle (2-D)
    uint8_t   reserved2[4]; // 88 Reserved
} ubloxeva8m_nav_pvt_t;

/* UBLOXEVA8M_CLASS_ACK */
#define UBLOXEVA8M_MSG_ACK_ACK 0x01
#define UBLOXEVA8M_MSG_ACK_NAK 0x00

/* UBLOXEVA8M_CLASS_CFG */
#define UBLOXEVA8M_MSG_CFG_PRT 0x00
#define UBLOXEVA8M_MSG_CFG_MSG 0x01
#define UBLOXEVA8M_MSG_CFG_NAV5 0x24

/**
 * UBX-CFG-PRT
 */
typedef struct __attribute__((packed,aligned(1))) {
    uint8_t     portID;
    uint8_t     reserved0;
    uint16_t    txReady;
    uint32_t    mode;
    uint32_t    reserved3;
    uint16_t    inProtoMask;
    uint16_t    outProtoMask;
    uint16_t    flags;
    uint16_t    reserved5;
} ubloxeva8m_cfg_prt_t;

/* ubloxeva8m_cfg_prt_t - outProtoMask */
#define UBLOXEVA8M_CFG_PRT_OUTPROTOMASK_UBX_MASK (1 << 0)
#define UBLOXEVA8M_CFG_PRT_OUTPROTOMASK_NMEA_MASK (1 << 1)
#define UBLOXEVA8M_CFG_PRT_OUTPROTOMASK_RTCM3_MASK (1 << 5)

/**
 * UBX-CFG-MSG
 */
typedef struct __attribute__((packed,aligned(1))) {
	uint8_t msgClass;
	uint8_t msgID;
	uint8_t rate;
} ubloxeva8m_cfg_msg_t;

/**
 * UBX-CFG-NAV5
 */
typedef struct __attribute__((packed,aligned(1))) {
	uint16_t mask;
	uint8_t dynModel;
	uint8_t fixMode;
	int32_t fixedAlt;
	uint32_t fixedAltVar;
	int8_t minElev;
	uint8_t drLimit;
	uint16_t pDop;
	uint16_t tDop;
	uint16_t pAcc;
	uint16_t tAcc;
	uint8_t staticHoldThresh;
	uint8_t dgnssTimeout;
	uint8_t cnoThreshNumSVs;
	uint8_t cnoThresh;
	uint8_t reserved1[2];
	uint16_t staticHoldMaxDist;
	uint8_t utcStandard;
	uint8_t reserved2[5];
} ubloxeva8m_cfg_nav5_t;

/* ubloxeva8m_cfg_nav5_t - mask */
#define UBLOXEVA8M_CFG_NAV5_MASK_DYN_MASK (1 << 0)
#define UBLOXEVA8M_CFG_NAV5_MASK_MINEL_MASK (1 << 1)
#define UBLOXEVA8M_CFG_NAV5_MASK_POSFIXMODE_MASK (1 << 2)
#define UBLOXEVA8M_CFG_NAV5_MASK_DRLIM_MASK (1 << 3)
#define UBLOXEVA8M_CFG_NAV5_MASK_POSMASK_MASK (1 << 4)
#define UBLOXEVA8M_CFG_NAV5_MASK_TIMEMASK_MASK (1 << 5)
#define UBLOXEVA8M_CFG_NAV5_MASK_STATICHOLDMASK_MASK (1 << 6)
#define UBLOXEVA8M_CFG_NAV5_MASK_DGPSMASK_MASK (1 << 7)
#define UBLOXEVA8M_CFG_NAV5_MASK_CNOTHRESHOLD_MASK (1 << 8)
#define UBLOXEVA8M_CFG_NAV5_MASK_UTC_MASK (1 << 10)


#endif /* SRC_UBLOXEVA8M_PROTOCOL_H_ */
