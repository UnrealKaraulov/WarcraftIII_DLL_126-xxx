#ifndef INFOPANEL_ROW_H_
#define INFOPANEL_ROW_H_

class UISimpleFrame;
class UISimpleTexture;
class UISimpleFontString;
class UILayoutFrame;
class Label;

namespace InfoPanel {
	//key为项目号(0-based)
	typedef std::map< uint32_t, UISimpleFontString* > TextContentType;

	//面板的一行，最左端固定为玩家种族图标 + 玩家号(1-based)
	//样式：一个细边框无背景的UISimpleFrame
	//大小：指定上下宽度；指定项目宽度；内容居中于位置
	//可按照 obj[0], obj[1]这样access项目，[0]为第一个项目
	class Row {
		uint32_t					itemCount;			//项目数量

		UISimpleFrame*				frame;				//用作定位和背景的基本框
		UISimpleFrame*				playerIconFrame;
		float						height;
		float						itemWidth;
		float						width; //不为0时强制为宽度
		bool						hidden;

		UISimpleTexture*			playerColorBg;		//玩家颜色
		UISimpleTexture*			playerRaceIcon;		//玩家种族图标
		Label*						playerIdLabel;
		int							playerId;			//玩家号(0-based)	 TODO增加玩家名
		bool						playerWantUpdate;	//是否在update中更新

		TextContentType				contentSimpleFontStrings;
		std::map<uint32_t, std::string>	contentText;

	public:

		Row(UISimpleFrame *parent, float inHeight, float inItemWidth, uint32_t inItemCount);
		~Row();

		void	setPlayerId (int inPlayerId);
		void	setText (uint32_t index, char *format, ...);
		void	setWidth (float width);

		UILayoutFrame *getFrame();
		float	getHeight() {return height;}

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

		void	show(bool flag = true);
		bool	isHidden();
		void	update();						//根据已有数据更新
	};

}

#endif