#pragma once
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
#define GPS_KEY_SCROLL_DELAY 150

#define gGameVersion (*(unsigned int *)0x60F428)

#define IS_PED_IN_CAR(ped) (*(unsigned char *)((unsigned int)ped + 0x3AC))
#define GET_PED_CAR(ped) (*(CVehicle **)((unsigned int)ped + 0x3A8))

#define BYTEn(x, n)   (*((BYTE*)&(x)+n))

enum eBlipsIII
{
	III_RADAR_SPRITE_NONE,
	III_RADAR_SPRITE_ASUKA,
	III_RADAR_SPRITE_BOMB,
	III_RADAR_SPRITE_CAT,
	III_RADAR_SPRITE_CENTRE, //DISABLED
	III_RADAR_SPRITE_COPCAR,
	III_RADAR_SPRITE_DON,
	III_RADAR_SPRITE_EIGHT,
	III_RADAR_SPRITE_EL,
	III_RADAR_SPRITE_ICE,
	III_RADAR_SPRITE_JOEY,
	III_RADAR_SPRITE_KENJI,
	III_RADAR_SPRITE_LIZ,
	III_RADAR_SPRITE_LUIGI,
	III_RADAR_SPRITE_NORTH,
	III_RADAR_SPRITE_RAY,
	III_RADAR_SPRITE_SAL,
	III_RADAR_SPRITE_SAVEHOUSE,
	III_RADAR_SPRITE_SPRAY,
	III_RADAR_SPRITE_TONY,
	III_RADAR_SPRITE_WEAPON
};

enum eBlipsVC
{
	RADAR_SPRITE_NONE,
	RADAR_SPRITE_CENTRE, //DISABLED
	RADAR_SPRITE_MAP_HERE,
	RADAR_SPRITE_NORTH,
	RADAR_SPRITE_AVERY,
	RADAR_SPRITE_BIKER,
	RADAR_SPRITE_CORTEZ,
	RADAR_SPRITE_DIAZ,
	RADAR_SPRITE_KENT,
	RADAR_SPRITE_LAWYER,
	RADAR_SPRITE_PHIL,
	RADAR_SPRITE_BIKERS,
	RADAR_SPRITE_BOATYARD,
	RADAR_SPRITE_MALIBUCLUB,
	RADAR_SPRITE_CUBANS,
	RADAR_SPRITE_FILM,
	RADAR_SPRITE_GUN,
	RADAR_SPRITE_HAITIANS,
	RADAR_SPRITE_HARDWARE,
	RADAR_SPRITE_SAVEHOUSE,
	RADAR_SPRITE_STRIPCLUB,
	RADAR_SPRITE_ICE,
	RADAR_SPRITE_KCABS,
	RADAR_SPRITE_LOVEFIST,
	RADAR_SPRITE_PRINTWORKS,
	RADAR_SPRITE_PROPERTY,
	RADAR_SPRITE_SUNYARD,
	RADAR_SPRITE_SPRAY,
	RADAR_SPRITE_TSHIRT,
	RADAR_SPRITE_TOMMY,
	RADAR_SPRITE_PHONE,
	RADAR_SPRITE_RADIO_WILDSTYLE,
	RADAR_SPRITE_RADIO_FLASH,
	RADAR_SPRITE_RADIO_KCHAT,
	RADAR_SPRITE_RADIO_FEVER,
	RADAR_SPRITE_RADIO_VROCK,
	RADAR_SPRITE_RADIO_VCPR,
	RADAR_SPRITE_RADIO_ESPANTOSO,
	RADAR_SPRITE_RADIO_EMOTION,
	RADAR_SPRITE_RADIO_WAVE
};

const char* BlipsGxtTableVC[] = { "LG_35", "DUMMY", "LG_01", "DUMMY", "LG_02", "LG_03", "LG_04", "LG_05", "LG_06", "LG_07", "LG_08", "LG_03", "LG_09", "LG_10",
"LG_11", "LG_12", "LG_13", "LG_14", "LG_15", "LG_16", "LG_37", "LG_17", "LG_18", "LG_19", "LG_20", "LG_21", "LG_36", "LG_22", "LG_23", "LG_24", "LG_25", "LG_26",
"LG_27", "LG_28", "LG_29", "LG_30", "LG_31", "LG_32", "LG_33", "LG_34"};

const char* BlipsGxtTableIII[] = { "Destination", "Asuka Kasen", "8-Ball's Garage", "Catalina", "DUMMY", "DUMMY", "Donald Love", "8-Ball", "ElBurro (Diablo)", "D-Ice (Hood)",
"Joey Leone", "Kenji Kasen", "King Courtney (Yardie)", "Luigi", "DUMMY", "Ray Machowski", "Salvatore Leone", "Safehouse", "Pay 'n' Spray", "Toni Cipriani", "Ammu-Nation" };

enum eGpsKeys
{
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
	union {
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
	union {
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
	CVector		   m_vecInitPos;
	CVector        m_vecCurPos;
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

struct RadarBlipIII
{
	unsigned int   m_dwBlipColour;
	unsigned int   m_dwBlipType;
	unsigned int   m_dwEntityHandle;
	CVector2D      m_vecInitPos;
	CVector        m_vecCurPos;
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
	union {
		CVector *targetPoint;
		CVector2D *targetPoint2d;
	};
	unsigned int color;
};

CPathNode *gapPathNodes[MAX_POINTS];
CPathNodeIII *gapPathNodes3[MAX_POINTS];
short gwPathNodesCount;
RwV2d gaPathPoints[MAX_POINTS];
char gCurrentGpsMode;
unsigned int gGpsTextTimer;
unsigned int gGpsAudioTimer;
unsigned int gGpsScrollTimer;
CVector gBlipBestPos;

CVehicle *(__cdecl *FindPlayerVehicle)();
bool (*IsPlayerOnAMission)() = (bool(*)()) 0x44FE30;
void(__thiscall *DoPathSearch)(void *, unsigned char, CVector, int, CVector, CPathNode **, short *, short, CVehicle *, float *, float, int);
void (__cdecl *TransformRealWorldPointToRadarSpaceIII)(CVector2D &,CVector2D const&);
void (__cdecl *TransformRadarPointToScreenSpaceIII)(CVector2D &,CVector2D const&);
float(__cdecl *RwIm2DGetNearScreenZ)();
void(__cdecl *RwRenderStateSet)(unsigned int, unsigned int);
int(__cdecl *RwD3D9SetRenderState)(unsigned int, unsigned int);
void(__cdecl *SetSpriteVertices)(float, float, float, float, float, float, float, float, unsigned int *, unsigned int *, unsigned int *, unsigned int *);
void(__cdecl *RwIm2DRenderPrimitive)(unsigned int, RwD3D9Vertex *, unsigned int);
CVehicle *(__thiscall *VehicleGetAt)(void *, unsigned int);
CPed *(__thiscall *PedGetAt)(void *, unsigned int);
CObject *(__thiscall *ObjectGetAt)(void *, unsigned int);
unsigned int(__cdecl *GetRadarTraceColour)(unsigned int, unsigned int);
void(__cdecl *ShowRadarTrace)(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned __int8);
void(__cdecl *ShowRadarTraceIII)(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char);
void(__cdecl *DrawRadarMap)();
void(__cdecl *DrawRadarMask)();
void(__cdecl *InitialiseRadar)();
void(__thiscall *PlayFrontEndSound)(void *, unsigned short, unsigned int);
void DrawLine(CVector2D *, CVector2D *, float, unsigned int);
void DrawPathLineMask();
void RwIm2DSetNearScreenZ(float);
void ProcessPathfind();
void DrawPathfind();
void DrawPathFindLine(RwV2d *, unsigned int, float, unsigned int, CPathNode**);
float GetSquaredDistanceBetweenPoints(CVector const&, CVector const&);
bool IsLineInsideRadar(CVector2D const&, CVector2D const&);
void ProcessModeSwitch();
PathLineInfo *GetPlaceInfo(PathLineInfo *);
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
RadarBlipIII *gRadarBlips3;
eLevelName *gCurrLevel;
unsigned int *g_TimeMs;
void *gAudio;

/* Text drawing - related things */
int TheText;
wchar_t*(__thiscall* GetText) 		(int, char *);
void(__cdecl *AsciiToUnicode)       (const char *ascii, short *pUni);
void(__cdecl *PrintString)          (float x, float y, short *text);
void(__cdecl *SetFontStyle)         (int style);
void(__cdecl *SetScale)             (float w, float h);
void(__cdecl *SetColor)             (unsigned int *color);
void(__cdecl *SetJustifyOn)         ();
void(__cdecl *SetDropShadowPosition)(int position);
void(__cdecl *SetPropOn)            ();
/* */

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
	return (float)sqrt(v3.x * v3.x + v3.y * v3.y + v3.z * v3.z);
}

CVector * GetCamPos()
{
	return (CVector *)(0x7E4688 + 0x7D8);
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
	return a.x * a.x + a.y * a.y <= 0.8f || b.x * b.x + b.y * b.y <= 1.0f;
}

bool IsLineInsideRadarIII(CVector2D const&a, CVector2D const&b)
{
	return a.x * a.x + a.y * a.y <= 1.44 || b.x * b.x + b.y * b.y <= 1.44;
}

void DrawPathLineMask()
{
	DrawRadarMask();
	unsigned int color;
	CVector2D points[2];
	if (gCurrentGpsMode != RADAR_SPRITE_CENTRE)
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

void DrawPathLineMaskIII()
{
	DrawRadarMask();
	unsigned int color;
	CVector2D points[2];
	if (gCurrentGpsMode != III_RADAR_SPRITE_CENTRE)
	{
		color = 0;
		points[0].x = 1;
		points[0].y = 1;
		points[1].x = -1;
		points[1].y = -1;
		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
		TransformRadarPointToScreenSpaceIII(points[0], points[0]);
		TransformRadarPointToScreenSpaceIII(points[1], points[1]);
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
	float angle = (float)atan2(dir.y, dir.x);
	shift[0].x = (float)(cos(angle - 1.5707963f) * width);
	shift[0].y = (float)(sin(angle - 1.5707963f) * width);
	shift[1].x = (float)(cos(angle + 1.5707963f) * width);
	shift[1].y = (float)(sin(angle + 1.5707963f) * width);
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
	for (unsigned int i = 0; i < (numPoints - 1); i++)
	{
		CVector2D temp, temp2;

		temp.x = gapPathNodes[i]->m_v2dPoint.x * 0.125f;
		temp.y = gapPathNodes[i]->m_v2dPoint.y * 0.125f;
		temp2.x = gapPathNodes[i + 1]->m_v2dPoint.x * 0.125f;
		temp2.y = gapPathNodes[i + 1]->m_v2dPoint.y * 0.125f;

		if (!(i & 1))
			TransformRealWorldPointToRadarSpace(points[i], temp);
		TransformRealWorldPointToRadarSpace(points[i + 1], temp2);
		if (IsLineInsideRadar(points[i], points[i + 1]))
		{
			if (!(i & 1))
				TransformRadarPointToScreenSpace(points[i], points[i]);
			TransformRadarPointToScreenSpace(points[i + 1], points[i + 1]);
			DrawLine(points[i], points[i + 1], width, color);
		}
		else TransformRadarPointToScreenSpace(points[i + 1], points[i + 1]);
	}
}

void DrawPathFindLineIII(RwV2d *points, unsigned int numPoints, float width, unsigned int color)
{
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
	for (unsigned int i = 0; i < (numPoints - 1); i++)
	{
		if (!(i & 1))
			TransformRealWorldPointToRadarSpaceIII(points[i], gapPathNodes3[i]->m_v2dPoint);
		TransformRealWorldPointToRadarSpaceIII(points[i + 1], gapPathNodes3[i + 1]->m_v2dPoint);
		if (IsLineInsideRadarIII(points[i], points[i + 1]))
		{
			if (!(i & 1))
				TransformRadarPointToScreenSpaceIII(points[i], points[i]);
			TransformRadarPointToScreenSpaceIII(points[i + 1], points[i + 1]);
			DrawLine(points[i], points[i + 1], width, color);
		}
		else TransformRadarPointToScreenSpaceIII(points[i + 1], points[i + 1]);
	}
}