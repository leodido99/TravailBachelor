# Create following http://docs.zephyrproject.org/application/application.html

# Board used
set(BOARD sodaq_one_v3)

# Include zephyr
include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(NONE)

# Application source files
target_sources(app PRIVATE src/main.c)