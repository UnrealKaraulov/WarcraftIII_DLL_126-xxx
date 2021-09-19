#include "stdafx.h"
#include "ToolTip.h"
#include "Button.h"

#include "IUIObject.h"//TODO

static std::set<ToolTip*> ToolTipSet;

static const uint32_t TOOLTIP_MAX_SIZE = 0x800;
static char ToolTipStrBuffer[TOOLTIP_MAX_SIZE];

ToolTip::ToolTip(): _frame(), _text() {}

ToolTip* ToolTip::Create(
	UISimpleFrame *parent,
	float width,
	float height,
	const char *formattedContent,
	bool dontStore
) 
{
	ToolTip* tooltip = new ToolTip();

	UISimpleFrame *sf = UISimpleFrame::Create(parent);
	sf->setWidth(width); sf->setHeight(height);
	sf->setBackground(Skin::getPathByName("ToolTipBackground"));
	sf->setBorder(Skin::getPathByName("ToolTipBorder"));
	sf->setBorderWidth(0.01f);//size ?
	sf->setPadding(0.002f);
	sf->applyTextureSettings();
	//sf->setAbsolutePosition( UILayoutFrame::POSITION_BOTTOM_LEFT, 0.3f, 0.3f );
	sf->setRelativePosition( POS_BL, parent, POS_BL, parent->baseLayoutFrame()->width + 0.01f, 0.0f );
	/*sf->setRelativePosition(
		UISimpleFrame::POSITION_LEFT,
		parent,
		UISimpleFrame::POSITION_TOP_RIGHT,
		0, 0
	);*/
	sf->show();
	

	tooltip->_frame = sf;

	UISimpleFontString* sfs = UISimpleFontString::Create(tooltip->_frame);
	sprintf_s(ToolTipStrBuffer, TOOLTIP_MAX_SIZE, "%s", formattedContent);//TODO½âÎö?
	sfs->initFont(UIObject::GetPathByName("InfoPanelTextFont"), 0.013f, false);//?
	sfs->setRelativePosition(POS_UL, tooltip->_frame, POS_L, 0.01f, 0);
	sfs->setText(ToolTipStrBuffer);


//	sf->setWidth( sfs->getTextWidth( ) );


	tooltip->_text = sfs;

	if (!dontStore) ToolTipSet.insert(tooltip);

	tooltip->applyPosition( );

	//sf->setWidth( sfs->getTextWidth( ) );
	//sf->setHeight( sfs->getTextHeight( ) );

	tooltip->applyPosition( );

	return tooltip;
}

void ToolTip::Destroy(ToolTip *tooltip){
	if (!tooltip) return;
	if (tooltip->_text) UISimpleFontString::Destroy(tooltip->_text);
	if (tooltip->_frame) UISimpleFrame::Destroy(tooltip->_frame);
	delete tooltip;
}

void ToolTip::bindButton(Button *button){
	if (this){
		_frame->setRelativePosition(
			UISimpleFrame::POSITION_TOP_LEFT, 
			button->getFrame(), 
			UISimpleFrame::POSITION_CENTER,
			0,0
		);
		applyPosition();
	}
}

void ToolTip::applyPosition(){
	if (this){
		
		_frame->setWidth( _text->getTextWidth( )  * War3WindowRatioX( ) + 0.02f  );
		_frame->setHeight( _text->getTextHeight( ) * War3WindowRatioY( ) + 0.01f  );

		_frame->applyPosition();
		_text->applyPosition();
	}
}

void ToolTip::show(){
	if (this)_frame->show();
}

void ToolTip::hide(){
	if (this)_frame->hide();
}

void ToolTip_Init(){
	ToolTipSet.clear();
}

void ToolTip_Cleanup(){
	for (std::set<ToolTip*>::iterator iter = ToolTipSet.begin();
		iter != ToolTipSet.end(); ++iter){
			ToolTip::Destroy(*iter);
	}
	ToolTipSet.clear();
}