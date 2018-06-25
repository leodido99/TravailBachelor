/** 
 * @file test_mode_record.h
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 25, 2018
 */
#ifndef SRC_RACE_APP_SERVER_TEST_MODE_RECORD_H_
#define SRC_RACE_APP_SERVER_TEST_MODE_RECORD_H_

#include "lora_rxpk_parser.h"
#include <string>

class test_mode_record {
private:
    double lat;
    double lon;
    int cnt;
    lora_rxpk_parser* pkt;
    std::string name;
public:
	test_mode_record(std::string name, double lat, double lon, uint32_t cnt, lora_rxpk_parser* pkt);
	virtual ~test_mode_record();
	std::string get_line();
};

#endif /* SRC_RACE_APP_SERVER_TEST_MODE_RECORD_H_ */
