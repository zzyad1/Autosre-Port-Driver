 /******************************************************************************
 *
 * Module:
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Zyad Mostafa
 ******************************************************************************/

#ifndef PORT_H
#define PORT_H

/* Id for the company in the AUTOSAR
 * for example Zyad's ID = 1000 :) */
#define PORT_VENDOR_ID    (1000U)

/* Port Module Id */
#define PORT_MODULE_ID    (124U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)
/*
* Macros for Port Status
*/
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)


/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* Port Pre-Compile Configuration Header file */
#include "Port_Cfg.h"

/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/

/* service ID for PORT init */
#define PORT_INIT_SID                   (uint8)0x00

/* service ID for PORT set pin direction */
#define PORT_SET_PIN_DIRECTION_SID          (uint8)0x01

/* service ID for PORT refresh pin direction */
#define PORT_REFRESH_PORT_DIRECTION_SID      (uint8)0x02

/* service ID for PORT get version info */
#define PORT_GET_VERSION_INFO_SID           (uint8)0x03

/* service ID for PORT set pin mode */
#define PORT_SET_PIN_MODE_SID               (uint8)0x04

/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Port Pin ID requested */
#define PORT_E_PARAM_PIN (uint8)0x0A

/* DET code to report Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE (uint8)0x0B

/* DET code to report Invalid Port Init */
#define PORT_E_PARAM_CONFIG (uint8)0x0C

/* DET code to report when mode is unchangeable */
#define PORT_E_PARAM_INVALID_MODE (uint8)0x0D

/* DET code to report when mode is unchangeable */
#define PORT_E_MODE_UNCHANGEABLE  (uint8)0x0E

/* DET called without module initialization  */
#define PORT_E_UNINIT (uint8)0x0F

/* DET APIs called with a Null Pointer */
#define PORT_E_PARAM_POINTER (uint8)0x10
/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
/* Data type for the symbolic name of a port pin */
typedef uint8 Port_PinType;

/* Different port pin modes */
typedef uint8 Port_PinModeType;

/* Possible directions of a port pin*/
typedef enum{
  /*Sets port pin as input*/
  PORT_PIN_IN,         
  /* Sets port pin as Output*/
  PORT_PIN_OUT,         
}Port_PinDirectionType;

/* Description: Enum to hold internal resistor type for PIN */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;

/* Port pin mode from mode list for use with Port_Init() function */

typedef struct
{ 
  /*port number from 0 -> 5*/
  uint8 port_num; 
  /*pin number from 0 -> 7 as max and 0 -> 4 as min */
  uint8 pin_num; 
  /* the direction of pin IF INPUT or OUTPUT */
  Port_PinDirectionType  Direction; 
  /* the internal resistor IF Disable, Pull up or Pull down */
  Port_InternalResistor  Resistor;               
  /* the initial value in output case IF high or low */
  uint8 Initial_value;                  
  /* pin's Mode */
  Port_PinModeType Mode;  
  /* pin direction changeability IF ON or OFF*/
   uint8 Direction_Change;       
  /* pin mode changeability IF ON or OFF*/
   uint8 Mode_Change;            
}Port_ConfigPins;

/* structure that is required for initialization API */
typedef struct Port_ConfigType
{
  /* array of pointers to config struct to optimize memory storage */
  const Port_ConfigPins Pins[ALL_PORTS_CONFIGURED_PINS];
}Port_ConfigType;

/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/
/* function for PORT initialization API */
void Port_Init( const Port_ConfigType* ConfigPtr );

#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
/* function for setting direction API */
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction );
#endif

/* function for refreshing port direction API */
void Port_RefreshPortDirection( void );

/* function for setting pin mode API */
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode );

/* function for getting version info API */
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo );
#endif
/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H */
