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

typedef struct __attribute__((packed,aligned(1))) {
	uint8_t msgClass;
	uint8_t msgID;
	uint8_t rate;
} ubloxeva8m_set_msg_rate_t;

/* UBLOXEVA8M_CLASS_ACK */
#define UBLOXEVA8M_MSG_ACK_ACK 0x01
#define UBLOXEVA8M_MSG_ACK_NAK 0x00

/* UBLOXEVA8M_CLASS_CFG */
#define UBLOXEVA8M_MSG_CFG_PRT 0x00
#define UBLOXEVA8M_MSG_CFG_MSG 0x01

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
} ubloxeva8m_port_configuration_ddc_t;

/* ubloxeva8m_port_configuration_ddc_t - outProtoMask */
#define UBLOXEVA8M_CFG_PRT_OUTPROTOMASK_UBX_MASK (1 << 0)
#define UBLOXEVA8M_CFG_PRT_OUTPROTOMASK_NMEA_MASK (1 << 1)
#define UBLOXEVA8M_CFG_PRT_OUTPROTOMASK_RTCM3_MASK (1 << 5)



#endif /* SRC_UBLOXEVA8M_PROTOCOL_H_ */
