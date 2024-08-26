/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Zyad Mostafa
 ******************************************************************************/
#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Dio Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC const Port_ConfigPins * PortConf = NULL_PTR;
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;
/************************************************************************************
* Service Name: Port_Init
* Service ID[hex]: 0x00
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Initializes the Port Driver module.
************************************************************************************/
void Port_Init( const Port_ConfigType* ConfigPtr )
{
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        if (ConfigPtr == NULL_PTR)
            {
              Det_ReportError(PORT_MODULE_ID,PORT_INSTANCE_ID, PORT_INIT_SID,PORT_E_PARAM_CONFIG);
        }
        else
    #endif
        {
           PortConf = ConfigPtr->Pins;     /* address of the first pins structure --> Channels[0] */
         // Port_Status = PORT_INITIALIZED;
          
          volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
          volatile uint32 delay = 0;
          
          uint8 Pin_Index = 0;              
          while (Pin_Index < ALL_PORTS_CONFIGURED_PINS)
          {     
            Pin_Index++;
            switch(PortConf[Pin_Index].port_num)
            {
                case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                         break;
                case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                         break;
                case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                         break;
                case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                         break;
                case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                         break;
                case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                         break;
            }
            /* Enable clock for PORT and allow time for clock to start*/
//            SYSCTL_RCGC2_REG |= (1<<PortConf[Pin_Index].port_num);
//            delay = SYSCTL_RCGC2_REG;

            if( ((PortConf[Pin_Index].port_num == 3) && (PortConf[Pin_Index].pin_num == 7)) || ((PortConf[Pin_Index].port_num == 5) && (PortConf[Pin_Index].pin_num == 0)) ) /* PD7 or PF0 */
            {
                *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;                     /* Unlock the GPIOCR register */   
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET) , PortConf[Pin_Index].pin_num);  /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            }
            else if( (PortConf[Pin_Index].port_num == 2) && (PortConf[Pin_Index].pin_num <= 3) ) /* PC0 to PC3 */
            {
                /* Do Nothing ...  this is the JTAG pins */
                return;
            }
            else
            {
                /* Do Nothing ... No need to unlock the commit register for this pin */
            }
            /**************************************************************************************************************************************************************************/
            /*For Setup :-
            * - Deriction Reg (Output or Input)
            * - If Inpute (Pull up or Pull Down)
            * - If Output Intial Valu setup
            */
            if(PortConf[Pin_Index].Direction == PORT_PIN_IN)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
              
              if(PortConf[Pin_Index].Resistor == PULL_UP)
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConf[Pin_Index].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }
              else if
              (PortConf[Pin_Index].Resistor == PULL_DOWN)
              {
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConf[Pin_Index].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              }
              else
              {
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET) , PortConf[Pin_Index].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET) , PortConf[Pin_Index].pin_num);       /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
              
              }
            }
            else if(PortConf[Pin_Index].Direction == PORT_PIN_OUT)
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);                /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
        
                if(PortConf[Pin_Index].Initial_value == STD_HIGH)
                {
                    SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , PortConf[Pin_Index].pin_num);          /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                }
                else
                {
                    CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                }
            }
            else 
            {
              /*Do Nothing*/
            }
            /**************************************************************************************************************************************************************************/
            /*For Setup :-
            * - Alternative Reg (0 For (DIO and ADC) And 1 For (CAN , UART ,....))
            * - Digital  (0 for ADC and 1 (CAN , UART ,....))
            * - Analog (1 for ADC and 0 (CAN , UART ,....))
            * - Control Reg (0 For DIO (And ADC) And Reg Number For all Alternatives )
            */
            if(PortConf[Pin_Index].Mode == PORT_PIN_MODE_DIO)
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PortConf[Pin_Index].pin_num * 4));     /* Clear the PMCx bits for this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */          
            }
            else if(PortConf[Pin_Index].Mode == PORT_PIN_MODE_ADC)
            {
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PortConf[Pin_Index].pin_num * 4));     /* Clear the PMCx bits for this pin */
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */          
           
            }
            else /****for another *****/
            {
              CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin_Index].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin_Index].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
              *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (PortConf[Pin_Index].Mode & (0x0000000F << (PortConf[Pin_Index].pin_num * 4)));     /* Clear the PMCx bits for this pin */
              SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin_Index].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */          
            
            }
          }
          Port_Status = PORT_INITIALIZED;
        }
}
/************************************************************************************
* Service Name: Port_SetPinDirection
* Service ID[hex]: 0x01 
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin , Direction , Port Pin ID number , Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Sets the port pin direction.
************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction )
{
  boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
        if(PortConf[Pin].Direction_Change == STD_OFF)
        {
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,PORT_E_DIRECTION_UNCHANGEABLE);
          error = TRUE;
        }
        else 
        {
        }
	if (Port_Status == PORT_NOT_INITIALIZED)
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,PORT_E_UNINIT);
          error = TRUE;
	}
	else
	{
	}
	if (Pin >= ALL_PORTS_CONFIGURED_PINS )
	{
          Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,PORT_E_PARAM_PIN);
          error = TRUE;
	} 
        else
	{
	}
        
#endif

        /* In-case there are no errors */
	if(FALSE == error)
        {
	  volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
          volatile uint32 delay = 0;
          switch(PortConf[Pin].port_num)
            {
                case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
                         break;
                case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
                         break;
                case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
                         break;
                case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
                         break;
                case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
                         break;
                case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
                         break;
            }
	if(Direction == PORT_PIN_IN)
        {
	        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin].pin_num);
        }
	else if(Direction == PORT_PIN_OUT)
        {
	        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin].pin_num);
        }
        else
        {
        }
        
}
}
#endif

/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Service ID[hex]: 0x02 
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Refreshes port direction.
************************************************************************************/
void Port_RefreshPortDirection( void )
{
  #if (PORT_DEV_ERROR_DETECT == STD_ON)
 if (Port_Status == PORT_NOT_INITIALIZED)
 {
   Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,PORT_E_UNINIT);
 }
  else
#endif
  {
   uint8 Pin_Index = 0;
   while (Pin_Index < ALL_PORTS_CONFIGURED_PINS)
   {
     if(PortConf[Pin_Index].Direction == STD_OFF)     /*Check that if POrt pin is UNchangable in RunTime*/ 
     {
       Pin_Index++;
        volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
       volatile uint32 delay = 0;
       switch(PortConf[Pin_Index].port_num)
       {
          case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
          break;
          case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
          break;
          case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
          break;
          case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
          break;
          case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
          break;
          case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
          break;
       }
       if(PortConf[Pin_Index].Direction == PORT_PIN_IN)
       {
         CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);
       }
       else if(PortConf[Pin_Index].Direction == PORT_PIN_OUT)
       {
         SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET) , PortConf[Pin_Index].pin_num);
       }
       else
       {
       }
     }
     else if(PortConf[Pin_Index].Direction== STD_ON)
     {
       continue;
     }
    
    }
  }
}

/************************************************************************************
* Service Name: Port_SetPinMode
* Service ID[hex]: 0x04
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin , Mode , Port pin ID number , New Port Pin mode to be set on port pin
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initialize the Dio module.
************************************************************************************/
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode )
{
  boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  if(Port_Status == PORT_NOT_INITIALIZED)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
    error = TRUE;
  }
  else
  {
  }
  if (Pin >= ALL_PORTS_CONFIGURED_PINS )
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID,PORT_E_PARAM_PIN);
  }
  else
  {
  }
  if (Mode > PORT_PIN_MODE_CORE)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID , PORT_E_PARAM_INVALID_MODE);
  } 
  else
  {
  }
  if(PortConf[Pin].Mode_Change == STD_OFF)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
    error = TRUE;    
  }
  else
  {
  }
#endif
  if (error == FALSE)
  {
    volatile uint32 * PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */
    volatile uint32 delay = 0;
    switch(PortConf[Pin].port_num)
    {
    case  0: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
    break;
    case  1: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
    break;
    case  2: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
    break;
    case  3: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
    break;
    case  4: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
    break;
    case  5: PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
    break;
    }
          
    if(Mode == PORT_PIN_MODE_DIO )
    {
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
      *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PortConf[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */          
            
    }
    else if (Mode == PORT_PIN_MODE_ADC)
    {
      SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
      *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (PortConf[Pin].pin_num * 4));     /* Clear the PMCx bits for this pin */
      CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */          
     }
     else /****for another *****/
     {
       CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET) , PortConf[Pin].pin_num);      /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET) , PortConf[Pin].pin_num);             /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
       *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (Mode & (0x0000000F << (PortConf[Pin].pin_num * 4)));     /* Clear the PMCx bits for this pin */
       SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET) , PortConf[Pin].pin_num);        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */          
     }
  }
  

}

/************************************************************************************
* Service Name: Port_GetVersionInfo 
* Service ID[hex]: 0x03
* Sync/Async: Synchronous
* Reentrancy: Non reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): versioninfo , Pointer to where to store the version information of this module.
* Return value: None
* Description: Returns the version information of this module.
************************************************************************************/
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo( Std_VersionInfoType* versioninfo )
{
  boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
  if(NULL_PTR == versioninfo)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
    error = TRUE;
  }
  else
  {
  }
  if(Port_Status == PORT_NOT_INITIALIZED)
  {
    Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
    error = TRUE;
  }
  else
  {
  }
#endif
  if(error == FALSE)
  {
    versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
    versioninfo->moduleID = (uint16)PORT_MODULE_ID;
    versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
    versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
    versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
  }
}
#endif
