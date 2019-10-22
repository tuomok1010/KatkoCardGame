#pragma once

#define SYSTEM_WINDOWS

//#define SYSTEM_LINUX

#ifdef SYSTEM_WINDOWS
	#define CLEARSCRN system("cls")
#endif

#ifdef SYSTEM_LINUX
	#define CLEARSCRN system("clear")
#endif