#include "Main.h"



SetGameAreaFOV SetGameAreaFOV_org = NULL;
SetGameAreaFOV SetGameAreaFOV_ptr;


float CustomFovFix = 1.0f;


int __fastcall SetGameAreaFOV_new(Matrix1* a1, int _unused, float a3, float a4, float a5, float a6)
{
	if (GetWindowXoffset == 0 || GetWindowYoffset == 0)
		return SetGameAreaFOV_ptr(a1, _unused, a3, a4, a5, a6);


	float ScreenX = *GetWindowXoffset;
	float ScreenY = *GetWindowYoffset;

	float v1 = 1.0f / sqrt(a4 * a4 + 1.0f);
	float v2 = tan(v1 * a3 * 0.5f);

	float v3 = v2 * a5;
	float v4 = v3 * a4;


	a1->flt1 = ((a5 * (4.0f / 3.0f)) / (ScreenX / ScreenY) * CustomFovFix) / v4; // Fix 4:3 to WindowX/WindowY
	a1->flt2 = 0.0f;
	a1->flt3 = 0.0f;
	a1->flt4 = 0.0f;
	a1->flt5 = 0.0f;


	a1->flt6 = a5 / v3;
	a1->flt7 = 0.0f;
	a1->flt8 = 0.0f;
	a1->flt9 = 0.0f;
	a1->flt10 = 0.0f;


	a1->flt11 = (a5 + a6) / (a6 - a5);
	a1->flt12 = 1.0f;
	a1->flt13 = 0.0f;
	a1->flt14 = 0.0f;


	a1->flt15 = a5 * (a6 * -2.0f) / (a6 - a5);
	a1->flt16 = 0.0f;


	return 0;
}


int EnableFixFOV = false;


int __stdcall SetWidescreenFixState(int widefixenable)
{
	EnableFixFOV = widefixenable;
	return 0;
}


int __stdcall SetCustomFovFix(float _CustomFovFix)
{
	CustomFovFix = _CustomFovFix;
	return 0;
}

Matrix1 globalmatrix;

void __fastcall SetGameAreaFOV_my(Matrix1* a1, int a2, float a3, float a4, float a5, float a6)
{
	if (EnableFixFOV)
	{
		SetGameAreaFOV_new(a1, a2, a3, a4, a5, a6);
	}
	else
	{
		SetGameAreaFOV_ptr(a1, a2, a3, a4, a5, a6);
	}

	globalmatrix = *a1;
}

