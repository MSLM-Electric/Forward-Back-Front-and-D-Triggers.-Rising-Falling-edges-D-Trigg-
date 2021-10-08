//#pragma once
#ifndef DTRIGGER16BITV2_OSM_h
#define DTRIGGER16BITV2_OSM_h

#if DTRIGGERV2_OSM_h
#include "DTriggerV2_osm.h"  //8bit implementation
#endif


/******************defines********************/
#define	tBIT15  0x8000
#define	tBIT14  0x4000
#define	tBIT13  0x2000
#define	tBIT12  0x1000
#define	tBIT11  0x0800
#define	tBIT10  0x0400
#define	tBIT9  0x0200
#define	tBIT8  0x0100

#ifndef DTRIGGERV2_OSM_h
#define	tBIT7  0x0080
#define	tBIT6  0x0040
#define	tBIT5  0x0020
#define	tBIT4  0x0010
#define	tBIT3  0x0008
#define	tBIT2  0x0004
#define	tBIT1  0x0002
#define	tBIT0  0x0001
#endif
/**********************************************/

//typedef enum
//{
//	tBIT15 = 0x8000,
//	tBIT14 = 0x4000,
//	tBIT13 = 0x2000,
//	tBIT12 = 0x1000,
//	tBIT11 = 0x0800,
//	tBIT10= 0x0400,
//	tBIT9 = 0x0200,
//	tBIT8 = 0x0100,
//	tBIT7 = 0x0080,
//	tBIT6 = 0x0040,
//	tBIT5 = 0x0020,
//	tBIT4 = 0x0010,
//	tBIT3 = 0x0008,
//	tBIT2 = 0x0004,
//	tBIT1 = 0x0002,
//	tBIT0 = 0x0001
//}BITxmask_type;

struct ForwardFrontsInternalRegs16bit_t
{
	unsigned int inputSignal;
	unsigned int FfrontRegister;
	unsigned int Qff;
};

struct BackFrontsInternalRegs16bit_t  //Ýòà ñòðóêòóðà ïîõîæà íà âåðõíþþ, òàê ÷òî ìîæíî îáîáùàòü-îáúåäèíÿòü (ò.å ýòî óäàëÿòü è âåðõíåå óíèâåðñàëèçîâàâ ïðèìåíÿòü)
{
	unsigned int inputSignal;
	unsigned int BfrontRegister;
	unsigned int Qbf;
};

struct D_TriggersInternalRegs16bit_t
{
	unsigned int DtrigRegK1;
	unsigned int DtrigRegK2;
	unsigned int inputSignal;
	unsigned int Q_Dtrig;
};

unsigned int ForwardFrontTrigger16bit(unsigned int a, unsigned int tBITx, ForwardFrontsInternalRegs16bit_t *FFRegs);
unsigned int BackFrontTrigger16bit(unsigned int a, unsigned int tBITx, BackFrontsInternalRegs16bit_t *BFRegs);
unsigned int D_Trigger16bit(unsigned int a, unsigned int tBITx, D_TriggersInternalRegs16bit_t *DTrigRegs);
unsigned char delayedPress16bit(unsigned int *countReg, unsigned char inputBit, unsigned char k);
void Reset_ForwardFrontTrigger16bit(unsigned int tBITx, ForwardFrontsInternalRegs16bit_t *FFRegs);


#endif // !DTRIGGER16BITV2_OSM_h

