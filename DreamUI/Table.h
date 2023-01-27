#ifndef TABLE_H_
#define TABLE_H_

#include "Frame.h"

class UISimpleFrame;
class UISimpleTexture;
class Button;
class Label;
class Observer;

typedef std::vector<std::vector<IUIObject*>> TableElemArr;
typedef std::vector<std::vector<bool>> TableElemAutoSizeArr;

struct TableTag
{
	Button* button;
	UISimpleTexture* triangle;
	unsigned int			row;
	bool				bMinimized;	//true为收起
};

class Table : public Frame
{
private:
	TableElemArr	elements;
	TableElemAutoSizeArr	elementsPassiveFlag;
	std::vector<IUIObject*> titleElements;
	unsigned int		rowCount;
	unsigned int		columnCount;
	bool			bShowRowFrame;
	bool			bShowTitle;
	bool			bInverse;
	float			titleHeight;

	UISimpleFrame* TitleFrame;
	std::vector<UISimpleFrame*> rowFrames;

	bool			bWantRefresh;

	std::vector<float>	columnMinWidths;
	float			rowPadding;
	float			columnPadding;

	bool rowFit;//如果为true，每行最多到最后一个元素位置

	//按标签分组
	std::map<unsigned int, TableTag>	tags;
	float							tagHeight;

public:
	Table(
		IUIObject* parent = UI_NULL,
		unsigned int rowCount = 1,
		unsigned int columnCount = 1,
		bool wantShowTitle = true,
		float titleHeight = 0,
		bool rowFitElement = false
	);
	virtual ~Table();

	void		showTitle(bool flag);
	void		setTitle(unsigned int column, IUIObject* elem);
	void		showRowFrame(bool flag);
	void		setInverse(bool flag);
	void		setTag(unsigned int beforeRow, const char* str, float height = 0.010f);

	unsigned int	sizeRow() const;
	unsigned int	sizeColumn() const;
	void		setRowColumnCount(unsigned int rowCount, unsigned int columnCount);
	void		setElementPadding(float rowPadding, float columnPadding);
	void setElement(unsigned int row, unsigned int column, IUIObject* elem, bool passiveHeight = false);
	IUIObject* getElement(unsigned int row, unsigned int column);
	template <class ReturnType>
	ReturnType getElement(unsigned int row, unsigned int column) {
		return (ReturnType)this->getElement(row, column);
	}
	void		setColumnMinWidth(unsigned int column, float minWidth);

	virtual void show(bool flag);
	virtual void refresh();
	void setRefresh();
	virtual void tick();
	virtual void setParent(UISimpleFrame* parent);

};


#endif