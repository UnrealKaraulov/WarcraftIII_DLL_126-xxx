#include "stdafx.h"
#include "InfoPanelPanel.h"
#include "InfoPanelRow.h"

#include "UISimpleFrame.h"
#include "UISimpleTexture.h"
#include "UISimpleFontString.h"
#include "Label.h"

#include "Tools.h"//MUSTDO

namespace InfoPanel {



	Row::Row(UISimpleFrame *parent, float inHeight, float inItemWidth, uint32_t inItemCount) :
		itemCount(inItemCount), height(inHeight), itemWidth(inItemWidth), hidden(false), width(0)
	{
		float raceIconSize = RACEICON_FACTOR * min(height, 0.02f);

		frame = UISimpleFrame::Create(parent);
		frame->setHeight(height);
		frame->setWidth(itemCount * itemWidth + 0.01f + raceIconSize);
		frame->setBackground("DreamDota3\\Textures\\GreyBackground.tga");
		frame->setPadding(0.002f);
		frame->setBorder("UI\\Widgets\\ToolTips\\Human\\human-tooltip-border.blp");
		frame->setBorderWidth(0.005f);
		frame->applyTextureSettings();

		playerIconFrame = UISimpleFrame::Create(frame);
		playerIconFrame->setHeight(raceIconSize * 0.44f);
		playerIconFrame->setWidth(raceIconSize * 0.83f);
		playerIconFrame->setRelativePosition(
			POS_L,
			frame,
			POS_L,
			0.0035f,
			0
		);

		playerWantUpdate = false;
		playerId = 0;


		playerIdLabel = new Label(frame, "1", min(height, 0.02f) * 0.4f, Color(1.f, 1.f, 1.f));
		playerIdLabel->setRelativePosition(
			POS_C,
			playerRaceIcon,
			POS_R,
			-(raceIconSize / 4.5f), 0
		);
	}

	Row::~Row()
	{
		contentText.clear();
		contentSimpleFontStrings.clear();
		delete (playerIdLabel);
		UISimpleTexture::Destroy(playerRaceIcon);
		UISimpleTexture::Destroy(playerColorBg);
		UISimpleFrame::Destroy(frame);
	}

	void Row::setPlayerId (int inPlayerId)
	{
		if (playerId != inPlayerId)
		{
			playerId = inPlayerId;
			playerWantUpdate = true;
		}
	}

	char RowSetTextTemp[500];
	void Row::setText (uint32_t index, char *format, ...)
	{
		va_list args;
		va_start(args, format);
		vsprintf_s(RowSetTextTemp, 500, format, args);
		va_end(args);
		contentText[index].assign(RowSetTextTemp);
	}

	void Row::setRelativePosition(
			uint32_t originPos, 
			UILayoutFrame* target, 
			uint32_t toPos, 
			float relativeX, 
			float relativeY			)
	{
		frame->setRelativePosition(
			originPos, 
			target, 
			toPos, 
			relativeX, 
			relativeY	);
	}

	void Row::setAbsolutePosition( uint32_t originPos, float absoluteX, float absoluteY	)
	{
		frame->setAbsolutePosition(	originPos, absoluteX, absoluteY );
	}

	void Row::update()//MUSTDO Æ¥Åä¿í¶È
	{
		if (hidden) return;
		if (playerWantUpdate)
		{
			char playerNumber[5];
			sprintf_s(playerNumber, 5, "%d", playerId+1);
			playerIdLabel->setText(playerNumber);
			//playerIdLabel->setColor((0xFFFFFFFF^color)|0xFF000000);
			playerWantUpdate = false;
		}

		if (width > 0){
			float raceIconSize = RACEICON_FACTOR * min(height, 0.02f);
			frame->setWidth(width + 0.01f + raceIconSize);
			frame->applyPosition();
		}

		UISimpleFontString *sfs = NULL;
		for (std::map<uint32_t, std::string>::iterator iter = contentText.begin();
			iter != contentText.end(); ++iter)
		{
			sfs = contentSimpleFontStrings[iter->first];
			if (!sfs)
			{
				sfs = UISimpleFontString::Create(frame);
				sfs->initFont(Skin::getPathByName("MasterFont"), 
					height * 0.75f, 
					1);
				sfs->setText(iter->second.c_str());
				sfs->setColorFloat(1,1,1,1);
				contentSimpleFontStrings[iter->first] = sfs;
			}
			else
			{
				sfs->setText(iter->second.c_str());
			}
			sfs->setRelativePosition(
				UILayoutFrame::POSITION_CENTER,
				frame,
				POS_L,
				RACEICON_FACTOR * min(height, 0.02f) + 0.005f
				+ (iter->first + 0.5f) * itemWidth
				, 
				0
			);
		}
	}

	void Row::show(bool flag)
	{
		if (!this) return;
		if (!flag)
		{
			frame->hide();
			hidden = true;
		}
		else
		{
			frame->show();
			hidden = false;
			update();
		}
	}

	bool Row::isHidden()
	{
		if (!this) return false;
		return hidden;
	}

	UILayoutFrame *Row::getFrame()
	{
		if (!this) return NULL;
		return frame;
	}

	void Row::setWidth(float width)
	{
		if (!this) return;
		this->width = width;
		update();
	}

}