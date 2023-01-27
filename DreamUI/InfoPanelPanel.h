#ifndef INFOPANEL_PANEL_H_
#define INFOPANEL_PANEL_H_

#include "InfoPanelRow.h"

namespace InfoPanel {
	const float RACEICON_FACTOR = 1.4f;
	const float TITLEICON_FACTOR = 0.65f;
	const float TITLELABEL_FACTOR = 0.45f;

	class Panel {
		Row* rows[12];
		UISimpleFrame* titleFrame;
		Label* titleName;
		std::map<unsigned int, UISimpleTexture*>	titleIcons;
		std::map<unsigned int, Label*>	titleLabels;
		std::map<unsigned int, float>	titleWidths;
		float itemWidth;
		int itemCount;
		float titleHeight;
		float rowHeight;
		bool hidden;

		float calcColumnCenter(unsigned int index);
	public:
		Panel(
			UISimpleFrame* parent,
			float inRowHeight,
			float inTitleHeight,
			float inItemWidth,
			unsigned int inItemCount
		);

		Panel(
			UISimpleFrame* parent,
			float inRowHeight
		);

		~Panel();

		void	setRelativePosition(
			unsigned int originPos,
			UILayoutFrame* target,
			unsigned int toPos,
			float relativeX,
			float relativeY
		);

		void	setAbsolutePosition(
			unsigned int originPos,
			float absoluteX,
			float absoluteY
		);

		void	setName(char* name);
		void	setColumnIcon(unsigned int index, char* path);
		void	setColumnLabel(unsigned int index, char* text);
		void	setColumnWidth(unsigned int index, float width);
		void	updateColumn(unsigned int index);
		Row* getRow(unsigned int playerId);
		void	show(bool flag);
		bool	isHidden();
	};


}

#endif