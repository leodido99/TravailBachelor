# =============================================================================
# $Id: Makefile 2280 2017-11-09 14:43:06Z BiseL $
# $Author: BiseL $
# $Revision: 2280 $
# $URL: https://svn/svn/ITS/SW/Technical/MakefileSyd/Makefile $
# Description:
# -----------------------------------------------------------------------------
# Makefile structure developed by Léonard Bise.
# Attemps to provides an easy framework to build any kind of application while 
# hiding some of the ugliness of makefile files.
# Makefile.inc contains all the required makefile management
# Makefile.cfg contains the specific configuration for the type of application
# to build
# =============================================================================

# Name of the output file
# Extension .a automatically builds a library
OUTNAME=race_app_server.out

# Add the relative folder where the source files are stored
SRCPATHS=./src ./src/rapidjson ./src/lora_pkt ./src/race_app_server ./src/shell ./src/shell/commands ./src/tools

# Add compilation flags
CFLAGS=

# Add linking flags
LDFLAGS=

# Linker script
LINKERSCRIPT=

# Add libraries
# Format: -l<lib_name>
# Exemple: libDrvEEPROM.a -> -lDrvEEPROM
#LD_LIBS=-lldap_r-2.4 -lssl -lgssapi_krb5 -lcrypto -lpq -lpqxx-4.0
LD_LIBS=-lpqxx -lpq

# Library paths
# Use the -L<lib_path> format
#LD_PATHS=-Llib/libldap -Llib/libssl -Llib/libgssapi_krb5 -Llib/libcrypto/ -Llib/libpq/ -Llib/libpqxx/

# Add required include path
# Format: -I<folder>
ifneq ($(TARGET),rpi)
INC+=-I$(HOME)/rpi-libs/usr/include
endif
#INC+=-Ilib/libpqxx/include

# If you want to use the same Makefile.cfg and Makefile.inc
# for several programs, you can specify the path to them
# below.
MAKEFILE_FILES=./makefiles/
ifeq ($(TARGET),rpi)
include $(MAKEFILE_FILES)MakefileRPi.cfg
else
include $(MAKEFILE_FILES)MakefileC++11.cfg
endif
include $(MAKEFILE_FILES)Makefile.inc

# =============================================================================
# BUILD TARGET
# -----------------------------------------------------------------------------
#  The targets to execute.
#  - $(OUTDIR): Builds the output directory if required
#  - $(SRCFILES): Check source files
#  - $(OUTFILE): Build output file. Creates all required objects
#  - objdump: Dumps the output file (Alter params with OBJDUMPFLAGS)
#  - srec: Creates srec file from the output file (Alter params with SRECFLAGS)
#  - bin: Creates binary file from the output file (Alter params with BINFLAGS)
#  - map: Creates a map file of the output file (Alter params with NMFLAGS)
#  - size: Print sizes of the output file (Alter params with SIZEFLAGS)
#  - strip: Strips the output file (Alter params with STRIPFLAGS)
#  - framac-metrics: Create the metrics of the objects files (Alter params with FRAMACFLAGS)
# =============================================================================
all: list $(OUTDIR) $(SRCFILES) $(OUTFILE)

