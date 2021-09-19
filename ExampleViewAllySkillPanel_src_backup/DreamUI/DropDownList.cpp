#include "stdafx.h"
#include "DropDownList.h"

#include "UISimpleFrame.h"
#include "UISimpleTexture.h"
#include "UISimpleButton.h"
#include "Observer.h"
#include "EventData.h"
#include "Label.h"
#include "Game.h"
#include "Event.h"
#include "GameEventObserver.h"

static std::map<UISimpleButton*, DropDownList> ButtonOwnerMap;

static void DropDownListHandler(Observer* ob, uint32_t evtId) 
{
	DropDownList *drop = *(ob->data<DropDownList*>());
	switch (evtId) {
	case EVENT_CLICK:
		//drop->toggle();
		break;
	default:
		break;
	}
}

DropDownList::DropDownList(
	IUIObject				*parent,
	float					titleHeight,
	float					itemHeight,
	DropDownListCallback	callback,
	float					minWidth,
	float					fontHeightFactor )
	: Frame( parent, minWidth, titleHeight ),
	textHeightFactor( fontHeightFactor ),
	alignHoriz( ALIGN_HORIZ_LEFT ),
	alignVert( ALIGN_VERT_CENTER ),
	marginHoriz( 0.001f ),
	marginVert( 0.001f ),
	itemHeight( itemHeight ),
	callback( callback )
{
	//titleOb
	DropDownList *_this = this;
	titleOb = Observer::Create( DropDownListHandler );
	titleOb->setData<DropDownList*>( &_this );

	//titleClicker
	titleClicker = UISimpleButton::Create( getFrame( ) );
	titleClicker->setWidth( minWidth );
	titleClicker->setHeight( titleHeight );
	titleClicker->setState( UISimpleButton::STATE_ENABLED );
	titleClicker->setMouseButton( UISimpleButton::MOUSEBUTTON_LEFT );
	titleClicker->setRelativePosition( POS_C, getFrame( ), POS_C, 0, 0 );
	titleClicker->setClickEventObserver( EVENT_CLICK, titleOb );
	SimpleButtonHook( titleClicker->base<war3::CSimpleButton*>( ) );

	//arrow
	//dropDownFrame

	setTexture( );
}


DropDownList::DropDownList(
	UISimpleFrame *parent,
	float					titleHeight,
	float					itemHeight,
	DropDownListCallback	callback,
	float					minWidth,
	float					fontHeightFactor )
	: Frame( NULL, minWidth, titleHeight ),
	textHeightFactor( fontHeightFactor ),
	alignHoriz( ALIGN_HORIZ_LEFT ),
	alignVert( ALIGN_VERT_CENTER ),
	marginHoriz( 0.001f ),
	marginVert( 0.001f ),
	itemHeight( itemHeight ),
	callback( callback )
{
	setParent( parent );


	//titleOb
	DropDownList *_this = this;
	titleOb = Observer::Create( DropDownListHandler );
	titleOb->setData<DropDownList*>( &_this );

	//titleClicker
	titleClicker = UISimpleButton::Create( getFrame( ) ? getFrame( ) : parent );


	titleClicker->setStateTexture( UISimpleButton::STATE_DISABLED, "UI\\Widgets\\EscMenu\\Human\\checkbox-background-disabled.blp" );
	titleClicker->setStateTexture( UISimpleButton::STATE_ENABLED, Skin::getPathByName( "ReplayCheckBoxNormal" ) );
	titleClicker->setStateTexture( UISimpleButton::STATE_PUSHED, Skin::getPathByName( "ReplayCheckBoxPressed" ) );

	titleClicker->setWidth( minWidth );
	titleClicker->setHeight( titleHeight );
	titleClicker->setState( UISimpleButton::STATE_ENABLED );
	titleClicker->setMouseButton( UISimpleButton::MOUSEBUTTON_LEFT );
	titleClicker->setRelativePosition( POS_C, getFrame( ), POS_C, 0, 0 );
	titleClicker->setClickEventObserver( EVENT_CLICK, titleOb );
	SimpleButtonHook( titleClicker->base<war3::CSimpleButton*>( ) );

	//arrow
	//dropDownFrame

	setTexture( );
}

DropDownList::~DropDownList()
{
	//ButtonOwnerMap
}

void DropDownList::setTexture( const char *bgTexPath /*= NULL*/, const char *bgDisTexPath /*= NULL*/, const char *borderTexPath /*= NULL*/, const char *borderDisTexPath /*= NULL*/, const char *arrowTexPath /*= NULL*/, const char *arrowDisTexPath /*= NULL */ )
{
	setBackground(bgTexPath ? bgTexPath : "UI\\widgets\\BattleNet\\bnet-tooltip-background.blp");
	setPadding(0.004f);
	setBorder(borderTexPath ? borderTexPath : "UI\\Widgets\\EscMenu\\Human\\editbox-border.blp",
		0.01f
	);
	//ÏÂÀ­£¬Èý½Ç
}
