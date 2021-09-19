#include "stdafx.h"
#include "Table.h"

#include "Button.h"
#include "Label.h"
#include "Observer.h"
#include "Event.h"

Table::Table (
	IUIObject *parent,
	uint32_t rowCount,
	uint32_t columnCount,
	bool wantShowTitle, 
	float titleHeight,
	bool rowFitElement ) 
	: Frame(parent, 0.01f, 0.01f), bWantRefresh(false), rowFit(rowFitElement),
	bShowRowFrame(true), bInverse(false), tagHeight()
{
	rowPadding = columnPadding = 0.005f;
	this->titleHeight = titleHeight;
	rowCount = columnCount = 0;
	setRowColumnCount(rowCount, columnCount);	
	TitleFrame = UISimpleFrame::Create ( getFrame());
	TitleFrame->setWidth(0.1f);
	TitleFrame->setHeight(0.03f);
	TitleFrame->setBackground("DreamDota3\\Textures\\GreyBackground.tga");
	TitleFrame->setBorder("UI\\Widgets\\EscMenu\\Human\\editbox-border.blp");
	TitleFrame->setBorderWidth(0.01f);
	TitleFrame->setPadding(0.004f);
	TitleFrame->applyTextureSettings();
	
	showTitle(wantShowTitle);
	
	bWantRefresh = true;
}

Table::~Table ()
{
	for (uint32_t i = 0; i < elements.size(); ++i) //删除所有行的所有元素
	{
		for (uint32_t j = 0; j < elements[i].size(); ++j)
		{
			if (elements[i][j]) delete elements[i][j];
		}
	}
	elements.clear();

	for (uint32_t i = 0; i < titleElements.size(); ++i)
	{
		if (titleElements[i]) delete titleElements[i];
	}
	titleElements.clear();

	UISimpleFrame::Destroy(TitleFrame);

	for (uint32_t i = 0; i < rowFrames.size(); ++i)
	{
		if(rowFrames[i]) UISimpleFrame::Destroy(rowFrames[i]);
	}
	rowFrames.clear();
	
	for (std::map<uint32_t, TableTag>::iterator iter = tags.begin(); 
		iter != tags.end(); ++iter)
	{
		Button *btn = iter->second.button;
		if (btn)
		{
			delete btn;
		}
		UISimpleTexture *triangle = iter->second.triangle;
		if (triangle)
		{
			UISimpleTexture::Destroy(triangle);
		}
	}
	tags.clear();
}

void TableTagHandler(Button *btn){
	TableTag *tbltg = (TableTag *)btn->getData(0);
	if (tbltg)
	{
		tbltg->bMinimized = !tbltg->bMinimized;
	}

	Table *tbl = (Table *)btn->getData(1);
	if (tbl)
	{
		tbl->refresh();
	}
}

void Table::setTag ( uint32_t beforeRow, const char *str, float height )
{
	//检查是否有不同位置的tag与输入字符串重复
	for (std::map<uint32_t, TableTag>::iterator iter = tags.begin();
		iter != tags.end(); ++iter)
	{
		if (strcmp(iter->second.button->getText(), str)==0)
		{
			if (iter->second.row != beforeRow)//不同位置，相同字符串
			{
				//OutputScreen(10, "diff pos: %d->%d, same str: %s",iter->first, beforeRow, str);
				delete iter->second.button;
				UISimpleTexture::Destroy(iter->second.triangle);
				tags.erase(iter);
				break;
			}
			else//相同位置，相同字符串
			{
				//OutputScreen(10, "same pos: %d->%d, same str: %s",iter->first, beforeRow, str);
				return;
			}
		}
	}

	tagHeight = height;

	if (!tags.count(beforeRow))//初始化
	{
		tags[beforeRow].bMinimized = false;
		tags[beforeRow].button = new Button(getFrame(), 0.15f, 0.024f, UISimpleButton::MOUSEBUTTON_LEFT, 
			UISimpleButton::STATE_ENABLED, TableTagHandler, NULL, true, true);
		tags[beforeRow].button->setTextAlign(ALIGN_HORIZ_LEFT, ALIGN_VERT_CENTER);
		tags[beforeRow].button->bindData(1, this);		//Table*
		tags[beforeRow].triangle = UISimpleTexture::Create(getFrame());
		tags[beforeRow].triangle->setWidth(0.008f);
		tags[beforeRow].triangle->setHeight(0.008f);
		tags[beforeRow].triangle->fillBitmap("UI\\Widgets\\Glues\\GlueScreen-Pulldown-Arrow.blp");
	}

	tags[beforeRow].button->bindData(0, &tags[beforeRow]);		//TableTag*
	tags[beforeRow].row = beforeRow;
	tags[beforeRow].button->setText(str);
	
	bWantRefresh = true;
}

void Table::showTitle ( bool flag )
{
	if (bShowTitle == flag)
	{
		return;
	}
	bShowTitle = flag;
	if (bShowTitle) TitleFrame->show();
	else TitleFrame->hide();
	bWantRefresh = true;
}

void Table::setRowColumnCount ( uint32_t rowCount, uint32_t columnCount )
{
	if (this->rowCount == rowCount && this->columnCount == columnCount)
	{
		return;
	}

	if (rowCount < this->rowCount) //减少行数
	{
		for (uint32_t i = rowCount; i < min(this->rowCount, elements.size()); ++i) //删除多余行的所有元素
		{
			for (uint32_t j = 0; j < elements[i].size(); ++j)
			{
				if (elements[i][j]) delete elements[i][j];
			}

			if (rowFrames.size() > i)
			{
				if (rowFrames[i]) 
				{
					UISimpleFrame::Destroy(rowFrames[i]);
				}
			}
		}
	}
	if (columnCount < this->columnCount)
	{
		for (uint32_t i = 0; i < min(rowCount, elements.size()); ++i) //删除剩余行的多余列元素
		{
			for (uint32_t j = columnCount; j < min(this->columnCount, elements[i].size()); ++j)
			{
				if (elements[i][j]) delete elements[i][j];
			}
		}
	}

	this->rowCount = rowCount;
	this->columnCount = columnCount;
	elements.resize(rowCount);
	elementsPassiveFlag.resize(rowCount);
	rowFrames.resize(rowCount);
	titleElements.resize(columnCount);
	columnMinWidths.resize(columnCount);
	for (uint32_t i = 0; i < rowCount; i++)
	{
		elements[i].resize(columnCount);
		elementsPassiveFlag[i].resize(columnCount);
	}
	bWantRefresh = true;
}

void Table::setElement( uint32_t row, uint32_t column, IUIObject *elem, bool passiveHeight /*= false */ )
{
	if (row < rowCount && column < columnCount)
	{
		if (elements[row][column] == elem && elementsPassiveFlag[row][column] == passiveHeight)
		{
			return;
		}
		if (elements[row][column])
		{
			delete elements[row][column];
		}
		elements[row][column] = elem;
		elementsPassiveFlag[row][column] = passiveHeight;
		//MUSTDO 设置owner
		bWantRefresh = true;
	}
}


IUIObject * Table::getElement ( uint32_t row, uint32_t column)
{
	if (row < rowCount && column < columnCount)
	{
		return elements[row][column];
	}
	return NULL;
}

void Table::setTitle( uint32_t column, IUIObject *elem )
{
	if (column < columnCount)
	{
		if (titleElements[column] == elem)
		{
			return;
		}
		titleElements[column] = elem;
		bWantRefresh = true;
	}
}

void Table::setColumnMinWidth( uint32_t column, float minWidth )
{
	if (column < columnCount)
	{
		if (columnMinWidths[column] != minWidth )
		{
			columnMinWidths[column] = minWidth;
			bWantRefresh = true;
		}
	}
}


void Table::refresh ( )
{
	Frame::refresh();

	float RowMinHeight = 0.01f;

	//计算每行每列大小，以最大为准，空元素为最小值
	//所有行都设为最大行高；每列各自拥有宽度
	float rowHeight = 0;
	std::map<uint32_t, float> columnWidths;
	std::map<uint32_t, float> columnSumWidths;
	IUIObject *elem;
	for (uint32_t i = 0; i < rowCount; i++) //第i行
	{
		for (uint32_t j = 0; j < columnCount; j++) //第j列
		{
			elem = elements[i][j];
			if (elem)
			{
				rowHeight = max( RowMinHeight, max( rowHeight, 
					elementsPassiveFlag[i][j] ? 0 : elem->height()
				) );
				columnWidths[j] = max( 0.005f, max( columnWidths[j], elem->width() ) );
			}
			if (titleElements[j])
			{
				columnWidths[j] = max( columnWidths[j], titleElements[j]->width() );
			}
			if (columnMinWidths[j])
			{
				columnWidths[j] = max( columnWidths[j], columnMinWidths[j] );
			}
		}
	}
	
	float totalWidth = 0;
	
	for (uint32_t i = 0; i < columnCount; i++)
	{
		totalWidth += ( columnWidths[i] + columnPadding  );
		columnSumWidths[i] = totalWidth;
	}	
	
	float TitleWidthPadding = 0.0035f;

	setWidth(totalWidth + TitleWidthPadding );
	setHeight(titleHeight>0 ? titleHeight : 0.0005f);
	TitleFrame->setWidth(totalWidth + TitleWidthPadding );
	TitleFrame->setHeight(titleHeight>0 ? titleHeight : 0.025f);
	if (bShowTitle) TitleFrame->show();
	else TitleFrame->hide();
	TitleFrame->setRelativePosition(POS_T, getFrame(), POS_T, 0, 0);
	TitleFrame->applyPosition();
	for (uint32_t j = 0; j < columnCount; j++)
	{
		IUIObject *obj = titleElements[j];
		if (obj)
		{
			obj->setParent(TitleFrame);
			if (!bInverse)
			{
				obj->setRelativePosition(POS_C, TitleFrame, POS_L, 
					TitleWidthPadding/(2) + columnSumWidths[j] - (columnWidths[j] + columnPadding )/2,
					0
				);
			}
			else
			{
				obj->setRelativePosition(POS_C, TitleFrame, POS_R, 
					-(TitleWidthPadding/(2) + columnSumWidths[j] - (columnWidths[j] + columnPadding )/2),
					0
				);
			}
			
			obj->show(true);
		}
	}

	bool bShowLastTaggedRow = true;
	UISimpleFrame *last = bShowTitle ? TitleFrame : getFrame();
	for (uint32_t i = 0; i < rowCount; i++)
	{
		bool bHasTag = tags.count(i) && tags[i].button != NULL;
		
		if (bHasTag)
		{
			bool bShowTaggedRow = !tags[i].bMinimized;
			bShowLastTaggedRow = bShowTaggedRow;

			tags[i].button->setWidth(
				max( totalWidth, (tags[i].button->getCaption()->width() + 0.009f) )
				+ 0.002f);
			tags[i].button->setHeight(tagHeight);
			tags[i].button->setRelativePosition((last==TitleFrame) ? POS_T : 
				(bInverse ? POS_UR : POS_UL), 
				last, (last==TitleFrame) ? POS_B: 
				(bInverse ? POS_BR : POS_BL), 
				0, 0);
			tags[i].triangle->setRelativePosition(
				POS_L, tags[i].button->getCaption()->getFrame(), POS_R, 
				0.001f, -0.0005f);
			last = tags[i].button->getButton();
		}

		UISimpleFrame *row;
		if (!(rowFrames[i]))
		{
			row = UISimpleFrame::Create(getFrame());
			row->setWidth(totalWidth + 0.002f );
			row->setHeight(rowHeight + rowPadding);
			row->setBackground(bShowRowFrame ? 
				"DreamDota3\\Textures\\GreyBackground.tga" 
			:	"DreamDota3\\Textures\\Transparent.tga");
			row->setBorder(bShowRowFrame ? "UI\\Widgets\\ToolTips\\Human\\human-tooltip-border.blp" : "DreamDota3\\Textures\\Transparent.tga");
			row->setBorderWidth( 0.005f );
			row->setPadding(0.002f);
			row->applyTextureSettings();

			rowFrames[i] = row;
		}
		else
		{
			row = rowFrames[i];
			row->setWidth(totalWidth + 0.002f);
			row->setHeight(rowHeight + rowPadding);
		}

		row->setRelativePosition((last==TitleFrame) ? POS_T : 
			(bInverse ? POS_UR : POS_UL), 
			last, (last==TitleFrame) ? POS_B: 
			(bInverse ? POS_BR : POS_BL), 
			0, 0);
		row->applyPosition();

		if (bShowLastTaggedRow)
		{
			//画元素
			for (uint32_t j = 0; j < columnCount; j++)
			{
				IUIObject *obj = elements[i][j];
				if (obj)
				{
					if (elementsPassiveFlag[i][j])
					{
						obj->setHeight(rowHeight);
					}
					obj->setParent(row);
					if (!bInverse)
					{
						obj->setRelativePosition(POS_C, row, POS_L, 
							columnSumWidths[j] - (columnWidths[j] + columnPadding)/2,
							0
						);
					}
					else
					{
						obj->setRelativePosition(POS_C, row, POS_R, 
							-(columnSumWidths[j] - (columnWidths[j] + columnPadding)/2),
							0
						);
					}
				}
			}
			//处理宽度匹配
			if (rowFit)
			{
				for (int j = columnCount-1; j >= 0; j--)
				{
					if (elements[i][j])
					{
						row->setWidth(columnSumWidths[j] + 0.0015f);
						break;
					}
				}
			}
		}
		
		if (bShowLastTaggedRow)
		{
			row->show();
			last = row;	//FIXME
		}
		else row->hide();
	}
	
}

void Table::tick()
{
	Frame::tick();
	if (bWantRefresh)
	{
		refresh();
		bWantRefresh = false;
	}
}

void Table::setElementPadding( float rowPadding, float columnPadding )
{
	if (this->rowPadding == rowPadding && this->columnPadding == columnPadding)
	{
		return;
	}
	this->rowPadding = rowPadding;
	this->columnPadding = columnPadding;
	bWantRefresh = true;
}

std::uint32_t Table::sizeRow() const
{
	return rowCount;
}

std::uint32_t Table::sizeColumn() const
{
	return columnCount;
}

void Table::show( bool flag )
{
	Frame::show(flag);
	if(flag) getFrame()->show();
	else getFrame()->hide();
}

void Table::setParent( UISimpleFrame *parent )
{
	Frame::setParent(parent);
	TitleFrame->setParent(getFrame());
}

void Table::showRowFrame( bool flag )
{
	if (bShowRowFrame == flag)
	{
		return;
	}
	bShowRowFrame = flag;
	bWantRefresh = true;
}

void Table::setInverse( bool flag )
{
	if (bInverse == flag)
	{
		return;
	}
	bInverse = flag;
	bWantRefresh = true;
}

void Table::setRefresh()
{
	bWantRefresh = true;
}
