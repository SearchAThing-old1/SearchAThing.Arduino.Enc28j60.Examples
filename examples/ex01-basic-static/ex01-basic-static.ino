/*
* The MIT License(MIT)
* Copyright(c) 2016 Lorenzo Delana, https://searchathing.com
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/

//===========================================================================
// EXAMPLE	: ex01-basic-static.ino
//===========================================================================
// Setup()
//   - Allocate the enc28j60 driver
//   - Allocate the network infrastructure manager
//   - Set static-ip-mode parameters
//   - Query a name through the dns server
// Loop()
//   - Maintenance loop routine to keep working default packet handlers
//     (arp,icmp,...)
//---------------------------------------------------------------------------
// Suggested defines for this example
//---------------------------------------------------------------------------
// SEARCHATHING_DISABLE;DEBUG;DEBUG_ASSERT;DPRINT_SERIAL
//

// SearchAThing.Arduino debug macro definitions
#include <SearchAThing.Arduino.Utils\DebugMacros.h>

//---------------------------------------------------------------------------
// Libraries
//---------------------------------------------------------------------------
#include <MemoryFree.h>
#include <SPI.h>

#include <SearchAThing.Arduino.Utils\Util.h>
using namespace SearchAThing::Arduino;

#include <SearchAThing.Arduino.Net\Checksum.h>
using namespace SearchAThing::Arduino::Net;

#include <SearchAThing.Arduino.Net\EthNet.h>
#include "Driver.h"
using namespace SearchAThing::Arduino::Enc28j60;

//---------------------------------------------------------------------------
// Global variables
//---------------------------------------------------------------------------

// network card driver
EthDriver *drv;

// network manager
EthNet *net;

//---------------------------------------------------------------------------
// Setup
//---------------------------------------------------------------------------
void setup()
{
	// init
	{
		// network card init ( mac = 00:00:6c:00:00:[01] )
		drv = new Driver(PrivateMACAddress(1));

		// network manager init [static-mode]
		net = new EthNet(drv, RamData::FromArray(IPV4_IPSIZE, 192, 168, 0, 40));

		// static network parameters
		net->SetNetmask(RamData::FromArray(IPV4_IPSIZE, 255, 255, 255, 0));
		net->SetGateway(RamData::FromArray(IPV4_IPSIZE, 192, 168, 0, 1));
		net->SetDns(RamData::FromArray(IPV4_IPSIZE, 8, 8, 8, 8));
		net->SetBroadcastAddress(RamData::FromArray(IPV4_IPSIZE, 192, 168, 0, 255));
	}

	DPrint(F("MAC\t")); DPrintHexBytesln(net->MacAddress());
	DPrint(F("IP\t")); DPrintBytesln(net->IpAddress());
	DPrintln(F("setup done"));
	DNewline();

	DPrint(F("github ip = ")); DPrintBytesln(net->ResolveIP("github.com"));

	PrintFreeMemory();
}

//---------------------------------------------------------------------------
// Loop
//---------------------------------------------------------------------------
void loop()
{
	net->Receive(); // receive packet ( if any )
	net->FlushRx(); // process packet using registered default handlers, then discard it
}
