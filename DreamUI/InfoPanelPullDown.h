#ifndef INFOPANEL_PULLDOWN_H_
#define INFOPANEL_PULLDOWN_H_

class UISimpleFrame;
class UISimpleButton;
class UISimpleTexture;
class UILayoutFrame;
class Label;
class Observer;

namespace InfoPanel {
	class PullDown;

	typedef void (*PullDownCallback) (PullDown*, unsigned int);

	class PullDown {
		UISimpleFrame* currentItemFrame;	//当前显示frame
		UISimpleTexture* pullDownArrow;		//表示下拉的三角箭头
		UISimpleButton* currentItemClicker;	//捕捉点击控件
		Observer* currentItemClickerOb;
		Label* currentItemText;	//当前显示文字

		UISimpleFrame* pullDownFrame;		//下拉显示frame
		bool				pullDownFrameIsHidden;
		std::vector<UISimpleButton*>	itemClickerSet;	//捕捉每一项点击控件
		std::vector<Observer*>			itemClickerSetOb;
		std::vector<Label*>				itemTextSet;	//每一项文字

		unsigned int			currentItemIndex;
		float				itemTextHeight;

		PullDownCallback	callback;

	public:
		PullDown(
			UISimpleFrame* parent,
			float				inWidth,
			float				inItemHeight,
			unsigned int			inItemCount,
			PullDownCallback	callback = NULL
		);

		~PullDown();

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

		void expand(bool flag);
		bool isExpanded();
		void toggle();
		void setIndex(unsigned int index);
		void toggleIndex(unsigned int index);
		unsigned int getIndex() const;
		void setText(unsigned int index, char* format, ...);
		void setColor(unsigned int index, float r, float g, float b, float a);
		void show(bool flag);

		static void Init();
	};

}

#endif