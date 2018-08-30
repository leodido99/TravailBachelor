/** 
 * @file race_tracking_packet.h
 * @brief The Race Tracking Packet definition
 *
 * This is the definition of the Race Tracking Packet which is sent
 * at regular interface and provides information on the competitor.
 *
 * @author Léonard Bise
 * @date   Jul 26, 2018
 */
#ifndef SRC_RACE_TRACKING_PACKET_H_
#define SRC_RACE_TRACKING_PACKET_H_

struct race_tracking_pkt {
	u32_t marker;
	u16_t id;
	u64_t timestamp;
	u8_t status;
	u16_t counter;
	s32_t latitude;
	s32_t longitude;
	u8_t nb_sv;
	u16_t pdop;
	u8_t heart_rate;
	u8_t cadence;
} __attribute__((packed,aligned(1)));

#endif /* SRC_RACE_TRACKING_PACKET_H_ */
