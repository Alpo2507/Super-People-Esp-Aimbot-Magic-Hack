//---------------------------------------------------------------------------

#ifndef MainH
#define MainH

#include <Classes.hpp>

#include "Memory.h"

//***** FUNCTIONS *****//

#define CopyBytes(Dest, Src) memcpy(Dest, (BYTE*)&Src, sizeof(Src))

//***** ITEMS *****//
#define ITEM_WOLF			370004000
#define ITEM_ARROW			391010000
#define ITEM_NOAH			900000000
#define ITEM_HP45			389010000
#define ITEM_HP90			389011000
#define ITEM_HP180			389012000
#define ITEM_HP360			389013000
#define ITEM_HP720			389014000
#define ITEM_MP120			389016000
#define ITEM_MP240			389017000
#define ITEM_MP480			389018000
#define ITEM_MP960			389019000
#define ITEM_MP1920			389020000
#define ITEM_KILLINGBLADE	700037000
#define ITEM_TS_SCROLL		381001000

//***** SKILLS *****//
#define SKILL_HP45			490010
#define SKILL_HP90			490011
#define SKILL_HP180			490012
#define SKILL_HP360			490013
#define SKILL_HP720			490014
#define SKILL_MP120			490016
#define SKILL_MP240			490017
#define SKILL_MP480			490018
#define SKILL_MP960			490019
#define SKILL_MP1920		490020
#define SKILL_CHAOSSPRINT	490223
#define SKILL_CHAOSSWORD	490219
#define SKILL_CHAOSBLADE	490226
#define SKILL_CHAOSPOT		490222
#define SKILL_TS_CAST		472001
#define SKILL_TS_BOWMAN		472310

//***** OTHER *****//

//Bag
const int iMaxInventorySlot = 28;
const int iMaxInnPageSlot = 24;
const int iMaxInnPage = 8;
const int iMaxInnSlot = iMaxInnPageSlot * iMaxInnPage;

//Trap
const int TRAP_iTrapDelay	= 1000;
const BYTE TRAP_bCryptByte	= 0x53;
const BYTE TRAP_bWSockByte	= 0x8B;
const int TRAP_iMaxSendAdr	= 0x500000;

//RecvFunction
const int RF_iExecDelay		= 250;

//////////////////////////////
//***** GLOBAL CLASSES *****//
/////////////////////////////

class CGeneral{
	public:
		//General
		inline void SetWindowTop(HWND hHWND, bool bTop){HWND hType = HWND_TOPMOST; if(!bTop)hType = HWND_NOTOPMOST; SetWindowPos(hHWND,hType,0,0,0,0,SWP_NOSIZE + SWP_NOMOVE);};
		bool ConnectServer(char* strIP, int iPort);
		void DisconnectServer();
		void PostMsgBox(char* strMsg, BYTE bButton, BYTE bFunction);
		void PostMsgOut(char* strMsg, DWORD dwColor);
		void PassEncrypt(char* strPass, char* pEncBuffer);
		//Default
		CGeneral(){};
};

class CCharBase{
	private:
		DWORD dwBase;
	public:
		//General
		inline void		SetBase(DWORD dwAddr){dwBase = dwAddr;};
		inline DWORD 	GetBase(){return dwBase;};
		inline WORD 	GetID(){return GetWORD(KO_MYID);};
		inline char*	GetName(){return (LPSTR)GetDWORD(KO_NICK);};
		inline bool 	IsUPC(){if(*(DWORD*)(dwBase + 0x30) == 2) return true; return false;};
		inline bool		IsAlive(){if(GetDWORD(0x388)==4 || GetDWORD(0x388)==5 || GetDWORD(0x388)==0xD)return false; else return true;};
		inline void		MoveTo(FLOAT X, FLOAT Y){SetFLOAT(KO_PTRX,X); SetFLOAT(KO_PTRY,Y); SetDWORD(KO_MOV1,2); SetDWORD(KO_MOV2,2);};
		inline void		SetSize(FLOAT fSize){SetFLOAT(KO_SIZE,fSize);};
		inline FLOAT	GetSize(){return GetFLOAT(KO_SIZE);};
		inline WORD		GetTargetID(){return GetWORD(KO_TGID);};
		bool IsEnemy();
		//Set memory
		inline void 	SetBYTE(DWORD dwOffset,BYTE bVal){*(BYTE*)(dwBase + dwOffset) = bVal;};
		inline void 	SetWORD(DWORD dwOffset,WORD wVal){*(WORD*)(dwBase + dwOffset) = wVal;};
		inline void 	SetDWORD(DWORD dwOffset,DWORD dwVal){*(DWORD*)(dwBase + dwOffset) = dwVal;};
		inline void		SetFLOAT(DWORD dwOffset,FLOAT fVal){*(FLOAT*)(dwBase + dwOffset) = fVal;};
		//Get memory
		inline BYTE 	GetBYTE(DWORD dwOffset){return *(BYTE*)(dwBase + dwOffset);};
		inline WORD 	GetWORD(DWORD dwOffset){return *(WORD*)(dwBase + dwOffset);};
		inline DWORD 	GetDWORD(DWORD dwOffset){return *(DWORD*)(dwBase + dwOffset);};
		inline FLOAT 	GetFLOAT(DWORD dwOffset){return *(FLOAT*)(dwBase + dwOffset);};
		//Default
		CCharBase(){dwBase = 0;};
		CCharBase(DWORD dwBaseAddr){dwBase = dwBaseAddr;};
};

//***** FUNCTION CLASSES *****//

class CPacket{
	private:
		BYTE bFuncIndex;
		int iSendCount;
	public:
		//General
		void PSend(BYTE* pBuf,size_t szBuf);
		BYTE GetIndex();
		inline bool GetConnState(){return *(BYTE*)(*(DWORD*)(KO_SOCK) + 0x4003C);};
		inline int GetPCount(){return iSendCount;};
		inline void AddPCount(){iSendCount += 1;};
		inline void ClearPCount(){iSendCount = 0;};
		//Set memory
		inline void PutBYTE(char* pBuffer,BYTE bVal){*(pBuffer) = bVal;};
		inline void PutWORD(char* pBuffer,WORD wVal){*(WORD*)(pBuffer) = wVal;};
		inline void PutDWORD(char* pBuffer,DWORD dwVal){*(DWORD*)(pBuffer) = dwVal;};
		inline void PutSTR(char* pBuffer,char* pStr,int iSize){memcpy(pBuffer,pStr,iSize);};
		//Get memory
		inline BYTE GetBYTE(char* pBuffer){return *(BYTE*)(pBuffer);};
		inline WORD GetWORD(char* pBuffer){ return *(WORD*)(pBuffer);};
		inline DWORD GetDWORD(char* pBuffer){return *(DWORD*)(pBuffer);};
		inline void GetSTR(char* sBuffer,char* dBuffer,int iSize){memcpy(dBuffer,sBuffer,iSize);};
		//Default
		CPacket(){iSendCount = 0; bFuncIndex = -1;};
};

class CArea{
	private:
		DWORD dwLastNearstBase;
	public:
		//General
		int GetUPCCount();
		int GetNPCCount();
		inline int GetAllCount(){return (GetUPCCount() + GetNPCCount());};
		DWORD GetUPCBase(WORD wID);
		DWORD GetNPCBase(WORD wID);
		DWORD GetCorpseBase(WORD wID);
		DWORD GetAnyBase(WORD wID);
		DWORD GetZEnemyBase();
		DWORD GetNPCByName(int iNameCount, char* ptrName[], char* sName,bool bNearst, bool bEnemy,bool bAlive, FLOAT fMaxDist, bool bLoop);
		DWORD GetUPCByName(int iNameCount, char* ptrName[], char* sName, bool bAlive, FLOAT fMaxDist);
		void SelectObject(CCharBase Object);
		//Default
		CArea(){dwLastNearstBase = 0;};
};

class CMath{
	public:
		//General
		inline FLOAT fDist(FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2);
		inline int iPerc(int HiVal, int LoVal){return 100 / (HiVal / LoVal);};
		//Default
		CMath(){};
};

class CCast{
	public:
		//General
		int CastCount();
		int CastNum(int iNo);
		int MakeCastNum(CCharBase dwBase, char* strTail);
		bool IsCasted(int Num);
		//Default
		CCast(){};

};

class CInvSlot{
	private:
		DWORD dwBase;
		int iCurrentSlot;
	public:
		//General
		inline int GetCurrentSlot(){return iCurrentSlot;};
		void SelectSlot(int iSlot);
		void SelectLeft();
		void SelectRight();
		inline DWORD GetBase(){return dwBase;};
		inline int dwItemNum(){return *(DWORD*)(*(DWORD*)(dwBase + 0x38)) + *(DWORD*)(*(DWORD*)(dwBase + 0x3C));};
		inline int iItemN(){return *(DWORD*)(dwBase + 0x40);};
		inline int iItemDur(){return *(DWORD*)(dwBase + 0x44);};
		inline char* strItemName(){return (LPSTR)(*(DWORD*)(*(DWORD*)(dwBase + 0x38) + 0xC));};
		inline bool IsFree(){if(dwBase) return false; else return true;};
		//Default
		CInvSlot(int iSlot);
		CInvSlot(){dwBase = 0; iCurrentSlot = -1;};
};

class CInnSlot{
	private:
		DWORD dwBase;
		int iCurrentSlot;
	public:
		//General
		inline int GetCurrentSlot(){return iCurrentSlot;};
		void SelectSlot(int iSlot);
		inline DWORD GetBase(){return dwBase;};
		inline int dwItemNum(){return *(DWORD*)(*(DWORD*)(dwBase + 0x38)) + *(DWORD*)(*(DWORD*)(dwBase + 0x3C));};
		inline int iItemN(){return *(DWORD*)(dwBase + 0x40);};
		inline int iItemDur(){return *(DWORD*)(dwBase + 0x44);};
		inline char* strItemName(){return (LPSTR)(*(DWORD*)(*(DWORD*)(dwBase + 0x38) + 0xC));};
		inline bool IsFree(){if(dwBase) return false; else return true;};
		//Default
		CInnSlot(int iSlot);
		CInnSlot(){dwBase = 0; iCurrentSlot = -1;};
};

class CBag{
	public:
		//General
		inline void SlotToPaged(int iSlot, int &outPage, int &outSlot){outPage = iSlot / iMaxInnPageSlot; outSlot = iSlot % iMaxInnPageSlot;};
		inline int PagedToSlot(int iPage, int iSlot){return (iPage * iMaxInnPageSlot) + iSlot;};
		//Inventory
		int FindInvItem(int iBegin, int dwNum);
		int FindFreeInvSlot(int iBegin);
		//Inn
		int FindInnItem(int iBegin, int dwNum);
		int FindFreeInnSlot(int iBegin);
		//Default
		CBag(){};
};

class CTrap{
	private:
		HANDLE hThread;
		DWORD dwThread;
		bool bState;

		inline static void _StartThread(void* param){((CTrap*)param)->Function();};
		void Function();
	public:
		//General
		inline bool IsSwitched(){return bState;};
		bool Switch(bool bOnOff);
		//Default
		CTrap(){bState = false;};
};

class CParty{
	private:
		int iCount;
		DWORD dwUser[8];
	public:
		//General
		void Refresh();
		inline DWORD GetUser(int iIndex){return dwUser[iIndex];};
		inline int GetCount(){return iCount;};
		//Default
		CParty(){iCount = 0;};
};

class CRecvFunction{
	private:
		BYTE bFuncIndex;
		HANDLE hMailSlot;
		HANDLE hThread;
		DWORD dwThread;
		LPVOID lFuncAdr;
		DWORD dwOldAdr;
		bool bState;
		bool bPaused;

		inline static void _StartThread(void* param){((CRecvFunction*)param)->ExecuteMessages();};
		void ExecuteMessages();
		//Message Functions
		void Exec_ChestDrop(char* pBuf);
		void Exec_ItemDrop(char* pBuf);
		void Exec_Chat(char* pBuf);
		void Exec_PartyRecv(char* pBuf);

		//Message Variables
		int iLastChestX;
		int iLastChestY;
		DWORD dwLastChestID;

	public:
		//General
		BYTE GetIndex();
		inline bool GetState(){return bState;};
		bool StartFunction();
		void StopFunction();
		void Pause(){bPaused=true;};
		void Resume(){bPaused=false;};
		//Message Variables
		bool bOnLootWay;
		//Default
		CRecvFunction(){bFuncIndex = -1; hMailSlot = 0; lFuncAdr = 0; bState = false; bPaused = false; bOnLootWay = false;};
};

class CTest{
	public:

	float GetPosZ(float fPosX, float fPosY);
	void InitPos(float fPosX, float fPosY, float fPosZ);

	CTest(){};
};

////////////////////////////////////////
//***** GLOBAL CLASS DEFINITIONS *****//
////////////////////////////////////////

extern CGeneral			_CGeneral;
extern CCharBase		_CCharBase;
extern CPacket			_CPacket;
extern CArea			_CArea;
extern CMath			_CMath;
extern CCast			_CCast;
extern CBag				_CBag;
extern CTrap			_CTrap;
extern CParty			_CParty;
extern CRecvFunction	_CRecvFunction;
extern CTest			_CTest;
//---------------------------------------------------------------------------
#endif
