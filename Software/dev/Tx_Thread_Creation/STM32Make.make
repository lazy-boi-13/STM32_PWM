##########################################################################################################################
# File automatically-generated by STM32forVSCode
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#   2024-04-27 - Added env file inclusion. 
#                Added way to overide: build directory, target name and optimisation.
#                Added GCC_PATH by env file to not make the makefile machine dependent.
#                Currently folder structure in build directory is preserved
#                Switching of debug/release build output folder now happens based on debug flag
#   2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# Environment Variables
######################################
# Imports the environment file in which the compiler and other tooling is set
# for the build machine.
# This can also be used to overwrite some makefile variables
file_exists = $(or $(and $(wildcard $(1)),1),0)
ifeq ($(call file_exists,.stm32env),1)
  include .stm32env
endif

######################################
# Target
######################################
# This is the name of the embedded target which will be build
# The final file name will also have debug or release appended to it.
TARGET ?= Tx_Thread_Creation

#######################################
# Build directories
#######################################
# Build path can be overwritten when calling make or setting the environment variable
# in .stm32env

BUILD_DIRECTORY ?= build


######################################
# Optimization
######################################
# Optimization is switched based upon the DEBUG variable. If set to 1
# it will be build in debug mode with the Og optimization flag (optimized for debugging).
# If set to 0 (false) then by default the variable is used in the configuration yaml
# This can also be overwritten using the environment variable or by overwriting it
# by calling make with the OPTIMIZATION variable e.g.:
# make -f STM32Make.make -j 16  OPTIMIZATION=Os

# variable which determines if it is a debug build
DEBUG ?= 1

# debug flags when debug is defined
OPTIMIZATION ?= -Og

RELEASE_DIRECTORY = $(BUILD_DIRECTORY)/debug
ifeq ($(DEBUG),1)
  # Sets debugging optimization -Og and the debug information output
  OPTIMIZATION_FLAGS += -Og -g -gdwarf -ggdb
  $(TARGET) := $(TARGET)-debug
  RELEASE_DIRECTORY := $(BUILD_DIRECTORY)/debug
else
  OPTIMIZATION_FLAGS += $(OPTIMIZATION)
  $(TARGET) := $(TARGET)-release
  RELEASE_DIRECTORY := $(BUILD_DIRECTORY)/release
endif

######################################
# source
######################################
# C sources
C_SOURCES =  \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_tim_ex.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c \
Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c \
Middlewares/ST/threadx/common/src/tx_block_allocate.c \
Middlewares/ST/threadx/common/src/tx_block_pool_cleanup.c \
Middlewares/ST/threadx/common/src/tx_block_pool_create.c \
Middlewares/ST/threadx/common/src/tx_block_pool_delete.c \
Middlewares/ST/threadx/common/src/tx_block_pool_info_get.c \
Middlewares/ST/threadx/common/src/tx_block_pool_initialize.c \
Middlewares/ST/threadx/common/src/tx_block_pool_prioritize.c \
Middlewares/ST/threadx/common/src/tx_block_release.c \
Middlewares/ST/threadx/common/src/tx_byte_allocate.c \
Middlewares/ST/threadx/common/src/tx_byte_pool_cleanup.c \
Middlewares/ST/threadx/common/src/tx_byte_pool_create.c \
Middlewares/ST/threadx/common/src/tx_byte_pool_delete.c \
Middlewares/ST/threadx/common/src/tx_byte_pool_info_get.c \
Middlewares/ST/threadx/common/src/tx_byte_pool_initialize.c \
Middlewares/ST/threadx/common/src/tx_byte_pool_prioritize.c \
Middlewares/ST/threadx/common/src/tx_byte_pool_search.c \
Middlewares/ST/threadx/common/src/tx_byte_release.c \
Middlewares/ST/threadx/common/src/tx_event_flags_cleanup.c \
Middlewares/ST/threadx/common/src/tx_event_flags_create.c \
Middlewares/ST/threadx/common/src/tx_event_flags_delete.c \
Middlewares/ST/threadx/common/src/tx_event_flags_get.c \
Middlewares/ST/threadx/common/src/tx_event_flags_info_get.c \
Middlewares/ST/threadx/common/src/tx_event_flags_initialize.c \
Middlewares/ST/threadx/common/src/tx_event_flags_set.c \
Middlewares/ST/threadx/common/src/tx_event_flags_set_notify.c \
Middlewares/ST/threadx/common/src/tx_initialize_high_level.c \
Middlewares/ST/threadx/common/src/tx_initialize_kernel_enter.c \
Middlewares/ST/threadx/common/src/tx_initialize_kernel_setup.c \
Middlewares/ST/threadx/common/src/tx_mutex_cleanup.c \
Middlewares/ST/threadx/common/src/tx_mutex_create.c \
Middlewares/ST/threadx/common/src/tx_mutex_delete.c \
Middlewares/ST/threadx/common/src/tx_mutex_get.c \
Middlewares/ST/threadx/common/src/tx_mutex_info_get.c \
Middlewares/ST/threadx/common/src/tx_mutex_initialize.c \
Middlewares/ST/threadx/common/src/tx_mutex_prioritize.c \
Middlewares/ST/threadx/common/src/tx_mutex_priority_change.c \
Middlewares/ST/threadx/common/src/tx_mutex_put.c \
Middlewares/ST/threadx/common/src/tx_queue_cleanup.c \
Middlewares/ST/threadx/common/src/tx_queue_create.c \
Middlewares/ST/threadx/common/src/tx_queue_delete.c \
Middlewares/ST/threadx/common/src/tx_queue_flush.c \
Middlewares/ST/threadx/common/src/tx_queue_front_send.c \
Middlewares/ST/threadx/common/src/tx_queue_info_get.c \
Middlewares/ST/threadx/common/src/tx_queue_initialize.c \
Middlewares/ST/threadx/common/src/tx_queue_prioritize.c \
Middlewares/ST/threadx/common/src/tx_queue_receive.c \
Middlewares/ST/threadx/common/src/tx_queue_send.c \
Middlewares/ST/threadx/common/src/tx_queue_send_notify.c \
Middlewares/ST/threadx/common/src/tx_semaphore_ceiling_put.c \
Middlewares/ST/threadx/common/src/tx_semaphore_cleanup.c \
Middlewares/ST/threadx/common/src/tx_semaphore_create.c \
Middlewares/ST/threadx/common/src/tx_semaphore_delete.c \
Middlewares/ST/threadx/common/src/tx_semaphore_get.c \
Middlewares/ST/threadx/common/src/tx_semaphore_info_get.c \
Middlewares/ST/threadx/common/src/tx_semaphore_initialize.c \
Middlewares/ST/threadx/common/src/tx_semaphore_prioritize.c \
Middlewares/ST/threadx/common/src/tx_semaphore_put.c \
Middlewares/ST/threadx/common/src/tx_semaphore_put_notify.c \
Middlewares/ST/threadx/common/src/tx_thread_create.c \
Middlewares/ST/threadx/common/src/tx_thread_delete.c \
Middlewares/ST/threadx/common/src/tx_thread_entry_exit_notify.c \
Middlewares/ST/threadx/common/src/tx_thread_identify.c \
Middlewares/ST/threadx/common/src/tx_thread_info_get.c \
Middlewares/ST/threadx/common/src/tx_thread_initialize.c \
Middlewares/ST/threadx/common/src/tx_thread_preemption_change.c \
Middlewares/ST/threadx/common/src/tx_thread_priority_change.c \
Middlewares/ST/threadx/common/src/tx_thread_relinquish.c \
Middlewares/ST/threadx/common/src/tx_thread_reset.c \
Middlewares/ST/threadx/common/src/tx_thread_resume.c \
Middlewares/ST/threadx/common/src/tx_thread_shell_entry.c \
Middlewares/ST/threadx/common/src/tx_thread_sleep.c \
Middlewares/ST/threadx/common/src/tx_thread_stack_analyze.c \
Middlewares/ST/threadx/common/src/tx_thread_stack_error_handler.c \
Middlewares/ST/threadx/common/src/tx_thread_stack_error_notify.c \
Middlewares/ST/threadx/common/src/tx_thread_suspend.c \
Middlewares/ST/threadx/common/src/tx_thread_system_preempt_check.c \
Middlewares/ST/threadx/common/src/tx_thread_system_resume.c \
Middlewares/ST/threadx/common/src/tx_thread_system_suspend.c \
Middlewares/ST/threadx/common/src/tx_thread_terminate.c \
Middlewares/ST/threadx/common/src/tx_thread_time_slice.c \
Middlewares/ST/threadx/common/src/tx_thread_time_slice_change.c \
Middlewares/ST/threadx/common/src/tx_thread_timeout.c \
Middlewares/ST/threadx/common/src/tx_thread_wait_abort.c \
Middlewares/ST/threadx/common/src/tx_time_get.c \
Middlewares/ST/threadx/common/src/tx_time_set.c \
Middlewares/ST/threadx/common/src/tx_timer_activate.c \
Middlewares/ST/threadx/common/src/tx_timer_change.c \
Middlewares/ST/threadx/common/src/tx_timer_create.c \
Middlewares/ST/threadx/common/src/tx_timer_deactivate.c \
Middlewares/ST/threadx/common/src/tx_timer_delete.c \
Middlewares/ST/threadx/common/src/tx_timer_expiration_process.c \
Middlewares/ST/threadx/common/src/tx_timer_info_get.c \
Middlewares/ST/threadx/common/src/tx_timer_initialize.c \
Middlewares/ST/threadx/common/src/tx_timer_system_activate.c \
Middlewares/ST/threadx/common/src/tx_timer_system_deactivate.c \
Middlewares/ST/threadx/common/src/tx_timer_thread_entry.c \
Middlewares/ST/threadx/common/src/txe_block_allocate.c \
Middlewares/ST/threadx/common/src/txe_block_pool_create.c \
Middlewares/ST/threadx/common/src/txe_block_pool_delete.c \
Middlewares/ST/threadx/common/src/txe_block_pool_info_get.c \
Middlewares/ST/threadx/common/src/txe_block_pool_prioritize.c \
Middlewares/ST/threadx/common/src/txe_block_release.c \
Middlewares/ST/threadx/common/src/txe_byte_allocate.c \
Middlewares/ST/threadx/common/src/txe_byte_pool_create.c \
Middlewares/ST/threadx/common/src/txe_byte_pool_delete.c \
Middlewares/ST/threadx/common/src/txe_byte_pool_info_get.c \
Middlewares/ST/threadx/common/src/txe_byte_pool_prioritize.c \
Middlewares/ST/threadx/common/src/txe_byte_release.c \
Middlewares/ST/threadx/common/src/txe_event_flags_create.c \
Middlewares/ST/threadx/common/src/txe_event_flags_delete.c \
Middlewares/ST/threadx/common/src/txe_event_flags_get.c \
Middlewares/ST/threadx/common/src/txe_event_flags_info_get.c \
Middlewares/ST/threadx/common/src/txe_event_flags_set.c \
Middlewares/ST/threadx/common/src/txe_event_flags_set_notify.c \
Middlewares/ST/threadx/common/src/txe_mutex_create.c \
Middlewares/ST/threadx/common/src/txe_mutex_delete.c \
Middlewares/ST/threadx/common/src/txe_mutex_get.c \
Middlewares/ST/threadx/common/src/txe_mutex_info_get.c \
Middlewares/ST/threadx/common/src/txe_mutex_prioritize.c \
Middlewares/ST/threadx/common/src/txe_mutex_put.c \
Middlewares/ST/threadx/common/src/txe_queue_create.c \
Middlewares/ST/threadx/common/src/txe_queue_delete.c \
Middlewares/ST/threadx/common/src/txe_queue_flush.c \
Middlewares/ST/threadx/common/src/txe_queue_front_send.c \
Middlewares/ST/threadx/common/src/txe_queue_info_get.c \
Middlewares/ST/threadx/common/src/txe_queue_prioritize.c \
Middlewares/ST/threadx/common/src/txe_queue_receive.c \
Middlewares/ST/threadx/common/src/txe_queue_send.c \
Middlewares/ST/threadx/common/src/txe_queue_send_notify.c \
Middlewares/ST/threadx/common/src/txe_semaphore_ceiling_put.c \
Middlewares/ST/threadx/common/src/txe_semaphore_create.c \
Middlewares/ST/threadx/common/src/txe_semaphore_delete.c \
Middlewares/ST/threadx/common/src/txe_semaphore_get.c \
Middlewares/ST/threadx/common/src/txe_semaphore_info_get.c \
Middlewares/ST/threadx/common/src/txe_semaphore_prioritize.c \
Middlewares/ST/threadx/common/src/txe_semaphore_put.c \
Middlewares/ST/threadx/common/src/txe_semaphore_put_notify.c \
Middlewares/ST/threadx/common/src/txe_thread_create.c \
Middlewares/ST/threadx/common/src/txe_thread_delete.c \
Middlewares/ST/threadx/common/src/txe_thread_entry_exit_notify.c \
Middlewares/ST/threadx/common/src/txe_thread_info_get.c \
Middlewares/ST/threadx/common/src/txe_thread_preemption_change.c \
Middlewares/ST/threadx/common/src/txe_thread_priority_change.c \
Middlewares/ST/threadx/common/src/txe_thread_relinquish.c \
Middlewares/ST/threadx/common/src/txe_thread_reset.c \
Middlewares/ST/threadx/common/src/txe_thread_resume.c \
Middlewares/ST/threadx/common/src/txe_thread_suspend.c \
Middlewares/ST/threadx/common/src/txe_thread_terminate.c \
Middlewares/ST/threadx/common/src/txe_thread_time_slice_change.c \
Middlewares/ST/threadx/common/src/txe_thread_wait_abort.c \
Middlewares/ST/threadx/common/src/txe_timer_activate.c \
Middlewares/ST/threadx/common/src/txe_timer_change.c \
Middlewares/ST/threadx/common/src/txe_timer_create.c \
Middlewares/ST/threadx/common/src/txe_timer_deactivate.c \
Middlewares/ST/threadx/common/src/txe_timer_delete.c \
Middlewares/ST/threadx/common/src/txe_timer_info_get.c \
Src/app_azure_rtos.c \
Src/app_threadx.c \
Src/main.c \
Src/stm32h7xx_hal_msp.c \
Src/stm32h7xx_hal_timebase_tim.c \
Src/stm32h7xx_it.c \
Src/syscalls.c \
Src/sysmem.c \
Src/system_stm32h7xx.c \
Src/tasks.c \
Src/timerpwm.c


CXX_SOURCES = \


# ASM sources
ASM_SOURCES =  \
Middlewares/ST/threadx/ports/cortex_m7/gnu/src/tx_thread_context_restore.S \
Middlewares/ST/threadx/ports/cortex_m7/gnu/src/tx_thread_context_save.S \
Middlewares/ST/threadx/ports/cortex_m7/gnu/src/tx_thread_interrupt_control.S \
Middlewares/ST/threadx/ports/cortex_m7/gnu/src/tx_thread_interrupt_disable.S \
Middlewares/ST/threadx/ports/cortex_m7/gnu/src/tx_thread_interrupt_restore.S \
Middlewares/ST/threadx/ports/cortex_m7/gnu/src/tx_thread_schedule.S \
Middlewares/ST/threadx/ports/cortex_m7/gnu/src/tx_thread_stack_build.S \
Middlewares/ST/threadx/ports/cortex_m7/gnu/src/tx_thread_system_return.S \
Middlewares/ST/threadx/ports/cortex_m7/gnu/src/tx_timer_interrupt.S \
Src/tx_initialize_low_level.S \
startup_stm32h723xx.s


#######################################
# Tools
#######################################
ARM_PREFIX = arm-none-eabi-
POSTFIX = "
PREFIX = "
# The gcc compiler bin path can be defined in the make command via ARM_GCC_PATH variable (e.g.: make ARM_GCC_PATH=xxx)
# or it can be added to the PATH environment variable.
# By default the variable be used from the environment file: .stm32env.
# if it is not defined

ifdef ARM_GCC_PATH
    CC = $(PREFIX)$(ARM_GCC_PATH)/$(ARM_PREFIX)gcc$(POSTFIX)
    CXX = $(PREFIX)$(ARM_GCC_PATH)/$(ARM_PREFIX)g++$(POSTFIX)
    AS = $(PREFIX)$(ARM_GCC_PATH)/$(ARM_PREFIX)gcc$(POSTFIX) -x assembler-with-cpp
    CP = $(PREFIX)$(ARM_GCC_PATH)/$(ARM_PREFIX)objcopy$(POSTFIX)
    SZ = $(PREFIX)$(ARM_GCC_PATH)/$(ARM_PREFIX)size$(POSTFIX)
    DP = $(PREFIX)$(ARM_GCC_PATH)/$(ARM_PREFIX)objdump$(POSTFIX)
else
  CC ?= $(ARM_PREFIX)gcc
  CXX ?= $(ARM_PREFIX)g++$
  AS ?= $(ARM_PREFIX)gcc -x assembler-with-cpp
  CP ?= $(ARM_PREFIX)objcopy
  SZ ?= $(ARM_PREFIX)size
  DP ?= $(ARM_PREFIX)objdump
endif

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
LSS = $(DP) -h -S


REMOVE_DIRECTORY_COMMAND = rm -fR
mkdir_function = mkdir -p $(1)
ifeq ($(OS),Windows_NT)
  convert_to_windows_path = $(strip $(subst /,\,$(patsubst %/,%,$(1))))
  REMOVE_DIRECTORY_COMMAND = cmd /c rd /s /q
  mkdir_function = cmd /e:on /c if not exist $(call convert_to_windows_path,$(1)) md $(call convert_to_windows_path,$(1))
endif


# Flash and debug tools
# Default is openocd however will be gotten from the env file when existing
OPENOCD ?= openocd


#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m7

# fpu
FPU = -mfpu=fpv5-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DSTM32H723xx \
-DTX_INCLUDE_USER_DEFINE_FILE \
-DUSE_HAL_DRIVER \
-DUSE_PWR_LDO_SUPPLY


# CXX defines
CXX_DEFS =  \
-DSTM32H723xx \
-DTX_INCLUDE_USER_DEFINE_FILE \
-DUSE_HAL_DRIVER \
-DUSE_PWR_LDO_SUPPLY


# AS includes
AS_INCLUDES = \

# C includes
C_INCLUDES =  \
-IDrivers/CMSIS/Device/ST/STM32H7xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/STM32H7xx_HAL_Driver/Inc \
-IDrivers/STM32H7xx_HAL_Driver/Inc/Legacy \
-IInc \
-IMiddlewares/ST/threadx/common/inc/ \
-IMiddlewares/ST/threadx/ports/cortex_m7/gnu/inc/



# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(C_INCLUDES) $(C_DEFS) $(OPTIMIZATION_FLAGS) 

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPTIMIZATION_FLAGS)

CXXFLAGS = $(MCU) $(CXX_DEFS) $(C_INCLUDES) $(OPTIMIZATION_FLAGS)

# Add additional flags
CFLAGS += -Wall -fdata-sections -ffunction-sections 
ASFLAGS += -Wall -fdata-sections -ffunction-sections 
CXXFLAGS += -fno-exceptions -fno-rtti 

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
CXXFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

# Output a list file for the compiled source file.
# This is a representative of the source code in assembly
ASSEMBLER_LIST_OUTPUT_FLAG = -Wa,-a,-ad,-alms=$(call add_release_directory,$<,lst)
CFLAGS += $(ASSEMBLER_LIST_OUTPUT_FLAG)
CXXFLAGS += $(ASSEMBLER_LIST_OUTPUT_FLAG)

#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32H723ZGTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = \


# Additional LD Flags from config file
ADDITIONALLDFLAGS = -Wl,--print-memory-usage -specs=nano.specs 

LDFLAGS = $(MCU) $(ADDITIONALLDFLAGS) -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIRECTORY)/$(TARGET).map,--cref -Wl,--gc-sections

#######################################
# build the application
#######################################
add_release_directory = $(sort $(addprefix $(RELEASE_DIRECTORY)/,$(addsuffix .$(2),$(basename $(notdir $(1))))))



OBJECTS = $(call add_release_directory,$(C_SOURCES),o)
OBJECTS += $(call add_release_directory,$(CXX_SOURCES),o)
OBJECTS += $(call add_release_directory,$(ASM_SOURCES),o)
vpath %.c $(sort $(dir $(C_SOURCES)))
vpath %.cc $(sort $(dir $(CXX_SOURCES)))
vpath %.cp $(sort $(dir $(CXX_SOURCES)))
vpath %.cxx $(sort $(dir $(CXX_SOURCES)))
vpath %.cpp $(sort $(dir $(CXX_SOURCES)))
vpath %.c++ $(sort $(dir $(CXX_SOURCES)))
vpath %.C $(sort $(dir $(CXX_SOURCES)))
vpath %.CPP $(sort $(dir $(CXX_SOURCES)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
vpath %.S $(sort $(dir $(ASM_SOURCES)))

#######################################
# all
#######################################
# note needs to be located as the first rule to be the default build rule
# default action: build all
all: $(RELEASE_DIRECTORY)/$(TARGET).elf $(RELEASE_DIRECTORY)/$(TARGET).hex $(RELEASE_DIRECTORY)/$(TARGET).bin $(RELEASE_DIRECTORY)/$(TARGET).lss 


# C build
$(RELEASE_DIRECTORY)/%.o: %.c STM32Make.make | $(RELEASE_DIRECTORY)
	$(CC) -c $(CFLAGS) $< -o $@

# C++ build 
$(RELEASE_DIRECTORY)/%.o: %.cc STM32Make.make | $(RELEASE_DIRECTORY)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(RELEASE_DIRECTORY)/%.o: %.cp STM32Make.make | $(RELEASE_DIRECTORY)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(RELEASE_DIRECTORY)/%.o: %.cxx STM32Make.make | $(RELEASE_DIRECTORY)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(RELEASE_DIRECTORY)/%.o: %.cpp STM32Make.make | $(RELEASE_DIRECTORY)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(RELEASE_DIRECTORY)/%.o: %.c++ STM32Make.make | $(RELEASE_DIRECTORY)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(RELEASE_DIRECTORY)/%.o: %.C STM32Make.make | $(RELEASE_DIRECTORY)
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(RELEASE_DIRECTORY)/%.o: %.CPP STM32Make.make | $(RELEASE_DIRECTORY)
	$(CXX) -c $(CXXFLAGS) $< -o $@

#Assembly build
$(RELEASE_DIRECTORY)/%.o: %.s STM32Make.make | $(RELEASE_DIRECTORY)
	$(AS) -c $(ASFLAGS) $< -o $@

$(RELEASE_DIRECTORY)/%.o: %.S STM32Make.make | $(RELEASE_DIRECTORY)
	$(AS) -c $(ASFLAGS) $< -o $@

$(RELEASE_DIRECTORY)/%.o: %.sx STM32Make.make | $(RELEASE_DIRECTORY)
	$(AS) -c $(ASFLAGS) $< -o $@

$(RELEASE_DIRECTORY)/$(TARGET).elf: $(OBJECTS) STM32Make.make | $(RELEASE_DIRECTORY)
	@echo $(OBJECTS) > $@.in
	$(CC) @$@.in $(LDFLAGS) -o $@
	$(SZ) $@

$(RELEASE_DIRECTORY)/%.hex: $(RELEASE_DIRECTORY)/%.elf | $(RELEASE_DIRECTORY)
	$(HEX) $< $@

$(RELEASE_DIRECTORY)/%.bin: $(RELEASE_DIRECTORY)/%.elf | $(RELEASE_DIRECTORY)
	$(BIN) $< $@

$(RELEASE_DIRECTORY)/%.lss: $(RELEASE_DIRECTORY)/%.elf | $(RELEASE_DIRECTORY)
	$(LSS) $< > $@

$(RELEASE_DIRECTORY):
	$(call mkdir_function, $@)

$(BUILD_DIRECTORY): | $(RELEASE_DIRECTORY)
	$(call mkdir_function, $@)


#######################################
# flash
#######################################
flash: all
	"$(OPENOCD)" -f ./openocd.cfg -c "program $(RELEASE_DIRECTORY)/$(TARGET).elf verify reset exit"

#######################################
# erase
#######################################
erase: all
	"$(OPENOCD)" -f ./openocd.cfg -c "init; reset halt; stm32h7x mass_erase 0; exit"

#######################################
# clean up
#######################################
clean:
	$(REMOVE_DIRECTORY_COMMAND) $(BUILD_DIRECTORY)

#######################################
# custom makefile rules
#######################################

	
#######################################
# dependencies
#######################################
-include $(wildcard $(RELEASE_DIRECTORY)/*.d)

# *** EOF ***