#include "DTrigger16bitV2_osm.h"

/*
  Copyright (c) 2019 Abdulhamidov Osim
  All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/


/*	    ______________________________________________________________________________________	    __
	___|			a (uint) - 16 x IN signals or commandVars 			  |________|  |________
            __												    __
	___|  |______________	FwdFrntT.Qff  16 x Outs - Calculating process is logical parallel _________|  |________
*/
unsigned int ForwardFrontTrigger16bit(unsigned int a, unsigned int tBITx, ForwardFrontsInternalRegs16bit_t *FFRegs)
{
	/*if (a == 1)
	{
		a = 0xFFFF;
	}*/
	a = a & tBITx;
	(*FFRegs).inputSignal = ((*FFRegs).inputSignal & ~tBITx) | a;
	(*FFRegs).Qff = (*FFRegs).inputSignal & (~(*FFRegs).FfrontRegister);
	(*FFRegs).FfrontRegister = (*FFRegs).inputSignal;
	return (*FFRegs).Qff;        //FFRegs->Qff;
}

//EXAMPLE of using
/*
#include <xc.h>
#include "DTrigger16bitV2_osm.h"

#define ON 1
#define OFF 0
#define DI_RUNMotor TRISBbits.7
#define DI_RUNPump TRISBbits.5
#define DI_STOPMotor TRISBbits.6
#define DQ_RUNMotor TRISAbits.0
#define DQ_RUNPummp TRISAbits.1

ForwardFrontsInternalRegs_t ForwardFTriggerReg;

int main(void)
{
    INIT();	
	while (true)
	{
		if (ForwardFrontTriggerbyte(DI_RUNMotor, tBIT0, &ForwardFTriggerReg) == tBIT0)
		{
			DQ_RUNMotor = ON;
		}
		if (ForwardFrontTriggerbyte(DI_STOPMotor, tBIT1, &ForwardFTriggerReg) == tBIT1)
		{
		    DQ_RUNMotor = OFF;
		}
	}
}//*/

//Ïðèìåð ïàðàëëåëüíîãî ñ÷èòûâàíèÿ è îïðåäåëåíèÿ ïðèñóòñòâèÿ ïåðåäíåãî ôðîíòà 3 ñëó÷àéíûõ âõîäíûõ ñèãíàëîâ:
//DI_RUNMotor - tBIT0
//DI_RUNPummp - tBIT5
//ReadTSensorOnce - tBIT4
//DI_STOPMotor - tBIT1
//Çàðèñîâàòü îäèí ðàç Ìåíþ íàñòðîåê â LCD - tBIT15
/*
ForwardFrontsInternalRegs16bit_t ForwardFTriggerReg;
int main(void)
{
	INIT();
	while (true)
	{
		ForwardFrontTrigger16bit((DI_RUNMotor * tBIT0) | (DI_RUNPump * tBIT5) | (ReadTSensorOnce * tBIT4) | (1 * tBIT15), tBIT0 | tBIT5 | tBIT4 | tBIT15, &ForwardFTriggerReg);
		if((ForwardFTriggerReg.Qff & tBIT0) == tBIT0)
		{
			DQ_RUNMotor = ON;
		}
		if((ForwardFTriggerReg.Qff & tBIT5) == tBIT5)
		{
			DI_RUNPump = ON;
		}
		if((ForwardFTriggerReg.Qff & tBIT4) == tBIT4)
		{
			ds18b20ReadTemp(ds18b20_1wireUnit, Tsensor2Address, data);
		}
		if((ForwardFTriggerReg.Qff & tBIT15) == tBIT15)
		{
			DrawSettinsMenuByLCD();
		}
	}
}
*/

/*	    __	      _____	      _		 ______
	___|  |______|	   |_________| |________|      |___	a (uint) - 16 x IN signals or commandVars
	    _________		      __________
	___|	     |_______________|		|__________	DTrig.Q_Dtrig  16 x Outs - Calculating process is logical parallel
*/
unsigned int D_Trigger16bit(unsigned int a, unsigned int tBITx, D_TriggersInternalRegs16bit_t *DTrigRegs)
{
	a = a & tBITx;
	(*DTrigRegs).inputSignal = ((*DTrigRegs).inputSignal & (~tBITx)) | a;
	(*DTrigRegs).Q_Dtrig = ((*DTrigRegs).inputSignal | (*DTrigRegs).Q_Dtrig) & (~(*DTrigRegs).DtrigRegK2);
	(*DTrigRegs).DtrigRegK1 = (*DTrigRegs).Q_Dtrig & (~(*DTrigRegs).inputSignal | (*DTrigRegs).DtrigRegK1);
	(*DTrigRegs).DtrigRegK2 = ((*DTrigRegs).DtrigRegK1 | (*DTrigRegs).DtrigRegK2) & (*DTrigRegs).inputSignal;
	return (*DTrigRegs).Q_Dtrig;
}

/*	_______
	       |_____________	a (uint) - 16 x IN signals or commandVars
	        __
	_______|  |__________	BackFrntT.Qbf

	   --->|--|<--- Tbf = Tff - 1 cycle fully executed program
*/
unsigned int BackFrontTrigger16bit(unsigned int a, unsigned int tBITx, BackFrontsInternalRegs16bit_t *BFRegs)
{
	a = a & tBITx;
	(*BFRegs).inputSignal = ((*BFRegs).inputSignal & (~tBITx)) | a;
	(*BFRegs).BfrontRegister = ((*BFRegs).inputSignal | (*BFRegs).BfrontRegister) & (~(*BFRegs).Qbf);
	(*BFRegs).Qbf = ~(*BFRegs).inputSignal & (*BFRegs).BfrontRegister;
	return (*BFRegs).Qbf;
}


unsigned char delayedPress16bit(unsigned int *countReg, unsigned char inputBit, unsigned char k)  //int have not sense
{
	if (inputBit != 0) { *countReg = *countReg + 1; }
	else { *countReg = 0; }
	if (*countReg == k)
	{
		*countReg = 0;
		return 1;
	}
	return 0;
}

/*
Example: ForwardFrontsInternalRegs16bit_t FwdFrTrig_MOTORs_Control;
         ForwardFrontsInternalRegs16bit_t FwdFrTrig_FANs_Control;
         int main(){...
	    ...
            Reset_ForwardFrontTrigger16bit(tBIT15 | tBIT3, &FwdFrTrig_MOTORs_Control);
	    Reset_ForwardFrontTrigger16bit(0xFFFF, &FwdFrTrig_FANs_Control);    //Reset all signals registers; or:
	    Reset_ForwardFrontTrigger16bit(tALLBITS, &FwdFrTrig_FANs_Control);    //Reset all signals registers;	    
Status: Not tested yet
*/
void Reset_ForwardFrontTrigger16bit(unsigned int tBITx, ForwardFrontsInternalRegs16bit_t *FFRegs)
{
	(*FFRegs).inputSignal &= ~tBITx;      
	(*FFRegs).FfrontRegister &= ~tBITx;
	(*FFRegs).Qff &= ~tBITx;
	return;        
}

/*Status: Not tested yet*/
void Reset_D_Trigger16bit(unsigned int tBITx, D_TriggersInternalRegs16bit_t  *DTrigRegs)
{
	(*DTrigRegs).inputSignal &= ~tBITx;
	(*DTrigRegs).DtrigRegK1 &= ~tBITx;
	(*DTrigRegs).DtrigRegK2 &= ~tBITx;
	(*DTrigRegs).Q_Dtrig &= ~tBITx;
	return;
}
