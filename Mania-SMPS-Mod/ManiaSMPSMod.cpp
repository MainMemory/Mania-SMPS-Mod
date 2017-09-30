// Mania-SMPS-Mod.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "..\mod-loader-common\ModLoaderCommon\IniFile.hpp"
#include "..\mania-mod-loader\ManiaModLoader\include\ManiaModLoader.h"
#include <unordered_map>
#include <algorithm>
using std::string;
using std::unordered_map;

BOOL(*PlaySong)(short song);
BOOL(*StopSong)();
BOOL(*PauseSong)();
BOOL(*ResumeSong)();
BOOL(*SetSongTempo)(unsigned int pct);
void(*SetVolume)(double vol);

struct struct_0
{
	int anonymous_0;
	int anonymous_1;
	float volume;
	int anonymous_3;
	int anonymous_4;
	int anonymous_5;
	int anonymous_6;
	int hasLoop;
	__int16 anonymous_8;
	_BYTE gap22[1];
	char playStatus;
};

struct MusicInfo
{
	__int16 field_0;
	_BYTE gap2[1];
	char Names[32][16];
	char field_203;
	int LoopStarts[16];
	int field_244;
	int CurrentSong;
	int field_24C;
	int field_250;
	int field_254;
	int field_258;
	int field_25C;
	int field_260;
	int field_264;
};

unordered_map<string, short> MusicChoices;

bool one_up;
int bluespheretime = -1;

DataArray(struct_0, stru_D7ACA0, 0xD7ACA0, 16);
int PlayMusicFile_r(char *name, unsigned int a2, int a3, unsigned int loopstart, int a5)
{
#ifdef _DEBUG
	PrintDebug("PlayMusicFile(\"%s\", %d, %d, %d, %d);\n", name, a2, a3, loopstart, a5);
#endif
	if (stru_D7ACA0[a2].playStatus == 3)
		return -1;
	string namestr = name;
	std::transform(namestr.begin(), namestr.end(), namestr.begin(), tolower);
	auto choice = MusicChoices.find(namestr);
	if (choice != MusicChoices.cend())
	{
		stru_D7ACA0[a2].anonymous_0 = *(int*)0xD7CEF0;
		stru_D7ACA0[a2].anonymous_4 = *(int*)0xD7CEF4;
		*(_DWORD *)&stru_D7ACA0[a2].anonymous_8 = 0x3FF00FF;
		stru_D7ACA0[a2].hasLoop = loopstart > 0;
		stru_D7ACA0[a2].anonymous_5 = 0;
		stru_D7ACA0[a2].volume = 1;
		stru_D7ACA0[a2].anonymous_1 = 0;
		stru_D7ACA0[a2].anonymous_3 = 0x10000;
		PlaySong(choice->second);
		stru_D7ACA0[a2].playStatus = 2;
		one_up = !_stricmp(name, "1up.ogg");
		if (!_stricmp(name, "bluespheresspd.ogg"))
			bluespheretime = 0;
		else
			bluespheretime = -1;
		return a2;
	}
	stru_D7ACA0[a2].playStatus = 0;
	return -1;
}

void SpeedUpMusic()
{
	SetSongTempo(66);
}

void SlowDownMusic()
{
	SetSongTempo(100);
}

VoidFunc(sub_599720, 0x599720);
void ResumeSound()
{
	ResumeSong();
	sub_599720();
}

VoidFunc(sub_5996C0, 0x5996C0);
void PauseSound()
{
	PauseSong();
	sub_5996C0();
}

const string SMPSMusicList[] = {
	"S3Title",
	"AngelIsland1",
	"AngelIsland2",
	"Hydrocity1",
	"Hydrocity2",
	"MarbleGarden1",
	"MarbleGarden2",
	"CarnivalNight1",
	"CarnivalNight2",
	"FlyingBattery1",
	"FlyingBattery2",
	"IceCap1",
	"IceCap2",
	"LaunchBase1",
	"LaunchBase2",
	"MushroomHill1",
	"MushroomHill2",
	"Sandopolis1",
	"Sandopolis2",
	"LavaReef1",
	"LavaReef2",
	"SkySanctuary",
	"DeathEgg1",
	"DeathEgg2",
	"SKMidboss",
	"Boss",
	"Doomsday",
	"MagneticOrbs",
	"SpecialStage",
	"SlotMachine",
	"GumballMachine",
	"S3Knuckles",
	"AzureLake",
	"BalloonPark",
	"DesertPalace",
	"ChromeGadget",
	"EndlessMine",
	"GameOver",
	"Continue",
	"ActClear",
	"S31Up",
	"ChaosEmerald",
	"S3Invincibility",
	"CompetitionMenu",
	"S3Midboss",
	"LevelSelect",
	"FinalBoss",
	"Drowning",
	"S3AllClear",
	"S3Credits",
	"SKKnuckles",
	"SKTitle",
	"SK1Up",
	"SKInvincibility",
	"SKAllClear",
	"SKCredits",
	"S3CCredits",
	"S3Continue",
	"SKTitle0525",
	"SKAllClear0525",
	"SKCredits0525",
	"GreenGrove1",
	"GreenGrove2",
	"RustyRuin1",
	"RustyRuin2",
	"VolcanoValley2",
	"VolcanoValley1",
	"SpringStadium1",
	"SpringStadium2",
	"DiamondDust1",
	"DiamondDust2",
	"GeneGadget1",
	"GeneGadget2",
	"PanicPuppet2",
	"FinalFight",
	"S3DEnding",
	"S3DSpecialStage",
	"PanicPuppet1",
	"S3DBoss2",
	"S3DBoss1",
	"S3DIntro",
	"S3DCredits",
	"S3DInvincibility",
	"S3DMenu",
	"S4E1Boss",
	"GreenHill",
	"Labyrinth",
	"Marble",
	"StarLight",
	"SpringYard",
	"ScrapBrain",
	"S1Invincibility",
	"S11Up",
	"S1SpecialStage",
	"S1Title",
	"S1Ending",
	"S1Boss",
	"FinalZone",
	"S1ActClear",
	"S1GameOver",
	"S1Continue",
	"S1Credits",
	"S1Drowning",
	"S1ChaosEmerald",
	"CasinoNight2P",
	"EmeraldHill",
	"Metropolis",
	"CasinoNight",
	"MysticCave",
	"MysticCave2P",
	"AquaticRuin",
	"S2DeathEgg",
	"S2SpecialStage",
	"S2Options",
	"S2Ending",
	"S2FinalBoss",
	"ChemicalPlant",
	"S2Boss",
	"SkyChase",
	"OilOcean",
	"WingFortress",
	"EmeraldHill2P",
	"S22PResults",
	"S2SuperSonic",
	"HillTop",
	"S2Title",
	"S2Invincibility",
	"S2HiddenPalace",
	"S2Credits",
	"CasinoNight2PBeta",
	"EmeraldHillBeta",
	"MetropolisBeta",
	"CasinoNightBeta",
	"MysticCaveBeta",
	"MysticCave2PBeta",
	"AquaticRuinBeta",
	"S2DeathEggBeta",
	"S2SpecialStageBeta",
	"S2OptionsBeta",
	"S2FinalBossBeta",
	"ChemicalPlantBeta",
	"S2BossBeta",
	"SkyChaseBeta",
	"OilOceanBeta",
	"WingFortressBeta",
	"EmeraldHill2PBeta",
	"S22PResultsBeta",
	"S2SuperSonicBeta",
	"HillTopBeta",
	"S3DSpecialStageBeta",
	"CarnivalNight1PC",
	"CarnivalNight2PC",
	"IceCap1PC",
	"IceCap2PC",
	"LaunchBase1PC",
	"LaunchBase2PC",
	"KnucklesPC",
	"CompetitionMenuPC",
	"UnusedPC",
	"CreditsPC",
	"S3InvincibilityPC"
};

unordered_map<string, short> songmap;

DataPointer(MusicInfo *, MusicSlots, 0xD84664);
DataPointer(float, MusicVolume, 0x639CC4);
int oldsong = -1;
char oldstatus = 0;
void SongStoppedCallback()
{
	if (oldsong != -1 && !one_up)
		oldstatus = stru_D7ACA0[oldsong].playStatus = 0;
	one_up = false;
}

extern "C"
{
	__declspec(dllexport) void OnFrame()
	{
		if (MusicSlots == nullptr) return;
		int song = MusicSlots->CurrentSong;
		if (song != -1)
		{
			char status = stru_D7ACA0[song].playStatus;
			if (song == oldsong && status != oldstatus)
				switch (status)
				{
				case 0:
					StopSong();
					break;
				case 2:
					ResumeSong();
					break;
				case 66:
					PauseSong();
					break;
#ifdef _DEBUG
				default:
					PrintDebug("Unknown status code %d\n", status);
					break;
#endif
				}
			oldstatus = status;
			SetVolume(stru_D7ACA0[song].volume * 0.5 * MusicVolume);
			if (bluespheretime != -1 && bluespheretime < 7200 && status == 2 && ++bluespheretime % 1800 == 0)
				SetSongTempo(100 - (8 * bluespheretime / 1800));
		}
		else
			oldstatus = 0;
		oldsong = song;
	}

#define GetFunc(name) name = (decltype(name))GetProcAddress(handle, #name)
	__declspec(dllexport) void Init(const char *path)
	{
		char pathbuf[MAX_PATH];
		strcpy_s(pathbuf, path);
		strcat_s(pathbuf, "\\SMPSOUT.dll");
		HMODULE handle = LoadLibraryA(pathbuf);
		if (!handle)
		{
			PrintDebug("Mania-SMPS-Mod: Could not load SMPSOUT.dll!\n");
			return;
		}
		BOOL(*InitializeDriver)() = GetFunc(InitializeDriver);
		void(*RegisterSongStoppedCallback)(void(*callback)) = GetFunc(RegisterSongStoppedCallback);
		const char **(*GetCustomSongs)(unsigned int &count) = GetFunc(GetCustomSongs);
		GetFunc(SetVolume);
		GetFunc(PlaySong);
		GetFunc(StopSong);
		GetFunc(PauseSong);
		GetFunc(ResumeSong);
		GetFunc(SetSongTempo);
		char olddir[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, olddir);
		SetCurrentDirectoryA(path);
		InitializeDriver();
		SetCurrentDirectoryA(olddir);
		RegisterSongStoppedCallback(SongStoppedCallback);
		SetVolume(0.5 * MusicVolume);
		size_t sng;
		for (sng = 0; sng < LengthOfArray(SMPSMusicList); sng++)
			songmap[SMPSMusicList[sng]] = (short)sng;
		unsigned int custcnt;
		const char **cust = GetCustomSongs(custcnt);
		for (unsigned int i = 0; i < custcnt; i++)
			songmap[cust[i]] = (short)(i + sng);
		strcpy_s(pathbuf, path);
		strcat_s(pathbuf, "\\config.ini");
		const IniFile *cfg = new IniFile(pathbuf);
		const IniGroup *grp = cfg->getGroup("");
		for (auto i = grp->cbegin(); i != grp->cend(); ++i)
		{
			string key = i->first;
			std::transform(key.begin(), key.end(), key.begin(), tolower);
			key.append(".ogg");
			const string &value = i->second;
			auto found = songmap.find(value);
			if (found != songmap.end())
				MusicChoices[key] = found->second;
			else
				PrintDebug("SMPS Mod: %s is not a valid option in key %s.\n", value.c_str(), key.c_str());
		}
		delete cfg;
		WriteJump((void*)0x5993A0, PlayMusicFile_r);
		WriteData((char*)0x599780, (char)0xC3);
		WriteData((char*)0x4016A7, (char)0xEB);
		WriteData((char*)0x401AD9, (char)0xEB);
		WriteData((char*)0x401BA0, (char)0xB8);
		WriteData((int*)0x401BA1, 0);
		WriteCall((void*)0x5CAE98, PauseSound);
		WriteCall((void*)0x5CAEDB, ResumeSound);
		WriteCall((void*)0x5CAF85, PauseSound);
		WriteCall((void*)0x5CAFA6, ResumeSound);
		WriteData((char*)0x5C96CF, (char)0xEB);
		if (MusicChoices.find("sneakers.ogg") == MusicChoices.cend())
		{
			WriteCall((void*)0x43DEB6, SpeedUpMusic);
			WriteCall((void*)0x47EB06, SlowDownMusic);
		}
	}

	__declspec(dllexport) ModInfo ManiaModInfo = { ModLoaderVer, GameVer };
}