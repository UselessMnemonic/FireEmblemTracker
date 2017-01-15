#pragma once

enum RM_FILE_LOCATION {
	SD,
	ROMFS
};

bool RMInit()
{
	Result sd = sdmcInit();
	Result romfs = romfsInit();
	return R_SUCCEEDED(sd) && R_SUCCEEDED(romfs);
}

void RMExit()
{
	sdmcExit();
	romfsExit();
}

FILE* rm_open(const char* filename, const char* type, RM_FILE_LOCATION loc)
{

}