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
		UISimpleFrame* currentItemFrame;	//��ǰ��ʾframe
		UISimpleTexture* pullDownArrow;		//��ʾ���������Ǽ�ͷ
		UISimpleButton* currentItemClicker;	//��׽����ؼ�
		Observer* currentItemClickerOb;
		Label* currentItemText;	//��ǰ��ʾ����

		UISimpleFrame* pullDownFrame;		//������ʾframe
		bool				pullDownFrameIsHidden;
		std::vector<UISimpleButton*>	itemClickerSet;	//��׽ÿһ�����ؼ�
		std::vector<Observer*>			itemClickerSetOb;
		std::vector<Label*>				itemTextSet;	//ÿһ������

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