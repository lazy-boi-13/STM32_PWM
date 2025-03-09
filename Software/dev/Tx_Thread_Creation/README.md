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

STM32 ADC-Multi channel Scan Continuos Mode 

ADC is triggered by a thread with the function HAL_ADC_Start_DMA(...)

Upon end of conversion the callback function: HAL_ADC_ConvCpltCallback(...) is used to copy the data in to the buffer



	These are the most important ADC Settings configurable in .ioc file:

		ADC Settings:

		Scan Conversion mode = enabled 
		Conversion data managment mode = DMA circular mode
		External Trigger conversion source = timer 1 capture Compare 1 event
		scan conversion mode = enabled
		continuous conversion mode = disabled
		end of conversion selection = end of sequence

		Timer Settings:

		Master Slave mode is not neccessary
		Clock Source = internal clock // internal clock @ 72 MHz
		prescaler = 7200
		Counter Period = 10000 ---> period is to be 1 Hz

		PWM Generation CHannel 1:
		pwm Mode 1 Pulse 5000 // meaning 50 % Duty Cycle at 1 HZ

		DMA Settings:

		transfer mode = peripheral to memory
		DMA Mode = circular	// DMA will keep triggering itself after the first transfer
			
									 


