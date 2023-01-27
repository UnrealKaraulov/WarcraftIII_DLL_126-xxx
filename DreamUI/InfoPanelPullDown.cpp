#include "stdafx.h"
#include "InfoPanelPullDown.h"

#include "InfoPanelPanel.h"
#include "UISimpleFrame.h"
#include "UISimpleTexture.h"
#include "UISimpleButton.h"
#include "Observer.h"
#include "EventData.h"
#include "Label.h"
#include "Game.h"
#include "Event.h"
#include "GameEventObserver.h"
#include "Tools.h"

namespace InfoPanel {

	static std::set<PullDown*> PullDownSet;
	static std::map<UISimpleButton*, std::pair<PullDown*, bool>> PullDownSimpleButtonMap;
	static std::map<PullDown*, unsigned int> PullDownActiveIndexMap;

	const float PULLDOWN_TITLE_FACTOR = 1.35f;
	const float PULLDOWN_TITLE_FRAME_FACTOR = 1.45f;

	static void onMouseUpDown(const Event* evt) {
		MouseEventData* data = evt->data<MouseEventData>();
		if (data->buttonPushed != NULL) {
			for (std::map<UISimpleButton*, std::pair<PullDown*, bool>>::iterator iter = PullDownSimpleButtonMap.begin();
				iter != PullDownSimpleButtonMap.end(); ++iter)
			{
				war3::CSimpleButton* b = iter->first->base<war3::CSimpleButton*>();
				PullDown* Pd = iter->second.first;
				if (b == data->buttonPushed
					|| (iter->first->containsXY(data->x, data->y) && Pd && Pd->isExpanded()))
				{
					switch (evt->id()) {
					case EVENT_MOUSE_DOWN:
						if (iter->second.second == true) //标题栏
						{
							if ((data->mouseCode & MOUSECODE::MOUSE_LEFT) != 0)
								iter->second.first->toggle();
						}
						else //下拉
						{
							iter->second.first->setIndex(PullDownActiveIndexMap[iter->second.first]);
						}
						break;
					case EVENT_MOUSE_UP:
						if (iter->second.second == false)
						{
							iter->second.first->setIndex(PullDownActiveIndexMap[iter->second.first]);
						}
						break;
					default:
						break;
					}
				}
			}
		}
		else
		{
			for (std::set<PullDown*>::iterator iter = PullDownSet.begin(); iter != PullDownSet.end(); ++iter)
			{
				(*iter)->expand(false);
			}
		}

	}

	static void PullDownHandler(Observer* ob, unsigned int evtId) {
		PullDown* pd = *(ob->data<PullDown*>());
		switch (evtId) {
		case EVENT_CLICK:
			//pd->toggle();
			break;
		default:
			break;
		}
	}

	struct PullDownItemHandlerParam {
		PullDown* pd;
		unsigned int index;
	};

	static void PullDownItemHandler(Observer* ob, unsigned int evtId) {
		PullDownItemHandlerParam* pParam = ob->data<PullDownItemHandlerParam>();
		PullDown* pd = pParam->pd;
		unsigned int index = pParam->index;
		switch (evtId) {
		case EVENT_CLICK:
			//pd->setIndex(index);
			break;
		case EVENT_MOUSE_OVER:
			PullDownActiveIndexMap[pd] = index;
			pd->setColor(index, 1, 0, 0, 1);
			SoundPlay("SubGroupSelectionChange", NULL, 0);
			break;
		case EVENT_MOUSE_OUT:
			PullDownActiveIndexMap[pd] = index;
			pd->setColor(index, 1, 1, 1, 1);
			break;
		default:
			break;
		}
	}

	PullDown::PullDown(
		UISimpleFrame* parent,
		float				inWidth,
		float				inItemHeight,
		unsigned int			inItemCount,
		PullDownCallback	callback)
		: currentItemIndex(0), pullDownFrameIsHidden(true)
	{
		itemTextHeight = inItemHeight * TITLEICON_FACTOR;
		this->callback = callback;

		currentItemFrame = UISimpleFrame::Create(parent);
		currentItemFrame->setWidth(inWidth);
		currentItemFrame->setHeight(inItemHeight * PULLDOWN_TITLE_FRAME_FACTOR);
		currentItemFrame->setBackground("DreamDota3\\Textures\\GreyBackground.tga");
		currentItemFrame->setPadding(0.004f);
		currentItemFrame->setBorder("UI\\Widgets\\EscMenu\\Human\\editbox-border.blp");
		currentItemFrame->setBorderWidth(0.01f);
		currentItemFrame->applyTextureSettings();

		pullDownArrow = UISimpleTexture::Create(currentItemFrame);
		pullDownArrow->setWidth(0.008f);
		pullDownArrow->setHeight(0.008f);
		pullDownArrow->fillBitmap("UI\\Widgets\\Glues\\GlueScreen-Pulldown-Arrow.blp");
		pullDownArrow->setRelativePosition(POS_R,
			currentItemFrame, POS_R, -0.008f, 0);

		PullDown* _this = this;
		currentItemClickerOb = Observer::Create(PullDownHandler);
		currentItemClickerOb->setData<PullDown*>(&_this);

		currentItemClicker = UISimpleButton::Create(currentItemFrame);
		currentItemClicker->setWidth(inWidth);
		currentItemClicker->setHeight(inItemHeight * PULLDOWN_TITLE_FRAME_FACTOR);
		currentItemClicker->setState(UISimpleButton::STATE_ENABLED);
		currentItemClicker->setMouseButton(UISimpleButton::MOUSEBUTTON_LEFT);
		currentItemClicker->setRelativePosition(UILayoutFrame::POSITION_CENTER,
			currentItemFrame, UILayoutFrame::POSITION_CENTER, 0, 0);
		currentItemClicker->setClickEventObserver(EVENT_CLICK, currentItemClickerOb);
		PullDownSimpleButtonMap[currentItemClicker] = std::pair<PullDown*, bool>(this, true);
		SimpleButtonHook(currentItemClicker->base<war3::CSimpleButton*>());

		currentItemText = new Label(currentItemFrame, "N/A", itemTextHeight * PULLDOWN_TITLE_FACTOR);
		currentItemText->setRelativePosition(POS_L, currentItemFrame,
			POS_L, 0.01f, 0);

		pullDownFrame = UISimpleFrame::Create(currentItemFrame);
		pullDownFrame->setWidth(inWidth);
		pullDownFrame->setHeight(itemTextHeight * inItemCount + 0.01f);
		pullDownFrame->setBackground("DreamDota3\\Textures\\GreyBackground.tga");
		pullDownFrame->setPadding(0.004f);
		pullDownFrame->setBorder("UI\\Widgets\\EscMenu\\Human\\editbox-border.blp");
		pullDownFrame->setBorderWidth(0.01f);
		pullDownFrame->applyTextureSettings();
		pullDownFrame->setRelativePosition(POS_UL,
			currentItemFrame, POS_BL, 0, 0.0025f);
		pullDownFrame->hide();

		PullDownItemHandlerParam param;
		itemClickerSetOb = std::vector<Observer*>(inItemCount);
		itemClickerSet = std::vector<UISimpleButton*>(inItemCount);
		itemTextSet = std::vector<Label*>(inItemCount);
		for (unsigned int i = 0; i < inItemCount; ++i) {
			param.pd = this;
			param.index = i;

			itemClickerSetOb[i] = Observer::Create(PullDownItemHandler);
			itemClickerSetOb[i]->setData<PullDownItemHandlerParam>(&param);

			itemClickerSet[i] = UISimpleButton::Create(pullDownFrame);
			itemClickerSet[i]->setWidth(inWidth);
			itemClickerSet[i]->setHeight(itemTextHeight);
			itemClickerSet[i]->setState(UISimpleButton::STATE_ENABLED);
			itemClickerSet[i]->setMouseButton(UISimpleButton::MOUSEBUTTON_LEFT);
			itemClickerSet[i]->setRelativePosition(UILayoutFrame::POSITION_TOP_CENTER,
				pullDownFrame, UILayoutFrame::POSITION_TOP_CENTER, 0, -itemTextHeight * i - 0.005f);
			itemClickerSet[i]->setClickEventObserver(EVENT_CLICK, itemClickerSetOb[i]);
			itemClickerSet[i]->setMouseEventObserver(EVENT_MOUSE_OVER, EVENT_MOUSE_OUT, itemClickerSetOb[i]);
			PullDownSimpleButtonMap[itemClickerSet[i]] = std::pair<PullDown*, bool>(this, false);
			SimpleButtonHook(itemClickerSet[i]->base<war3::CSimpleButton*>());

			itemTextSet[i] = new Label(pullDownFrame, "N/A", itemTextHeight);
			itemTextSet[i]->setTextColor(Color::WHITE);
			itemTextSet[i]->setRelativePosition(POS_L, pullDownFrame, POS_UL, 0.01f, -itemTextHeight * (i + 0.5f) - 0.005f);
		}
		PullDownSet.insert(this);
	}

	PullDown::~PullDown()
	{
		for (std::vector<UISimpleButton*>::iterator iter = itemClickerSet.begin();
			iter != itemClickerSet.end(); ++iter)
		{
			UISimpleButton::Destroy(*iter);
		}
		itemClickerSet.clear();
		for (std::vector<Observer*>::iterator iter = itemClickerSetOb.begin();
			iter != itemClickerSetOb.end(); ++iter)
		{
			Observer::Destroy(*iter);
		}
		itemClickerSetOb.clear();
		for (std::vector<Label*>::iterator iter = itemTextSet.begin();
			iter != itemTextSet.end(); ++iter)
		{
			delete (*iter);
		}
		itemTextSet.clear();


		UISimpleFrame::Destroy(pullDownFrame);
		delete (currentItemText);
		UISimpleButton::Destroy(currentItemClicker);
		Observer::Destroy(currentItemClickerOb);
		UISimpleTexture::Destroy(pullDownArrow);
		UISimpleFrame::Destroy(currentItemFrame);
	}

	void PullDown::setRelativePosition(unsigned int originPos, UILayoutFrame* target, unsigned int toPos, float relativeX, float relativeY) {
		this->currentItemFrame->setRelativePosition(originPos, target, toPos, relativeX, relativeY);
	}

	void PullDown::setAbsolutePosition(unsigned int originPos, float absoluteX, float absoluteY) {
		this->currentItemFrame->setAbsolutePosition(originPos, absoluteX, absoluteY);
	}

	void PullDown::expand(bool flag)
	{
		pullDownFrameIsHidden = !flag;
		if (flag)
		{
			pullDownFrame->show();
		}
		else
		{
			pullDownFrame->hide();
		}
	}


	bool PullDown::isExpanded()
	{
		return !pullDownFrameIsHidden;
	}


	void PullDown::toggle() {
		pullDownFrameIsHidden = !pullDownFrameIsHidden;
		if (pullDownFrameIsHidden)
		{
			pullDownFrame->hide();
		}
		else
		{
			pullDownFrame->show();
		}
	}

	void PullDown::setIndex(unsigned int index) {
		expand(false);
		currentItemText->setText(itemTextSet[index]->getText());
		if (currentItemIndex != index)
		{
			if (callback) this->callback(this, index);
			currentItemIndex = index;
			SoundPlay("GlueScreenClick", NULL, 0);
		}
	}

	void PullDown::toggleIndex(unsigned int index) {
		
		if (getIndex() != index)
		{
			setIndex(index);
		}
		else
		{
			setIndex(0);
		}
	}

	static char RowSetTextTemp[500];
	void PullDown::setText(unsigned int index, char* format, ...) {
		va_list args;
		va_start(args, format);
		vsprintf_s(RowSetTextTemp, 500, format, args);
		va_end(args);
		itemTextSet[index]->setText(RowSetTextTemp);
		if (currentItemIndex == index)
		{
			currentItemText->setText(RowSetTextTemp);
		}
	}

	void PullDown::setColor(unsigned int index, float r, float g, float b, float a)
	{
		itemTextSet[index]->setTextColor(Color(r, g, b, a));
	}

	void PullDown::Init()
	{
		PullDownSimpleButtonMap.clear();
		PullDownActiveIndexMap.clear();
		PullDownSet.clear();
		MainDispatcher()->listen(EVENT_MOUSE_DOWN, onMouseUpDown);
		MainDispatcher()->listen(EVENT_MOUSE_UP, onMouseUpDown);
	}

	unsigned int PullDown::getIndex() const
	{
		return currentItemIndex;
	}

	void PullDown::show(bool flag)
	{
		if (flag)
		{
			currentItemFrame->show();
		}
		else
		{
			currentItemFrame->hide();
		}
	}

}