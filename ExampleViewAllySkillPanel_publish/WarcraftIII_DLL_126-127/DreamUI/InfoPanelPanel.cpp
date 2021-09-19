#include "stdafx.h"
#include "InfoPanelPanel.h"
#include "UISimpleFrame.h"
#include "UISimpleTexture.h"
#include "Label.h"

namespace InfoPanel {
	Panel::Panel(
		UISimpleFrame *parent,
		float inRowHeight )
		: rowHeight( inRowHeight ), titleName( NULL )
	{
		titleFrame = UISimpleFrame::Create( parent );
		titleFrame->setHeight( 0.01f );
		titleFrame->setWidth( 0.01f ); //只用来左上角对齐
		UILayoutFrame *last = titleFrame;
		for ( int i = 0; i < 12; i++ )
		{
			rows[ i ] = new Row( titleFrame, inRowHeight, 0.1f, 1 );
			rows[ i ]->setPlayerId( i );
			rows[ i ]->update( );
			//	if (Jass::GetPlayerSlotState(Jass::Player(i)) == Jass::PLAYER_SLOT_STATE_PLAYING )
			//	{
			rows[ i ]->setRelativePosition(
				POS_UL,
				last,
				POS_BL,
				0, 0
			);
			last = rows[ i ]->getFrame( );
			//}
		//	else
			//{
		///		rows[i]->show(false);
		//	}
		}
		show( false ); hidden = true;
	}

	Panel::Panel(
		UISimpleFrame *parent,
		float inRowHeight,
		float inTitleHeight,
		float inItemWidth,
		uint32_t inItemCount )
		: itemCount( inItemCount ), itemWidth( inItemWidth ), titleHeight( inTitleHeight ), rowHeight( inRowHeight )
	{
		float raceIconSize = RACEICON_FACTOR * min( inRowHeight, 0.02f );

		titleFrame = UISimpleFrame::Create( parent );
		titleFrame->setBackground( "DreamDota3\\Textures\\GreyBackground.tga" );
		titleFrame->setPadding( 0.004f );
		titleFrame->setBorder( "UI\\Widgets\\EscMenu\\Human\\editbox-border.blp" );
		//Skin::getPathByName("EscMenuButtonBorder"));
		titleFrame->setBorderWidth( 0.01f );
		titleFrame->applyTextureSettings( );
		titleFrame->setHeight( inTitleHeight + 0.0035f );
		titleFrame->setWidth( inItemCount * itemWidth + 0.01f + raceIconSize + 0.0035f );

		titleName = new Label( titleFrame, "", titleHeight * TITLEICON_FACTOR );
		titleName->setRelativePosition( POS_C, titleFrame, POS_C, 0, 0 );

		UILayoutFrame *last = titleFrame;
		for ( int i = 0; i < 12; i++ )
		{
			rows[ i ] = new Row( titleFrame, inRowHeight, inItemWidth, inItemCount );
			rows[ i ]->setPlayerId( i );
			rows[ i ]->update( );
			//if (Jass::GetPlayerSlotState(Jass::Player(i)) == Jass::PLAYER_SLOT_STATE_PLAYING
				//&&	Jass::GetPlayerSelectable(Jass::Player(i)) == true
		//	)
			//{
			rows[ i ]->setRelativePosition(
				UILayoutFrame::POSITION_TOP_CENTER,
				last,
				UILayoutFrame::POSITION_BOTTOM_CENTER,
				0, 0
			);
			last = rows[ i ]->getFrame( );
			//}
			//else
			//{
			//	rows[i]->show(false);
			//}
		}
		show( false ); hidden = true;
	}

	Panel::~Panel( )
	{
		for ( int i = 0; i < 12; i++ )
		{
			delete rows[ i ];
		}
		for ( std::map<uint32_t, UISimpleTexture*>::iterator iter = titleIcons.begin( );
			iter != titleIcons.end( ); ++iter )
		{
			if ( iter->second ) { UISimpleTexture::Destroy( iter->second ); }
		}
		for ( std::map<uint32_t, Label*>::iterator iter = titleLabels.begin( );
			iter != titleLabels.end( ); ++iter )
		{
			if ( iter->second ) { delete ( iter->second ); }
		}
		titleIcons.clear( );
		titleLabels.clear( );
		titleWidths.clear( );
		if ( titleName ) { delete ( titleName ); }
		UISimpleFrame::Destroy( titleFrame );
	}

	void Panel::setRelativePosition(
		uint32_t originPos,
		UILayoutFrame* target,
		uint32_t toPos,
		float relativeX,
		float relativeY
	)
	{
		titleFrame->setRelativePosition( originPos, target, toPos, relativeX, relativeY );
	}

	void Panel::setAbsolutePosition(
		uint32_t originPos,
		float absoluteX,
		float absoluteY
	)
	{
		titleFrame->setAbsolutePosition( originPos, absoluteX, absoluteY );
	}

	//辅助函数
	float Panel::calcColumnCenter( uint32_t index )
	{
		float w = 0;
		for ( uint32_t i = 0; i < index; ++i )
		{
			w += titleWidths.count( i ) ? titleWidths[ i ] : itemWidth;
		}
		w += titleWidths.count( index ) ? titleWidths[ index ] / 2 : itemWidth / 2;
		return w;
	}

	void Panel::updateColumn( uint32_t index ) {
		if ( titleIcons.count( index ) )
		{
			UISimpleTexture *st = titleIcons[ index ];
			st->setRelativePosition(
				UILayoutFrame::POSITION_CENTER,
				titleFrame,
				POS_L,
				RACEICON_FACTOR * min( rowHeight, 0.02f ) + 0.005f
				+ calcColumnCenter( index ),
				0
			);
		}
		else if ( titleLabels.count( index ) )
		{
			Label *lb = titleLabels[ index ];
			lb->setRelativePosition(
				UILayoutFrame::POSITION_CENTER,
				titleFrame,
				POS_L,
				RACEICON_FACTOR * min( rowHeight, 0.02f ) + 0.005f
				+ calcColumnCenter( index ),
				0
			);
		}
	}

	void Panel::setColumnIcon( uint32_t index, char *path ) {
		UISimpleTexture *st = UISimpleTexture::Create( titleFrame );
		st->setHeight( titleHeight * TITLEICON_FACTOR );
		st->setWidth( titleHeight * TITLEICON_FACTOR );
		st->fillBitmap( path );
		titleIcons[ index ] = st;
		updateColumn( index );
	}

	void Panel::setColumnLabel( uint32_t index, char *text )
	{
		Label *lb = new Label( titleFrame, text, titleHeight * TITLELABEL_FACTOR );
		titleLabels[ index ] = lb;
		updateColumn( index );
	}

	void Panel::setColumnWidth( uint32_t index, float width )
	{
		titleWidths[ index ] = width;
		float w = 0;
		for ( uint32_t i = 0; i < ( uint32_t )itemCount; ++i )
		{
			w += titleWidths.count( i ) ? titleWidths[ i ] : itemWidth;
		}
		titleFrame->setWidth( w + 0.01f + RACEICON_FACTOR * min( rowHeight, 0.02f ) + 0.0035f );
		titleFrame->applyPosition( );
		for ( uint32_t i = index; i < ( uint32_t )itemCount; ++i )
		{
			updateColumn( i );
		}
	}

	void Panel::setName( char *name ) {
		if ( !this ) return;
		titleName->setText( name );
	}

	Row* Panel::getRow( uint32_t playerId )
	{
		if ( !this ) return NULL;
		return rows[ playerId ];
	}

	bool Panel::isHidden( )
	{
		if ( !this ) return false;
		return hidden;
	}

	void Panel::show( bool flag )
	{
		if ( !this ) return;
		hidden = !flag;
		if ( flag ) titleFrame->show( ); else titleFrame->hide( );
	}

}