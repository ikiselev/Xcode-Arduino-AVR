# You should at least check the settings for
# MCU ....... The AVR device you compile for
# F_CPU ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected. We recommend that you leave it undefined and
#                add settings like this to your ~/.avrduderc file:
#                   default_programmer = "stk500v2"
#                   default_serial = "avrdoper"
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.
FRAMEWORK_PATH   := /Users/$(USER)/Dropbox/Arduino/Framework
CORE_LIB_PATH    := $(FRAMEWORK_PATH)/cores/arduino
BOARDS_TXT       := $(FRAMEWORK_PATH)/boards.txt
APP_LIB_PATH     := $(FRAMEWORK_PATH)/../Libraries

## DEBUG:
TARGET := TestArduino
BOARD_TAG := nano
SKETCH_EXTENSION = ino
# DEBUG END

PROGRAMMER = -c usbasp

##########################################################################
OBJDIR  = Build

#for what platform?
PLATFORM         := Arduino
PLATFORM_TAG      = ARDUINO=105


# For computing fuse byte values for other devices and options see
# the fuse bit calculator at http://www.engbedded.com/fusecalc/
FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0x24:m

####

# Rules for making a c++ file from the main sketch (.pde)
#
PDEHEADER      = \\\#include \"Arduino.h\"  

# Tool-chain names
#
CC      = avr-gcc
CXX     = avr-g++
AR      = avr-ar
OBJDUMP = avr-objdump
OBJCOPY = avr-objcopy
SIZE    = avr-size
NM      = avr-nm


#######


# Function PARSE_BOARD data retrieval from boards.txt
# result = $(call PARSE_BOARD 'boardname','parameter')
#
PARSE_BOARD = $(shell if [ -f $(BOARDS_TXT) ]; then grep ^$(1).$(2)= $(BOARDS_TXT) | cut -d = -f 2; fi; )

# Function PARSE_FILE data retrieval from specified file
# result = $(call PARSE_FILE 'boardname','parameter','filename')
#
PARSE_FILE = $(shell if [ -f $(3) ]; then grep ^$(1).$(2) $(3) | cut -d = -f 2; fi; )


ifndef BOARD_TAG
	$(error BOARD_TAG not defined)
endif

MCU   = $(call PARSE_BOARD,$(BOARD_TAG),build.mcu)
F_CPU = $(call PARSE_BOARD,$(BOARD_TAG),build.f_cpu)



NEW_TAG := $(strip $(OBJDIR)/$(BOARD_TAG)-TAG) #



VARIANT      = $(call PARSE_BOARD,$(BOARD_TAG),build.variant)
VARIANT_PATH = $(FRAMEWORK_PATH)/variants/$(VARIANT)

EXTRA_CPPFLAGS = -MMD -I$(VARIANT_PATH) $(addprefix -D, $(PLATFORM_TAG))


$(info Board: $(BOARD_TAG))
$(info MCU: $(MCU))

###########################################################################

s5              = $(subst .h,,$(subst $(CORE_LIB_PATH)/,,$(wildcard $(CORE_LIB_PATH)/*.h))) # */
CORE_LIBS_LIST  = $(subst $(USER_LIB_PATH)/,,$(filter-out $(EXCLUDE_LIST),$(s5)))


# CORE sources
#
ifdef CORE_LIB_PATH
    CORE_C_SRCS     = $(wildcard $(CORE_LIB_PATH)/*.c) # */
    CORE_CPP_SRCS = $(filter-out %main.cpp, $(wildcard $(CORE_LIB_PATH)/*.cpp $(CORE_LIB_PATH)/*/*.cpp)) # */

    CORE_OBJ_FILES  = $(CORE_C_SRCS:.c=.o) $(CORE_CPP_SRCS:.cpp=.o) $(CORE_AS_SRCS:.S=.o) 
    CORE_OBJS       = $(patsubst $(CORE_LIB_PATH)/%,$(OBJDIR)/%,$(CORE_OBJ_FILES))
endif

# Arduino Libraries
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
    FLAG = 1
    APP_LIB_C_SRC     = $(wildcard $(patsubst %,%/*.c,$(APP_LIBS))) # */
    APP_LIB_CPP_SRC   = $(wildcard $(patsubst %,%/*.cpp,$(APP_LIBS))) # */
    APP_LIB_OBJS      = $(patsubst $(APP_LIB_PATH)/%.c,$(OBJDIR)/libs/%.o,$(APP_LIB_C_SRC))
    APP_LIB_OBJS     += $(patsubst $(APP_LIB_PATH)/%.cpp,$(OBJDIR)/libs/%.o,$(APP_LIB_CPP_SRC))
endif 


# LOCAL sources
#
LOCAL_C_SRCS    = $(wildcard *.c)
LOCAL_CPP_SRCS = $(wildcard *.cpp)
LOCAL_CC_SRCS   = $(wildcard *.cc)


# Use of implicit rule for LOCAL_PDE_SRCS
#
LOCAL_AS_SRCS   = $(wildcard *.S)
LOCAL_OBJ_FILES = $(LOCAL_C_SRCS:.c=.o) $(LOCAL_CPP_SRCS:.cpp=.o) \
		$(LOCAL_PDE_SRCS:.$(SKETCH_EXTENSION)=.o) \
		$(LOCAL_CC_SRCS:.cc=.o) $(LOCAL_AS_SRCS:.S=.o)
LOCAL_OBJS      = $(patsubst %,$(OBJDIR)/%,$(LOCAL_OBJ_FILES))


################################

# All the objects
#
REMOTE_OBJS = $(CORE_OBJS) $(BUILD_CORE_OBJS) $(APP_LIB_OBJS) $(BUILD_APP_LIB_OBJS) $(VARIANT_OBJS) $(USER_OBJS)
OBJS        = $(REMOTE_OBJS) $(LOCAL_OBJS)

# Dependency files
#
#DEPS   = $(LOCAL_OBJS:.o=.d)
DEPS   = $(OBJS:.o=.d)


$(info Processing Rules)
##########################################################################

# Rules
# ----------------------------------
#

# Main targets
#
TARGET_A   = $(OBJDIR)/$(TARGET).a
TARGET_HEX = $(OBJDIR)/$(TARGET).hex
TARGET_ELF = $(OBJDIR)/$(TARGET).elf
TARGET_BIN = $(OBJDIR)/$(TARGET).bin
TARGETS    = $(OBJDIR)/$(TARGET).*


TARGET_HEXBIN = $(TARGET_HEX)
TARGET_EEP    =

# List of dependencies
#
DEP_FILE   = $(OBJDIR)/depends.mk


# Executables
#
REMOVE  = rm -r
MV      = mv -f
CAT     = cat
ECHO    = echo


# General arguments
#
SYS_INCLUDES  = $(patsubst %,-I%,$(APP_LIBS))
SYS_INCLUDES += $(patsubst %,-I%,$(BUILD_APP_LIBS))
#SYS_INCLUDES += $(patsubst %,-I%,$(USER_LIBS))

SYS_OBJS      = $(wildcard $(patsubst %,%/*.o,$(APP_LIBS))) # */
SYS_OBJS     += $(wildcard $(patsubst %,%/*.o,$(BUILD_APP_LIBS))) # */
#SYS_OBJS     += $(wildcard $(patsubst %,%/*.o,$(USER_LIBS))) # */

CPPFLAGS      = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I$(CORE_LIB_PATH)
CPPFLAGS     += $(SYS_INCLUDES) -g -Os -w -Wall -ffunction-sections -fdata-sections
CPPFLAGS     += $(EXTRA_CPPFLAGS)


ifdef USE_GNU99
    CFLAGS        = -std=gnu99
endif

CXXFLAGS      = -fno-exceptions
ifdef EXTRA_CXXFLAGS
    CXXFLAGS += $(EXTRA_CXXFLAGS)
endif

ASFLAGS       = -mmcu=$(MCU) -x assembler-with-cpp
LDFLAGS       = -mmcu=$(MCU) -Wl,--gc-sections -Os $(EXTRA_LDFLAGS)


ifndef OBJCOPYFLAGS
    OBJCOPYFLAGS  = -Oihex -R .eeprom
endif

OBJCOPYFLAGS  = -Oihex -R .eeprom

#########################################################################
# COMPILE
#

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



$(info Core files ok)

# Libraries
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


$(info Libraries files ok)


# Variant files
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


# Local sources
#
$(OBJDIR)/%.o: %.c
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

$(OBJDIR)/%.o: %.cc
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(OBJDIR)/%.o: 	%.cpp
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


# Sketch
#
$(OBJDIR)/%.cpp: %.$(SKETCH_EXTENSION)
	@$(ECHO) $(PDEHEADER) > $@
	@$(CAT)  $< >> $@
#	@$(ECHO) $(PDEHEADER) > $(OBJDIR)/text.txt
#	@$(CAT)  $< >> $(OBJDIR)/text.txt

$(OBJDIR)/%.o: $(OBJDIR)/%.cpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -I. $< -o $@

$(OBJDIR)/%.d: $(OBJDIR)/%.cpp
	$(CXX) -MM $(CPPFLAGS) $(CXXFLAGS) -I. $< -MF $@ -MT $(@:.d=.o)



#########################################################################
# Link
# In original was with $(TARGET_A), but it don't works for me
#
$(TARGET_ELF): 	$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(LOCAL_OBJS) -lm


#########################################################################
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






all: 		message_all clean compile

compile:	message_compile $(OBJDIR) $(TARGET_HEXBIN) $(TARGET_EEP)
		@echo $(BOARD_TAG) > $(NEW_TAG)

clean:
		@if [ ! -d $(OBJDIR) ]; then mkdir $(OBJDIR); fi
		@echo "nil" > $(OBJDIR)/nil
		@echo "---- Clean ----"
		-@rm -r $(OBJDIR)/* # */ 


message_all:
		@echo "==== All ===="

message_compile:
		@echo "---- Compile ----"
