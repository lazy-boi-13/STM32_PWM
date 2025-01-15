#include <stdio.h>  // definition of printf

#include "tasks.h"
#include "stdbool.h"
#include "main.h" // redefined printf
#include "app_threadx.h"

uint8_t addresses[1] = {RS485_ENC0};  // data to send with readposition command
uint8_t DataR[2] = {0,0}; // array to catch encoder response 

void App_Delay(uint32_t Delay);
bool verifyChecksumRS485(uint16_t message);
void setStateRS485(uint8_t state);


void MainThread(UART_HandleTypeDef *huart)
{

  while (1)
  {
    for(int encoder = 0; encoder < sizeof(addresses); ++encoder)
    {

      setStateRS485(RS485_T_TX); //put the transciver into transmit mode

      HAL_UART_Transmit(huart, &addresses[encoder], 1, 100);    //send the command to get position. All we have to do is send the node address


      // after the readposition command has been sent it takes the encoder 11uS to respond with 
      // 20 bits of data, at a rate of 115200 bits/s that takes 185 uS to get position after read
      // command has been sent 
      
      setStateRS485(RS485_T_RX); //set the transceiver back into receive mode for the encoder response

      HAL_UART_Receive(huart, DataR, 2,100);
      

      uint16_t currentPosition = DataR[0]; // low byte comes first

      currentPosition |= DataR[1] << 8; // high byte next, OR it into our 16 bit holder


      
      if (verifyChecksumRS485(currentPosition))
      {
        //we got back a good position, so just mask away the checkbits
        currentPosition &= 0x3FFF;

        //If the resolution is 12-bits, then shift position
        if (RESOLUTION == 12)
        {
          currentPosition = currentPosition >> 2;
        }
          printf("current Position: %d\n", currentPosition);  
      }
      else
      {
        printf("%s\n", "error: Invalid checksum.");
      }


      HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin); // indicates the code is running

      App_Delay(100);

    } // end of for
  } // end of while

}


void ThreadOne(void)
{
  /* Infinite loop */
  while(1)
  {
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    /* Delay for 500ms (App_Delay is used to avoid context change). */
    App_Delay(50);
  }
}

void ThreadTwo(void)
{
    /* Infinite loop */
  while (1)
  {
    HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
    /* Delay for 300ms */
    App_Delay(30);
    
  }
}


bool verifyChecksumRS485(uint16_t message)
{
  //using the equation on the datasheet we can calculate the checksums and then make sure they match what the encoder sent.
  //checksum is invert of XOR of bits, so start with 0b11, so things end up inverted
  uint16_t checksum = 0x3;
  for(int i = 0; i < 14; i += 2)
  {
    checksum ^= (message >> i) & 0x3;
  }
  return checksum == (message >> 14);
}

/*
 * This function sets the state of the RS485 transceiver. We send it that state we want. Recall above I mentioned how we need to do this as quickly
 * as possible. To be fast, we are not using the digitalWrite functions but instead will access the avr io directly. I have shown the direct access
 * method and left commented the digitalWrite method.
 */
void setStateRS485(uint8_t state)
{
  //switch case to find the mode we want
  switch (state)
  {
    case RS485_T_TX:
      /*     
      Transmitting: 
      set ~RE to HIGH -> sets RXD to High Impedance
      set DE to HIGH -> sets TXD to either High or low and thus Y and or Z to high or low     
      */ 
      HAL_GPIO_WritePin(Recieve_Enable_GPIO_Port, Recieve_Enable_Pin, GPIO_PIN_SET);
      HAL_GPIO_WritePin(Drive_Enable_GPIO_Port, Drive_Enable_Pin, GPIO_PIN_SET);
      break;
    case RS485_T_RX:
      /*
      Recieving: 
      set ~RE to LOW -> sets RXD to either HIGH low or Don't care depending on A-B Voltage
      set DE to LOW -> sets TXD to Don't care and Y and Z to high Impedance
      */ 
      HAL_GPIO_WritePin(Recieve_Enable_GPIO_Port, Recieve_Enable_Pin, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(Drive_Enable_GPIO_Port, Drive_Enable_Pin,GPIO_PIN_RESET);
      break;

  }

}


/**
  * @brief  Application Delay function.
  * @param  Delay : number of ticks to wait
  * @retval None
  */
void App_Delay(uint32_t Delay)
{
  UINT initial_time = tx_time_get();
  while ((tx_time_get() - initial_time) < Delay);
}
