#include "stdafx.h"
#include "UIObject.h"
#include "Offsets.h"
#include "Tools.h"
#include "UISimpleFrame.h"

static UISimpleFrame GameSimpleConsole;
static UISimpleFrame GameMinimapScreenFrame;

UISimpleFrame* UIObject::GetGameSimpleConsole( ) {
	war3::CGameUI* gameUI = GameUIObjectGet( );
	war3::CSimpleFrame* console = gameUI ? ( war3::CSimpleFrame* )( gameUI->simpleConsole ) : NULL;
	if ( console ) {
		GameSimpleConsole = UISimpleFrame( console );
		return &GameSimpleConsole;
	}
	return NULL;
}

UISimpleFrame* UIObject::CreateGameSimpleConsole(/*....*/ ) {
	war3::CGameUI* gameUI = GameUIObjectGet( );
	war3::CSimpleFrame* console = gameUI ? ( war3::CSimpleFrame* )( gameUI->simpleConsole ) : NULL;
	if ( console ) {
			//...
	}
	return NULL;
}

UISimpleFrame* UIObject::GetMinimapFrame() {
	war3::CGameUI* gameUI = GameUIObjectGet();
	//OutputScreen(10, "gameUI = 0x%X, world = 0x%X", gameUI, gameUI->world);
	war3::CSimpleFrame* frame = gameUI ? (war3::CSimpleFrame*)(gameUI->minimapScreenFrame) : NULL;
	if (frame){
		GameMinimapScreenFrame = UISimpleFrame(frame);
		//GameMinimapScreenFrame.hide();
		return &GameMinimapScreenFrame;
	}
	return NULL;
}

const char* UIObject::GetPathByName(const char* name, const char* type) {
	return dreamaero::generic_fast_call<const char*>(
		Offset(SKIN_GETPATHBYNAME), 
		name,
		type
	);
}