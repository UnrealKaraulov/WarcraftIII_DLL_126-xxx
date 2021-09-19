#ifndef INFOPANEL_PANEL_H_
#define INFOPANEL_PANEL_H_

#include "InfoPanelRow.h"

namespace InfoPanel {
	const float RACEICON_FACTOR = 1.4f;
	const float TITLEICON_FACTOR = 0.65f;
	const float TITLELABEL_FACTOR = 0.45f;

	class Panel {
		Row* rows[12];
		UISimpleFrame *titleFrame;
		Label *titleName;
		std::map<uint32_t, UISimpleTexture*>	titleIcons;
		std::map<uint32_t, Label*>	titleLabels;
		std::map<uint32_t, float>	titleWidths;
		float itemWidth;
		int itemCount;
		float titleHeight;
		float rowHeight;
		bool hidden;

		float calcColumnCenter(uint32_t index);
	public:
		Panel(
			UISimpleFrame *parent,
			float inRowHeight,
			float inTitleHeight,
			float inItemWidth,
			uint32_t inItemCount
		);

		Panel(
			UISimpleFrame *parent,
			float inRowHeight
		);

		~Panel();

		void	setRelativePosition(
			uint32_t originPos, 
			UILayoutFrame* target, 
			uint32_t toPos, 
			float relativeX, 
			float relativeY
		);

		void	setAbsolutePosition(
			uint32_t originPos, 
			float absoluteX, 
			float absoluteY
		);

		void	setName (char *name);
		void	setColumnIcon (uint32_t index, char *path);
		void	setColumnLabel (uint32_t index, char *text);
		void	setColumnWidth (uint32_t index, float width);
		void	updateColumn (uint32_t index);
		Row*	getRow (uint32_t playerId);
		void	show(bool flag);
		bool	isHidden();
	};


}

#endif