#include "stdafx.h"
#include "Button.h"
#include "Label.h"
#include "ToolTip.h"

#include "Tools.h"

#include "Game.h"
#include "GameEventObserver.h"

static std::set<Button*> ButtonSet;
static std::set<Button*> ButtonSetAll;//无条件保存所有实例

static char ButtonDefaultNameStr[ 0x50 ];
uint32_t Button::indexTotal = 0;

void Button_EventHandler( Observer *ob, uint32_t evtId ) {
	Button *button = *( ob->data<Button*>( ) );
	if ( !button ) OutputScreen( 10, "|cffff0000Error: Button_EventHandler(0x%X, 0x%X)|r", ob, evtId );
	//TODO 需要按钮按下事件
	switch ( evtId ) {
	case EVENT_CLICK:
		button->push( );
		SoundPlay( "GlueScreenClick", NULL, 0 );
		break;
	case EVENT_MOUSE_OVER:
		//OutputScreen(10, "button = 0x%X", button->getButton()->base<war3::CSimpleButton*>());
		button->showToolTip( );
		break;
	case EVENT_MOUSE_OUT:
		button->hideToolTip( );
		break;
	default:
		break;
	}
}

static void onMouseDown( const Event* evt ) {
	MouseEventData *data = evt->data<MouseEventData>( );
	if ( data->buttonPushed != NULL ) {
		for ( std::set<Button*>::iterator iter = ButtonSetAll.begin( );
			iter != ButtonSetAll.end( ); ++iter )
		{
			war3::CSimpleButton* b = ( *iter )->getButton( )->base<war3::CSimpleButton*>( );
			if ( b == data->buttonPushed ) {
				if ( b->currentState == UISimpleButton::STATE_ENABLED		//按钮为可用状态
					&& ( ( b->mouseButtonFlags >> 4 ) & data->mouseCode )		//所按鼠标键值被按钮接受//TODO 这里两个mouse code不一致，需要统一
					)
				{
					//OutputScreen(10, "mouseButtonFlags = 0x%X, mouseCode = 0x%X", b->mouseButtonFlags, data->mouseCode);
					( *iter )->controlCoverNormal->hide( );
					( *iter )->controlCoverPushed->show( );
					( *iter )->getButton( )->setState( UISimpleButton::STATE_PUSHED );
					break;
				}
			}
		}
	}
}

static void onMouseUp( const Event* evt ) {
	MouseEventData *data = evt->data<MouseEventData>( );
	if ( data->buttonPushed != NULL ) {
		for ( std::set<Button*>::iterator iter = ButtonSetAll.begin( );
			iter != ButtonSetAll.end( ); ++iter )
		{
			war3::CSimpleButton* b = ( *iter )->getButton( )->base<war3::CSimpleButton*>( );
			if ( b == data->buttonPushed ) {
				if ( b->currentState == UISimpleButton::STATE_PUSHED ) {
					( *iter )->controlCoverPushed->hide( );
					( *iter )->controlCoverNormal->show( );
					( *iter )->controlCoverDisabled->hide( );
					( *iter )->getButton( )->setState( UISimpleButton::STATE_ENABLED );
					break;
				}
			}
		}
	}
}

void Button::enable( bool flag ) {
	if ( !this ) return;
	//OutputScreen(10, "button is %s", flag? "enabled" : "disabled");
	if ( flag ) {
		this->getButton( )->setState( UISimpleButton::STATE_ENABLED );
		this->controlCoverDisabled->hide( );
		this->controlCoverNormal->show( );
		this->controlCoverPushed->hide( );
		//this->_displayText->setColorFloat(1, 1, 1, 1);
		_caption->activate( true );
		this->_isDisabled = false;
	}
	else {
		this->getButton( )->setState( UISimpleButton::STATE_DISABLED );
		this->controlCoverDisabled->show( );
		this->controlCoverNormal->hide( );
		this->controlCoverPushed->hide( );
		//this->_displayText->setColorFloat(0.5f, 0.5f, 0.5f, 1);
		_caption->activate( false );
		this->_isDisabled = true;
	}
}

void Button::showText( bool flag ) {
	if ( !this ) return;
	if ( flag ) {
		//this->_displayText->setColorFloatAlpha(1);
		//this->_displayTextShadow->setColorFloatAlpha(.9f);
		_caption->show( true );
		this->_isTextShown = true;
	}
	else {
		//this->_displayText->setColorFloatAlpha(0);
		//this->_displayTextShadow->setColorFloatAlpha(0);
		_caption->show( false );
		this->_isTextShown = false;
	}
}

bool Button::isTextShown( ) {
	return _isTextShown;
}


Button::Button( ) : index( ), _backdrop( ), _control( ),
_controlCallback( ),
_caption( ),
_tooltip( ),
controlCoverNormal( ), controlCoverPushed( ), controlCoverDisabled( )
{
	_isDisabled = false;
	_isTextShown = true;
}

void Button::SetControlCoverNormal( const char * backgroundpath, const char * borderpath, float borderwidth, float padding )
{
	controlCoverNormal->setBackground( backgroundpath );
	controlCoverNormal->setBorder( borderpath );
	controlCoverNormal->setBorderWidth( borderwidth );
	controlCoverNormal->setPadding( padding );
	controlCoverNormal->applyTextureSettings( );
}

void Button::SetControlCoverPushed( const char * backgroundpath, const char * borderpath, float borderwidth, float padding )
{
	controlCoverPushed->setBackground( backgroundpath );
	controlCoverPushed->setBorder( borderpath );
	controlCoverPushed->setBorderWidth( borderwidth );
	controlCoverPushed->setPadding( padding );
	controlCoverPushed->applyTextureSettings( );
}

void Button::SetControlCoverDisabled( const char * backgroundpath, const char * borderpath, float borderwidth, float padding )
{
	controlCoverDisabled->setBackground( backgroundpath );
	controlCoverDisabled->setBorder( borderpath );
	controlCoverDisabled->setBorderWidth( borderwidth );
	controlCoverDisabled->setPadding( padding );
	controlCoverDisabled->applyTextureSettings( );
}



Button::Button(
	UISimpleFrame*			parent,
	float					width,
	float					height,
	uint32_t				mouseClickFlag,
	UISimpleButton::State	initClickState,
	ButtonCallback			callback,
	const char*				tooltip,
	bool					dontStore,
	bool					noSurface,
	float					fontHeight
) : index( ), _backdrop( ), _control( ),
_controlCallback( ), _caption( ), _fontHeight( fontHeight ), _tooltip( ),
controlCoverNormal( ), controlCoverPushed( ), controlCoverDisabled( )
{
	this->_isDisabled = false;
	this->_isTextShown = true;
	//this->Init(parent, width, height, mouseClickFlag, initClickState, callback, tooltip, dontStore);
	Button *button = this;
	button->eventObserver = Observer::Create( Button_EventHandler );
	button->eventObserver->setData<Button*>( &button );
	button->index = indexTotal++;

	UISimpleFrame * sf = UISimpleFrame::Create( parent );
	sf->setWidth( width );
	sf->setHeight( height );
	sf->show( );//默认没有纹理
	button->_backdrop = sf;

	//按钮表面，普通状态
	sf = UISimpleFrame::Create( button->_backdrop );
	sf->setWidth( width );
	sf->setHeight( height );
	sf->setRelativePosition( UILayoutFrame::POSITION_CENTER, button->_backdrop, UILayoutFrame::POSITION_CENTER, 0, 0 );
	if ( !noSurface ) {
		sf->setBackground( Skin::getPathByName( "EscMenuButtonBackground" ) );
		sf->setBorder( Skin::getPathByName( "EscMenuButtonBorder" ) );
		sf->setBorderWidth( 0.015f );
		sf->setPadding( 0.005f );
		sf->applyTextureSettings( );
	}
	sf->show( );
	button->controlCoverNormal = sf;

	//按钮表面，按下状态
	sf = UISimpleFrame::Create( button->_backdrop );
	sf->setWidth( width );
	sf->setHeight( height );
	sf->setRelativePosition( UILayoutFrame::POSITION_CENTER, button->_backdrop, UILayoutFrame::POSITION_CENTER, 0, 0 );
	if ( !noSurface ) {
		sf->setBackground( Skin::getPathByName( "EscMenuButtonPushedBackground" ) );
		sf->setBorder( Skin::getPathByName( "EscMenuButtonPushedBorder" ) );
		sf->setBorderWidth( 0.015f );
		sf->setPadding( 0.005f );
		sf->applyTextureSettings( );
	}
	sf->hide( );
	button->controlCoverPushed = sf;

	//按钮表面，禁用状态
	sf = UISimpleFrame::Create( button->_backdrop );
	sf->setWidth( width );
	sf->setHeight( height );
	sf->setRelativePosition( UILayoutFrame::POSITION_CENTER, button->_backdrop, UILayoutFrame::POSITION_CENTER, 0, 0 );
	if ( !noSurface ) {
		sf->setBackground( Skin::getPathByName( "EscMenuButtonDisabledBackground" ) );
		sf->setBorder( Skin::getPathByName( "EscMenuButtonDisabledBorder" ) );
		sf->setBorderWidth( 0.015f );
		sf->setPadding( 0.005f );
		sf->applyTextureSettings( );
	}
	sf->hide( );
	button->controlCoverDisabled = sf;

	UISimpleButton *sb = UISimpleButton::Create( button->_backdrop );
	//sb->setMouseButton(UISimpleButton::MOUSEBUTTON_LEFT | UISimpleButton::MOUSEBUTTON_MIDDLE | UISimpleButton::MOUSEBUTTON_RIGHT);
	sb->setMouseButton( mouseClickFlag );
	sb->setWidth( width );
	sb->setHeight( height );
	sb->setRelativePosition( UILayoutFrame::POSITION_CENTER, button->_backdrop, UILayoutFrame::POSITION_CENTER, 0, 0 );

	sb->show( );
	sb->setMouseEventObserver( EVENT_MOUSE_OVER, EVENT_MOUSE_OUT, button->eventObserver );
	sb->setClickEventObserver( EVENT_CLICK, button->eventObserver );
	SimpleButtonHook( sb->base<war3::CSimpleButton*>( ) );
	button->_control = sb;
	button->_controlCallback = callback;

	//sprintf_s(ButtonDefaultNameStr, 0x50, "|cffffcc00B|rutton %d", button->index);
	//UISimpleFontString *sfss = UISimpleFontString::Create(button->_control);
	//UISimpleFontString *sfs = UISimpleFontString::Create(button->_control);
	//sfss->initFont(
	//	UIObject::GetPathByName("EscMenuTextFont")
	//	, fontHeight, false);
	//sfs->initFont(
	//	UIObject::GetPathByName("EscMenuTextFont")
	//	, fontHeight, false);
	//sfss->setColorFloat(0,0,0,.9f);
	//sfss->setRelativePosition(POS_C, sfs, POS_C, BUTTON_TEXT_OFFSET_X, BUTTON_TEXT_OFFSET_Y);
	//sfs->setRelativePosition(POS_C, button->_control, POS_C, 0, 0);
	//sfs->setText(ButtonDefaultNameStr);//默认显示按钮id
	//sprintf_s(ButtonDefaultNameStr, 0x50, "Button %d", button->index);
	//sfss->setText(ButtonDefaultNameStr);

	//button->_displayTextShadow = sfss;
	//button->_displayText	 = sfs;

	sprintf_s( ButtonDefaultNameStr, 0x50, "|cffffcc00B|rutton %d", button->index );
	_caption = new Label(
		button->_control,
		ButtonDefaultNameStr,
		fontHeight,
		Color::GOLD,
		Color( .5f, .5f, .5f, 1.f ),
		Color( 0, 0, 0, .9f ),
		UIObject::GetPathByName( "EscMenuTextFont" ),
		false
	);

	if ( tooltip ) {
		ToolTip *tp = ToolTip::Create(
			button->_control,
			0.15f,//MUSTDO 自动调整
			0.03f,
			tooltip,
			true
		);
		//tp->bindButton( button );
		tp->hide( );
		button->_tooltip = tp;
	}
	else {
		button->_tooltip = NULL;
	}

	setTextAlign( ALIGN_HORIZ_CENTER, ALIGN_VERT_CENTER );

	if ( !dontStore ) ButtonSet.insert( button );
	ButtonSetAll.insert( button );
}

Button::~Button( ) {
	boundData.clear( );
	ToolTip::Destroy( this->_tooltip );
	//UISimpleFontString::Destroy(this->_displayText);
	//UISimpleFontString::Destroy(this->_displayTextShadow);
	delete _caption;
	UISimpleFrame::Destroy( this->controlCoverNormal );
	UISimpleFrame::Destroy( this->controlCoverPushed );
	UISimpleFrame::Destroy( this->controlCoverDisabled );
	UISimpleButton::Destroy( this->_control );
	UISimpleFrame::Destroy( this->_backdrop );
	Observer::Destroy( this->eventObserver );
	ButtonSetAll.erase( this );
}

void Button::applyPosition( ) {//FIXME 似乎没用
	_backdrop->applyPosition( );
	_control->applyPosition( );
	_tooltip->applyPosition( );
}

void Button::setAbsolutePosition( UISimpleButton::Position originPos, float x, float y ) {
	_backdrop->setAbsolutePosition( originPos, x, y );
	applyPosition( );	//貌似没意义
}

void Button::setRelativePosition( uint32_t originPos, UILayoutFrame* target, uint32_t toPos, float relativeX, float relativeY ) {
	_backdrop->setRelativePosition( originPos, target, toPos, relativeX, relativeY );
	applyPosition( );	//貌似没意义?
}

void Button::setText( const char *text ) {
	if ( !this || !text ) return;
	_caption->setText( "%s", text );

	_org_text = text;
}
void Button::setText( std::string text ) {
	if ( !this ) return;
	_caption->setText( "%s", text.c_str( ) );
	_org_text = text;
}

void Button::Bold( bool flag )
{
	if ( !this ) return;
	if ( flag )
		_caption->setText( "|c00FF7F00%s|", _org_text.c_str( ) );
	else
		_caption->setText( "%s", _org_text.c_str( ) );
}

const char* Button::getText( ) const
{
	return _caption->getText( );
}


void Button::push( ) {
	if ( !this ) return;
	if ( this->_isDisabled ) return;
	if ( _controlCallback ) _controlCallback( this );
}

void Button::showToolTip( ) {
	if ( !this ) return;
	if ( _tooltip )
	{
		_tooltip->applyPosition( );
		_tooltip->show( );
	}
}

void Button::hideToolTip( ) {
	if ( !this ) return;
	if ( _tooltip )
	{
		_tooltip->hide( );
	}
}

UILayoutFrame* Button::getFrame( ) {
	if ( !this ) return NULL;
	return _control;
}

UISimpleButton* Button::getButton( ) {
	if ( !this ) return NULL;
	return _control;
}


Label* Button::getCaption( )
{
	if ( !this ) return NULL;
	return _caption;
}


void Button::Init( ) {
	ButtonSet.clear( );
	ButtonSetAll.clear( );
	MainDispatcher( )->listen( EVENT_MOUSE_DOWN, onMouseDown );
	MainDispatcher( )->listen( EVENT_MOUSE_UP, onMouseUp );
}

void Button::Cleanup( ) {
	for ( std::set<Button*>::iterator iter = ButtonSet.begin( );
		iter != ButtonSet.end( ); ++iter ) {
		delete ( *iter );
	}
	ButtonSet.clear( );
	ButtonSetAll.clear( );
}

void Button::bindData( int key, void *dataObj )
{
	boundData[ key ] = dataObj;
}

void* Button::getData( int key )
{
	if ( boundData.count( key ) ) return boundData[ key ];
	return NULL;
}

void Button::setWidth( float width )
{
	_backdrop->setWidth( width );
	_control->setWidth( width );
	controlCoverNormal->setWidth( width );
	controlCoverPushed->setWidth( width );
	controlCoverDisabled->setWidth( width );
}

void Button::setHeight( float height )
{
	_backdrop->setHeight( height );
	_control->setHeight( height );
	controlCoverNormal->setHeight( height );
	controlCoverPushed->setHeight( height );
	controlCoverDisabled->setHeight( height );
	_caption->setTextSize( height );
}

void Button::setTextAlign( TextAlignmentHorizontal alignHoriz, TextAlignmentVertical alignVert )
{
	if ( alignHoriz == ALIGN_HORIZ_CENTER )
	{
		if ( alignVert == ALIGN_VERT_CENTER )
		{
			_caption->setRelativePosition( POS_C, _control, POS_C );
		}
		else if ( alignVert == ALIGN_VERT_TOP )
		{
			_caption->setRelativePosition( POS_T, _control, POS_T );
		}
		else if ( alignVert == ALIGN_VERT_BOTTOM )
		{
			_caption->setRelativePosition( POS_B, _control, POS_B );
		}
	}

	else if ( alignHoriz == ALIGN_HORIZ_LEFT )
	{
		if ( alignVert == ALIGN_VERT_CENTER )
		{
			_caption->setRelativePosition( POS_L, _control, POS_L );
		}
		else if ( alignVert == ALIGN_VERT_TOP )
		{
			_caption->setRelativePosition( POS_UL, _control, POS_UL );
		}
		else if ( alignVert == ALIGN_VERT_BOTTOM )
		{
			_caption->setRelativePosition( POS_BL, _control, POS_BL );
		}
	}

	else if ( alignHoriz == ALIGN_HORIZ_RIGHT )
	{
		if ( alignVert == ALIGN_VERT_CENTER )
		{
			_caption->setRelativePosition( POS_R, _control, POS_R );
		}
		else if ( alignVert == ALIGN_VERT_TOP )
		{
			_caption->setRelativePosition( POS_UR, _control, POS_UR );
		}
		else if ( alignVert == ALIGN_VERT_BOTTOM )
		{
			_caption->setRelativePosition( POS_BR, _control, POS_BR );
		}
	}



}
