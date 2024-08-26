/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Configration for Port Driver.
 *
 * Author: Zyad Mostafa
 ******************************************************************************/
#ifndef PORT_CFG_H
#define PORT_CFG_H

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)

/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_ON)

/* pre-compile option for PORT SET PIN DIRECTION API */
#define PORT_SET_PIN_DIRECTION_API            (STD_ON)

/* Number of the Pins To be configure*/
#define ALL_PORTS_CONFIGURED_PINS              (43U)
   
/*Port Index*/  
#define PORT_A     (0U)
#define PORT_B     (1U)
#define PORT_C     (2U)
#define PORT_D     (3U)
#define PORT_E     (4U)
#define PORT_F     (5U) 

/*Pin Index*/    
#define PORT_PIN0 				(uint8)0
#define PORT_PIN1 				(uint8)1
#define PORT_PIN2 				(uint8)2
#define PORT_PIN3 				(uint8)3
#define PORT_PIN4 				(uint8)4
#define PORT_PIN5 				(uint8)5
#define PORT_PIN6 				(uint8)6
#define PORT_PIN7 				(uint8)7 
   
/*Port Pin IDs*/
#define PORT_PIN_MODE_ADC		(uint8)0
#define PORT_PIN_MODE_DIO		(uint8)1
#define PORT_PIN_MODE_UART		(uint8)2
#define PORT_PIN_MODE_USB		(uint8)3
#define PORT_PIN_MODE_I2C		(uint8)4
#define PORT_PIN_MODE_CAN		(uint8)5
#define PORT_PIN_MODE_PWM		(uint8)6
#define PORT_PIN_MODE_SSI		(uint8)7
#define PORT_PIN_MODE_QEI		(uint8)8
#define PORT_PIN_MODE_GPT		(uint8)9
#define PORT_PIN_MODE_NMI		(uint8)10
#define PORT_PIN_MODE_ANALOG_COMP	(uint8)11
#define PORT_PIN_MODE_CORE		(uint8)12

#endif /* DIO_CFG_H */
