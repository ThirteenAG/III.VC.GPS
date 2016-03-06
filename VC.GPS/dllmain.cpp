#include <Windows.h>
#include <string>
#include "..\common\common.h"
#include "..\common\injector\injector.hpp"

void GetMemoryAddresses()
{
	FindPlayerVehicle = (CVehicle *(__cdecl *)())0x4BC1E0; 
	IsPlayerOnAMission = (bool(*)()) 0x44FE30;
	DoPathSearch = (void(__thiscall *)(void *, unsigned char, CVector, int, CVector, CPathNode **, short *, short, CVehicle *, float *, float, int))0x439070; 
	//TransformRealWorldPointToRadarSpace = (void(__cdecl *)(CVector2D &, CVector2D const&))0x4A50D0;
	//TransformRadarPointToScreenSpace = (void(__cdecl *)(CVector2D &, CVector2D const&))0x4A5040;
	RwIm2DGetNearScreenZ = (float(__cdecl *)())0x649B80;
	RwRenderStateSet = (void(__cdecl *)(unsigned int, unsigned int))0x649BA0;
	RwD3D9SetRenderState = (int(__cdecl *)(unsigned int, unsigned int))0x6582A0;
	SetSpriteVertices = (void(__cdecl *)(float, float, float, float, float, float, float, float, unsigned int *, unsigned int *, unsigned int *, unsigned int *))0x5781C0;
	RwIm2DRenderPrimitive = (void(__cdecl *)(unsigned int, RwD3D9Vertex *, unsigned int))0x649C10;
	GetRadarTraceColour = (unsigned int(__cdecl *)(unsigned int, unsigned int))0x4C3050;
	ShowRadarTrace = (void(__cdecl *)(float, float, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char, unsigned __int8)) 0x4C32F0;
	VehicleGetAt = (CPed *(__thiscall *)(void *, unsigned int))0x451C70;
	PedGetAt = (CVehicle *(__thiscall *)(void *, unsigned int))0x451CB0;
	ObjectGetAt = (CObject *(__thiscall *)(void *, unsigned int))0x451C30;
	DrawRadarMap = (void(__cdecl *)())0x4C17C0;
	DrawRadarMask = (void(__cdecl *)())0x4C1A20;
	InitialiseRadar = (void(__cdecl *)())0x4C6200;
	PlayFrontEndSound = (void(__thiscall *)(void *, unsigned short, unsigned int))0x5F9960;
	gPathfind = (void *)0x9B6E5C;
	gRadarMapZShift = (float *)0x699530;
	gSpriteVertices = (RwD3D9Vertex *)0x7D4040;
	gScreenWidth = (int *)0x9B48E4;
	gScreenHeight = (int *)0x9B48E8;
	gVehiclePool = (void **)0xA0FDE4;
	gPedPool = (void **)0x97F2AC;
	gObjectPool = (void **)0x94DBE0;
	gRwEngine = (unsigned int *)0x7870C0; 
	gRadarRange = (float *)0x974BEC;
	gRadarBlips = (RadarBlip *)0x7D7D38;
	gCurrLevel = (eLevelName *)0xA0D9AC;
	gAudio = (void *)0xA10B8A;
	g_TimeMs = (unsigned int *)0x974B2C;
	GetText = (wchar_t *(__thiscall *)(int, char *))0x584F30;
	TheText = 0x94B220;
	AsciiToUnicode = (void(__cdecl *)(const char *, short *)) 0x552500;
	PrintString = (void(__cdecl *)(float, float, short *)) 0x551040;
	SetFontStyle = (void(__cdecl *)(int)) 0x54FFE0;
	SetScale = (void(__cdecl *)(float, float)) 0x550230;
	SetColor = (void(__cdecl *)(unsigned int *)) 0x550170;
	SetJustifyOn = (void(__cdecl *)()) 0x550150;
	SetDropShadowPosition = (void(__cdecl *)(int)) 0x54FF20;
	SetPropOn = (void(__cdecl *)()) 0x550020;
}

void Init()
{
	GetMemoryAddresses();
	injector::MakeCALL(0x4C5D4B, ProcessPathfind);
	injector::MakeCALL(0x4C17C5, DrawPathLineMask);
	injector::MakeCALL(0x4A4896, InitialiseGps);
	injector::MakeNOP(0x4C1D49, 5);
}


void InitialiseGps()
{
	InitialiseRadar();
	gCurrentGpsMode = RADAR_SPRITE_NONE;
	gGpsTextTimer = 0;
	gGpsAudioTimer = 0;
}

void PrintGpsText()
{
	if (*g_TimeMs < gGpsTextTimer + GPS_TEXT_TIME)
	{
		std::wstring text;

		if (gCurrentGpsMode == RADAR_SPRITE_CENTRE)
		{
			text = L"GPS Mode: Disabled";
		}
		else
		{
			auto legendtext = GetText(TheText, (char*)BlipsGxtTableVC[gCurrentGpsMode]);
			text = L"GPS Mode: ";
			text += legendtext;
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
		TransformRadarPointToScreenSpace(point, point);
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
					if (gCurrentGpsMode > RADAR_SPRITE_RADIO_WAVE)
						gCurrentGpsMode = RADAR_SPRITE_NONE;
				}
				else
				{
					if (gCurrentGpsMode < RADAR_SPRITE_NONE)
						gCurrentGpsMode = RADAR_SPRITE_RADIO_WAVE;
				}

				if (gCurrentGpsMode > RADAR_SPRITE_CENTRE && gCurrentGpsMode < RADAR_SPRITE_AVERY)
					gCurrentGpsMode = bKeyUp == 1 ? RADAR_SPRITE_AVERY : RADAR_SPRITE_CENTRE;

				if (gCurrentGpsMode == RADAR_SPRITE_CENTRE || gCurrentGpsMode == RADAR_SPRITE_NONE)
					break;

				for (RadarBlip *blip = gRadarBlips; blip != &gRadarBlips[32]; blip++)
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
		if (gCurrentGpsMode != RADAR_SPRITE_CENTRE && *g_TimeMs > gGpsAudioTimer + GPS_AUDIO_DELAY)
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
	RadarBlip *bestBlip = NULL;
	CVector blipPos = { 0.0f, 0.0f, 0.0f };
	float distance = 9999800001.99f;
	float newDistance = 0.0f;
	unsigned int color = 0;
	int point = 0;

	for (RadarBlip *blip = gRadarBlips; blip != &gRadarBlips[32]; blip++)
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
			if (gCurrentGpsMode == RADAR_SPRITE_NONE && !IsPlayerOnAMission()) //shows path to racing blip when not on mission
				return;

			GetPlaceInfo(&info);
			if (info.targetPoint)
			{
				DoPathSearch(gPathfind, PATHNODE_VEHICLE_PATH, playerCar->m_sCoords.m_sMatrix.pos, -1, *info.targetPoint, gapPathNodes, &gwPathNodesCount, MAX_POINTS, playerCar, NULL, 999999.0f, -1);
				if (gwPathNodesCount > 1)
					DrawPathFindLine(gaPathPoints, gwPathNodesCount, LINE_WIDTH / (*gRadarRange), info.color);
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