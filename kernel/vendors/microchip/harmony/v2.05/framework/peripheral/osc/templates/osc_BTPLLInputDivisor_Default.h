/*******************************************************************************
  OSC Peripheral Library Template Implementation

  File Name:
    osc_BTPLLInputDivisor_Default.h

  Summary:
    OSC PLIB Template Implementation

  Description:
    This header file contains template implementations
    For Feature : BTPLLInputDivisor
    and its Variant : Default
    For following APIs :
        PLIB_OSC_ExistsBTPLLInputDivisor
        PLIB_OSC_BTPLLInputDivisorSet
        PLIB_OSC_BTPLLInputDivisorGet

*******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
*******************************************************************************/

//DOM-IGNORE-END

#ifndef _OSC_BTPLLINPUTDIVISOR_DEFAULT_H
#define _OSC_BTPLLINPUTDIVISOR_DEFAULT_H

//******************************************************************************
/* Function :  OSC_ExistsBTPLLInputDivisor_Default

  Summary:
    Implements Default variant of PLIB_OSC_ExistsBTPLLInputDivisor

  Description:
    This template implements the Default variant of the PLIB_OSC_ExistsBTPLLInputDivisor function.
*/

#define PLIB_OSC_ExistsBTPLLInputDivisor PLIB_OSC_ExistsBTPLLInputDivisor
PLIB_TEMPLATE bool OSC_ExistsBTPLLInputDivisor_Default( OSC_MODULE_ID index )
{
    return true;
}


//******************************************************************************
/* Function :  OSC_BTPLLInputDivisorSet_Default

  Summary:
    Implements Default variant of PLIB_OSC_BTPLLInputDivisorSet 

  Description:
    This template implements the Default variant of the PLIB_OSC_BTPLLInputDivisorSet function.
*/

PLIB_TEMPLATE void OSC_BTPLLInputDivisorSet_Default( OSC_MODULE_ID index , uint16_t PLLInDiv )
{
    BTPLLCONbits.BTPLLIDIV = PLLInDiv;
    Nop();
    Nop();
}


//******************************************************************************
/* Function :  OSC_BTPLLInputDivisorGet_Default

  Summary:
    Implements Default variant of PLIB_OSC_BTPLLInputDivisorGet 

  Description:
    This template implements the Default variant of the PLIB_OSC_BTPLLInputDivisorGet function.
*/

PLIB_TEMPLATE uint16_t OSC_BTPLLInputDivisorGet_Default( OSC_MODULE_ID index )
{
    return (uint16_t)BTPLLCONbits.BTPLLIDIV;
}


#endif /*_OSC_BTPLLINPUTDIVISOR_DEFAULT_H*/

/******************************************************************************
 End of File
*/

