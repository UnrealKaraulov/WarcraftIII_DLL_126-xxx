#include "stdafx.h"
#include "UITest.h"
#include "Event.h"
#include "Game.h"
#include "Tools.h"
#include "Timer.h"

#include "UISimpleTexture.h"
#include "Texture.h" 
#include "Table.h"
#include "Label.h"
#include "CheckBox.h"

void UITest() 
{
	Texture *tx = new Texture(UI_NULL, 0.038f, 0.038f, "ReplaceableTextures\\CommandButtons\\BTNAdvStruct.blp");
	tx->setAbsolutePosition(POS_C, 0.4f, 0.3f);
}

static void onLocalChat (const Event *evt) 
{
	LocalChatEventData* data = evt->data<LocalChatEventData>();
	const char* text = data->content;
	if (text[0] == 'u') 
	{
		OutputScreen(10, "UITest");
		UITest();
	}
}

void UITest_Init() 
{
	MainDispatcher()->listen(EVENT_LOCAL_CHAT, onLocalChat);
}

void UITest_Cleanup() 
{
}