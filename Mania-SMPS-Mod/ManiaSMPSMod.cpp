// Mania-SMPS-Mod.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "..\mod-loader-common\ModLoaderCommon\IniFile.hpp"
#include "..\mania-mod-loader\ManiaModLoader\include\ManiaModLoader.h"
#include <unordered_map>
#include <algorithm>
using std::string;
using std::unordered_map;

BOOL(*LoadSong)(short song);
BOOL(*PlaySong)();
BOOL(*StopSong)();
BOOL(*PauseSong)();
BOOL(*ResumeSong)();
BOOL(*SetSongTempo)(double multi);
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

DataArray(struct_0, stru_26B818, 0x0026B818, 16);
int PlayMusicFile_r(char *name, unsigned int slot, int a3, unsigned int loopstart, int a5)
{
#ifdef _DEBUG
	PrintDebug("PlayMusicFile(\"%s\", %d, %d, %d, %d);\n", name, slot, a3, loopstart, a5);
#endif
	if (stru_26B818[slot].playStatus == 3)
		return -1;
	string namestr = name;
	std::transform(namestr.begin(), namestr.end(), namestr.begin(), tolower);
	auto choice = MusicChoices.find(namestr);
	if (choice != MusicChoices.cend())
	{
		stru_26B818[slot].anonymous_0 = *(int*)(baseAddress + 0x00A5CC88);
		stru_26B818[slot].anonymous_4 = *(int*)(baseAddress + 0x00A5CC8C);
		*(_DWORD *)&stru_26B818[slot].anonymous_8 = 0x3FF00FF;
		stru_26B818[slot].hasLoop = loopstart > 0;
		stru_26B818[slot].anonymous_5 = 0;
		stru_26B818[slot].volume = 1;
		stru_26B818[slot].anonymous_1 = 0;
		stru_26B818[slot].anonymous_3 = 0x10000;

		LoadSong(choice->second);
		PlaySong();
		stru_26B818[slot].playStatus = 2;
		one_up = !_stricmp(name, "1up.ogg");
		if (!_stricmp(name, "bluespheresspd.ogg"))
			bluespheretime = 0;
		else
			bluespheretime = -1;
		return slot;
	}
	stru_26B818[slot].playStatus = 0;
	return -1;
}

void SpeedUpMusic()
{
	SetSongTempo(1.5);
}

void SlowDownMusic()
{
	SetSongTempo(1);
}

VoidFunc(ResumeSound, 0x001BC9C0);
void ResumeSound_SMPS()
{
	ResumeSong();
	ResumeSound();
}

VoidFunc(PauseSound, 0x001BC960);
void PauseSound_SMPS()
{
	PauseSong();
	PauseSound();
}

unordered_map<string, short> songmap;

DataPointer(MusicInfo *, MusicSlots, 0xAC6E08);
int oldsong = -1;
char oldstatus = 0;
void SongStoppedCallback()
{
	if (oldsong != -1 && !one_up)
		oldstatus = stru_26B818[oldsong].playStatus = 0;
	one_up = false;
}

double bluespheretempos[]{ 1, 1.086956521739130434782608, 1.19047619047619047619, 1.31578947368421052631, 1.470588235294117647 };

extern "C"
{
	__declspec(dllexport) void OnFrame()
	{
		if (MusicSlots == nullptr) return;
		int song = MusicSlots->CurrentSong;
		if (song != -1)
		{
			char status = stru_26B818[song].playStatus;
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
			SetVolume(stru_26B818[song].volume * 0.5 * MusicVolume);
			if (bluespheretime != -1 && bluespheretime < 7200 && status == 2 && ++bluespheretime % 1800 == 0)
				SetSongTempo(bluespheretempos[bluespheretime / 1800]);
		}
		else
			oldstatus = 0;
		oldsong = song;
	}

#define GetFunc(name) name = (decltype(name))GetProcAddress(handle, "SMPS_"#name)
	__declspec(dllexport) void Init(const char *path)
	{
		char pathbuf[MAX_PATH];
		strcpy_s(pathbuf, path);
		strcat_s(pathbuf, "\\SMPSPlay.dll");
		HMODULE handle = LoadLibraryA(pathbuf);
		if (!handle)
		{
			PrintDebug("Mania-SMPS-Mod: Could not load SMPSPlay.dll!\n");
			return;
		}
		BOOL(*InitializeDriver)();
		GetFunc(InitializeDriver);
		void(*RegisterSongStoppedCallback)(void(*callback));
		GetFunc(RegisterSongStoppedCallback);
		const char **(*GetSongNames)(unsigned int &count);
		GetFunc(GetSongNames);
		GetFunc(SetVolume);
		GetFunc(LoadSong);
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
		unsigned int custcnt;
		const char **cust = GetSongNames(custcnt);
		for (unsigned int i = 0; i < custcnt; i++)
			songmap[cust[i]] = (short)i;
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
		WriteJump((void*)(baseAddress + 0x001BC640), PlayMusicFile_r);
		WriteData((char*)(baseAddress + 0x001BCA20), (char)0xC3);
		WriteData((char*)(baseAddress + 0x001FD61F), (char)0xEB);
		WriteData((char*)(baseAddress + 0x001BC5AE), (char)0xEB);
		WriteJump((void*)(baseAddress + 0x001C8705), PauseSound_SMPS);
		WriteJump((void*)(baseAddress + 0x001C8727), ResumeSound_SMPS);
		WriteCall((void*)(baseAddress + 0x001FE743), PauseSound_SMPS);
		WriteCall((void*)(baseAddress + 0x001FE764), ResumeSound_SMPS);
		if (MusicChoices.find("sneakers.ogg") == MusicChoices.cend())
		{
			WriteCall((void*)(baseAddress + 0x000A8C08), SpeedUpMusic);
			WriteCall((void*)(baseAddress + 0x000C27E8), SlowDownMusic);
		}
	}

	__declspec(dllexport) ModInfo ManiaModInfo = { ModLoaderVer, GameVer };
}