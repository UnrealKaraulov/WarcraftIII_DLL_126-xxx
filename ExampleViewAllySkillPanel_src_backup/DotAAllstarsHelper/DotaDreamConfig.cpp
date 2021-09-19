#include "Main.h"
#include "DotaClickHelper.h"
#include <Tools.h>
#include <UISimpleFrame.h>
#include <Button.h>
#include <CheckBox.h>
#include <DropDownList.h>
#include <Label.h>
#include <ToolTip.h>
#include <HotkeyButton.h>
#include <EditBoxButton.h>
#include <Game.h>
#include <War3Window.h>
#include <KeyCode.h>
#include "IniReader.h"
#include "IniWriter.h"

Button * ShowConfigButton = NULL;

bool ConfigWindowHidden = true;
bool ConfigWindowInitialized = false;
UISimpleFrame * mainFrame = NULL;
UISimpleFrame * navigationFrame = NULL;
UISimpleFrame * contentFrame = NULL;

const float OPTIONMENU_WIDTH = 0.6f;
const float OPTIONMENU_HEIGHT = 0.375f;
const float OPTIONMENU_CENTER_X = 0.385f;
const float OPTIONMENU_CENTER_Y = 0.36f;
const float NAVIGATION_WIDTH = 0.16f;


UISimpleFrame * contentFrame1 = NULL;
UISimpleFrame * contentFrame2 = NULL;
UISimpleFrame * contentFrame3 = NULL;
UISimpleFrame * contentFrame4 = NULL;
UISimpleFrame * contentFrame5 = NULL;
UISimpleFrame * contentFrame6 = NULL;

Button * ShowConfigButton1 = NULL;
Button * ShowConfigButton2 = NULL;
Button * ShowConfigButton3 = NULL;
Button * ShowConfigButton4 = NULL;
Button * ShowConfigButton5 = NULL;
Button * ShowConfigButton6 = NULL;

Label * ConfigLabel1 = NULL;
Label * ConfigLabel2 = NULL;
Label * ConfigLabel3 = NULL;
Label * ConfigLabel4 = NULL;
Label * ConfigLabel5 = NULL;
Label * ConfigLabel6 = NULL;

Label * contentLabel1 = NULL;



bool IsAnyHotkeyIsActive( )
{
	return  HotkeyButton::IsAnyHotkeyActive( );
}

bool IsAnyEditBoxIsActive( )
{
	return EditBoxButton::IsAnyEditBoxActive( );
}

void CurrentEditBoxRemoveCharacter( bool right )
{
	EditBoxButton::RemoveCharacterFromCurrentEditBox( right );
}

void CurrentEditBoxMoveCursorLeft( )
{
	EditBoxButton::CurrentEditBoxMoveCursorLeft( );
}

void CurrentEditBoxMoveCursorRight( )
{
	EditBoxButton::CurrentEditBoxMoveCursorRight( );
}

void CurrentEditBoxEnterText( std::wstring text )
{
	EditBoxButton::EnterTextToCurrentEditBox( text );
}

void DisableInputForAnyHotkeyAndEditBox( )
{
	EditBoxButton::StopInput( );
	//HotkeyButton::StopInput( );
}



void ShowCategory0(  )
{
	contentFrame1->hide( );
	contentFrame2->hide( );
	contentFrame3->hide( );
	contentFrame4->hide( );
	contentFrame5->hide( );
	contentFrame6->hide( );
	contentLabel1->show(true );

	ShowConfigButton1->Bold( false );
	ShowConfigButton2->Bold( false );
	ShowConfigButton3->Bold( false );
	ShowConfigButton4->Bold( false );
	ShowConfigButton5->Bold( false );
	ShowConfigButton6->Bold( false );

	UI_Refresh( );
}
void ShowCategory1( Button *button )
{
	contentFrame1->show( );
	contentFrame2->hide( );
	contentFrame3->hide( );
	contentFrame4->hide( );
	contentFrame5->hide( );
	contentFrame6->hide( );
	contentLabel1->show( false );


	ShowConfigButton1->Bold( true );
	ShowConfigButton2->Bold( false );
	ShowConfigButton3->Bold( false );
	ShowConfigButton4->Bold( false );
	ShowConfigButton5->Bold( false );
	ShowConfigButton6->Bold( false );

	UI_Refresh( );
}
void ShowCategory2( Button *button )
{
	contentFrame1->hide( );
	contentFrame2->show( );
	contentFrame3->hide( );
	contentFrame4->hide( );
	contentFrame5->hide( );
	contentFrame6->hide( );
	contentLabel1->show( false );

	ShowConfigButton1->Bold( false );
	ShowConfigButton2->Bold( true );
	ShowConfigButton3->Bold( false );
	ShowConfigButton4->Bold( false );
	ShowConfigButton5->Bold( false );
	ShowConfigButton6->Bold( false );
	UI_Refresh( );
}
void ShowCategory3( Button *button )
{
	contentFrame1->hide( );
	contentFrame2->hide( );
	contentFrame3->show( );
	contentFrame4->hide( );
	contentFrame5->hide( );
	contentFrame6->hide( );
	contentLabel1->show( false );


	ShowConfigButton1->Bold( false );
	ShowConfigButton2->Bold( false );
	ShowConfigButton3->Bold( true );
	ShowConfigButton4->Bold( false );
	ShowConfigButton5->Bold( false );
	ShowConfigButton6->Bold( false );

	UI_Refresh( );
}
void ShowCategory4( Button *button )
{
	contentFrame1->hide( );
	contentFrame2->hide( );
	contentFrame3->hide( );
	contentFrame4->show( );
	contentFrame5->hide( );
	contentFrame6->hide( );
	contentLabel1->show( false );

	ShowConfigButton1->Bold( false );
	ShowConfigButton2->Bold( false );
	ShowConfigButton3->Bold( false );
	ShowConfigButton4->Bold( true );
	ShowConfigButton5->Bold( false );
	ShowConfigButton6->Bold( false );
	UI_Refresh( );
}

void ShowCategory5( Button *button )
{
	contentFrame1->hide( );
	contentFrame2->hide( );
	contentFrame3->hide( );
	contentFrame4->hide( );
	contentFrame5->show( );
	contentFrame6->hide( );
	contentLabel1->show( false );


	ShowConfigButton1->Bold( false );
	ShowConfigButton2->Bold( false );
	ShowConfigButton3->Bold( false );
	ShowConfigButton4->Bold( false );
	ShowConfigButton5->Bold( true );
	ShowConfigButton6->Bold( false );
	UI_Refresh( );
}

void ShowCategory6( Button *button )
{
	contentFrame1->hide( );
	contentFrame2->hide( );
	contentFrame3->hide( );
	contentFrame4->hide( );
	contentFrame5->hide( );
	contentFrame6->show( );
	contentLabel1->show( false );


	ShowConfigButton1->Bold( false );
	ShowConfigButton2->Bold( false );
	ShowConfigButton3->Bold( false );
	ShowConfigButton4->Bold( false );
	ShowConfigButton5->Bold( false );
	ShowConfigButton6->Bold( true );
	UI_Refresh( );
}

void ConfigWindow_Show( )
{
	mainFrame->show( );
	//mainFrame->Focus( );
	ShowCategory0( );
}

void ConfigWindow_Hide( )
{
	mainFrame->hide( );
	//mainFrame->ClearFocus( );
}

void ToggleConfigWindowState( Button *btn )
{
	ConfigWindowHidden = !ConfigWindowHidden;

	if ( ShowConfigButton )
	{
		if ( ConfigWindowHidden )
		{
			ConfigWindow_Hide( );
			ShowConfigButton->setText( "Show dota config" );
		}
		else
		{
			ConfigWindow_Show( );
			ShowConfigButton->setText( "Hide dota config" );
		}
	}
}

void ConfigWindowKeyDown( const Event * evt )
{
	KeyboardEventData *data = evt->data<KeyboardEventData>( );
	int keyCode = data->code;
	if ( keyCode == KEYCODE::KEY_F7 )
	{
		ToggleConfigWindowState( NULL );
	}
}


//int SkillPanelHotkeys_NORMAL[ 12 ];
//int SkillPanelHotkeys_QUICK[ 12 ];
//int SkillPanelHotkeys_AUTO[ 12 ];
//


struct ConfigItem
{
	int type; // 1 hotkey 2 integer 3 string 4 bool
	std::string section;
	std::string key;
	int intval;
	bool boolval;
	std::string strval;
};

std::vector< ConfigItem *> ConfigItemList;



void SaveDotaConfig( )
{
	CIniWriter DotaConfigIni( configfilename );

	for ( auto s : ConfigItemList )
	{
		if ( s->type == 1 )
			DotaConfigIni.WriteString( s->section.c_str( ), s->key.c_str( ), CovertKeyCodeToString( s->intval ).c_str( ) );
		else if ( s->type == 2 )
			DotaConfigIni.WriteInteger( s->section.c_str( ), s->key.c_str( ), s->intval );
		else if ( s->type == 3 )
			DotaConfigIni.WriteString( s->section.c_str( ), s->key.c_str( ), s->strval.c_str( ) );
		else if ( s->type == 4 )
			DotaConfigIni.WriteBOOLean( s->section.c_str( ), s->key.c_str( ), s->boolval );
	}
}

void NeedUpdateConfig( Button* button )
{
	SaveDotaConfig( );
}



static void NeedUpdateConfig2( CheckBox* cb, bool flag ) {
	SaveDotaConfig( );
}

static void NeedUpdateConfig3( DropDownList* db, DropDownListItem dbi )
{
	SaveDotaConfig( );
}

std::string GetNormalHotkeyNameByID( int id )
{
	switch ( id )
	{
	case 0:
		return "BindMove";

	case 1:
		return "BindStop";

	case 2:
		return "BindHold";

	case 3:
		return "BindAttack";


	case 4:
		return "BindPatrol";

	case 5:
		return "SkillSlot5";

	case 6:
		return "SkillSlot6";

	case 7:
		return "BindOpenHeroSkills";


	case 8:
		return "SkillSlot1";

	case 9:
		return "SkillSlot2";

	case 10:
		return "SkillSlot3";

	case 11:
		return "SkillSlot4";


	default:
		break;
	}

	return "";
}


std::string GetQuickHotkeyNameByID( int id )
{
	switch ( id )
	{
	case 5:
		return "QuickCastSlot5";
	case 6:
		return "QuickCastSlot6";
	case 8:
		return "QuickCastSlot1";
	case 9:
		return "QuickCastSlot2";
	case 10:
		return "QuickCastSlot3";
	case 11:
		return "QuickCastSlot4";

	default:
		break;
	}

	return "";
}


CheckBox * AddCheckBox( UISimpleFrame * parent, float posx, float posy, std::string section, std::string key, std::string name )
{
	ConfigItem * tmpConfigItem = new ConfigItem( );
	CIniReader DotaConfigIni( configfilename );


	tmpConfigItem->boolval = key.empty( ) ? 0 : DotaConfigIni.ReadBOOLean( section.c_str( ), key.c_str( ), FALSE );

	CheckBox *	cbMenuCheckBox;
	cbMenuCheckBox = new CheckBox(
		parent,
		0.02f, NeedUpdateConfig2,
		&tmpConfigItem->boolval,
		0, 0, tmpConfigItem->boolval );

	cbMenuCheckBox->setRelativePosition(
		POS_UL,
		parent,
		POS_UL,
		posx, posy );
	tmpConfigItem->type = key.empty( ) ? -1 : 4;
	tmpConfigItem->section = section;
	tmpConfigItem->key = key;


	Label * lbMenuLabel = new Label( parent, name.c_str( ), 0.013f );
	lbMenuLabel->setRelativePosition(
		POS_L,
		cbMenuCheckBox->getFrame( ),
		POS_R,
		0.01f, 0 );

	ConfigItemList.push_back( tmpConfigItem );
	return cbMenuCheckBox;
}




DropDownList * AddDropBox( UISimpleFrame * parent, float posx, float posy, std::string section, std::string key, std::string name )
{
	ConfigItem * tmpConfigItem = new ConfigItem( );
	CIniReader DotaConfigIni( configfilename );


	tmpConfigItem->boolval = key.empty( ) ? 0 : DotaConfigIni.ReadBOOLean( section.c_str( ), key.c_str( ), FALSE );

	DropDownList *	cbMenuCheckBox;
	cbMenuCheckBox = new DropDownList( parent,
		0.06f, 0.04f, NeedUpdateConfig3, 0.1f, 0.08f );

	cbMenuCheckBox->setRelativePosition(
		POS_UL,
		parent,
		POS_UL,
		posx, posy );



	tmpConfigItem->type = key.empty( ) ? -1 : 4;
	tmpConfigItem->section = section;
	tmpConfigItem->key = key;


	Label * lbMenuLabel = new Label( parent, name.c_str( ), 0.013f );
	lbMenuLabel->setRelativePosition(
		POS_L,
		cbMenuCheckBox->getFrame( ),
		POS_R,
		0.01f, 0 );

	ConfigItemList.push_back( tmpConfigItem );
	return cbMenuCheckBox;
}


HotkeyButton * AddHotkeyButton( UISimpleFrame * parent, float posx, float posy, std::string section, std::string key )
{
	ConfigItem * tmpConfigItem = new ConfigItem( );
	CIniReader DotaConfigIni( configfilename );


	tmpConfigItem->intval = key.empty( ) ? 0 : CovertStringToKeyCode( DotaConfigIni.ReadString( section.c_str( ), key.c_str( ), "0" ) );

	HotkeyButton *	BtnMenuHotkey;
	BtnMenuHotkey = new HotkeyButton(
		parent,
		0.11f, 0.035f,
		&tmpConfigItem->intval,
		tmpConfigItem->intval,
		0, 0, NeedUpdateConfig/*, true */ );



	BtnMenuHotkey->setRelativePosition(
		POS_UL,
		parent,
		POS_UL,
		posx, posy );
	tmpConfigItem->type = key.empty( ) ? -1 : 1;
	tmpConfigItem->section = section;
	tmpConfigItem->key = key;

	ConfigItemList.push_back( tmpConfigItem );
	return BtnMenuHotkey;
}

EditBoxButton * AddEditBox( UISimpleFrame * parent, float posx, float posy, std::string section, std::string key )
{
	ConfigItem * tmpConfigItem = new ConfigItem( );
	CIniReader DotaConfigIni( configfilename );


	tmpConfigItem->strval = key.empty( ) ? "" : DotaConfigIni.ReadString( section.c_str( ), key.c_str( ), "" );

	EditBoxButton *	BtnMenuHotkey;
	BtnMenuHotkey = new EditBoxButton(
		parent,
		0.11f, 0.035f,
		&tmpConfigItem->strval,
		tmpConfigItem->strval, NeedUpdateConfig/*, true */ );

	BtnMenuHotkey->setRelativePosition(
		POS_UL,
		parent,
		POS_UL,
		posx, posy );
	tmpConfigItem->type = key.empty( ) ? -1 : 3;
	tmpConfigItem->section = section;
	tmpConfigItem->key = key;

	ConfigItemList.push_back( tmpConfigItem );
	return BtnMenuHotkey;
}



void AddLabel( )
{

}

void AddCheckBox( )
{

}


void InitConfigMenu1( )
{
	AddCheckBox( contentFrame1, 0.02f, -0.05, "GAMEOPTIONS", "AutoFPSLimit", "Auto FPS Limit" );

}

void InitConfigMenu2( )
{
	AddCheckBox( contentFrame2, 0.02f, -0.05, "HEROOPTIONS", "Meepo_NumbersOverheadClones", "Numbers Overhead Meepo Clones" );

}

void InitConfigMenu3( )
{
	//AddDropBox( contentFrame3, 0.02f, -0.05, "VISUALS", "EscClearsChat", "Press Esc for clear chat" );


	AddEditBox( contentFrame3, 0.02f, -0.20, "GAMEOPTIONS", "MaxFPS" );
}

void InitConfigMenu4( )
{
	int id = 0;
	for ( int Y = 0; Y < 3; Y++ )
	{
		for ( int X = 0; X < 4; X++ )
		{
			AddHotkeyButton( contentFrame4, 0.02f + ( X * 0.1f ), -0.05f + ( Y * -0.035f ), "HOTKEYS", GetNormalHotkeyNameByID( id ) );
			id++;
		}

	}

	id = 0;
	for ( int Y = 0; Y < 3; Y++ )
	{
		for ( int X = 0; X < 4; X++ )
		{
			auto BtnMenuHotkey = AddHotkeyButton( contentFrame4, 0.02f + ( X * 0.1f ), -0.18f + ( Y * -0.035f ), "HOTKEYS", GetQuickHotkeyNameByID( id ) );


			if ( ( id >= 5 && id <= 6 )
				|| ( id >= 8 && id <= 11 ) )
			{
				;
			}
			else
			{
				BtnMenuHotkey->setText( "" );
				BtnMenuHotkey->enable( false );
			}
			id++;
		}
	}


	AddCheckBox( contentFrame4, 0.02f, -0.30, "HOTKEYS", "DisableAllDefaultHotkeys", "Disable All Default Hotkeys" );
	AddCheckBox( contentFrame4, 0.02f, -0.33, "HOTKEYS", "ShopsQWERTY", "Using QWERTY for all shops" );



}


void InitConfigMenu5( )
{
	
}

void InitConfigMenu6( )
{
	
}
const char * configfilename = ".\\config.dota.ini";

void __stdcall SetConfigFileName( const char * configfilenam )
{
	configfilename = configfilenam;
}

void LoadDotaConfig( )
{
	CIniReader DotaConfigIni( configfilename );

}

void __stdcall ShowConfigEditButton( BOOL show )
{
	if ( show )
		ShowConfigButton->_backdrop->show( ); 
	else 
		ShowConfigButton->_backdrop->hide( );
}

void InitConfigWindow( const Event * )
{
	if ( !ConfigWindowInitialized )
	{
		LoadDotaConfig( );

		cout << "Load config";

		ShowConfigButton = new Button(
			UISimpleFrame::GetGameSimpleConsole( ),
			0.09f, 0.027f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			&ToggleConfigWindowState,
			NULL,
			false,
			false,
			0.011f
		);


		ShowConfigButton->setText( "Show dota config" );
		ShowConfigButton->setAbsolutePosition( UILayoutFrame::POSITION_BOTTOM_RIGHT,
			0.79f, 0.155f );

		ShowConfigButton->_backdrop->hide( );

		MainDispatcher( )->listen( EVENT_KEY_DOWN, ConfigWindowKeyDown );


		mainFrame = UISimpleFrame::Create( UISimpleFrame::GetGameSimpleConsole( ) );
		mainFrame->setBackground( Skin::getPathByName( "EscMenuBackground" ) );
		mainFrame->setBorder( Skin::getPathByName( "EscMenuBorder" ) );
		mainFrame->setBorderWidth( 0.015f );
		mainFrame->setPadding( 0.005f );
		mainFrame->applyTextureSettings( );
		mainFrame->setWidth( OPTIONMENU_WIDTH );
		mainFrame->setHeight( OPTIONMENU_HEIGHT );
		mainFrame->setAbsolutePosition( UILayoutFrame::POSITION_CENTER, OPTIONMENU_CENTER_X, OPTIONMENU_CENTER_Y );
		mainFrame->applyPosition( );
		mainFrame->show( );


	

		navigationFrame = UISimpleFrame::Create( mainFrame );
		navigationFrame->setBackground( Skin::getPathByName( "EscMenuBackground" ) );
		navigationFrame->setBorder( Skin::getPathByName( "EscMenuBorder" ) );
		navigationFrame->setBorderWidth( 0.015f );
		navigationFrame->setPadding( 0.005f );
		navigationFrame->applyTextureSettings( );
		navigationFrame->setWidth( NAVIGATION_WIDTH );
		navigationFrame->setHeight( OPTIONMENU_HEIGHT );
		navigationFrame->setRelativePosition( POS_UL, mainFrame, POS_UL, 0, 0 );
		navigationFrame->applyPosition( );
		navigationFrame->show( );

		contentFrame = UISimpleFrame::Create( mainFrame );
		contentFrame->setBackground( Skin::getPathByName( "EscMenuBackground" ) );
		contentFrame->setBorder( Skin::getPathByName( "EscMenuBorder" ) );
		contentFrame->setBorderWidth( 0.015f );
		contentFrame->setPadding( 0.005f );
		contentFrame->applyTextureSettings( );
		contentFrame->setWidth( OPTIONMENU_WIDTH - NAVIGATION_WIDTH );
		contentFrame->setHeight( OPTIONMENU_HEIGHT );
		contentFrame->setRelativePosition( UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0 );
		contentFrame->applyPosition( );
		contentFrame->show( );


		contentLabel1 = new Label( contentFrame, "Dota Allstars Config Editor. Version: |cFFFF0000FATAL ERROR|r", 0.02f );
		contentLabel1->setRelativePosition(
			POS_UL,
			contentFrame,
			POS_UL,
			0.02f, -0.01f );

		contentFrame1 = UISimpleFrame::Create( mainFrame );
		contentFrame1->setBackground( Skin::getPathByName( "EscMenuBackground" ) );
		contentFrame1->setBorder( "" );
		contentFrame1->setBorderWidth( 0.0f );
		contentFrame1->setPadding( 0.005f );
		contentFrame1->applyTextureSettings( );
		contentFrame1->setWidth( OPTIONMENU_WIDTH - NAVIGATION_WIDTH );
		contentFrame1->setHeight( OPTIONMENU_HEIGHT );
		contentFrame1->setRelativePosition( UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0 );
		contentFrame1->applyPosition( );

		ConfigLabel1 = new Label( contentFrame1, "Game Settings Section", 0.02f );
		ConfigLabel1->setRelativePosition(
			POS_UL,
			contentFrame1,
			POS_UL,
			0.02f, -0.01f );


		InitConfigMenu1( );

		contentFrame1->hide( );

		contentFrame2 = UISimpleFrame::Create( mainFrame );
		contentFrame2->setBackground( Skin::getPathByName( "EscMenuBackground" ) );
		contentFrame2->setBorder( "" );
		contentFrame2->setBorderWidth( 0.0f );
		contentFrame2->setPadding( 0.005f );
		contentFrame2->applyTextureSettings( );
		contentFrame2->setWidth( OPTIONMENU_WIDTH - NAVIGATION_WIDTH );
		contentFrame2->setHeight( OPTIONMENU_HEIGHT );
		contentFrame2->setRelativePosition( UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0 );
		contentFrame2->applyPosition( );
		ConfigLabel2 = new Label( contentFrame2, "Specific settings for some heroes", 0.02f );
		ConfigLabel2->setRelativePosition(
			POS_UL,
			contentFrame2,
			POS_UL,
			0.02f, -0.01f );

		InitConfigMenu2( );

		contentFrame2->hide( );

		contentFrame3 = UISimpleFrame::Create( mainFrame );
		contentFrame3->setBackground( Skin::getPathByName( "EscMenuBackground" ) );
		contentFrame3->setBorder( "" );
		contentFrame3->setBorderWidth( 0.0f );
		contentFrame3->setPadding( 0.005f );
		contentFrame3->applyTextureSettings( );
		contentFrame3->setWidth( OPTIONMENU_WIDTH - NAVIGATION_WIDTH );
		contentFrame3->setHeight( OPTIONMENU_HEIGHT );
		contentFrame3->setRelativePosition( UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0 );
		contentFrame3->applyPosition( );
		ConfigLabel3 = new Label( contentFrame3, "Visuals Settings", 0.02f );
		ConfigLabel3->setRelativePosition(
			POS_UL,
			contentFrame3,
			POS_UL,
			0.02f, -0.01f );


		InitConfigMenu3( );

		contentFrame3->hide( );


		contentFrame4 = UISimpleFrame::Create( mainFrame );
		contentFrame4->setBackground( Skin::getPathByName( "EscMenuBackground" ) );
		contentFrame4->setBorder( "" );
		contentFrame4->setBorderWidth( 0.0f );
		contentFrame4->setPadding( 0.005f );
		contentFrame4->applyTextureSettings( );
		contentFrame4->setWidth( OPTIONMENU_WIDTH - NAVIGATION_WIDTH );
		contentFrame4->setHeight( OPTIONMENU_HEIGHT );
		contentFrame4->setRelativePosition( UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0 );
		contentFrame4->applyPosition( );

		ConfigLabel4 = new Label( contentFrame4, "HOTKEYS. SKILLS AND QUICK CAST", 0.02f );
		ConfigLabel4->setRelativePosition(
			POS_UL,
			contentFrame4,
			POS_UL,
			0.02f, -0.01f );


		InitConfigMenu4( );

		contentFrame4->hide( );

		contentFrame5 = UISimpleFrame::Create( mainFrame );
		contentFrame5->setBackground( Skin::getPathByName( "EscMenuBackground" ) );
		contentFrame5->setBorder( "" );
		contentFrame5->setBorderWidth( 0.0f );
		contentFrame5->setPadding( 0.005f );
		contentFrame5->applyTextureSettings( );
		contentFrame5->setWidth( OPTIONMENU_WIDTH - NAVIGATION_WIDTH );
		contentFrame5->setHeight( OPTIONMENU_HEIGHT );
		contentFrame5->setRelativePosition( UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0 );
		contentFrame5->applyPosition( );

		ConfigLabel5 = new Label( contentFrame5, "Messages on game start", 0.02f );
		ConfigLabel5->setRelativePosition(
			POS_UL,
			contentFrame5,
			POS_UL,
			0.02f, -0.01f );


		InitConfigMenu5( );

		contentFrame5->hide( );

		contentFrame6 = UISimpleFrame::Create( mainFrame );
		contentFrame6->setBackground( Skin::getPathByName( "EscMenuBackground" ) );
		contentFrame6->setBorder( "" );
		contentFrame6->setBorderWidth( 0.0f );
		contentFrame6->setPadding( 0.005f );
		contentFrame6->applyTextureSettings( );
		contentFrame6->setWidth( OPTIONMENU_WIDTH - NAVIGATION_WIDTH );
		contentFrame6->setHeight( OPTIONMENU_HEIGHT );
		contentFrame6->setRelativePosition( UILayoutFrame::POSITION_TOP_RIGHT, mainFrame, UILayoutFrame::POSITION_TOP_RIGHT, 0, 0 );
		contentFrame6->applyPosition( );

		ConfigLabel6 = new Label( contentFrame6, "Quick chat messages", 0.02f );
		ConfigLabel6->setRelativePosition(
			POS_UL,
			contentFrame6,
			POS_UL,
			0.02f, -0.01f );


		InitConfigMenu6( );

		contentFrame6->hide( );



		ShowConfigButton1 = new Button( navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory1, 
			"Game Settings Section",
			true
		);


		ShowConfigButton1->setText( "GAMEOPTIONS" );
		ShowConfigButton1->setRelativePosition( POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-( 0.035f * 0 + 0.03f )
		);
		ShowConfigButton1->applyPosition( );

		ShowConfigButton2 = new Button( navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory2,
			"Specific settings for some heroes", 
			true
		);
		ShowConfigButton2->setText( "HEROOPTIONS" );
		ShowConfigButton2->setRelativePosition( POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-( 0.035f * 1 + 0.03f )
		);
		ShowConfigButton2->applyPosition( );


		ShowConfigButton3 = new Button( navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory3,
			"Visuals Settings", 
			true
		);
		ShowConfigButton3->setText( "VISUALS" );
		ShowConfigButton3->setRelativePosition( POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-( 0.035f * 2 + 0.03f )
		);
		ShowConfigButton3->applyPosition( );


		ShowConfigButton4 = new Button( navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory4,
			"HOTKEYS. SKILLS AND QUICK CAST",
			true
		);
		ShowConfigButton4->setText( "HOTKEYS" );
		ShowConfigButton4->setRelativePosition( POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-( 0.035f * 3 + 0.03f )
		);
		ShowConfigButton4->applyPosition( );


		ShowConfigButton5 = new Button( navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory5,
			"Messages on game start",
			true
		);
		ShowConfigButton5->setText( "ONSTART" );
		ShowConfigButton5->setRelativePosition( POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-( 0.035f * 4 + 0.03f )
		);
		ShowConfigButton5->applyPosition( );



		ShowConfigButton6 = new Button( navigationFrame,
			0.114f, 0.035f,
			UISimpleButton::MOUSEBUTTON_LEFT,
			UISimpleButton::STATE_ENABLED,
			ShowCategory6,
			"Quick chat messages",
			true
		);
		ShowConfigButton6->setText( "QUICKCHAT" );
		ShowConfigButton6->setRelativePosition( POS_UL, navigationFrame, POS_UL,
			0.0285f,
			-( 0.035f * 5 + 0.03f )
		);
		ShowConfigButton6->applyPosition( );

		ShowCategory0(  );

		mainFrame->hide( );

		ConfigWindowInitialized = true;
	}

}


void CleanupConfigWindow( const Event * )
{
	if ( ConfigWindowInitialized )
	{
		HotkeyButton::Cleanup( );
		EditBoxButton::Cleanup( );
		ConfigItemList.clear( );


		/*	UISimpleFrame::Destroy(contentFrame);
			UISimpleFrame::Destroy(contentFrame1);
			UISimpleFrame::Destroy(contentFrame2);
			UISimpleFrame::Destroy(contentFrame3);
			UISimpleFrame::Destroy(contentFrame4);*/

			//UISimpleFrame::Destroy(navigationFrame);
		//UISimpleFrame::Destroy(mainFrame);
	//delete mainFrame;
		//	delete contentFrame;
			//delete navigationFrame;
	}
	ConfigWindowInitialized = false;
}

void RegisterConfigWindow( )
{
	MainDispatcher( )->listen( EVENT_GAME_START, InitConfigWindow );
	MainDispatcher( )->listen( EVENT_GAME_END, CleanupConfigWindow );
}


