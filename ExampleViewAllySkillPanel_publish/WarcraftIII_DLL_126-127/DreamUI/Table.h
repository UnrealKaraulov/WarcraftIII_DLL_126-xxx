#ifndef TABLE_H_
#define TABLE_H_

#include "Frame.h"

class UISimpleFrame;
class UISimpleTexture;
class Button;
class Label;
class Observer;

typedef std::vector<std::vector<IUIObject *>> TableElemArr;
typedef std::vector<std::vector<bool>> TableElemAutoSizeArr;

struct TableTag
{
	Button				*button;
	UISimpleTexture		*triangle;
	uint32_t			row;
	bool				bMinimized;	//true为收起
};

class Table : public Frame
{
private:
	TableElemArr	elements;
	TableElemAutoSizeArr	elementsPassiveFlag;
	std::vector<IUIObject *> titleElements;
	uint32_t		rowCount;
	uint32_t		columnCount;
	bool			bShowRowFrame;
	bool			bShowTitle;
	bool			bInverse;
	float			titleHeight;
	
	UISimpleFrame	*TitleFrame;
	std::vector<UISimpleFrame*> rowFrames;
	
	bool			bWantRefresh;

	std::vector<float>	columnMinWidths;
	float			rowPadding;
	float			columnPadding;

	bool rowFit;//如果为true，每行最多到最后一个元素位置
	
	//按标签分组
	std::map<uint32_t, TableTag>	tags;
	float							tagHeight;
	
public:
	Table (
		IUIObject *parent = UI_NULL,
		uint32_t rowCount = 1,
		uint32_t columnCount = 1,
		bool wantShowTitle = true,
		float titleHeight = 0,
		bool rowFitElement = false
	);
	virtual ~Table();
	
	void		showTitle ( bool flag );
	void		setTitle ( uint32_t column, IUIObject *elem );
	void		showRowFrame ( bool flag );
	void		setInverse ( bool flag );
	void		setTag ( uint32_t beforeRow, const char *str, float height = 0.010f );

	uint32_t	sizeRow ( ) const;
	uint32_t	sizeColumn ( ) const;
	void		setRowColumnCount ( uint32_t rowCount, uint32_t columnCount );
	void		setElementPadding ( float rowPadding, float columnPadding );
	void setElement ( uint32_t row, uint32_t column, IUIObject *elem, bool passiveHeight = false );
	IUIObject*	getElement( uint32_t row, uint32_t column );
	template <class ReturnType>
	ReturnType getElement ( uint32_t row, uint32_t column) {
		return (ReturnType)this->getElement(row, column);
	}
	void		setColumnMinWidth ( uint32_t column, float minWidth );

	virtual void show ( bool flag );
	virtual void refresh ( );
	void setRefresh ( );
	virtual void tick ( );
	virtual void setParent ( UISimpleFrame *parent );
	
};


#endif