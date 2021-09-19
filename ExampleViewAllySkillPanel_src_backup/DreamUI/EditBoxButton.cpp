#include "EditBoxButton.h"


#include <utility>
#include <codecvt>

inline std::string WStringToString(std::wstring s)
{
	if (s.empty())
		return "";
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(s);
}

inline std::wstring StringToWString(std::string s)
{
	if (s.empty())
		return L"";
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(s);
}



static std::set<EditBoxButton *> EditBoxButtonSet;

static void CallbackEditBoxButton( Button *btn ) {
	EditBoxButton *btnhot = ( EditBoxButton * )btn;

	for ( std::set<EditBoxButton *>::iterator iter = EditBoxButtonSet.begin( );
		iter != EditBoxButtonSet.end( ); ++iter )
	{
		if ( ( *iter )->isEnteringText( ) && ( *iter ) != btn ) {
			( *iter )->toggleEnteringText( );
		}
	}


	btnhot->toggleEnteringText( );
	if ( btnhot->isEnteringText( ) ) {
		btnhot->setTimer( 35 );
	}
	else {
		btnhot->setTimer( -1 );
	}
	btnhot->setText(WStringToString( btnhot->_org_text ) );
}


EditBoxButton::EditBoxButton(
	UISimpleFrame*			parent,
	float					width,
	float					height,
	std::string *			text,
	std::string 			defaulttext,
	ButtonCallback			callback,
	bool					noSurface

) : Button( parent, width, height, UISimpleButton::MOUSEBUTTON_LEFT, UISimpleButton::STATE_ENABLED, CallbackEditBoxButton, NULL, true, true ),
_enteringText( false ), _pVarText( text ),
editBoxButtonCallback( callback )
{
	if ( text )
		*text = defaulttext;
	else if ( text )
		*text = "";

	controlCoverPushed->setBackground( Skin::getPathByName( "EscMenuEditBoxBackground" ) );
	controlCoverPushed->setBorder( Skin::getPathByName( "EscMenuEditBoxBorder" ) );
	controlCoverPushed->setBorderWidth( 0.009f );
	controlCoverPushed->setPadding( 0.005f, 0.005f, 0.005f, 0.005f );
	controlCoverPushed->applyTextureSettings( );
	controlCoverPushed->hide( );
	controlCoverPushed->show( );

	controlCoverNormal->setBackground( Skin::getPathByName( "EscMenuEditBoxBackground" ) );
	controlCoverNormal->setBorder( Skin::getPathByName( "EscMenuEditBoxBorder" ) );
	controlCoverNormal->setBorderWidth( 0.009f );
	controlCoverNormal->setPadding( 0.005f, 0.005f, 0.005f, 0.005f );
	controlCoverNormal->applyTextureSettings( );
	controlCoverNormal->show( );
	controlCoverNormal->hide( );


	controlCoverDisabled->setBackground( Skin::getPathByName( "EscMenuEditBoxBackground" ) );
	controlCoverDisabled->setBorder( Skin::getPathByName( "EscMenuEditBoxBorder" ) );
	controlCoverDisabled->setBorderWidth( 0.009f );
	controlCoverDisabled->setPadding( 0.005f, 0.005f, 0.005f, 0.005f );
	controlCoverDisabled->applyTextureSettings( );
	controlCoverDisabled->show( );
	controlCoverDisabled->hide( );


	_org_text = StringToWString( defaulttext );


	setText(WStringToString( _org_text ) );

	_offset = _org_text.size( );

	setTimer( -1 );

	//setTextAlign( TextAlignmentHorizontal::ALIGN_HORIZ_LEFT, TextAlignmentVertical::ALIGN_VERT_CENTER );

	EditBoxButtonSet.insert( this );
}


EditBoxButton::~EditBoxButton( )
{

}

void EditBoxButton::UpdateTextAndCallback( )
{
	(*_pVarText) = WStringToString( _org_text );

	if ( editBoxButtonCallback )
		editBoxButtonCallback( this );
}

void EditBoxButton::DisplayCurrentText( bool withcursor )
{
	std::wstring _text_for_print = _org_text;
	if ( withcursor )
		_text_for_print.insert( _offset, L"||" );
	setText(WStringToString( _text_for_print ) );
}

void EditBoxButton::toggleEnteringText( ) { _enteringText = !_enteringText; }
bool EditBoxButton::isEnteringText( ) { return _enteringText; }
int EditBoxButton::getTimer( ) { return _timer; }
void EditBoxButton::setTimer( int times ) { _timer = times; }


static void EditBoxUpdate( Timer* tm )
{
	for ( std::set<EditBoxButton *>::iterator iter = EditBoxButtonSet.begin( );
		iter != EditBoxButtonSet.end( ); ++iter )
	{

		if ( ( *iter )->isEnteringText( ) ) {
			( *iter )->setTimer( ( *iter )->getTimer( ) - 1 );
			if ( ( *iter )->getTimer( ) <= 0 )
			{
				( *iter )->setTimer( 35 );
			}

			if ( ( *iter )->getTimer( ) > 18 && ( *iter )->getTimer( ) < 35 )
			{
				( *iter )->DisplayCurrentText( true );
			}
			else
			{
				( *iter )->DisplayCurrentText( );
			}

			break;
		}
	}
}

void EditBoxButton::Init( )
{
	EditBoxButtonSet.clear( );
	GetTimer( 0.04, EditBoxUpdate, true, TimeType::TimeGame )->start( );
}
void EditBoxButton::Cleanup( )
{
	EditBoxButtonSet.clear( );
}

bool EditBoxButton::IsAnyEditBoxActive( )
{
	for ( std::set<EditBoxButton *>::iterator iter = EditBoxButtonSet.begin( );
		iter != EditBoxButtonSet.end( ); ++iter )
	{
		if ( ( *iter )->isEnteringText( ) ) {
			return true;
		}
	}

	return false;
}
void EditBoxButton::EnterText( std::wstring text )
{
	if ( _offset <= ( int )_org_text.size( ) && _offset >= 0 )
	{
		_org_text.insert( _offset, text );
		DisplayCurrentText( true );
		UpdateTextAndCallback( );
	}
}

void EditBoxButton::EnterTextToCurrentEditBox( std::wstring text )
{
	for ( std::set<EditBoxButton *>::iterator iter = EditBoxButtonSet.begin( );
		iter != EditBoxButtonSet.end( ); ++iter )
	{
		if ( ( *iter )->isEnteringText( ) ) {

			auto oldsize = ( *iter )->_org_text.size( );
			( *iter )->EnterText( text );
			( *iter )->_offset += ( *iter )->_org_text.size( ) - oldsize;
			return;
		}
	}
}

void EditBoxButton::RemoveCharacter( )
{
	if ( _org_text.size( ) > 0 && _offset > 0 && _offset <= ( int )_org_text.size( ) )
	{
		_offset--;
		_org_text.erase( _offset, 1 );
		DisplayCurrentText( true );
		UpdateTextAndCallback( );
	}
}

void EditBoxButton::RemoveCharacterRight( )
{
	if ( _org_text.size( ) > 0 && _offset >= 0 && _offset < ( int )_org_text.size( ) )
	{
		_org_text.erase( _offset, 1 );
		DisplayCurrentText( true ); 
		UpdateTextAndCallback( );
	}
}


void EditBoxButton::RemoveCharacterFromCurrentEditBox( bool right )
{
	for ( std::set<EditBoxButton *>::iterator iter = EditBoxButtonSet.begin( );
		iter != EditBoxButtonSet.end( ); ++iter )
	{
		if ( ( *iter )->isEnteringText( ) ) {
			if ( right )
				( *iter )->RemoveCharacterRight( );
			else
				( *iter )->RemoveCharacter( );
			return;
		}
	}
}


void EditBoxButton::MoveCursorLeft( )
{
	_offset--;

	if ( _offset < 0 )
	{
		_offset = _org_text.size( );
	}
}

void EditBoxButton::CurrentEditBoxMoveCursorLeft( )
{
	for ( std::set<EditBoxButton *>::iterator iter = EditBoxButtonSet.begin( );
		iter != EditBoxButtonSet.end( ); ++iter )
	{
		if ( ( *iter )->isEnteringText( ) ) {

			( *iter )->MoveCursorLeft( );
			return;
		}
	}
}


void EditBoxButton::MoveCursorRight( )
{
	_offset++;

	if ( _offset > ( int )_org_text.size( ) )
	{
		_offset = 0;
	}
}


void EditBoxButton::CurrentEditBoxMoveCursorRight( )
{
	for ( std::set<EditBoxButton *>::iterator iter = EditBoxButtonSet.begin( );
		iter != EditBoxButtonSet.end( ); ++iter )
	{
		if ( ( *iter )->isEnteringText( ) ) {

			( *iter )->MoveCursorRight( );
			return;
		}
	}
}

void EditBoxButton::StopInput( )
{
	for ( std::set<EditBoxButton *>::iterator iter = EditBoxButtonSet.begin( );
		iter != EditBoxButtonSet.end( ); ++iter )
	{
		if ( ( *iter )->isEnteringText( ) ) {
			( *iter )->toggleEnteringText( );
			( *iter )->setText(WStringToString( ( *iter )->_org_text ) );
		}
	}
}
