
## <b>Tx_Thread_Creation application description</b>

This application is for controlling multiple servos and or steppers via azure_rtos

## <b>generating code from CubeMX</b>

This code is a modified version of the example Tx_Thread_Creation from the CubeMX Example selector

The code needs to be generated using the Makefile toolchain only that way the VsCode Extension
will work.


## <b>flashing from VsCode with stm32forVsCode extension</b>

openocd >0.12 with stlink support is required 

it is possible to manually flash:

    openocd -f <board_config>.cfg -c "program <binary_file>.elf verify reset exit"

which in this case would look like this:
    
    openocd -f openocd.cfg -c "program Tx_Thread_Creation.elf verify reset exit"



## <b>Add a new task</b>

Entry function in app_threadx.c executes callback to main.c where neccesary info can be passed. the tasks are defined in a separate file (tasks.c), this is because when generating code from cubeMX only the callback functions need to be redefined.

create a new thread in app_threadx.c along other defined threads with tx_thread_create(...)
and add an entry function.

create function CallThreadXX in main.h CallThreadXX

create function pointer to CallthreadXX in app_threadx.c

define callback function in main.c which calls function defined in tasks.c





