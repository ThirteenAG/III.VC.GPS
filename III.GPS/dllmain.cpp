#include <Windows.h>
#include <string>
#include "..\common\common.h"
#include "..\common\injector\injector.hpp"

void GetMemoryAddresses()
{
	FindPlayerVehicle = (CVehicle *(__cdecl *)())0x4A10C0;
	IsPlayerOnAMission = (bool(*)()) 0x439410;
	DoPathSearch = (void(__thiscall *)(void *, unsigned char, CVector, int, CVector, CPathNode **, short *, short, CVehicle *, float *, float, int))0x42B040;
	TransformRealWorldPointToRadarSpaceIII = (void(__cdecl *)(CVector2D &, CVector2D const&))0x4A50D0;
	TransformRadarPointToScreenSpaceIII = (void(__cdecl *)(CVector2D &, CVector2D const&))0x4A5040;
	RwIm2DGetNearScreenZ = (float(__cdecl *)())0x5A43A0;
	RwRenderStateSet = (void(__cdecl *)(unsigned int, unsigned int))0x5A43C0;
	RwD3D9SetRenderState = (int(__cdecl *)(unsigned int, unsigned int))0x5B3CF0;
	SetSpriteVertices = (void(__cdecl *)(float, float, float, float, float, float, float, float, unsigned int *, unsigned int *, unsigned int *, unsigned int *))0x51F070;
	RwIm2DRenderPrimitive = (void(__cdecl *)(unsigned int, RwD3D9Vertex *, unsigned int))0x5A4430;
	GetRadarTraceColour = (unsigned int(__cdecl *)(unsigned int, unsigned int))0x4A5BB0;
	ShowRadarTraceIII = (void(__cdecl *)(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char)) 0x4A5870;
	VehicleGetAt = (CPed *(__thiscall *)(void *, unsigned int))0x43EAF0;
	PedGetAt = (CVehicle *(__thiscall *)(void *, unsigned int))0x43EB30;
	ObjectGetAt = (CObject *(__thiscall *)(void *, unsigned int))0x43EAB0;
	DrawRadarMap = (void(__cdecl *)())0x4A6C20;
	DrawRadarMask = (void(__cdecl *)())0x4A69C0;
	InitialiseRadar = (void(__cdecl *)())0x4A3EF0;
	PlayFrontEndSound = (void(__thiscall *)(void *, unsigned short, unsigned int))0x57CC20;
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
	gRadarBlips3 = (RadarBlipIII *)0x6ED5E0;
	gCurrLevel = (eLevelName *)0x941514;
	gAudio = (void *)0x95CDBE;
	g_TimeMs = (unsigned int *)0x885B48;
	GetText = (wchar_t *(__thiscall *)(int, char *))0x52BFB0;
	TheText = 0x941520;
	AsciiToUnicode = (void(__cdecl *)(const char *, short *)) 0x5009C0;
	PrintString = (void(__cdecl *)(float, float, short *)) 0x500F50;
	SetFontStyle = (void(__cdecl *)(int)) 0x501DB0;
	SetScale = (void(__cdecl *)(float, float)) 0x501B80;
	SetColor = (void(__cdecl *)(unsigned int *)) 0x501BD0;
	SetJustifyOn = (void(__cdecl *)()) 0x501C60;
	SetDropShadowPosition = (void(__cdecl *)(int)) 0x501E70;
	SetPropOn = (void(__cdecl *)()) 0x501DA0;
}

void Init()
{
	GetMemoryAddresses();
	injector::MakeCALL(0x4A42D6, ProcessPathfind);
	injector::MakeCALL(0x4A6C8D, DrawPathLineMaskIII);
	injector::MakeCALL(0x48C4F6, InitialiseGps);
	injector::MakeNOP(0x4A6B50, 5);
}


void InitialiseGps()
{
	InitialiseRadar();
	gCurrentGpsMode = III_RADAR_SPRITE_NONE;
	gGpsTextTimer = 0;
	gGpsAudioTimer = 0;
}

void PrintGpsText()
{
	if (*g_TimeMs < gGpsTextTimer + GPS_TEXT_TIME)
	{
		std::wstring text;

		if (gCurrentGpsMode == III_RADAR_SPRITE_CENTRE)
		{
			text = L"GPS Mode: Disabled";
		}
		else
		{
			short legendtext[128];
			AsciiToUnicode((char*)BlipsGxtTableIII[gCurrentGpsMode], legendtext); //GetText(TheText, );
			text = L"GPS Mode: ";
			text += (wchar_t*)legendtext;
		}

		unsigned int color = 0xFFFFFFFF;
		SetFontStyle(0);
		SetScale(0.7f, 1.0f);
		SetColor(&color);
		SetJustifyOn();
		SetDropShadowPosition(1);
		SetPropOn();
		CVector2D point;
		point.x = -1.0;
		point.y = -1.0;
		TransformRadarPointToScreenSpaceIII(point, point);
		PrintString(point.x, point.y + 10.0f, (short*)text.c_str());
		SetDropShadowPosition(0);
		SetFontStyle(0);
	}
}

void ProcessModeSwitch()
{
	unsigned char oldMode = gCurrentGpsMode;
	if (*g_TimeMs > gGpsScrollTimer + GPS_KEY_SCROLL_DELAY)
	{
		uint8_t bKeyUp = 0;
		bool bBlipFound = false;
		if ((GetAsyncKeyState(GPS_KEY_UP) & 1)) //DPADUP: todo
			bKeyUp = 1;
		else if (GetAsyncKeyState(GPS_KEY_DOWN) & 1)
			bKeyUp = 2;

		if (bKeyUp)
		{
			do
			{
				bKeyUp == 1 ? gCurrentGpsMode++ : gCurrentGpsMode--;

				if (bKeyUp == 1)
				{
					if (gCurrentGpsMode > III_RADAR_SPRITE_WEAPON)
						gCurrentGpsMode = III_RADAR_SPRITE_NONE;
				}
				else
				{
					if (gCurrentGpsMode < III_RADAR_SPRITE_NONE)
						gCurrentGpsMode = III_RADAR_SPRITE_WEAPON;
				}

				if (gCurrentGpsMode == III_RADAR_SPRITE_CENTRE && gCurrentGpsMode == III_RADAR_SPRITE_COPCAR)
					gCurrentGpsMode = bKeyUp == 1 ? III_RADAR_SPRITE_DON : III_RADAR_SPRITE_CAT;

				if (gCurrentGpsMode == III_RADAR_SPRITE_CENTRE || gCurrentGpsMode == III_RADAR_SPRITE_NONE)
					break;

				for (RadarBlipIII *blip = gRadarBlips3; blip != &gRadarBlips3[32]; blip++)
				{
					if (blip->m_wBlipSprite == gCurrentGpsMode && blip->m_bActive)
					{
						bBlipFound = true;
					}
				}
			} while (!bBlipFound);
			gGpsScrollTimer = *g_TimeMs;
		}
	}
	if (oldMode != gCurrentGpsMode)
	{
		gGpsTextTimer = *g_TimeMs;
		if (gCurrentGpsMode != III_RADAR_SPRITE_CENTRE && *g_TimeMs > gGpsAudioTimer + GPS_AUDIO_DELAY)
		{
			PlayFrontEndSound(gAudio, 149, 0);
			gGpsAudioTimer = *g_TimeMs;
		}
	}
	PrintGpsText();
}

PathLineInfo *GetPlaceInfo(PathLineInfo *info)
{
	CPlaceable *entity = NULL;
	RadarBlipIII *bestBlip = NULL;
	CVector blipPos = { 0.0f, 0.0f, 0.0f };
	float distance = 9999800001.99f;
	float newDistance = 0.0f;
	unsigned int color = 0;
	int point = 0;

	for (RadarBlipIII *blip = gRadarBlips3; blip != &gRadarBlips3[32]; blip++)
	{
		if (blip->m_bActive && blip->m_wBlipSprite == gCurrentGpsMode)
		{
			if (blip->m_dwBlipType > 0 && blip->m_dwBlipType < 4)
			{
				switch (blip->m_dwBlipType)
				{
				case BLIP_CAR: // 1
					entity = VehicleGetAt(*gVehiclePool, blip->m_dwEntityHandle);
					break;
				case BLIP_PED: // 2
					entity = PedGetAt(*gPedPool, blip->m_dwEntityHandle);
					if (entity && IS_PED_IN_CAR(entity))
						entity = GET_PED_CAR(entity);
					break;
				case BLIP_OBJECT: // 3
					entity = ObjectGetAt(*gObjectPool, blip->m_dwEntityHandle);
				}
				if (entity)
				{
					blipPos.x = entity->m_sCoords.m_sMatrix.pos.x;
					blipPos.y = entity->m_sCoords.m_sMatrix.pos.y;
					blipPos.z = entity->m_sCoords.m_sMatrix.pos.z;
				}
				else continue;
			}
			else
			{
				blipPos.x = blip->m_vecCurPos.x;
				blipPos.y = blip->m_vecCurPos.y;
				blipPos.z = blip->m_vecCurPos.z;
			}
			newDistance = GetSquaredDistanceBetweenPoints(blipPos, *GetCamPos());
			if (newDistance < distance)
			{
				distance = newDistance;
				gBlipBestPos.x = blipPos.x;
				gBlipBestPos.y = blipPos.y;
				gBlipBestPos.z = blipPos.z;
				bestBlip = blip;
			}
		}
	}
	if (bestBlip)
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
	return info;
}

void ProcessPathfind()
{
	DrawRadarMap();
	PathLineInfo info = {0};
	CVehicle *playerCar = FindPlayerVehicle();
	if (playerCar)
	{
		ProcessModeSwitch();
		if (gCurrentGpsMode != RADAR_SPRITE_CENTRE)
		{
			if (gCurrentGpsMode == III_RADAR_SPRITE_NONE && !IsPlayerOnAMission())
				return;

			GetPlaceInfo(&info);
			if (info.targetPoint)
			{
				DoPathSearch(gPathfind, PATHNODE_VEHICLE_PATH, playerCar->m_sCoords.m_sMatrix.pos, -1, *info.targetPoint, (CPathNode**)gapPathNodes3, &gwPathNodesCount, MAX_POINTS, playerCar, NULL, 999999.0f, -1);
				if (gwPathNodesCount > 1)
					DrawPathFindLineIII(gaPathPoints, gwPathNodesCount, LINE_WIDTH / (*gRadarRange), info.color);
			}
		}
	}
}


BOOL APIENTRY DllMain(HMODULE /*hModule*/, DWORD reason, LPVOID /*lpReserved*/)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		Init();
	}
	return TRUE;
}