//---------------------------------------------------------------------------


#pragma hdrstop

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>

#include "Main.h"
#include "MainForm.h"

CGeneral		_CGeneral;
CCharBase		_CCharBase;
CPacket			_CPacket;
CArea			_CArea;
CMath			_CMath;
CCast			_CCast;
CBag			_CBag;
CTrap			_CTrap;
CParty			_CParty;
CRecvFunction	_CRecvFunction;
CTest			_CTest;

//---------------------------------------------------------------------------

#pragma package(smart_init)

//***** CGeneral *****//

bool CGeneral::ConnectServer(char* strIP, int iPort){
LPVOID tmpIP = strIP;
int tmpPort = iPort;
DWORD dwRet = 0;
__asm{
MOV EDX,[KO_SCK2]
MOV ECX,[KO_SOCK]
PUSH tmpPort
PUSH tmpIP
PUSH EDX
MOV EAX,KO_CONN
CALL EAX
MOV dwRet,EAX
}
if(dwRet)
	return false;
return true;
}

void CGeneral::DisconnectServer(){
DWORD CallAddr = KO_DISC;
__asm{
MOV ECX,[KO_SOCK]
CALL [&CallAddr]
MOV ECX,[KO_SOCK+4]
CALL [&CallAddr]
}
}

void CGeneral::PostMsgBox(char* strMsg, BYTE bButton, BYTE bFunction){
int msgLen = strlen(strMsg);
char *alloc = new char[msgLen+96+1];
memset(alloc,0,msgLen+96+1);
memcpy(alloc+96,strMsg,msgLen);
*(DWORD*)(alloc+36) = (DWORD)(alloc+96);
*(DWORD*)(alloc+40) = msgLen;
DWORD Addr1 = (DWORD)(alloc);
DWORD Addr2 = (DWORD)(alloc+32);
DWORD Addr3 = (DWORD)(alloc+64);
__asm{
XOR EAX,EAX
PUSH 0
MOV AL,bFunction
PUSH EAX
MOV AL,bButton
PUSH EAX
PUSH Addr1
PUSH Addr2
PUSH Addr3
MOV EAX,KO_MSBX
CALL EAX
ADD ESP,0x18
}
delete [] alloc;
}

void CGeneral::PostMsgOut(char* strMsg, DWORD dwColor){
int msgLen = strlen(strMsg);
char *alloc = new char[msgLen+32+1];
memset(alloc,0,msgLen+32+1);
memcpy(alloc+32,strMsg,msgLen);
*(DWORD*)(alloc+4) = (DWORD)(alloc+32);
*(DWORD*)(alloc+8) = msgLen;
DWORD Addr1 = (DWORD)(alloc);
__asm{
MOV ECX,[KO_DLGA]
MOV ECX,[ECX+DLG_OFF1]
PUSH dwColor
PUSH Addr1
MOV EAX,KO_MSOP
CALL EAX
}
delete [] alloc;
}

void CGeneral::PassEncrypt(char* strPass, char* pEncBuffer){
DWORD TmpECX = 0x9BF9C8;
LPVOID InBuffer = strPass;
LPVOID OutBuffer = pEncBuffer;
__asm{
	PUSH OutBuffer
	PUSH 1000
	PUSH InBuffer
	LEA ECX,&TmpECX
	MOV EAX,0x854520
	CALL EAX
}
}

//***** CCharBase *****//

bool CCharBase::IsEnemy(){
bool bRet;
DWORD CharBase = this->GetBase();
__asm{
MOV ECX,[KO_CHAR]
PUSH CharBase
MOV EAX,KO_ISEN
CALL EAX
MOV bRet,AL
}
return bRet;
}

//***** CPacket *****//

void CPacket::PSend(BYTE* pBuf,size_t szBuf){
//melih
DWORD* ptrTHRD = (DWORD*)KO_THRD;
DWORD tmpThread 	= *ptrTHRD;
*ptrTHRD = GetCurrentThreadId();
LPVOID Buffer = pBuf;
int Size = szBuf;
DWORD CallAddr = *(DWORD*)((GetIndex() * 4) + *(DWORD*)KO_SOCK + 0x40064);
__asm{
MOV ECX,[KO_SOCK]
PUSH Size
PUSH Buffer
MOV EAX,CallAddr
CALL EAX
}
*ptrTHRD = tmpThread;
*(BYTE*)KO_PFIX = 0;
AddPCount();
}

BYTE CPacket::GetIndex(){
DWORD Idx = 0;
__asm{
LEA EAX,&Idx
PUSH EAX
PUSH 4
PUSH KO_SDFX
PUSH 2
MOV EAX,KO_INDX
CALL EAX
ADD ESP,0x10
}
bFuncIndex = (BYTE)Idx;
return (BYTE)Idx;
}

//***** CArea *****//

int CArea::GetUPCCount(){
int iCount = 0;

DWORD dwTmpBase	= *(DWORD*)(KO_AREA);
DWORD dwStart	= *(DWORD*)(dwTmpBase + 0x3C);
DWORD dwBase;

dwTmpBase = *(DWORD*)(dwStart);

LOOP:
if(dwTmpBase == dwStart)
	goto RETN;

dwBase = *(DWORD*)(dwTmpBase + 0x10);
if(dwBase)
	iCount += 1;

NEXT:
__asm{
LEA ECX,&dwTmpBase
MOV EAX,KO_NXTP
CALL EAX
}
goto LOOP;

RETN:
return iCount;
}

int CArea::GetNPCCount(){
int iCount = 0;

DWORD dwTmpBase	= *(DWORD*)(KO_AREA);
DWORD dwStart	= *(DWORD*)(dwTmpBase + 0x2C);
DWORD dwBase;

dwTmpBase = *(DWORD*)(dwStart);

LOOP:
if(dwTmpBase == dwStart)
	goto RETN;

dwBase = *(DWORD*)(dwTmpBase + 0x10);
if(dwBase)
	iCount += 1;

NEXT:
__asm{
LEA ECX,&dwTmpBase
MOV EAX,KO_NXTM
CALL EAX
}
goto LOOP;

RETN:
return iCount;
}

DWORD CArea::GetUPCBase(WORD wID){
DWORD dwBase;
DWORD dwID = (DWORD)wID;
__asm{
MOV ECX,[KO_AREA]
PUSH 1
PUSH dwID
MOV EAX,KO_FPBS
CALL EAX
MOV dwBase,EAX
}
return dwBase;
}

DWORD CArea::GetNPCBase(WORD wID){
DWORD dwBase;
DWORD dwID = (DWORD)wID;
__asm{
MOV ECX,[KO_AREA]
PUSH 1
PUSH dwID
MOV EAX,KO_FMBS
CALL EAX
MOV dwBase,EAX
}
return dwBase;
}

DWORD CArea::GetCorpseBase(WORD wID){
DWORD dwBase = 0;
DWORD dwID = (DWORD)wID;
__asm{
LEA EAX,&dwID
PUSH EAX
LEA EAX,&dwBase
PUSH EAX
MOV ECX,[KO_AREA]
ADD ECX,0x48
MOV EAX,KO_NPCB
CALL EAX
}
if(!dwBase)
	return 0;
if(dwBase == *(DWORD*)(*(DWORD*)(KO_AREA) + 0x4C))
	return 0;
return *(DWORD*)(dwBase + 0x10);
}

DWORD CArea::GetAnyBase(WORD wID){
DWORD dwBase;

if(!wID || wID == 0xFFFF)
	return 0;

if(wID < 10000)
	dwBase = GetUPCBase(wID);
else
	dwBase = GetNPCBase(wID);

if(!dwBase)
	dwBase = GetCorpseBase(wID);

return dwBase;
}

DWORD CArea::GetZEnemyBase(){
DWORD dwBase;
__asm{
MOV EAX,[KO_CHAR]
ADD EAX,KO_POSX
MOV ECX,[KO_AREA]
PUSH EAX
PUSH 0
MOV EAX,KO_FPOZ
CALL EAX
MOV dwBase,EAX
}
if(dwBase) dwLastNearstBase = dwBase;
return dwBase;
}

DWORD CArea::GetNPCByName(int iNameCount, char* ptrName[], char* sName, bool bNearst, bool bEnemy, bool bAlive, FLOAT fMaxDist, bool bLoop){
if(!iNameCount) return 0;

SIZE_T iNameLen;		//for single name
SIZE_T* ptrNameLen;		//for multi names
bool bSingleName = false;

if(ptrName){
	ptrNameLen = new SIZE_T[iNameCount];
	for (int i = 0; i < iNameCount; i++) {
		ptrNameLen[i] = strlen(ptrName[i]);
	}
}
else
{
	if(!sName)	return 0;
	iNameLen = strlen(sName);
	bSingleName = true;
}

DWORD dwTmpBase,dwStart,dwRetBase;
FLOAT fRetDist,pDist;
CCharBase pNPC;
SIZE_T iCurLen;
DWORD TmpVal;

BEGIN:
	dwTmpBase	= *(DWORD*)(KO_AREA);
	dwStart	= *(DWORD*)(dwTmpBase + 0x2C);
	dwRetBase	= 0;
	fRetDist;

	dwTmpBase = *(DWORD*)(dwStart);

LOOP:
	if(dwTmpBase == dwStart){
		if(bLoop && !dwRetBase)
			goto BEGIN;
		goto RETN;
	}

	pNPC = *(DWORD*)(dwTmpBase + 0x10);
	if(!pNPC.GetBase())
		goto NEXT;

	if(bAlive){
		if( !pNPC.IsAlive() )
			goto NEXT;
	}

	if(bEnemy){
		TmpVal = pNPC.GetDWORD(0x93C);
		if(TmpVal){
			if(*(BYTE*)(TmpVal + 0xA) == 1 || *(BYTE*)(TmpVal + 0xA) == 3 || *(BYTE*)(TmpVal + 0xA) == 4)
				goto NEXT;
		}

		if( !pNPC.IsEnemy() )
			goto NEXT;
	}

	pDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pNPC.GetFLOAT(KO_POSX),pNPC.GetFLOAT(KO_POSY));

	if(fMaxDist){
		if(pDist > fMaxDist)
			goto NEXT;
	}

	iCurLen = strlen(pNPC.GetName());

	if(bSingleName){
	if(iNameLen != iCurLen)
		goto NEXT;

	if(strcmp(sName,pNPC.GetName()) != 0)
		goto NEXT;

	if(bNearst){
		if(dwRetBase){
			if(pDist < fRetDist){
				dwRetBase = pNPC.GetBase();
				fRetDist = pDist;
			}
		}else{
			dwRetBase = pNPC.GetBase();
			fRetDist = pDist;
		}
	}else{
	dwRetBase = pNPC.GetBase();
	goto RETN;
	}

	}else{
		for (int i = 0; i < iNameCount; i++) {
			if(ptrNameLen[i] != iCurLen)
				continue;

			if(strcmp(ptrName[i],pNPC.GetName()) != 0)
				continue;

				if(bNearst){
					if(dwRetBase){
						if(pDist < fRetDist){
							dwRetBase = pNPC.GetBase();
							fRetDist = pDist;
						}
					}else{
						dwRetBase = pNPC.GetBase();
						fRetDist = pDist;
					}
				}else{
					dwRetBase = pNPC.GetBase();
					goto RETN;
				}

			break;
		}
	}

NEXT:
	__asm{
		LEA ECX,&dwTmpBase
		MOV EAX,KO_NXTM
		CALL EAX
	}
	goto LOOP;

RETN:
	if(ptrName)
		delete [] ptrNameLen;
	if(dwRetBase)
		dwLastNearstBase = dwRetBase;
	return dwRetBase;
}

DWORD CArea::GetUPCByName(int iNameCount, char* ptrName[], char* sName, bool bAlive, FLOAT fMaxDist){
if(!iNameCount) return 0;

SIZE_T iNameLen;		//for single name
SIZE_T* ptrNameLen;		//for multi names
bool bSingleName = false;

if(ptrName){
	ptrNameLen = new SIZE_T[iNameCount];
	for (int i = 0; i < iNameCount; i++) {
		ptrNameLen[i] = strlen(ptrName[i]);
	}
}
else
{
	if(!sName)	return 0;
	iNameLen = strlen(sName);
	bSingleName = true;
}

DWORD dwTmpBase,dwStart,dwRetBase;
FLOAT fRetDist,pDist;
CCharBase pUPC;
SIZE_T iCurLen;

BEGIN:
	dwTmpBase	= *(DWORD*)(KO_AREA);
	dwStart	= *(DWORD*)(dwTmpBase + 0x3C);
	dwRetBase	= 0;
	fRetDist;

	dwTmpBase = *(DWORD*)(dwStart);

LOOP:
	if(dwTmpBase == dwStart)
		goto RETN;

	pUPC = *(DWORD*)(dwTmpBase + 0x10);
	if(!pUPC.GetBase())
		goto NEXT;

	if(bAlive){
		if(!pUPC.IsAlive())
			goto NEXT;
	}

	pDist = _CMath.fDist(_CCharBase.GetFLOAT(KO_POSX),_CCharBase.GetFLOAT(KO_POSY),pUPC.GetFLOAT(KO_POSX),pUPC.GetFLOAT(KO_POSY));

	if(fMaxDist){
		if(pDist > fMaxDist)
			goto NEXT;
	}

	iCurLen = strlen(pUPC.GetName());

	if(bSingleName){
	if(iNameLen != iCurLen)
		goto NEXT;

	if(strcmp(sName,pUPC.GetName()) != 0)
		goto NEXT;

	dwRetBase = pUPC.GetBase();
	goto RETN;

	}else{
		for (int i = 0; i < iNameCount; i++) {
			if(ptrNameLen[i] != iCurLen)
				continue;

			if(strcmp(ptrName[i],pUPC.GetName()) != 0)
				continue;

			dwRetBase = pUPC.GetBase();
			goto RETN;
		}
	}

NEXT:
	__asm{
		LEA ECX,&dwTmpBase
		MOV EAX,KO_NXTP
		CALL EAX
	}
	goto LOOP;

RETN:
	if(ptrName)
		delete [] ptrNameLen;
	return dwRetBase;
}

void CArea::SelectObject(CCharBase Object){
DWORD dwBase = Object.GetBase();
__asm{
MOV ECX,[KO_DLGA]
PUSH dwBase
MOV EAX,KO_SETM
CALL EAX
}
}

//***** CMath *****//

inline FLOAT CMath::fDist(FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2){
return pow((double)((X2-X1)*(X2-X1)) + (double)((Y2-Y1)*(Y2-Y1)),0.5) / 4;
}

//***** CCast *****//

int CCast::CastCount(){
DWORD TMP = *(DWORD*)(KO_DLGA);
TMP = *(DWORD*)(TMP+DLG_OFF2);
TMP = *(DWORD*)(TMP+0x4);
return *(DWORD*)(TMP+0xD0);
}

int CCast::CastNum(int iNo){
if(!CastCount())return 0;
DWORD TMP = *(DWORD*)(KO_DLGA);
TMP = *(DWORD*)(TMP+DLG_OFF2);
TMP = *(DWORD*)(TMP+0x4);
TMP = *(DWORD*)(TMP+0xCC);
for (int i = 1; i <= iNo; i++) {
TMP = *(DWORD*)(TMP);
}
TMP = *(DWORD*)(TMP+0x8);
if(TMP)
	return *(DWORD*)(TMP);
return 0;
}

int CCast::MakeCastNum(CCharBase dwBase, char* strTail){
int iClass = dwBase.GetDWORD(KO_CLSS);

char temp[7]; memset(temp,0,7);
itoa(iClass,temp,10);
strcat(temp,strTail);
return atoi(temp);
}

bool CCast::IsCasted(int Num){
int count = CastCount();
if(!count) return false;

for (int i=0; i <= count; i++) {
if(CastNum(i)==Num) return true;
}
return false;
}

//***** CInvSlot *****//

CInvSlot::CInvSlot(int iSlot){
if(iSlot == -1){
dwBase = 0;
return;
}
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + DLG_OFF3);
Tmp = *(DWORD*)(Tmp + (0x134 + (4 * (iSlot + 26))));
dwBase = Tmp;
iCurrentSlot = iSlot;
}

void CInvSlot::SelectSlot(int iSlot){
if(iSlot == -1){
dwBase = 0;
return;
}
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + DLG_OFF3);
Tmp = *(DWORD*)(Tmp + (0x134 + (4 * (iSlot + 26))));
dwBase = Tmp;
iCurrentSlot = iSlot;
}

void CInvSlot::SelectLeft(){
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + DLG_OFF3);
Tmp = *(DWORD*)(Tmp + 0x184);
dwBase = Tmp;
iCurrentSlot = -1;
}

void CInvSlot::SelectRight(){
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + DLG_OFF3);
Tmp = *(DWORD*)(Tmp + 0x17C);
dwBase = Tmp;
iCurrentSlot = -1;
}

//***** CInnSlot *****//

CInnSlot::CInnSlot(int iSlot){
if(iSlot == -1){
dwBase = 0;
return;
}
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + DLG_OFF4);
Tmp = *(DWORD*)(Tmp + ((iSlot * 4)+0xC8));
dwBase = Tmp;
iCurrentSlot = iSlot;
}

void CInnSlot::SelectSlot(int iSlot){
if(iSlot == -1){
dwBase = 0;
return;
}
DWORD Tmp = *(DWORD*)(KO_DLGA);
Tmp = *(DWORD*)(Tmp + DLG_OFF4);
Tmp = *(DWORD*)(Tmp + ((iSlot * 4)+0xC8));
dwBase = Tmp;
iCurrentSlot = iSlot;
}

//***** CBag *****//

int CBag::FindInvItem(int iBegin, int dwNum){
CInvSlot pSlot;
int iRetSlot = -1;

for (int i = iBegin; i < iMaxInventorySlot; i++) {
pSlot.SelectSlot(i);
if(pSlot.IsFree())
	continue;
if(pSlot.dwItemNum() == dwNum){
iRetSlot = i;
break;
}
}

return iRetSlot;
}

int CBag::FindFreeInvSlot(int iBegin){
CInvSlot pSlot;
int iRetSlot = -1;

for (int i = iBegin; i < iMaxInventorySlot; i++) {
pSlot.SelectSlot(i);
if(pSlot.IsFree()){
iRetSlot = i;
break;
}
}

return iRetSlot;
}

int CBag::FindInnItem(int iBegin, int dwNum){
CInnSlot pSlot;
int iRetSlot = -1;

for (int i = iBegin; i < iMaxInnSlot; i++) {
pSlot.SelectSlot(i);
if(pSlot.IsFree())
	continue;
if(pSlot.dwItemNum() == dwNum){
iRetSlot = i;
break;
}
}

return iRetSlot;
}

int CBag::FindFreeInnSlot(int iBegin){
CInnSlot pSlot;
int iRetSlot = -1;

for (int i = iBegin; i < iMaxInnSlot; i++) {
pSlot.SelectSlot(i);
if(pSlot.IsFree()){
iRetSlot = i;
break;
}
}

return iRetSlot;
}

//***** CTrap *****//

bool CTrap::Switch(bool bOnOff){
if(bOnOff){
if(IsSwitched())
	return false;

hThread = CreateThread(0,0,(LPTHREAD_START_ROUTINE)_StartThread,this,0,&dwThread);
bState = true;
}else{
if(!IsSwitched())
	return false;

TerminateThread(hThread,0);
CloseHandle(hThread);
hThread=0;
dwThread=0;
bState = false;
}
return true;
}

void CTrap::Function(){
bool bTrapped;
DWORD WSOCKADR	=	(DWORD) GetProcAddress(GetModuleHandleA("WS2_32.dll"),"send");
DWORD SENDADR	=	*(DWORD*)(KO_SOCK) + (_CPacket.GetIndex() * 4) + 0x40064;
if(!WSOCKADR || !SENDADR)return;

LOOP:
bTrapped=false;
if(*(BYTE*)(KO_CRYP) != TRAP_bCryptByte) bTrapped=true;
if(*(BYTE*)(WSOCKADR) != TRAP_bWSockByte) bTrapped=true;
if(*(DWORD*)(SENDADR) >= TRAP_iMaxSendAdr) bTrapped=true;
if(!bTrapped){
Sleep(TRAP_iTrapDelay);
goto LOOP;
}
TerminateProcess(GetCurrentProcess(),0);
}

//***** CParty *****//

void CParty::Refresh(){
DWORD dwBase = *(DWORD*)(KO_PRTY);
dwBase = *(DWORD*)(dwBase + PTY_OFF5);

int UserCount = *(DWORD*)(dwBase + 0x360);
iCount = UserCount;

for (int n = UserCount; n < 8; n++) {
dwUser[n] = 0;
}

if(!UserCount)
	return;

dwBase = *(DWORD*)(dwBase + 0x35C);
DWORD tmpUser;

for (int i = 0; i < UserCount; i++) {
tmpUser = dwBase;

for (int y = 0; y < (i + 1); y++) {
tmpUser = *(DWORD*)(tmpUser);
}

dwUser[i] = tmpUser;
}

}

//***** CRecvFunction *****//

BYTE CRecvFunction::GetIndex(){
DWORD Idx = 0;
__asm{
LEA EAX,&Idx
PUSH EAX
PUSH 4
PUSH KO_RCFX
PUSH 1
MOV EAX,KO_INDX
CALL EAX
ADD ESP,0x10
}
bFuncIndex = (BYTE)Idx;
return (BYTE)Idx;
}

bool CRecvFunction::StartFunction(){

if(bState)
	return false;

BYTE FuncIdx = GetIndex();
DWORD _KO_RCVA = KO_RCVA + (FuncIdx * 4);
DWORD _KO_RCVB = *(DWORD*)(_KO_RCVA);
dwOldAdr = _KO_RCVB;

HMODULE hKernel32		= GetModuleHandleA("Kernel32.dll");
DWORD adrCreateFile		= (DWORD) GetProcAddress(hKernel32,"CreateFileA");
DWORD adrWriteFile		= (DWORD) GetProcAddress(hKernel32,"WriteFile");
DWORD adrCloseFile		= (DWORD) GetProcAddress(hKernel32,"CloseHandle");

LPVOID FuncAdr = VirtualAlloc(0,1024,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
if(!FuncAdr)return false;

char* RF_strMSName	= "//./mailslot/kojd";
size_t szBasePath = strlen(RF_strMSName);

char pid[5];	ultoa(GetCurrentProcessId(),pid,10);
size_t szPid	=	strlen(pid);
char *MSlotName = new char[szBasePath+szPid+1];
memset(MSlotName,0,szBasePath+szPid+1);
memcpy(MSlotName,RF_strMSName,szBasePath);
strcat(MSlotName,pid);

HANDLE MSlotHandle = CreateMailslotA(MSlotName,0,0,0);
if((DWORD)(MSlotHandle)==0xFFFFFFFF){
VirtualFree(FuncAdr,0,MEM_RELEASE);
delete [] MSlotName;
return false;
}
hMailSlot	=	MSlotHandle;

DWORD tmpAddr = (DWORD)(FuncAdr)+0x400;
memcpy((LPVOID)(tmpAddr),MSlotName,strlen(MSlotName));

tmpAddr = (DWORD)(FuncAdr)+0x32A;
*(DWORD*)(tmpAddr) = adrCreateFile;

tmpAddr = (DWORD)(FuncAdr)+0x334;
*(DWORD*)(tmpAddr) = adrWriteFile;

tmpAddr = (DWORD)(FuncAdr)+0x33E;
*(DWORD*)(tmpAddr) = adrCloseFile;

tmpAddr = (DWORD)(FuncAdr)+0x208;
*(DWORD*)(tmpAddr) = _KO_RCVB;

tmpAddr = (DWORD)(FuncAdr)+0x212;
*(DWORD*)(tmpAddr) = _KO_RCVA;

tmpAddr = (DWORD)(FuncAdr)+0x21C;
*(DWORD*)(tmpAddr) = (DWORD)(FuncAdr);

BYTE PATCH[]={	0x52,0x89,0x0D,0,0,0,0,
				0x89,0x05,0,0,0,0,
				0x8B,0x4E,0x04,0x89,0x0D,0,0,0,0,
				0x8B,0x56,0x08,0x89,0x15,0,0,0,0,
				0x81,0xF9,0x00,0x10,0x00,0x00,0x7D,0x3E,0x50,0x68,0x80,0x00,0x00,0x00,0x6A,0x04,0x6A,0x00,0x6A,0x01,0x68,0x00,0x00,0x00,0x40,0x68,0,0,0,0,
				0xFF,0x15,0,0,0,0,
				0x83,0xF8,0xFF,0x74,0x1D,0x50,0x6A,0x00,0x54,0xFF,0x35,0,0,0,0,
				0xFF,0x35,0,0,0,0,
				0x50,0xFF,0x15,0,0,0,0,
				0xFF,0x15,0,0,0,0,
				0x8B,0x0D,0,0,0,0,
				0x8B,0x05,0,0,0,0,
				0x5A,0xFF,0x25,0,0,0,0,
};

tmpAddr = (DWORD)(FuncAdr)+0x320;
CopyBytes(PATCH+3,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x3B6;
CopyBytes(PATCH+9,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x1F4;
CopyBytes(PATCH+18,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x1FE;
CopyBytes(PATCH+27,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x400;
CopyBytes(PATCH+57,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x32A;
CopyBytes(PATCH+63,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x1F4;
CopyBytes(PATCH+78,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x1FE;
CopyBytes(PATCH+84,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x334;
CopyBytes(PATCH+91,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x33E;
CopyBytes(PATCH+97,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x320;
CopyBytes(PATCH+103,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x3B6;
CopyBytes(PATCH+109,tmpAddr);

tmpAddr = (DWORD)(FuncAdr)+0x208;
CopyBytes(PATCH+116,tmpAddr);

memcpy(FuncAdr,PATCH,sizeof(PATCH));

*(DWORD*)(_KO_RCVA)=(DWORD)(FuncAdr);

delete [] MSlotName;

lFuncAdr = FuncAdr;
bPaused = false;
bState = true;

hThread = CreateThread(0,0,(LPTHREAD_START_ROUTINE)_StartThread,this,0,&dwThread);

return true;
}

void CRecvFunction::StopFunction(){
if(!bState)
	return;

DWORD _KO_RCVA = KO_RCVA + (bFuncIndex * 4);
*(DWORD*)(_KO_RCVA) = dwOldAdr;

if(hThread){
	TerminateThread(hThread,0);
	CloseHandle(hThread);
}

if(lFuncAdr)
	VirtualFree(lFuncAdr,0,MEM_RELEASE);

bState = false;
bPaused = false;
hThread = 0;
dwThread = 0;
}

float CTest::GetPosZ(float fPosX, float fPosY)
{
	float fRetZ = 0;
	__asm{
		PUSH 1
		PUSH fPosY
		PUSH fPosX
		MOV EAX,[0xC26C30]
		MOV ECX,[EAX+0x1C]
		MOV EAX,[ECX]
		CALL [EAX+0x34]
		FSTP &fRetZ
	}
	return fRetZ;
}

void CTest::InitPos(float fPosX, float fPosY, float fPosZ)
{
	char pNewPos[12];	// 4*3 = 12 bytes memory allocation.

	*(FLOAT*)(pNewPos) 		= fPosX;
	*(FLOAT*)(pNewPos + 4) 	= fPosZ;
	*(FLOAT*)(pNewPos + 8) 	= fPosY;

	__asm
	{
		LEA EAX,pNewPos
		PUSH EAX
		MOV ECX,[KO_DLGA]
		MOV EAX,0x7FB870
		CALL EAX
    }

}

