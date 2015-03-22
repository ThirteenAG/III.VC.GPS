/* Organized & Written by DK (c) */
/* Adaptation for VC by ThirteenAG */

#include "..\includes\CPatch.h"
#include <math.h>
#include <stdint.h>

#define GTA_VC_1_0     0xD9000001

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

#define IS_PED_IN_CAR(ped) (*(unsigned char *)((unsigned int)ped + 0x3AC))
#define GET_PED_CAR(ped) (*(CVehicle **)((unsigned int)ped + 0x3A8))

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
	LEVEL_BEACH = 1,
	LEVEL_MAINLAND,
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

struct CVector2DShort
{
	signed short x;
	signed short y;
};

struct CVectorShort
{
	signed short x;
	signed short y;
	signed short z;
};

struct CPathNode //size 0x14
{
	short unk1;
	short unk2;
	union{
		CVector2DShort     m_v2dPoint;
		CVectorShort       m_vPoint;
	};
	char unk3;
	char unk4;
	short nextPointIndex;
	char unk5;
	char unk6;
	char flags; 
	char unk7;
	char unk8;
	char unk9;
};

class CPathNodeIII
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
	unsigned int   m_unk;
	CVector        m_v3dPosn;
	unsigned short m_wIndex;
	unsigned char  m_bBlipBrightness;
	unsigned char  m_bActive;
		unsigned int   m_unk2;
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
//void (__cdecl *TransformRealWorldPointToRadarSpace)(CVector2D &,CVector2D const&);
//void (__cdecl *TransformRadarPointToScreenSpace)(CVector2D &,CVector2D const&);
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
void(__cdecl *ShowRadarTrace)(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned __int8);
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
CPathNodeIII *gapPathNodes3[MAX_POINTS];
short gwPathNodesCount;
RwV2d gaPathPoints[MAX_POINTS];
char gCurrentGpsMode;
unsigned int gGpsTextTimer;
unsigned int gGpsAudioTimer;
unsigned int gGpsScrollTimer;

CVector gPayNSprayPos[] = { -7.80468f, -1257.64f, 10.8187f, 325.083f, 431.137f, 11.5872f, -874.696f, -116.696f, 11.9875f, -1007.95f, -841.779f, 8.59442f, -910.002f, -1264.71f, 12.4923f };
CVector gHospitalPos[] = { -114.858f, -976.137f, 9.96831f, -818.822f, 1073.68f, 17.8946f, -866.415f, -474.236f, 8.82334f };
CVector gSafehousePos[] = { 219.3f, -1273.5f, 11.5f, -378.3f, -597.2f, 25.8f, -1033.8f, -846.9f, 13.0f, -992.7f, 193.4f, 11.4f, -664.1f, -1476.3f, 13.8f, -878.5f, -575.1f, 11.2f, -1059.6f, -282.2f, 11.2f, 0.0f, 957.9f, 11.1f, 499.5f, -66.9f, 11.4f, -562.8f, 699.7f, 20.6f, 99.5f, -1468.5f, 9.9f, 428.4f, 605.9f, 12.2f, 304.5f, 376.3f, 12.7f, -834.8f, 1306.9f, 11.0f, 88.5f, -804.7f, 11.2f, 14.0f, -1500.7f, 12.7f, 531.4f, 1273.7f, 17.6f };
CVector gAmmuNationPos[] = { -66.149f, -1478.8f, 12.3526f, 480.705f, 1124.4f, 27.1471f, -674.878f, 1190.41f, 13.2734f };
CVector gBombShopPos[] = { -1161.32f, -1402.47f, 12.798f };
CVector gPolicePos[] = { 399.871f, -469.391f, 8.90852f, 500.648f, 513.397f, 14.8978f, -845.285f, -668.411f, 21.271f, -666.96f, 759.279f, 13.0088f };
CVector gFireStationPos[] = { -692.587f, 958.558f, 9.77301f, -1802.05f, -889.964f, 27.5194f, -1506.82f, -463.503f, 27.7881f };
CVector gBlipBestPos;


void TransformRadarPointToScreenSpace(CVector2D & a1, CVector2D const& a2)
{
	if (*(BYTE*)0x869665)
	{
		a1.x = (*(float*)0x68FD0C * *(float*)0x869670 * a2.x + *(float*)0x68FD10 * *(float*)0x869670 + *(float*)0x869674)
			* (float)*gScreenWidth
			* **(float**)0x4C1B1A;
		a1.y = (*(float*)0x869678 - *(float*)0x68FD1C * *(float*)0x869670 - *(float*)0x68FD18 * *(float*)0x869670 * a2.y)
			* (float)*gScreenHeight
			* **(float**)0x4C1B46;
	}
	else
	{
		float v1 = (float)*gScreenWidth * **(float**)0x4C1B1A * *(float*)0x68FD24;
		a1.x = a2.x * v1 * *(float*)0x68FD28 + v1 * *(float*)0x68FD28 + *(float*)0x68FD2C;
		a1.y = *(float*)0x68FD30 * (float)*gScreenHeight * **(float**)0x4C1B46 * *(float*)0x68FD28
			+ (float)*gScreenHeight
			- *(float*)0x68FD34 * (float)*gScreenHeight * **(float**)0x4C1B46
			- a2.y * *(float*)0x68FD30 * (float)*gScreenHeight * **(float**)0x4C1B46 * *(float*)0x68FD28;
	}
}

void TransformRealWorldPointToRadarSpace(CVector2D & a1, CVector2D const& a2)
{
	float v9 = *(float *)0x68FD40 / *(float *)0x974BEC;
	float v10 = (a2.x - *(float *)0x704734) * v9;
	float v11 = (a2.y - *(float *)0x704738) * v9;
	a1.x = *(float *)0x70483C * v11 + *(float *)0x704840 * v10;
	a1.y = *(float *)0x704840 * v11 - *(float *)0x70483C * v10;
}

float GetDistance(CVector *v1, CVector *v2)
{
	CVector v3;
	v3.x = v2->x - v1->x;
	v3.y = v2->y - v1->y;
	v3.z = v2->z - v1->z;
	return sqrt(v3.x * v3.x + v3.y * v3.y + v3.z * v3.z);
}

CVector * GetCamPos()
{
	return (CVector *)(0x7E4688 + 0x7D8);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpReserved)
{
	if(reason == DLL_PROCESS_ATTACH)
	{
		switch(gGameVersion)
		{
		case GTA_VC_1_0:
			FindPlayerVehicle = (CVehicle *(__cdecl *)())0x4BC1E0; //+
			DoPathSearch = (void (__thiscall *)(void *, unsigned char, CVector, int, CVector, CPathNode **, short *, short, CVehicle *, float *, float, int))0x439070; //+
			//TransformRealWorldPointToRadarSpace = (void(__cdecl *)(CVector2D &, CVector2D const&))0x4A50D0;
			//TransformRadarPointToScreenSpace = (void(__cdecl *)(CVector2D &, CVector2D const&))0x4A5040;
			RwIm2DGetNearScreenZ = (float(__cdecl *)())0x649B80;//+
			RwRenderStateSet = (void(__cdecl *)(unsigned int, unsigned int))0x649BA0;//+
			RwD3D9SetRenderState = (int(__cdecl *)(unsigned int, unsigned int))0x6582A0;///+
			SetSpriteVertices = (void(__cdecl *)(float, float, float, float, float, float, float, float, unsigned int *, unsigned int *, unsigned int *, unsigned int *))0x5781C0;//+
			RwIm2DRenderPrimitive = (void(__cdecl *)(unsigned int, RwD3D9Vertex *, unsigned int))0x649C10;//+
			GetRadarTraceColour = (unsigned int(__cdecl *)(unsigned int, unsigned int))0x4C3050;//+
			ShowRadarTrace = (void(__cdecl *)(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned __int8)) 0x4C32F0;//+
			VehicleGetAt = (CPed *(__thiscall *)(void *, unsigned int))0x451C70;//+
			PedGetAt = (CVehicle *(__thiscall *)(void *, unsigned int))0x451CB0;//+
			ObjectGetAt = (CObject *(__thiscall *)(void *, unsigned int))0x451C30;//+
			DrawRadarMap = (void(__cdecl *)())0x4C17C0;//+
			DrawRadarMask = (void(__cdecl *)())0x4C1A20;//+
			InitialiseRadar = (void(__cdecl *)())0x4C6200;//+
			PlayFrontEndSound = (void(__thiscall *)(void *, unsigned short, unsigned int))0x5F9960;//+
			gPathfind = (void *)0x9B6E5C;//+
			gRadarMapZShift = (float *)0x699530;//+
			gSpriteVertices = (RwD3D9Vertex *)0x7D4040;//+?
			gScreenWidth = (int *)0x9B48E4;//+
			gScreenHeight = (int *)0x9B48E8;//+
			gVehiclePool = (void **)0xA0FDE4;//+
			gPedPool = (void **)0x97F2AC;//+
			gObjectPool = (void **)0x94DBE0;//+
			gRwEngine = (unsigned int *)0x7870C0; //+
			gRadarRange = (float *)0x974BEC;
			gRadarBlips = (RadarBlip *)0x7D7D38;//+
			gCurrLevel = (eLevelName *)0xA0D9AC;//+
			gAudio = (void *)0xA10B8A;//+
			g_TimeMs = (unsigned int *)0x974B2C;//+
			AsciiToUnicode = (void(__cdecl *)(const char *, short *)) 0x552500;//+
			PrintString = (void(__cdecl *)(float, float, short *)) 0x551040;//+
			SetFontStyle = (void(__cdecl *)(int)) 0x54FFE0;//+
			SetScale = (void(__cdecl *)(float, float)) 0x550230;//+
			SetColor = (void(__cdecl *)(unsigned int *)) 0x550170;//+
			SetJustifyOn = (void(__cdecl *)()) 0x550150;//+
			SetDropShadowPosition = (void(__cdecl *)(int)) 0x54FF20;//+
			SetPropOn = (void(__cdecl *)()) 0x550020;//+
			CPatch::RedirectCall(0x4C5D4B, ProcessPathfind);//+
			CPatch::RedirectCall(0x4C17C5, DrawPathLineMask);//+
			CPatch::RedirectCall(0x4A4896, InitialiseGps);//+
			CPatch::Nop(0x4C1D49, 5);//+
			break;
		default:
			MessageBox(0, "Unknown GTA VC version", "gps.asi", MB_OK);
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
			AsciiToUnicode("GPS Mode: Ammu-Nation", text);
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
		point.x = -1.0;
		point.y = -1.0;
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
	float Dist;
	float defDist;
	int point;
	point = 0;
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
		defDist = GetDistance(GetCamPos(), &gSafehousePos[0]);
		for (auto i = 0; i < sizeof(gSafehousePos) / sizeof(gSafehousePos[0]); i++)
		{
			Dist = GetDistance(GetCamPos(), &gSafehousePos[i]);

			if (defDist > Dist)
			{
				defDist = Dist;
				point = i;
			}
		}
		info->targetPoint = &gSafehousePos[point];
		info->color = 0xFFB07A81;
		break;
	case GPS_MODE_PAYNSPRAY:
		defDist = GetDistance(GetCamPos(), &gPayNSprayPos[0]);
		for (auto i = 0; i < sizeof(gPayNSprayPos) / sizeof(gPayNSprayPos[0]); i++)
		{
			Dist = GetDistance(GetCamPos(), &gPayNSprayPos[i]);

			if (defDist > Dist)
			{
				defDist = Dist;
				point = i;
			}
		}
		info->targetPoint = &gPayNSprayPos[point];
		info->color = 0xFF00C7B1;
		break;
	case GPS_MODE_AMMUNATION:
		defDist = GetDistance(GetCamPos(), &gAmmuNationPos[0]);
		for (auto i = 0; i < sizeof(gAmmuNationPos) / sizeof(gAmmuNationPos[0]); i++)
		{
			Dist = GetDistance(GetCamPos(), &gAmmuNationPos[i]);

			if (defDist > Dist)
			{
				defDist = Dist;
				point = i;
			}
		}
		info->targetPoint = &gAmmuNationPos[point];
		info->color = 0xFF613CC0;
		break;
	case GPS_MODE_BOMBSHOP:
		defDist = GetDistance(GetCamPos(), &gBombShopPos[0]);
		for (auto i = 0; i < sizeof(gBombShopPos) / sizeof(gBombShopPos[0]); i++)
		{
			Dist = GetDistance(GetCamPos(), &gBombShopPos[i]);

			if (defDist > Dist)
			{
				defDist = Dist;
				point = i;
			}
		}
		info->targetPoint = &gBombShopPos[point];
		info->color = 0xFF738C34;
		break;
	case GPS_MODE_HOSPITAL:
		defDist = GetDistance(GetCamPos(), &gHospitalPos[0]);
		for (auto i = 0; i < sizeof(gHospitalPos) / sizeof(gHospitalPos[0]); i++)
		{
			Dist = GetDistance(GetCamPos(), &gHospitalPos[i]);

			if (defDist > Dist)
			{
				defDist = Dist;
				point = i;
			}
		}
		info->targetPoint = &gHospitalPos[point];
		info->color = 0xFFD99600;
		break;
	case GPS_MODE_POLICE:
		defDist = GetDistance(GetCamPos(), &gPolicePos[0]);
		for (auto i = 0; i < sizeof(gPolicePos) / sizeof(gPolicePos[0]); i++)
		{
			Dist = GetDistance(GetCamPos(), &gPolicePos[i]);

			if (defDist > Dist)
			{
				defDist = Dist;
				point = i;
			}
		}
		info->targetPoint = &gPolicePos[point];
		info->color = 0xFFEFD8C8;
		break;
	case GPS_MODE_FIRESTATION:
		defDist = GetDistance(GetCamPos(), &gFireStationPos[0]);
		for (auto i = 0; i < sizeof(gFireStationPos) / sizeof(gFireStationPos[0]); i++)
		{
			Dist = GetDistance(GetCamPos(), &gFireStationPos[i]);

			if (defDist > Dist)
			{
				defDist = Dist;
				point = i;
			}
		}
		info->targetPoint = &gFireStationPos[point];
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
		if ((HIBYTE(GetKeyState(GPS_KEY_DOWN)) == 0xFF) || (*(BYTE*)(0x7DBCC0) == 0xFFu)) //DPADUP
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
		ShowRadarTrace(posn.x, posn.y, 3, BYTEn(info->color, 0), BYTEn(info->color, 1), BYTEn(info->color, 2), 255, 0);
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
		CVector2D temp, temp2;

		temp.x = gapPathNodes[i]->m_v2dPoint.x * 0.125f;
		temp.y = gapPathNodes[i]->m_v2dPoint.y * 0.125f;
		temp2.x = gapPathNodes[i + 1]->m_v2dPoint.x * 0.125f;
		temp2.y = gapPathNodes[i + 1]->m_v2dPoint.y * 0.125f;

		if(!(i & 1))
			TransformRealWorldPointToRadarSpace(points[i], temp);
		TransformRealWorldPointToRadarSpace(points[i + 1], temp2);
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
					&gwPathNodesCount, MAX_POINTS, playerCar, NULL, 999999.0f, -1);
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
	return a.x * a.x + a.y * a.y <= 0.90 || b.x * b.x + b.y * b.y <= 0.95;
}