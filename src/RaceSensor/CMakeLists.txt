# Create following http://docs.zephyrproject.org/application/application.html

# Board used
set(BOARD sodaq_one_v3)

# Include zephyr
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(NONE)

# Application source files
target_sources(app PRIVATE
	       	   src/main.c
	       	   src/debug.c
	       	   src/packet_manager.c
	       	   src/race_sensor_shell.c
	       	   src/drivers/LSM303AGR.c
	           src/drivers/RN2483_lora.c
	           src/drivers/UBloxEVA8M.c
	           src/drivers/leds.c	           
	           )

# Include directories
zephyr_include_directories(
	src/
	src/drivers
)