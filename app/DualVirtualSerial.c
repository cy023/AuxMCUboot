/*
            LUFA Library
    Copyright (C) Dean Camera, 2021.

dean [at] fourwalledcubicle [dot] com
        www.lufa-lib.org
*/

/*
  Copyright 2021  Dean Camera (dean [at] fourwalledcubicle [dot] com)
  
  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.
  
  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

/** \file
 *
 *  Main source file for the DualVirtualSerial demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "DualVirtualSerial.h"
#include "driver/commuch.h"
#include "driver/auxmcu_io.h"
#include "driver/interrupt.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the first CDC interface,
 *  which sends strings to the host for each joystick movement.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial1_CDC_Interface =
    {
        .Config =
            {
                .ControlInterfaceNumber   = INTERFACE_ID_CDC1_CCI,
                .DataINEndpoint           =
                    {
                        .Address          = CDC1_TX_EPADDR,
                        .Size             = CDC_TXRX_EPSIZE,
                        .Banks            = 1,
                    },
                .DataOUTEndpoint =
                    {
                        .Address          = CDC1_RX_EPADDR,
                        .Size             = CDC_TXRX_EPSIZE,
                        .Banks            = 1,
                    },
                .NotificationEndpoint =
                    {
                        .Address          = CDC1_NOTIFICATION_EPADDR,
                        .Size             = CDC_NOTIFICATION_EPSIZE,
                        .Banks            = 1,
                    },
            },
    };

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another. This is for the second CDC interface,
 *  which echos back all received data from the host.
 */
USB_ClassInfo_CDC_Device_t VirtualSerial2_CDC_Interface =
    {
        .Config =
            {
                .ControlInterfaceNumber   = INTERFACE_ID_CDC2_CCI,
                .DataINEndpoint           =
                    {
                        .Address          = CDC2_TX_EPADDR,
                        .Size             = CDC_TXRX_EPSIZE,
                        .Banks            = 1,
                    },
                .DataOUTEndpoint =
                    {
                        .Address          = CDC2_RX_EPADDR,
                        .Size             = CDC_TXRX_EPSIZE,
                        .Banks            = 1,
                    },
                .NotificationEndpoint =
                    {
                        .Address          = CDC2_NOTIFICATION_EPADDR,
                        .Size             = CDC_NOTIFICATION_EPSIZE,
                        .Banks            = 1,
                    },

            },
    };

/** Event handler for the library USB Connection event. */
void EVENT_USB_Device_Connect(void)
{
    com_channel_init();
    uart_int_init();
}

/** Event handler for the library USB Disconnection event. */
void EVENT_USB_Device_Disconnect(void)
{
    com_channel_deinit();
    uart_int_deinit();
}

/** Event handler for the library USB Configuration Changed event. */
void EVENT_USB_Device_ConfigurationChanged(void)
{
    bool ConfigSuccess = true;

    ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial1_CDC_Interface);
    ConfigSuccess &= CDC_Device_ConfigureEndpoints(&VirtualSerial2_CDC_Interface);

    // LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}

/** Event handler for the library USB Control Request reception event. */
void EVENT_USB_Device_ControlRequest(void)
{
    CDC_Device_ProcessControlRequest(&VirtualSerial1_CDC_Interface);
    CDC_Device_ProcessControlRequest(&VirtualSerial2_CDC_Interface);
}

/** CDC class driver callback function the processing of changes to the virtual
 *  control lines sent from the host..
 *
 *  \param[in] CDCInterfaceInfo  Pointer to the CDC class interface configuration structure being referenced
 */
void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t *const CDCInterfaceInfo)
{
    /* You can get changes to the virtual CDC lines in this callback; a common
    use-case is to use the Data Terminal Ready (DTR) flag to enable and
    disable CDC communications in your application when set to avoid the
    application blocking while waiting for a host to become ready and read
    in the pending data from the USB endpoints.
    */
    bool HostReady = (CDCInterfaceInfo->State.ControlLineStates.HostToDevice & CDC_CONTROL_LINE_OUT_DTR) != 0;

    (void)HostReady;

    // FIXME: 
    if (CDCInterfaceInfo->State.ControlLineStates.HostToDevice == 3)
    {
        uart_int_init();
    }
    else if (CDCInterfaceInfo->State.ControlLineStates.HostToDevice == 2)
    {
        uart_int_deinit();
    }

    if (CDCInterfaceInfo == &VirtualSerial1_CDC_Interface)
    {
        // CDC interface 1's host is ready to send/receive data if HostReady is true
    }
    else
    {
        // CDC interface 2's host is ready to send/receive data if HostReady is true
    }
}
