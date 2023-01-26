#ifndef DROP_DOWN_LIST_H_
#define DROP_DOWN_LIST_H_

#include "Frame.h"
#include "Texture.h"
#include "UISimpleButton.h"
#include "Label.h"

struct DropDownListItem;
class DropDownList;
typedef void (*DropDownListCallback) (DropDownList*, DropDownListItem);

struct DropDownListItemInfo
{
	unsigned int				itemIdentifier;
	std::string				itemCaption;
	DropDownListCallback	itemCallback;
	bool					hasHotkey;
	unsigned int				hotkey;
	bool					hotkeyShift;
	bool					hotkeyCtrl;
	bool					hotkeyAlt;

	DropDownListItemInfo(
		unsigned int inNum,
		const char* inText,
		DropDownListCallback inCallback = NULL,
		bool useHotkey = false,
		unsigned int hotkeyValue = 0,
		bool shift = false,
		bool ctrl = false,
		bool alt = false
	)
		: itemIdentifier(inNum), itemCallback(inCallback),
		hasHotkey(useHotkey), hotkey(hotkeyValue), hotkeyShift(shift), hotkeyCtrl(ctrl), hotkeyAlt(alt)
	{
		itemCaption.assign(inText ? inText : "N/A");
	}
};

struct DropDownListItem
{
	UISimpleButton* clicker;
	Observer* ob;
	Label* text;
};

class DropDownList : public Frame
{
private:
	Texture* arrow;				//������ͷ��־
	UISimpleButton* titleClicker;		//��Ӧ��ǰ����
	Observer* titleOb;
	Frame* dropDownFrame;		//������
	std::map<unsigned int, DropDownListItem>
		dropDownItems;
	unsigned int					currentItem;
	DropDownListCallback		callback;


	TextAlignmentHorizontal		alignHoriz;			//����ˮƽ�������ң����뷽ʽ
	TextAlignmentVertical		alignVert;			//���ִ�ֱ�������£����뷽ʽ
	float						marginHoriz;		//����ˮƽ�߾�
	float						marginVert;			//���ִ�ֱ�߾�
	float						itemHeight;
	float						textHeightFactor;	//��������еĸ߶ȱ���

public:
	DropDownList(
		IUIObject* parent,
		float					titleHeight,
		float					itemHeight,
		DropDownListCallback	callback = NULL,
		float					minWidth = 0,
		float					fontHeightFactor = 0	//Ĭ���Զ�
	);
	DropDownList(
		UISimpleFrame* parent,
		float					titleHeight,
		float					itemHeight,
		DropDownListCallback	callback = NULL,
		float					minWidth = 0,
		float					fontHeightFactor = 0	//Ĭ���Զ�
	);

	virtual ~DropDownList();

	void	setTexture(
		const char* bgTexPath = NULL,
		const char* bgDisTexPath = NULL,
		const char* borderTexPath = NULL,
		const char* borderDisTexPath = NULL,
		const char* arrowTexPath = NULL,
		const char* arrowDisTexPath = NULL
	);

	void	setColor(
		const Color		titleColor = Color::GOLD,
		const Color		itemColor = Color::WHITE,
		const Color		disabledColor = Color::GREY,
		const Color		highlightColor = Color::RED
	);

	void	setTextAlignment(
		TextAlignmentHorizontal	methodHoriz,
		TextAlignmentVertical	methodVert,
		float					marginHoriz,
		float					marginVert
	);

	void	expand(bool flag);
	void	toggle();
	void	addItem(DropDownListItem listItem);
	void	clearItem();
	void	chooseItem(unsigned int identifier);

};

#endif