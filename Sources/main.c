/* ###################################################################
 **     Filename    : main.c
**     Project     : prueba_KL25Z
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2014-07-08, 13:59, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!//putada que no sirve 
** @file main.c
** @version 01.01
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Bit1.h"
#include "Bit2.h"
#include "Bit3.h"
#include "TU1.h"
#include "Servo.h"
#include "PwmLdd1.h"
#include "WAIT1.h"
#include "Motors.h"
#include "PwmLdd3.h"
#include "Camera_Analog.h"
#include "AdcLdd1.h"
#include "Camera_Clock.h"
#include "BitIoLdd3.h"
#include "Camera_SI.h"
#include "BitIoLdd4.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

#define SERVO_LEFT 17800
#define SERVO_RIGHT 19400

#define PIX_MIN 0
#define PIX_MAX 128

uint8_t Camera_Bin[128], u8_Centro = 0;
uint8_t Camera_Values[128];
uint8_t Camera_Threshold;
word servoduty = 0;
uint8_t u8_PixelCntr = 0;


/* User includes (#include below this line is not maintained by Processor Expert) */
void delay(uint32_t time){
	while(time--){
		asm ("nop");
	}
}

void Read_Camera(void){
	int i;
	uint8_t u16_MaxValue = 0, u16_threshold = 0;
	uint16_t u16_suma = 0;
	
	u8_PixelCntr = 0;
	Camera_SI_SetVal();
	Camera_Clock_SetVal();
	Camera_Analog_Measure(TRUE);	
	Camera_SI_ClrVal();
	for(i=0;i<128;i++){
		Camera_Clock_ClrVal();
		Camera_Analog_GetValue8(&Camera_Values[i]);
		delay(60);
		Camera_Clock_SetVal();
		Camera_Analog_Measure(TRUE);
		
		if(Camera_Values[i] > u16_MaxValue) {
			u16_MaxValue  = (uint16_t)Camera_Values[i];
		}
	}
	Camera_Clock_ClrVal();

	u16_threshold = (u16_MaxValue/20) * 17;
	
	for(i=0;i<128;i++){
		if (Camera_Values[i] > u16_threshold){
			u16_suma += i;
			u8_PixelCntr++;
		}
		
		else {
			Camera_Values[i] = 0;
		}
	}
	
	if(u8_PixelCntr < 15)
		u8_Centro = (uint8_t)(u16_suma / u8_PixelCntr);

	return;
}


void Servo_Line (uint8_t centro){
	servoduty = (word)(SERVO_LEFT + (((SERVO_RIGHT-SERVO_LEFT)/128)*centro));
	Servo_SetDutyUS(servoduty);
	return;
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
 {
  /* Write your local variable definition here */
	int i;
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  /* Write your code here */
  
  Camera_Analog_Calibrate(TRUE);
  for(;;) {
	  	  	  
	  Read_Camera();
	  Servo_Line(u8_Centro);
	  //Servo_SetDutyUS(19500);
	  WAIT1_Waitms(5);
	  

  }

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
