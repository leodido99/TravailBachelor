/** 
 * @file testmodehandler.cpp
 * @brief Module description
 *
 * Detailed module description
 *
 * @author Léonard Bise
 * @date   Jun 21, 2018
 */
#include "test_mode_handler.h"
#include <iostream>

test_mode_handler::test_mode_handler() {


}

test_mode_handler::~test_mode_handler() {
}

void test_mode_handler::handle(lora_rxpk_parser rxpk) {
	std::cout << "I handle this: " << rxpk << std::endl;
}
