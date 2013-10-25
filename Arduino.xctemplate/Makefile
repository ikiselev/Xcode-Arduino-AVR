#
# PATHS
#
ARDUINO_PATHER   = /Users/$(USER)/Dropbox/Arduino
FRAMEWORK_PATH   = $(ARDUINO_PATHER)/Framework

CORE_LIB_PATH    := $(FRAMEWORK_PATH)/cores/arduino
APP_LIB_PATH     := $(FRAMEWORK_PATH)/Libraries
BOARDS_TXT       := $(FRAMEWORK_PATH)/boards.txt

USER_LIB_PATH  = $(wildcard $(ARDUINO_PATHER)/?ibraries)

TOOLCHAIN_BIN_DIRECTORY = /usr/local/CrossPack-AVR/bin

#
# MAIN FUNCTION
#
TAKE_LOCAL_MAIN_FUNCTION = 0

#
# CONSOLE OVERRIDE
#
BOARD_TAG := atmega8
SKETCH_EXTENSION = ino



PROGRAMMER = usbasp



#
# LIBRARIES
# You need to add folder names of used libraries
#
APP_LIBS_LIST =
USER_LIBS_LIST =


#
# EXCLUDE LIST
#
EXCLUDE_LIBS = WiFi Esplora Robot_Control Robot_Control/utility Robot_Motor GSM TFT TFT/utility SoftwareSerial
EXCLUDE_NAMES  = Example example Examples examples Archive archive Archives archives Documentation documentation Reference reference
EXCLUDE_NAMES += ArduinoTestSuite
EXCLUDE_NAMES += $(EXCLUDE_LIBS)
EXCLUDE_LIST   = $(addprefix %,$(EXCLUDE_NAMES))










#TODO: Узнать почему несколько скетчей - плохо
ifneq ($(SKETCH_EXTENSION),cpp)
    ifeq ($(words $(wildcard *.$(SKETCH_EXTENSION))), 0)
        $(error No $(SKETCH_EXTENSION) sketch)
    endif

    ifneq ($(words $(wildcard *.$(SKETCH_EXTENSION))), 1)
        $(error More than one $(SKETCH_EXTENSION) sketch)
    endif
endif



#
# VARIABLES
#
TARGET := sketch
OBJDIR = ../build/Makefile


ifeq ($(TAKE_LOCAL_MAIN_FUNCTION),1)
	MAIN_CPP = main.cpp
else
	MAIN_CPP = $(CORE_LIB_PATH)/main.cpp
endif


# Function PARSE_BOARD data retrieval from boards.txt
# result = $(call PARSE_BOARD 'boardname','parameter')
PARSE_BOARD = $(shell if [ -f $(BOARDS_TXT) ]; then grep ^$(1).$(2)= $(BOARDS_TXT) | cut -d = -f 2; fi; )


MCU   = $(call PARSE_BOARD,$(BOARD_TAG),build.mcu)
F_CPU = $(call PARSE_BOARD,$(BOARD_TAG),build.f_cpu)


NEW_TAG := $(strip $(OBJDIR)/$(BOARD_TAG)-TAG) #
OLD_TAG := $(strip $(wildcard $(OBJDIR)/*-TAG)) # */

ifneq ($(OLD_TAG),$(NEW_TAG))
    CHANGE_FLAG := 1
else
    CHANGE_FLAG := 0
endif


# MAKE cpp FROM ino
PDEHEADER      = \\\#include \"Arduino.h\"

# FOR GREP main.cpp FROM FRAMEWORK PATH
PDEHEADER_SYS  = \\\#include \<Arduino.h\>

SKETCHES = $(addprefix $(OBJDIR)/, $(subst .$(SKETCH_EXTENSION),.cpp,$(wildcard *.$(SKETCH_EXTENSION))))
SKETCH_OBJS = $(SKETCHES:.cpp=.o)


#include sketches into main.cpp
SKETCH_INCLUDE = \\\#include \"$(sketchName)\"
SKETCH_INCLUDES = $(foreach sketchName,$(subst .$(SKETCH_EXTENSION),.cpp,$(wildcard *.$(SKETCH_EXTENSION))),$(SKETCH_INCLUDE))


#
# TOOL-CHAIN NAMES
#
CC      = $(TOOLCHAIN_BIN_DIRECTORY)/avr-gcc
CXX     = $(TOOLCHAIN_BIN_DIRECTORY)/avr-g++
AR      = $(TOOLCHAIN_BIN_DIRECTORY)/avr-ar
OBJDUMP = $(TOOLCHAIN_BIN_DIRECTORY)/avr-objdump
OBJCOPY = $(TOOLCHAIN_BIN_DIRECTORY)/avr-objcopy
SIZE    = $(TOOLCHAIN_BIN_DIRECTORY)/avr-size
NM      = $(TOOLCHAIN_BIN_DIRECTORY)/avr-nm



VARIANT      = $(call PARSE_BOARD,$(BOARD_TAG),build.variant)
VARIANT_PATH = $(FRAMEWORK_PATH)/variants/$(VARIANT)


PLATFORM_TAG   = ARDUINO=105
MCU_FLAG_NAME  = mmcu
EXTRA_LDFLAGS  = 
EXTRA_CPPFLAGS = -MMD -I$(VARIANT_PATH) $(addprefix -D, $(PLATFORM_TAG))

#
# AVRDUDE
#
AVRDUDE = $(TOOLCHAIN_BIN_DIRECTORY)/avrdude

ifndef AVRDUDE_COM_OPTS
	AVRDUDE_COM_OPTS  = -c $(PROGRAMMER) -p$(MCU)
    #AVRDUDE_COM_OPTS  = -q -V -F -p$(MCU) -C$
endif


# fuses if you're using e.g. ISP
ifndef ISP_LOCK_FUSE_PRE
	ISP_LOCK_FUSE_PRE  = $(call PARSE_BOARD,$(BOARD_TAG),bootloader.unlock_bits)
endif

ifndef ISP_LOCK_FUSE_POST
	ISP_LOCK_FUSE_POST = $(call PARSE_BOARD,$(BOARD_TAG),bootloader.lock_bits)
endif

ifndef ISP_HIGH_FUSE
	ISP_HIGH_FUSE      = $(call PARSE_BOARD,$(BOARD_TAG),bootloader.high_fuses)
endif

ifndef ISP_LOW_FUSE
	ISP_LOW_FUSE       = $(call PARSE_BOARD,$(BOARD_TAG),bootloader.low_fuses)
endif

ifndef ISP_EXT_FUSE
	ISP_EXT_FUSE       = $(call PARSE_BOARD,$(BOARD_TAG),bootloader.extended_fuses)
endif



FUSES =  -e -U lock:w:$(ISP_LOCK_FUSE_PRE):m -U hfuse:w:$(ISP_HIGH_FUSE):m -U lfuse:w:$(ISP_LOW_FUSE):m

ifdef ISP_EXT_FUSE
	FUSES += -U efuse:w:$(ISP_EXT_FUSE):m
endif


# CORE libraries
#
s5              = $(subst .h,,$(subst $(CORE_LIB_PATH)/,,$(wildcard $(CORE_LIB_PATH)/*.h))) # */
CORE_LIBS_LIST  = $(subst $(USER_LIB_PATH)/,,$(filter-out $(EXCLUDE_LIST),$(s5)))


# CORE sources
#
ifdef CORE_LIB_PATH
    CORE_C_SRCS     = $(wildcard $(CORE_LIB_PATH)/*.c) # */
    
    ifeq ($(strip $(TAKE_LOCAL_MAIN_FUNCTION)),1)
    	CORE_CPP_SRCS = $(filter-out %main.cpp, $(wildcard $(CORE_LIB_PATH)/*.cpp $(CORE_LIB_PATH)/*/*.cpp)) # */
    else
        CORE_CPP_SRCS = $(wildcard $(CORE_LIB_PATH)/*.cpp $(CORE_LIB_PATH)/*/*.cpp) # */
    endif

    CORE_OBJ_FILES  = $(CORE_C_SRCS:.c=.o) $(CORE_CPP_SRCS:.cpp=.o) $(CORE_AS_SRCS:.S=.o) 
    CORE_OBJS       = $(patsubst $(CORE_LIB_PATH)/%,$(OBJDIR)/%,$(CORE_OBJ_FILES))
endif


# Application Arduino
#
ifeq ($(APP_LIBS_LIST),)
    s1         = $(realpath $(sort $(dir $(wildcard $(APP_LIB_PATH)/*/*.h $(APP_LIB_PATH)/*/*/*.h)))) # */
    APP_LIBS_LIST = $(subst $(APP_LIB_PATH)/,,$(filter-out $(EXCLUDE_LIST),$(s1)))
endif

ifndef APP_LIBS
ifneq ($(APP_LIBS_LIST),0)
	s4         = $(patsubst %,$(APP_LIB_PATH)/%,$(APP_LIBS_LIST))
	APP_LIBS   = $(realpath $(sort $(dir $(foreach dir,$(s4),$(wildcard $(dir)/*.h $(dir)/*/*.h $(dir)/*/*/*.h)))))
    
endif
endif

ifndef APP_LIB_OBJS
    APP_LIB_C_SRC     = $(wildcard $(patsubst %,%/*.c,$(APP_LIBS))) # */
    APP_LIB_CPP_SRC   = $(wildcard $(patsubst %,%/*.cpp,$(APP_LIBS))) # */
    APP_LIB_OBJS      = $(patsubst $(APP_LIB_PATH)/%.c,$(OBJDIR)/libs/%.o,$(APP_LIB_C_SRC))
    APP_LIB_OBJS     += $(patsubst $(APP_LIB_PATH)/%.cpp,$(OBJDIR)/libs/%.o,$(APP_LIB_CPP_SRC))
endif 


# USER libraries
#
ifndef USER_LIBS_LIST
	s2               = $(realpath $(sort $(dir $(wildcard $(USER_LIB_PATH)/*/*.h)))) # */
    USER_LIBS_LIST   = $(subst $(USER_LIB_PATH)/,,$(filter-out $(EXCLUDE_LIST),$(s2)))
endif

ifneq ($(USER_LIBS_LIST),0)
    s3               = $(patsubst %,$(USER_LIB_PATH)/%,$(USER_LIBS_LIST))
	USER_LIBS        = $(realpath $(sort $(dir $(foreach dir,$(s3),$(wildcard $(dir)/*.h $(dir)/*/*.h $(dir)/*/*/*.h)))))

    USER_LIB_CPP_SRC = $(wildcard $(patsubst %,%/*.cpp,$(USER_LIBS))) # */
    USER_LIB_C_SRC   = $(wildcard $(patsubst %,%/*.c,$(USER_LIBS))) # */

    USER_OBJS        = $(patsubst $(USER_LIB_PATH)/%.cpp,$(OBJDIR)/libs/%.o,$(USER_LIB_CPP_SRC))
    USER_OBJS       += $(patsubst $(USER_LIB_PATH)/%.c,$(OBJDIR)/libs/%.o,$(USER_LIB_C_SRC))
endif


# LOCAL sources
#
LOCAL_C_SRCS    = $(wildcard *.c)

ifeq ($(strip $(TAKE_LOCAL_MAIN_FUNCTION)),1)
	LOCAL_CPP_SRCS = $(wildcard *.cpp)
else
    LOCAL_CPP_SRCS = $(filter-out %main.cpp, $(wildcard *.cpp))
endif


LOCAL_CC_SRCS   = $(wildcard *.cc)

# Use of implicit rule for LOCAL_PDE_SRCS
# wtf???
#LOCAL_PDE_SRCS  = $(wildcard *.$(SKETCH_EXTENSION))
LOCAL_AS_SRCS   = $(wildcard *.S)
LOCAL_OBJ_FILES = $(LOCAL_C_SRCS:.c=.o) $(LOCAL_CPP_SRCS:.cpp=.o) \
		$(LOCAL_PDE_SRCS:.$(SKETCH_EXTENSION)=.o) \
		$(LOCAL_CC_SRCS:.cc=.o) $(LOCAL_AS_SRCS:.S=.o)
LOCAL_OBJS      = $(patsubst %,$(OBJDIR)/%,$(LOCAL_OBJ_FILES))



# All the objects
# ??? Does order matter?
#
REMOTE_OBJS = $(CORE_OBJS) $(BUILD_CORE_OBJS) $(APP_LIB_OBJS) $(BUILD_APP_LIB_OBJS) $(VARIANT_OBJS) $(USER_OBJS)
OBJS        = $(REMOTE_OBJS) $(LOCAL_OBJS)

# Dependency files
#
DEPS   = $(OBJS:.o=.d)

#
# Main targets
#
TARGET_A   = $(OBJDIR)/$(TARGET).a
TARGET_HEX = $(OBJDIR)/$(TARGET).hex
TARGET_ELF = $(OBJDIR)/$(TARGET).elf
TARGET_BIN = $(OBJDIR)/$(TARGET).bin
TARGETS    = $(OBJDIR)/$(TARGET).*

ifndef TARGET_HEXBIN
    TARGET_HEXBIN = $(TARGET_HEX)
endif

ifndef TARGET_EEP
    TARGET_EEP    =
endif

# List of dependencies
#
DEP_FILE   = $(OBJDIR)/depends.mk

# Executables
#
REMOVE  = rm -r
MV      = mv -f
CAT     = cat
ECHO    = echo
GREP_INVERSE = $(shell echo grep -v \"$(1)\" $(2))

# General arguments
#
SYS_INCLUDES  = $(patsubst %,-I%,$(APP_LIBS))
SYS_INCLUDES += $(patsubst %,-I%,$(BUILD_APP_LIBS))
SYS_INCLUDES += $(patsubst %,-I%,$(USER_LIBS))

SYS_OBJS      = $(wildcard $(patsubst %,%/*.o,$(APP_LIBS))) # */
SYS_OBJS     += $(wildcard $(patsubst %,%/*.o,$(BUILD_APP_LIBS))) # */
SYS_OBJS     += $(wildcard $(patsubst %,%/*.o,$(USER_LIBS))) # */

CPPFLAGS      = -$(MCU_FLAG_NAME)=$(MCU) -DF_CPU=$(F_CPU) -I$(CORE_LIB_PATH)
CPPFLAGS     += $(SYS_INCLUDES) -g -Os -w -Wall -ffunction-sections -fdata-sections
CPPFLAGS     += $(EXTRA_CPPFLAGS)

ifdef USE_GNU99
    CFLAGS        = -std=gnu99
endif

# CXX = flags for C++ only
# CPP = flags for both C and C++
#
CXXFLAGS      = -fno-exceptions
ifdef EXTRA_CXXFLAGS
    CXXFLAGS += $(EXTRA_CXXFLAGS)
endif

ASFLAGS       = -$(MCU_FLAG_NAME)=$(MCU) -x assembler-with-cpp
LDFLAGS       = -$(MCU_FLAG_NAME)=$(MCU) -Wl,--gc-sections -Os $(EXTRA_LDFLAGS)

ifndef OBJCOPYFLAGS
    OBJCOPYFLAGS  = -Oihex -R .eeprom
endif


CURRENT_DIR   := $(shell pwd)
CURRENT_DIR   := $(shell echo '$(CURRENT_DIR)' | sed 's/ /\\\ /g')


main_preprocess:
	@echo --------------------MAIN PREPROCESS FROM: $(MAIN_CPP)--------------------------------
	@$(ECHO) $(PDEHEADER_SYS) > $(OBJDIR)/main.cpp
	@$(ECHO) $(SKETCH_INCLUDES) >> $(OBJDIR)/main.cpp
	@$(call GREP_INVERSE,$(PDEHEADER_SYS),$(MAIN_CPP)) >> $(OBJDIR)/main.cpp



# 2- APPlication Arduino library sources
#
$(OBJDIR)/libs/%.o: $(APP_LIB_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/libs/%.o: $(APP_LIB_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/libs/%.o: $(BUILD_APP_LIB_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/libs/%.o: $(BUILD_APP_LIB_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/libs/%.d: $(APP_LIB_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/libs/%.d: $(APP_LIB_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/libs/%.d: $(BUILD_APP_LIB_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/libs/%.d: $(BUILD_APP_LIB_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)


# 3- USER library sources
#
$(OBJDIR)/libs/%.o: $(USER_LIB_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/libs/%.o: $(USER_LIB_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/libs/%.d: $(USER_LIB_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/libs/%.d: $(USER_LIB_PATH)/%.c
	@mkdir -p $(dir $@)
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)


# 4- LOCAL sources
# .o rules are for objects, .d for dependency tracking
# 
$(OBJDIR)/%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

#vot tak vot
$(LOCAL_OBJS): $(OBJDIR)/%.o: 	%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: %.S
	$(CC) -c $(CPPFLAGS) $(ASFLAGS) $< -o $@

$(OBJDIR)/%.o: %.s
	$(CC) -c $(CPPFLAGS) $(ASFLAGS) $< -o $@

$(OBJDIR)/%.d: %.c
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/%.d: %.cpp
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/%.d: %.S
	$(CC) -MM $(CPPFLAGS) $(ASFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/%.d: %.s
	$(CC) -MM $(CPPFLAGS) $(ASFLAGS) $< -MF $@ -MT $(@:.d=.o)


# 5- SKETCH pde/ino -> cpp -> o file
#
$(OBJDIR)/%.cpp: %.$(SKETCH_EXTENSION)
	@$(ECHO) $(PDEHEADER) > $@
	@$(CAT)  $< >> $@

	
	
#
$(SKETCH_OBJS): $(OBJDIR)/%.o: $(OBJDIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -I. $< -o $@

$(OBJDIR)/%.d: $(OBJDIR)/%.cpp
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) -I. $< -MF $@ -MT $(@:.d=.o)


# 6- VARIANT files
#
$(OBJDIR)/libs/%.o: $(VARIANT_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(VARIANT_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/libs/%.d: $(VARIANT_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/%.d: $(VARIANT_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)


# 1- CORE files
#
$(OBJDIR)/%.o: $(CORE_LIB_PATH)/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(CORE_LIB_PATH)/%.S
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(CORE_LIB_PATH)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: $(BUILD_CORE_LIB_PATH)/%.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: $(BUILD_CORE_LIB_PATH)/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.d: $(CORE_LIB_PATH)/%.c
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/%.d: $(CORE_LIB_PATH)/%.cpp
	$(CXX) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/%.d: $(BUILD_CORE_LIB_PATH)/%.c
	$(CC) -MM $(CPPFLAGS) $(CFLAGS) $< -MF $@ -MT $(@:.d=.o)

$(OBJDIR)/%.d: $(BUILD_CORE_LIB_PATH)/%.cpp
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) $< -MF $@ -MT $(@:.d=.o)


# 7- Link
# ----------------------------------
#
$(TARGET_ELF): $(OBJS)
		@echo "---- Link ---- "
		@$(AR) rcs $(TARGET_A) $(REMOTE_OBJS) $(SKETCH_OBJS)
		$(CC) $(LDFLAGS) -o $@ $(LOCAL_OBJS) $(TARGET_A) -lm


# 8- Final conversions
# ----------------------------------
#
$(OBJDIR)/%.hex: $(OBJDIR)/%.elf
	$(OBJCOPY) -Oihex -R .eeprom $< $@

$(OBJDIR)/%.bin: $(OBJDIR)/%.elf
	$(OBJCOPY) -Obinary -v $< $@

$(OBJDIR)/%.eep: $(OBJDIR)/%.elf
	-$(OBJCOPY) -Oihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $@

$(OBJDIR)/%.lss: $(OBJDIR)/%.elf
	$(OBJDUMP) -h -S $< > $@

$(OBJDIR)/%.sym: $(OBJDIR)/%.elf
	$(NM) -n $< > $@


# Size of file
# ----------------------------------
#
ifeq ($(TARGET_HEXBIN),$(TARGET_HEX))
    HEXSIZE = $(SIZE) --target=ihex --totals $(CURRENT_DIR)/$(TARGET_HEX) | grep TOTALS | tr '\t' . | cut -d. -f2 | tr -d ' '
else ifeq ($(TARGET_HEXBIN),$(TARGET_BIN))
    BINSIZE = $(SIZE) --target=binary --totals $(CURRENT_DIR)/$(TARGET_BIN) | grep TOTALS | tr '\t' . | cut -d. -f2 | tr -d ' '
endif

ELFSIZE = $(SIZE) $(CURRENT_DIR)/$(TARGET_ELF)
MAX_FLASH_SIZE = $(call PARSE_BOARD,$(BOARD_TAG),upload.maximum_size)
RAMSIZE = $(SIZE) $(CURRENT_DIR)/$(TARGET_ELF) | sed '1d' | awk '{t=$$3 + $$2} END {print t}'


ifneq ($(MAX_FLASH_SIZE),)
    MAX_FLASH_BYTES   = 'bytes (of a '$(MAX_FLASH_SIZE)' byte maximum)'
else
    MAX_FLASH_BYTES   = bytes
endif

ifneq ($(MAX_RAM_SIZE),)
    MAX_RAM_BYTES   = 'bytes (of a '$(MAX_RAM_SIZE)' byte maximum)'
else
    MAX_RAM_BYTES   = bytes
endif



# 0- Info
#
info:

ifneq ($(MAKECMDGOALS),boards)
ifneq ($(MAKECMDGOALS),clean)

		@echo ==== Info ====
		@echo ---- Project ----
		@echo 'Target		'$(MAKECMDGOALS)
		@echo 'Name		'$(PROJECT_NAME)
		@echo 'Tag			'$(BOARD_TAG)
		@echo 'Extension		'$(SKETCH_EXTENSION)

		@echo ---- Board ----
		@echo 'Name		$(call PARSE_BOARD,$(BOARD_TAG),name)'
		@echo 'Frequency		'$(F_CPU)
		@echo 'MCU			'$(MCU)


		@echo ---- Libraries ----
		@echo . Core libraries
		@echo $(CORE_LIBS_LIST)

ifneq ($(BUILD_CORE_LIBS_LIST),)
		@echo $(BUILD_CORE_LIBS_LIST)
endif
		@echo 
		@echo . Application libraries of Arduino
		@echo $(APP_LIBS_LIST)
		@echo 
		@echo . User lib list from $(USER_LIB_PATH)
		@echo $(USER_LIBS_LIST)
		@echo 
		@echo . Local libraries from $(CURRENT_DIR)

ifneq ($(wildcard *.h),)
		@echo $(subst .h,,$(wildcard *.h))
else
		@echo 0
endif
		@echo 
		@echo ==== Info done ====
endif
endif


# Rules
# ----------------------------------
#
all: 		info message_all clean compile upload end_all

build: 		info message_build clean compile end_build


compile:	info message_compile main_preprocess $(SKETCH_OBJS) $(OBJDIR) $(TARGET_HEXBIN) $(TARGET_EEP) size
		@echo $(BOARD_TAG) > $(NEW_TAG)

$(OBJDIR):
		@echo "---- Build ---- "
		@mkdir $(OBJDIR)

$(DEP_FILE):	$(OBJDIR) $(DEPS)
		@echo "9-" $<
		@cat $(DEPS) > $(DEP_FILE)


upload:
		@echo "---- Upload ---- "

		$(AVRDUDE) $(AVRDUDE_COM_OPTS) -Uflash:w:$(TARGET_HEX):i

		@echo "==== upload done ==== "


ispload:	$(TARGET_HEX)
		@echo "---- ISP upload ---- "
		$(AVRDUDE) $(AVRDUDE_COM_OPTS) $(FUSES)
		$(AVRDUDE) $(AVRDUDE_COM_OPTS) -D -U flash:w:$(TARGET_HEX):i
		$(AVRDUDE) $(AVRDUDE_COM_OPTS) -U lock:w:$(ISP_LOCK_FUSE_POST):m



size:
		@echo "---- Size ----"
		@if [ -f $(TARGET_HEX) ]; then echo 'Binary sketch size: ' $(shell $(HEXSIZE)) $(MAX_FLASH_BYTES); echo; fi

		@if [ -f $(TARGET_BIN) ]; then echo 'Binary sketch size:' $(shell $(BINSIZE)) $(MAX_FLASH_BYTES); echo; fi
		@if [ -f $(TARGET_ELF) ]; then echo 'Estimated SRAM used:' $(shell $(RAMSIZE)) $(MAX_RAM_BYTES); echo; fi


clean:	changed
		@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
		@echo "---- Clean ----"
		@echo "nil" > $(OBJDIR)/nil
		-@rm -r $(OBJDIR)/* # */ 

changed:
ifeq ($(CHANGE_FLAG),1)
		@echo "---- Clean changed ----"
		@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
		@echo "nil" > $(OBJDIR)/nil
		$(REMOVE) $(OBJDIR)/* # */
endif

depends:	$(DEPS)
		@echo "---- Depends ---- "
		@cat $(DEPS) > $(DEP_FILE)

message_all:
		@echo "==== All ===="

message_build:
		@echo "==== Build ===="

message_compile:
		@echo "---- Compile ----"

message_upload:
		@echo "==== Upload ===="

end_all:
		@echo "==== All done ==== "

end_build:
		@echo "==== Build done ==== "

                
.PHONY:	all clean main_preprocess depends upload size