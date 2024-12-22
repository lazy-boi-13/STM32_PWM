
## <b>Tx_Thread_Creation application description</b>

This application is for controlling multiple servos and or steppers

## <b>generating code from CubeMX</b>

This code is a modified version of the example Tx_Thread_Creation from the CubeMX Example selector
everytime the .ioc file is updated the main.c file is set back to the original code.

The code needs to be generated using the Makefile toolchain only that way the VsCode Extension
will work.

The linkerScript has to be created with stm32CubeIDE toolchain, it won't work using makefile because
unfortunately there is a bug.

as long as Set heap size, stack size or stack location does not change the old linker script in the
folder old can be used where also the latest main.c file should be saved before generating code