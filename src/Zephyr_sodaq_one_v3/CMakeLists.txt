# Create following http://docs.zephyrproject.org/application/application.html

# Board used - Use custom board
#set(BOARD adafruit_feather_m0_basic_proto)
#set(BOARD_ROOT ${CMAKE_SOURCE_DIR})
set(BOARD sodaq_one_v3)

# Include zephyr
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(NONE)

# Application source files
target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/RN2483_lora.c)
target_sources(app PRIVATE src/LSM303AGR.c)
target_sources(app PRIVATE src/UBloxEVA8M.c)