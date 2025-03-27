circular
## <b>Tx_Thread_Creation application description</b>

This application is for controlling multiple servos and or steppers via azure_rtos

## <b>generating code from CubeMX</b>

This code is a modified version of the example Tx_Thread_Creation from the CubeMX Example selector

The code needs to be generated using the Makefile toolchain only that way the VsCode Extension will work.


## <b>flashing from VsCode with stm32forVsCode extension</b>

openocd >0.12 with stlink support is required 

it is possible to manually flash:

    openocd -f <board_config>.cfg -c "program <binary_file>.elf verify reset exit"

which in this case would look like this:
    
    openocd -f openocd.cfg -c "program Tx_Thread_Creation.elf verify reset exit"



## <b>Add a new task</b>

Add user code in main.c only in the designated sections otherwise the costum code will be overwritten when generating code from CubeMX

/* USER CODE BEGIN ... */

insert user code here <---

/* USER CODE END ... */

Entry function in app_threadx.c executes callback to main.c where neccesary info can be passed. the tasks are defined in a separate file (tasks.c), this is because the tasks are big functions hence the main.c file would be a bit too long and confusing.

declare function CallThreadXX in main.h

create a new thread in app_threadx.c along other defined threads and priorities with tx_thread_create(...)

define function pointer to CallthreadXX  and entry fucntion with callback to main in app_threadx.c

define callback function in main.c which calls function defined in tasks.c with necessary parameters

## <b>ADC Configuration</b>

STM32 ADC-Multi channel Scan Mode 

ADC1 is sampling two channels at 10Hz.

HAL_ADC_Start(..) is called once and runs continuously in circular mode

a conversion is triggered by timer 2 trigger out event every 100ms

for the H7 series the the buffer needs to be put into a non cacheable area:

	the RAM_D3 Memory area has its base address and size as seen in the specific linker script
	here STM32H723GTx_FLASH.ld:
	
	RAM_D3  (xrw)    : ORIGIN = 0x38000000,   LENGTH = 16K

	this area is not reserved so we can with no problem reserve space for the buffer there:

	/* RAM MODIFICATION  NON-CHACHABLE AREA FOR ADC BUFFER */

	  .nocache (NOLOAD) :
  	{
		. = ALIGN(16);
		*(.nocache)
	
	} >RAM_D3

	in the .ioc file under System core-> CORTEX_M7 with i and D Cache enabled 


	![Image caption](pictures/Memory RegionSettings.png)


	then add the array to the memory area we reserved and the keyword volatile to avoid compiler optimizations:

	volatile uint16_t ADC_BUF[2] __attribute__((section(".nocache")));
	

	## <b>Working with git</b>

	discard all changes and reset the current branch history

		git fetch origin

		git reset --hard origin/main
	
	push current version of project, ignore merge conflicts

		git push --force origin


