
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


STM32 ADC-Multi channel Scan Coninous Mode 

In this mode the ADC only has to be triggered once to start conversion. As it's running it will keep triggering itself after completion of regular group conversion.

the ADC is started using:    HAL_ADC_Start_DMA(...) (if conversion launched by software is configured, otherwise the timer can also trigger conversion)

upon end of conversion the callback function: HAL_ADC_ConvCpltCallback(...) can be used to fetch the data

ADC Settings:

	DMA Settings:

	DMA Mode = circular
	transfer mode = peripheral to memory
	Data width = word
	increment address -> memory

	Parameter settings:
	
	External Trigger conversion source = timer 1 capture/compare event
	scan conversion mode = enabled
	continuous conversion mode = enabled
	end of conversion selection = end of sequence
	Clock Prescaler = Asynchronous clock divided by 1
	Channel sampling time = 1.5 cycles

