#pragma once
typedef signed int( __fastcall * IsDrawSkillPanel )( void *UnitAddr, int addr1 );
typedef signed int( __thiscall * DrawSkillPanel )( void *UnitAddr, int OID );
typedef signed int( __fastcall * IsDrawSkillPanelOverlay )( void *UnitAddr, int addr1 );
typedef signed int( __thiscall * DrawSkillPanelOverlay )( void *UnitAddr, int OID );
typedef int( __thiscall * IsNeedDrawUnitOrigin )( void *UnitAddr );
typedef int( __thiscall * IsNeedDrawUnit2 )( int UnitAddr );
extern IsNeedDrawUnit2 IsNeedDrawUnit2org;
extern IsNeedDrawUnit2 IsNeedDrawUnit2ptr;
extern IsDrawSkillPanel IsDrawSkillPanel_org;
extern IsDrawSkillPanel IsDrawSkillPanel_ptr;
extern IsDrawSkillPanelOverlay IsDrawSkillPanelOverlay_org;
extern IsDrawSkillPanelOverlay IsDrawSkillPanelOverlay_ptr;




signed int __fastcall  IsDrawSkillPanel_my( void *UnitAddr, int addr1 );
signed int __fastcall  IsDrawSkillPanelOverlay_my( void *UnitAddr, int addr1 );
int __fastcall IsNeedDrawUnit2_my( int UnitAddr, int unused );