#ifndef PCH_H
#define PCH_H

using namespace std;

#define __int64 long long
typedef unsigned __int64 uill;
typedef uill uint64;
typedef unsigned int uint32;

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#pragma comment(lib, "dwmapi.lib")
#include <dwmapi.h>
#include <string>
#include <winternl.h>
#pragma comment(lib, "ntdll.lib");
#include <ntstatus.h>
#include <TlHelp32.h>
#include <tchar.h>

struct D3DMatrix
{
	float _11 = 0.f, _12 = 0.f, _13 = 0.f, _14 = 0.f;
	float _21 = 0.f, _22 = 0.f, _23 = 0.f, _24 = 0.f;
	float _31 = 0.f, _32 = 0.f, _33 = 0.f, _34 = 0.f;
	float _41 = 0.f, _42 = 0.f, _43 = 0.f, _44 = 0.f;
};
struct Vector3f
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
};
struct Vector4f
{
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;
};
struct FTTransform2_t
{
	Vector4f Rotation;
	Vector3f Translation;
	float ddd{ 0.f };
	Vector3f Scale3D;
};

#include "Overlay.h"
#include "ESP.h"
#include "MemoryManager.h"
#include "ViewMatrixManager.h"

#endif