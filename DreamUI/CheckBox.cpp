#include "stdafx.h"
#include "CheckBox.h"
#include "Observer.h"
#include "Tools.h"
#include "EventData.h"


void CheckBox::executeCallback()
{
	if (boundCallback)
	{
		bWantExecuteCallback = true;
	}
}


void CheckBoxEventHandler(Observer* ob, unsigned int evtId)
{
	CheckBox* cb = *(ob->data<CheckBox*>());
	switch (evtId) {
	case EVENT_CLICK:
		cb->toggle();
		SoundPlay("InterfaceClick", NULL, 0);
		cb->executeCallback();
		break;
	default:
		break;
	}
}


void CheckBox::setTexture(const char* bgTexPath, const char* bgDisTexPath, const char* bgPushTexPath, const char* checkTexPath, const char* checkDisTexPath)
{
	strBgTexPath.assign(bgTexPath ? bgTexPath : Skin::getPathByName("ReplayCheckBoxNormal"));
	strBgDisTexPath.assign(bgDisTexPath ? bgDisTexPath : "UI\\Widgets\\EscMenu\\Human\\checkbox-background-disabled.blp");
	strBgPushTexPath.assign(bgPushTexPath ? bgPushTexPath : Skin::getPathByName("ReplayCheckBoxPressed"));
	strCheckTexPath.assign(checkTexPath ? checkTexPath : "ui\\widgets\\glues\\gluescreen-checkbox-check.blp");
	strCheckDisTexPath.assign(checkDisTexPath ? checkDisTexPath : "ui\\widgets\\glues\\gluescreen-checkbox-checkdisabled.blp");
	controlButton->setStateTexture(UISimpleButton::STATE_DISABLED, strBgDisTexPath.c_str());
	controlButton->setStateTexture(UISimpleButton::STATE_ENABLED, strBgTexPath.c_str());
	controlButton->setStateTexture(UISimpleButton::STATE_PUSHED, strBgPushTexPath.c_str());
	checkTexture->setTexture(strCheckTexPath.c_str(), strCheckDisTexPath.c_str());
}


void CheckBox::init(float size, CheckBoxCallback callback, const char* profileAppName, const char* profileKeyName, bool* bindVar, bool defaultChecked)
{
	CheckBox* cb = this;
	eventObserver = Observer::Create(CheckBoxEventHandler);
	eventObserver->setData<CheckBox*>(&cb);

	bWantApplyActive = false;
	bWantExecuteCallback = false;

	controlButton = UISimpleButton::Create(
		getFrame(), size, size,
		strBgDisTexPath.c_str(),
		strBgTexPath.c_str(),
		strBgPushTexPath.c_str(),
		UISimpleButton::MOUSEBUTTON_LEFT,
		UISimpleButton::STATE_ENABLED
	);
	controlButton->setRelativePosition(POS_C, getFrame(), POS_C, 0, 0);
	controlButton->setClickEventObserver(EVENT_CLICK, eventObserver);
	controlButton->show();

	checkTexture = new Texture(UI_NULL, size, size, strCheckTexPath.c_str(), strCheckDisTexPath.c_str());
	checkTexture->setParent(controlButton);
	checkTexture->setOwner(this);
	checkTexture->setRelativePosition(POS_C, controlButton, POS_C);

	setTexture();

	bindCallback(callback);
	bindProfile(profileAppName, profileKeyName, defaultChecked);
	bindVariable(bindVar);
}

CheckBox::CheckBox(UISimpleFrame* parent, float size /* = 0.024f */, CheckBoxCallback callback /* = NULL */, bool* bindVar /* = NULL */, const char* profileAppName /* = NULL */, const char* profileKeyName /* = NULL */, bool defaultChecked /* = false */)
	: Frame(NULL, size, size), bIsChecked(defaultChecked), boundVar(NULL)
{
	setParent(parent);
	init(size, callback, profileAppName, profileKeyName, bindVar, defaultChecked);
}

CheckBox::CheckBox(IUIObject* parent, float size /* = 0.024f */, CheckBoxCallback callback /* = NULL */, bool* bindVar /* = NULL */, const char* profileAppName /* = NULL */, const char* profileKeyName /* = NULL */, bool defaultChecked /* = false */)
	: Frame(parent, size, size),
	bIsChecked(defaultChecked), boundVar(NULL)
{
	init(size, callback, profileAppName, profileKeyName, bindVar, defaultChecked);
}

CheckBox::~CheckBox()
{
	delete checkTexture;
	UISimpleButton::Destroy(controlButton);
	Observer::Destroy(eventObserver);
}

void CheckBox::check(bool flag)
{
	//if (bIsChecked != flag)
	//{
	bIsChecked = flag;

	if (boundVar)
	{
		*boundVar = bIsChecked;
	}
	if (boundProfileApp.length() && boundProfileKey.length())
	{

	}
	//}
	checkTexture->show(bIsChecked);
}

void CheckBox::toggle()
{
	check(!bIsChecked);
}

bool CheckBox::isChecked() const
{
	return bIsChecked;
}

void CheckBox::bindCallback(CheckBoxCallback callback)
{
	boundCallback = callback;
}

void CheckBox::bindVariable(bool* var)
{
	if (!var) return;
	boundVar = var;
	*var = isChecked();
}

void CheckBox::bindProfile(const char* appName, const char* keyName, bool defaultChecked /*= true */)
{
	boundProfileApp.assign(appName ? appName : "");
	boundProfileKey.assign(keyName ? keyName : "");
	check(defaultChecked);
}

void CheckBox::setWidth(float width)
{
	Frame::setWidth(width);
	checkTexture->setWidth(width);
	controlButton->setWidth(width);
}

void CheckBox::setHeight(float height)
{
	Frame::setHeight(height);
	checkTexture->setHeight(height);
	controlButton->setHeight(height);
}

void CheckBox::setSize(float size)
{
	Frame::setSize(size);
	checkTexture->setSize(size);
	controlButton->setWidth(size);
	controlButton->setHeight(size);
}

void CheckBox::setScale(float scale)
{
	Frame::setScale(scale);
	controlButton->setScale(scale);
	checkTexture->setScale(scale);
}

void CheckBox::activate(bool flag)
{
	if (this)
	{
		Frame::activate(flag);
		bWantApplyActive = true;
	}
}

void CheckBox::tick()
{
	Frame::tick();
	if (bWantApplyActive)
	{
		controlButton->setState(isActive() ? UISimpleButton::STATE_ENABLED : UISimpleButton::STATE_DISABLED);
		checkTexture->activate(isActive());
		bWantApplyActive = false;
	}
	if (bWantExecuteCallback)
	{
		if (boundCallback)
		{
			boundCallback(this, bIsChecked);
		}
		bWantExecuteCallback = false;
	}
}