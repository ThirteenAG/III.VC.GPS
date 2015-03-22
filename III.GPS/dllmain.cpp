/* Organized & Written by DK (c) */

#include "..\includes\CPatch.h"
#include <math.h>

#define GTA_3_1_0     0x6B636F52
#define GTA_3_1_1     0x49535049

#define MAX_POINTS 200

#define rwRENDERSTATETEXTURERASTER 1
#define rwPRIMTYPETRIFAN 5
#define D3DRS_ALPHAFUNC 25
#define D3DCMP_GREATER 5
#define LINE_WIDTH 1400.0f
#define GPS_TEXT_TIME 2000
#define GPS_AUDIO_DELAY 250
#define GPS_KEY_SCROLL_DELAY 300

#define gGameVersion (*(unsigned int *)0x60F428)

#define IS_PED_IN_CAR(ped) (*(unsigned char *)((unsigned int)ped + 0x314))
#define GET_PED_CAR(ped) (*(CVehicle **)((unsigned int)ped + 0x310))

#define BYTEn(x, n)   (*((BYTE*)&(x)+n))

enum eGpsMode
{
	GPS_MODE_DEFAULT,
	GPS_MODE_SAFEHOUSE,
	GPS_MODE_PAYNSPRAY,
	GPS_MODE_AMMUNATION,
	GPS_MODE_BOMBSHOP,
	GPS_MODE_HOSPITAL,
	GPS_MODE_POLICE,
	GPS_MODE_FIRESTATION,
	GPS_MODE_DISABLED
};

enum eGpsKeys
{
	GPS_KEY_SWITCH = VK_MENU,         // Alt
	GPS_KEY_DEFAULT = VK_NUMPAD0,     // Num0
	GPS_KEY_SAFEHOUSE = VK_NUMPAD1,   // Num1
	GPS_KEY_PAYNSPRAY = VK_NUMPAD2,   // Num2
	GPS_KEY_AMMUNATION = VK_NUMPAD3,  // Num3
	GPS_KEY_BOMBSHOP = VK_NUMPAD4,    // Num4
	GPS_KEY_HOSPITAL = VK_NUMPAD5,    // Num5
	GPS_KEY_POLICE = VK_NUMPAD6,      // Num6
	GPS_KEY_FIRESTATION = VK_NUMPAD7, // Num7
	GPS_KEY_DISABLE = VK_SUBTRACT,    // -
	GPS_KEY_UP = VK_PRIOR,            // PgUp
	GPS_KEY_DOWN = VK_NEXT            // PgDown
};

enum eLevelName
{
	LEVEL_PORTLAND = 1,
	LEVEL_STAUNTON,
	LEVEL_SHORESIDE,
	LEVEL_FORCE_DWORD = 0x7FFFFFFF
};

struct RwV2d
{
	float x;
	float y;
};

struct RwV3d
{
	float x;
	float y;
	float z;
};

struct RwD3D9Vertex
{
	float        x;
	float        y;
	float        z;
	float        rhw;
	unsigned int emissiveColor;
	float        u;
	float        v;
};

struct RwMatrix
{
	RwV3d right;
	unsigned int flags;
	RwV3d top;
	char _pad0[4];
	RwV3d at;
	char _pad1[4];
	RwV3d pos;
	char _pad2[4];
};

class CMatrix
{
public:
	RwMatrix       m_sMatrix;
	class CMatrix *m_pMatrix;
	unsigned char  m_bHaveRwMatrix;
	char _pad[3];
};

class CPlaceable
{
public:
	void *vmt;
	CMatrix m_sCoords;
};

typedef CPlaceable CVehicle;
typedef CPlaceable CPed;
typedef CPlaceable CObject;
typedef RwV3d CVector;
typedef RwV2d CVector2D;

enum ePathNodeType
{
	PATHNODE_VEHICLE_PATH,
	PATHNODE_PED_PATH
};

class CPathNode
{
public:
	union{
		CVector2D     m_v2dPoint;
		CVector       m_vPoint;
	};
	unsigned int field_C;
	unsigned int field_10;
	unsigned short field_14;
	unsigned short field_16;
	unsigned short field_18;
	unsigned char m_bFlags;
	unsigned char field_1B;
	unsigned char m_bNumFloodFillGroups;
	unsigned char field_1D;
	unsigned char field_1E;
	unsigned char field_1F;
};

enum eBlipType
{
	BLIP_COORD = 0,
	BLIP_CAR,
	BLIP_PED,
	BLIP_OBJECT
};

struct RadarBlip
{
	unsigned int   m_dwBlipColour;
	unsigned int   m_dwBlipType;
	unsigned int   m_dwEntityHandle;
	CVector2D      m_v2dPosn;
	CVector        m_v3dPosn;
	unsigned short m_wIndex;
	unsigned char  m_bBlipBrightness;
	unsigned char  m_bActive;
	float          m_f3dMarkerAnimState;
	unsigned short m_wBlipScale;
	unsigned short m_wBlipDisplay;
	unsigned short m_wBlipSprite;
	char _pad[2];
};

struct PathLineInfo
{
	union{
		CVector *targetPoint;
		CVector2D *targetPoint2d;
	};
	unsigned int color;
};

CVehicle *(__cdecl *FindPlayerVehicle)();
void (__thiscall *DoPathSearch)(void *, unsigned char, CVector, int, CVector, CPathNode **, short *, short, CVehicle *, float *, float,
								int);
void (__cdecl *TransformRealWorldPointToRadarSpace)(CVector2D &,CVector2D const&);
void (__cdecl *TransformRadarPointToScreenSpace)(CVector2D &,CVector2D const&);
float (__cdecl *RwIm2DGetNearScreenZ)();
void (__cdecl *RwRenderStateSet)(unsigned int, unsigned int);
int (__cdecl *RwD3D9SetRenderState)(unsigned int, unsigned int);
void (__cdecl *SetSpriteVertices)(float, float, float, float, float, float, float, float, unsigned int *, unsigned int *,
								  unsigned int *, unsigned int *);
void (__cdecl *RwIm2DRenderPrimitive)(unsigned int, RwD3D9Vertex *, unsigned int);
CVehicle *(__thiscall *VehicleGetAt)(void *, unsigned int);
CPed *(__thiscall *PedGetAt)(void *, unsigned int);
CObject *(__thiscall *ObjectGetAt)(void *, unsigned int);
unsigned int (__cdecl *GetRadarTraceColour)(unsigned int, unsigned int);
void (__cdecl *ShowRadarTrace)(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char);
void (__cdecl *DrawRadarMap)();
void (__cdecl *DrawRadarMask)();
void (__cdecl *InitialiseRadar)();
void (__thiscall *PlayFrontEndSound)(void *, unsigned short, unsigned int);
void DrawLine(CVector2D *, CVector2D *, float, unsigned int);
void DrawPathLineMask();
void RwIm2DSetNearScreenZ(float);
void ProcessPathfind();
void DrawPathfind();
void DrawPathFindLine(RwV2d *, unsigned int, float, unsigned int);
float GetSquaredDistanceBetweenPoints(CVector const&, CVector const&);
bool IsLineInsideRadar(CVector2D const&, CVector2D const&);
void ProcessModeSwitch();
PathLineInfo *GetPlaceInfo(PathLineInfo *, CVehicle *, eGpsMode, eLevelName);
void InitialiseGps();
void DrawPlaceMarker(PathLineInfo *);
void *gPathfind;
float *gRadarMapZShift;
RwD3D9Vertex *gSpriteVertices;
int *gScreenWidth;
int *gScreenHeight;
void **gVehiclePool;
void **gPedPool;
void **gObjectPool;
unsigned int *gRwEngine;
float *gRadarRange;
RadarBlip *gRadarBlips;
eLevelName *gCurrLevel;
unsigned int *g_TimeMs;
void *gAudio;

/* Text drawing - related things */
void (__cdecl *AsciiToUnicode)       (const char *ascii, short *pUni);
void (__cdecl *PrintString)          (float x, float y, short *text);
void (__cdecl *SetFontStyle)         (int style);
void (__cdecl *SetScale)             (float w, float h);
void (__cdecl *SetColor)             (unsigned int *color);
void (__cdecl *SetJustifyOn)         ();
void (__cdecl *SetDropShadowPosition)(int position);
void (__cdecl *SetPropOn)            ();
/* */

CPathNode *gapPathNodes[MAX_POINTS];
short gwPathNodesCount;
RwV2d gaPathPoints[MAX_POINTS];
char gCurrentGpsMode;
unsigned int gGpsTextTimer;
unsigned int gGpsAudioTimer;
unsigned int gGpsScrollTimer;

CVector gPayNSprayPos[3] = { 925.4008f, -354.1194f, 10.8415f,    375.325f, -493.7094f, 26.1748f,    -1138.3365f, 34.6745f, 58.8237f };
CVector gHospitalPos[3]  = { 1144.25f, -596.875f, 14.9144f,    185.9603f, -22.5466f, 16.2168f,    -1252.5568f, -145.3662f, 58.7841f };
CVector gSafehousePos[3] = { 888.5625f, -308.375f, 8.6157f,    102.986f, -482.0966f, 15.9284f,    -665.4944f, 2.6482f, 18.9413f };
CVector gAmmuNationPos[2] = { 1065.6227f, -395.2242f, 14.9727f,    351.9858f, -711.2619f, 26.1681f };
CVector gBombShopPos[3] = { 1281.9111f, -99.5585f, 14.7837f,    375.7814f, -576.8163f, 26.1748f,    -1082.2286f, 59.0901f, 58.6068f };
CVector gPolicePos[3] = { 1143.875f, -675.1875f, 14.9727f,    341.4585f, -1137.1821f, 22.9809f,    -1258.9365f, -45.0972f, 58.8871f };
CVector gFireStationPos[3] = { 1111.663f, -44.9626f, 7.4925f,    -76.7527f, -435.435f, 16.1798f,    -840.2806f, -458.3479f, 11.1182f };
CVector gBlipBestPos;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		switch(gGameVersion)
		{
		case GTA_3_1_0:
			FindPlayerVehicle = (CVehicle *(__cdecl *)())0x4A10C0;
			DoPathSearch = (void (__thiscall *)(void *, unsigned char, CVector, int, CVector, CPathNode **, short *, short, CVehicle *, 
				float *, float, int))0x42B040;
			TransformRealWorldPointToRadarSpace = (void (__cdecl *)(CVector2D &, CVector2D const&))0x4A50D0;
			TransformRadarPointToScreenSpace = (void (__cdecl *)(CVector2D &, CVector2D const&))0x4A5040;
			RwIm2DGetNearScreenZ = (float (__cdecl *)())0x5A43A0;
			RwRenderStateSet = (void (__cdecl *)(unsigned int, unsigned int))0x5A43C0;
			RwD3D9SetRenderState = (int (__cdecl *)(unsigned int, unsigned int))0x5B3CF0;
			SetSpriteVertices = (void (__cdecl *)(float, float, float, float, float, float, float, float, unsigned int *,
				unsigned int *, unsigned int *, unsigned int *))0x51F070;
			RwIm2DRenderPrimitive = (void (__cdecl *)(unsigned int, RwD3D9Vertex *, unsigned int))0x5A4430;
			GetRadarTraceColour = (unsigned int (__cdecl *)(unsigned int, unsigned int))0x4A5BB0;
			ShowRadarTrace = (void (__cdecl *)(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char))
				0x4A5870;
			VehicleGetAt = (CPed *(__thiscall *)(void *, unsigned int))0x43EAF0;
			PedGetAt = (CVehicle *(__thiscall *)(void *, unsigned int))0x43EB30;
			ObjectGetAt = (CObject *(__thiscall *)(void *, unsigned int))0x43EAB0;
			DrawRadarMap = (void (__cdecl *)())0x4A6C20;
			DrawRadarMask = (void (__cdecl *)())0x4A69C0;
			InitialiseRadar = (void (__cdecl *)())0x4A3EF0;
			PlayFrontEndSound = (void (__thiscall *)(void *, unsigned short, unsigned int))0x57CC20;
			gPathfind = (void *)0x8F6754;
			gRadarMapZShift = (float *)0x5FF2B4;
			gSpriteVertices = (RwD3D9Vertex *)0x6E9168;
			gScreenWidth = (int *)0x8F436C;
			gScreenHeight = (int *)0x8F4370;
			gVehiclePool = (void **)0x9430DC;
			gPedPool = (void **)0x8F2C60;
			gObjectPool = (void **)0x880E28;
			gRwEngine = (unsigned int *)0x661228;
			gRadarRange = (float *)0x8E281C;
			gRadarBlips = (RadarBlip *)0x6ED5E0;
			gCurrLevel = (eLevelName *)0x941514;
			gAudio = (void *)0x95CDBE;
			g_TimeMs              = (unsigned int *) 0x885B48;
			AsciiToUnicode        = (void (__cdecl *)(const char *, short *)) 0x5009C0;
			PrintString           = (void (__cdecl *)(float, float, short *)) 0x500F50;
			SetFontStyle          = (void (__cdecl *)(int)                  ) 0x501DB0;
			SetScale              = (void (__cdecl *)(float, float)         ) 0x501B80;
			SetColor              = (void (__cdecl *)(unsigned int *)       ) 0x501BD0;
			SetJustifyOn          = (void (__cdecl *)()                     ) 0x501C60;
			SetDropShadowPosition = (void (__cdecl *)(int)                  ) 0x501E70;
			SetPropOn             = (void (__cdecl *)()                     ) 0x501DA0;
			CPatch::RedirectCall(0x4A42D6, ProcessPathfind);
			CPatch::RedirectCall(0x4A6C8D, DrawPathLineMask);
			CPatch::RedirectCall(0x48C4F6, InitialiseGps);
			CPatch::Nop(0x4A6B50, 5);
			break;
		case GTA_3_1_1:
			FindPlayerVehicle = (CVehicle *(__cdecl *)())0x4A11B0;
			DoPathSearch = (void (__thiscall *)(void *, unsigned char, CVector, int, CVector, CPathNode **, short *, short, CVehicle *, 
				float *, float, int))0x42B040;
			TransformRealWorldPointToRadarSpace = (void (__cdecl *)(CVector2D &, CVector2D const&))0x4A51C0;
			TransformRadarPointToScreenSpace = (void (__cdecl *)(CVector2D &, CVector2D const&))0x4A5130;
			RwIm2DGetNearScreenZ = (float (__cdecl *)())0x5A4660;
			RwRenderStateSet = (void (__cdecl *)(unsigned int, unsigned int))0x5A4680;
			RwD3D9SetRenderState = (int (__cdecl *)(unsigned int, unsigned int))0x5B3FB0;
			SetSpriteVertices = (void (__cdecl *)(float, float, float, float, float, float, float, float, unsigned int *,
				unsigned int *, unsigned int *, unsigned int *))0x51F2A0;
			RwIm2DRenderPrimitive = (void (__cdecl *)(unsigned int, RwD3D9Vertex *, unsigned int))0x5A46F0;
			GetRadarTraceColour = (unsigned int (__cdecl *)(unsigned int, unsigned int))0x4A5CA0;
			ShowRadarTrace = (void (__cdecl *)(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char))
				0x4A5960;
			VehicleGetAt = (CPed *(__thiscall *)(void *, unsigned int))0x43EAF0;
			PedGetAt = (CVehicle *(__thiscall *)(void *, unsigned int))0x43EB30;
			ObjectGetAt = (CObject *(__thiscall *)(void *, unsigned int))0x43EAB0;
			DrawRadarMap = (void (__cdecl *)())0x4A6D10;
			DrawRadarMask = (void (__cdecl *)())0x4A6AB0;
			InitialiseRadar = (void (__cdecl *)())0x4A3FE0;
			PlayFrontEndSound = (void (__thiscall *)(void *, unsigned short, unsigned int))0x57CF70;
			gPathfind = (void *)0x8F690C;
			gRadarMapZShift = (float *)0x5FF09C;
			gSpriteVertices = (RwD3D9Vertex *)0x6E9168;
			gScreenWidth = (int *)0x8F4420;
			gScreenHeight = (int *)0x8F4424;
			gVehiclePool = (void **)0x943294;
			gPedPool = (void **)0x8F2D14;
			gObjectPool = (void **)0x880DD8;
			gRwEngine = (unsigned int *)0x661228;
			gRadarRange = (float *)0x8E27CC;
			gRadarBlips = (RadarBlip *)0x6ED5E0;
			gCurrLevel = (eLevelName *)0x9416CC;
			gAudio = (void *)0x95CF76;
			g_TimeMs              = (unsigned int *) 0x885AF8;
			AsciiToUnicode        = (void (__cdecl *)(const char *, short *)) 0x500AA0;
			PrintString           = (void (__cdecl *)(float, float, short *)) 0x501030;
			SetFontStyle          = (void (__cdecl *)(int)                  ) 0x501E90;
			SetScale              = (void (__cdecl *)(float, float)         ) 0x501C60;
			SetColor              = (void (__cdecl *)(unsigned int *)       ) 0x501CB0;
			SetJustifyOn          = (void (__cdecl *)()                     ) 0x501D40;
			SetDropShadowPosition = (void (__cdecl *)(int)                  ) 0x501F50;
			SetPropOn             = (void (__cdecl *)()                     ) 0x501E80;
			CPatch::RedirectCall(0x4A43C6, ProcessPathfind);
			CPatch::RedirectCall(0x4A6D7D, DrawPathLineMask);
			CPatch::RedirectCall(0x48C5F6, InitialiseGps);
			CPatch::Nop(0x4A6C40, 5);
			break;
		default:
			MessageBox(0, "Unknown GTA 3 version", "gps.asi", MB_OK);
		}
	}
	return TRUE;
}

void PrintGpsText()
{
	unsigned int color;
	CVector2D point;
	short text[128];
	if(*g_TimeMs < gGpsTextTimer + GPS_TEXT_TIME)
	{
		switch(gCurrentGpsMode)
		{
		case GPS_MODE_DISABLED:
			AsciiToUnicode("GPS Mode: Disabled", text);
			break;
		case GPS_MODE_DEFAULT:
			AsciiToUnicode("GPS Mode: Default", text);
			break;
		case GPS_MODE_AMMUNATION:
			if(*gCurrLevel != LEVEL_SHORESIDE)
				AsciiToUnicode("GPS Mode: Ammu-Nation", text);
			else
				AsciiToUnicode("GPS Error: can't find Ammu-Nation", text);
			break;
		case GPS_MODE_BOMBSHOP:
			AsciiToUnicode("GPS Mode: Bomb Shop", text);
			break;
		case GPS_MODE_FIRESTATION:
			AsciiToUnicode("GPS Mode: Fire Station", text);
			break;
		case GPS_MODE_HOSPITAL:
			AsciiToUnicode("GPS Mode: Hospital", text);
			break;
		case GPS_MODE_PAYNSPRAY:
			AsciiToUnicode("GPS Mode: Pay'N'Spray", text);
			break;
		case GPS_MODE_POLICE:
			AsciiToUnicode("GPS Mode: Police", text);
			break;
		case GPS_MODE_SAFEHOUSE:
			AsciiToUnicode("GPS Mode: Safehouse", text);
			break;
		default:
			AsciiToUnicode("GPS Error: Unknown command", text);
		}
		color = 0xFFFFFFFF;
		SetFontStyle(0);
		SetScale(0.7f, 1.0f);
		SetColor(&color);
		SetJustifyOn();
		SetDropShadowPosition(1);
		SetPropOn();
		point.x = -1.0f;
		point.y = -1.0f;
		TransformRadarPointToScreenSpace(point, point);
		PrintString(point.x, point.y + 10.0f, text);
		SetDropShadowPosition(0);
		SetFontStyle(0);
	}
}

PathLineInfo *GetPlaceInfo(PathLineInfo *info, CVehicle *playerVeh, char mode, eLevelName level)
{
	CPlaceable *entity;
	RadarBlip *bestBlip;
	CVector blipPos;
	float distance, newDistance;
	unsigned int color;
	switch(mode)
	{
	case GPS_MODE_DEFAULT:
		entity = NULL;
		bestBlip = NULL;
		distance = 9999800001.99f;
		for(RadarBlip *blip = gRadarBlips; blip != &gRadarBlips[32]; blip++)
		{
			if(blip->m_bActive && !blip->m_wBlipSprite)
			{
				if(blip->m_dwBlipType > 0 && blip->m_dwBlipType < 4)
				{
					switch(blip->m_dwBlipType)
					{
					case BLIP_CAR: // 1
						entity = VehicleGetAt(*gVehiclePool, blip->m_dwEntityHandle);
						break;
					case BLIP_PED: // 2
						entity = PedGetAt(*gPedPool, blip->m_dwEntityHandle);
						if(entity && IS_PED_IN_CAR(entity))
							entity = GET_PED_CAR(entity);
						break;
					case BLIP_OBJECT: // 3
						entity = ObjectGetAt(*gObjectPool, blip->m_dwEntityHandle);
					}
					if(entity)
					{
						blipPos.x = entity->m_sCoords.m_sMatrix.pos.x;
						blipPos.y = entity->m_sCoords.m_sMatrix.pos.y;
						blipPos.z = entity->m_sCoords.m_sMatrix.pos.z;
					}
					else continue;
				}
				else
				{
					blipPos.x = blip->m_v3dPosn.x;
					blipPos.y = blip->m_v3dPosn.y;
					blipPos.z = blip->m_v3dPosn.z;
				}
				newDistance = GetSquaredDistanceBetweenPoints(blipPos, playerVeh->m_sCoords.m_sMatrix.pos);
				if(newDistance < distance)
				{
					distance = newDistance;
					gBlipBestPos.x = blipPos.x;
					gBlipBestPos.y = blipPos.y;
					gBlipBestPos.z = blipPos.z;
					bestBlip = blip;
				}
			}
		}
		if(bestBlip)
		{
			color = GetRadarTraceColour(bestBlip->m_dwBlipColour, bestBlip->m_bBlipBrightness);
			BYTEn(info->color, 0) = BYTEn(color, 3);
			BYTEn(info->color, 1) = BYTEn(color, 2);
			BYTEn(info->color, 2) = BYTEn(color, 1);
			BYTEn(info->color, 3) = 255;
			info->targetPoint = &gBlipBestPos;
		}
		else
		{
			info->targetPoint = NULL;
			info->color = 0;
		}
		break;
	case GPS_MODE_SAFEHOUSE:
		info->targetPoint = &gSafehousePos[level - 1];
		info->color = 0xFFB07A81;
		break;
	case GPS_MODE_PAYNSPRAY:
		info->targetPoint = &gPayNSprayPos[level - 1];
		info->color = 0xFF00C7B1;
		break;
	case GPS_MODE_AMMUNATION:
		if(level != LEVEL_SHORESIDE)
		{
			info->targetPoint = &gAmmuNationPos[level - 1];
			info->color = 0xFF613CC0;
		}
		else
		{
			info->targetPoint = NULL;
			info->color = 0;
		}
		break;
	case GPS_MODE_BOMBSHOP:
		info->targetPoint = &gBombShopPos[level - 1];
		info->color = 0xFF738C34;
		break;
	case GPS_MODE_HOSPITAL:
		info->targetPoint = &gHospitalPos[level - 1];
		info->color = 0xFFD99600;
		break;
	case GPS_MODE_POLICE:
		info->targetPoint = &gPolicePos[level - 1];
		info->color = 0xFFEFD8C8;
		break;
	case GPS_MODE_FIRESTATION:
		info->targetPoint = &gFireStationPos[level - 1];
		info->color = 0xFF0B69EB;
		break;
	default:
		info->targetPoint = NULL;
		info->color = 0;
	}
	return info;
}

void ProcessModeSwitch()
{
	unsigned char oldMode = gCurrentGpsMode;
	if(HIBYTE(GetKeyState(GPS_KEY_SWITCH)) == 0xFF)
	{
		if(HIBYTE(GetKeyState(GPS_KEY_DISABLE)) == 0xFF)
			gCurrentGpsMode = GPS_MODE_DISABLED;
		else if(HIBYTE(GetKeyState(GPS_KEY_DEFAULT)) == 0xFF)
			gCurrentGpsMode = GPS_MODE_DEFAULT;
		else if(HIBYTE(GetKeyState(GPS_KEY_AMMUNATION)) == 0xFF)
			gCurrentGpsMode = GPS_MODE_AMMUNATION;
		else if(HIBYTE(GetKeyState(GPS_KEY_BOMBSHOP)) == 0xFF)
			gCurrentGpsMode = GPS_MODE_BOMBSHOP;
		else if(HIBYTE(GetKeyState(GPS_KEY_FIRESTATION)) == 0xFF)
			gCurrentGpsMode = GPS_MODE_FIRESTATION;
		else if(HIBYTE(GetKeyState(GPS_KEY_HOSPITAL)) == 0xFF)
			gCurrentGpsMode = GPS_MODE_HOSPITAL;
		else if(HIBYTE(GetKeyState(GPS_KEY_PAYNSPRAY)) == 0xFF)
			gCurrentGpsMode = GPS_MODE_PAYNSPRAY;
		else if(HIBYTE(GetKeyState(GPS_KEY_POLICE)) == 0xFF)
			gCurrentGpsMode = GPS_MODE_POLICE;
		else if(HIBYTE(GetKeyState(GPS_KEY_SAFEHOUSE)) == 0xFF)
			gCurrentGpsMode = GPS_MODE_SAFEHOUSE;
	}
	else if(*g_TimeMs > gGpsScrollTimer + GPS_KEY_SCROLL_DELAY)
	{
		if ((HIBYTE(GetKeyState(GPS_KEY_DOWN)) == 0xFF) || (*(BYTE*)(0x6F0370) == 0xFFu)) //DPADUP
		{
			gCurrentGpsMode++;
			if(gCurrentGpsMode > GPS_MODE_DISABLED)
				gCurrentGpsMode = GPS_MODE_DEFAULT;
			gGpsScrollTimer = *g_TimeMs;
		}
		else if(HIBYTE(GetKeyState(GPS_KEY_UP)) == 0xFF)
		{
			gCurrentGpsMode--;
			if(gCurrentGpsMode < GPS_MODE_DEFAULT)
				gCurrentGpsMode = GPS_MODE_DISABLED;
			gGpsScrollTimer = *g_TimeMs;
		}
	}
	if(oldMode != gCurrentGpsMode)
	{
		gGpsTextTimer = *g_TimeMs;
		if(gCurrentGpsMode != GPS_MODE_DISABLED && *g_TimeMs > gGpsAudioTimer + GPS_AUDIO_DELAY)
		{
			PlayFrontEndSound(gAudio, 149, 0);
			gGpsAudioTimer = *g_TimeMs;
		}
	}
	PrintGpsText();
}

void InitialiseGps()
{
	InitialiseRadar();
	gCurrentGpsMode = GPS_MODE_DEFAULT;
	gGpsTextTimer = 0;
	gGpsAudioTimer = 0;
}

void DrawPlaceMarker(PathLineInfo *info)
{
	CVector2D posn;
	TransformRealWorldPointToRadarSpace(posn, *info->targetPoint2d);
	if(posn.y * posn.y + posn.x * posn.x < 1.0)
	{
		TransformRadarPointToScreenSpace(posn, posn);
		ShowRadarTrace(posn.x, posn.y, 3, BYTEn(info->color, 0), BYTEn(info->color, 1), BYTEn(info->color, 2), 255);
	}
}

void DrawPathLineMask()
{
	DrawRadarMask();
	unsigned int color;
	CVector2D points[2];
	if(gCurrentGpsMode != GPS_MODE_DISABLED)
	{
		color = 0;
		points[0].x = 1;
		points[0].y = 1;
		points[1].x = -1;
		points[1].y = -1;
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
		TransformRadarPointToScreenSpace(points[0], points[0]);
		TransformRadarPointToScreenSpace(points[1], points[1]);
		SetSpriteVertices(points[1].x, (float)*gScreenHeight, points[1].x, 0.0, 0.0, (float)*gScreenHeight, 0.0, 0.0, &color,
			&color, &color, &color);
		RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, gSpriteVertices, 4);
		SetSpriteVertices(points[0].x, points[0].y, points[0].x, 0.0, points[1].x, points[0].y, points[1].x, 0.0, &color,
			&color, &color, &color);
		RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, gSpriteVertices, 4);
		SetSpriteVertices(points[0].x, (float)*gScreenHeight, points[0].x, points[1].y, points[1].x, (float)*gScreenHeight,
			points[1].x, points[1].y, &color, &color, &color, &color);
		RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, gSpriteVertices, 4);
		SetSpriteVertices((float)*gScreenWidth, (float)*gScreenHeight, (float)*gScreenWidth, 0.0, points[0].x, (float)*gScreenHeight,
			points[0].x, 0.0, &color, &color, &color, &color);
		RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, gSpriteVertices, 4);
	}
	RwD3D9SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
}

void DrawLine(CVector2D const&a, CVector2D const&b, float width, unsigned int color)
{
	CVector2D point[4], shift[2], dir;
	width /= 2.0;
	dir.x = b.x - a.x;
	dir.y = b.y - a.y;
	float angle = atan2(dir.y, dir.x);
	shift[0].x = cos(angle - 1.5707963f) * width;
	shift[0].y = sin(angle - 1.5707963f) * width;
	shift[1].x = cos(angle + 1.5707963f) * width;
	shift[1].y = sin(angle + 1.5707963f) * width;
	point[0].x = a.x + shift[1].x;
	point[0].y = a.y + shift[1].y;
	point[1].x = b.x + shift[1].x;
	point[1].y = b.y + shift[1].y;
	point[2].x = a.x + shift[0].x;
	point[2].y = a.y + shift[0].y;
	point[3].x = b.x + shift[0].x;
	point[3].y = b.y + shift[0].y;
	float oldZ = RwIm2DGetNearScreenZ();
	RwIm2DSetNearScreenZ(oldZ + *gRadarMapZShift);
	SetSpriteVertices(point[0].x, point[0].y, point[1].x, point[1].y, point[2].x, point[2].y, point[3].x, point[3].y, &color,
		&color, &color, &color);
	RwIm2DSetNearScreenZ(oldZ);
	RwIm2DRenderPrimitive(rwPRIMTYPETRIFAN, gSpriteVertices, 4);
}

void DrawPathFindLine(RwV2d *points, unsigned int numPoints, float width, unsigned int color)
{
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	for(unsigned int i = 0; i < (numPoints - 1); i++)
	{
		if(!(i & 1))
			TransformRealWorldPointToRadarSpace(points[i], gapPathNodes[i]->m_v2dPoint);
		TransformRealWorldPointToRadarSpace(points[i+1], gapPathNodes[i+1]->m_v2dPoint);
		if(IsLineInsideRadar(points[i], points[i+1]))
		{
			if(!(i & 1))
				TransformRadarPointToScreenSpace(points[i], points[i]);
			TransformRadarPointToScreenSpace(points[i+1], points[i+1]);
			DrawLine(points[i], points[i+1], width, color);
		}
		else TransformRadarPointToScreenSpace(points[i+1], points[i+1]);
	}
}

void ProcessPathfind()
{
	DrawRadarMap();
	PathLineInfo info;
	CVehicle *playerCar = FindPlayerVehicle();
	if(playerCar)
	{
		ProcessModeSwitch();
		if(gCurrentGpsMode != GPS_MODE_DISABLED)
		{
			GetPlaceInfo(&info, playerCar, gCurrentGpsMode, *gCurrLevel);
			if(info.targetPoint)
			{
				DoPathSearch(gPathfind, PATHNODE_VEHICLE_PATH, playerCar->m_sCoords.m_sMatrix.pos, -1, *info.targetPoint, gapPathNodes,
					&gwPathNodesCount, MAX_POINTS, playerCar, NULL, 999999.0, -1);
				if(gwPathNodesCount > 1)
					DrawPathFindLine(gaPathPoints, gwPathNodesCount, LINE_WIDTH /(*gRadarRange), info.color);
				if(gCurrentGpsMode != GPS_MODE_DEFAULT)
					DrawPlaceMarker(&info);
			}
		}
	}
}

void RwIm2DSetNearScreenZ(float z)
{
	*(float *)(*gRwEngine + 0x18) = z;
}

float GetSquaredDistanceBetweenPoints(CVector const&a, CVector const&b)
{
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	float dz = b.z - a.z;
	return dz * dz + dy * dy + dx * dx;
}

bool IsLineInsideRadar(CVector2D const&a, CVector2D const&b)
{
	return a.x * a.x + a.y * a.y <= 1.44 || b.x * b.x + b.y * b.y <= 1.44;
}