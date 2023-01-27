#ifndef CHECKBOX_H_
#define CHECKBOX_H_

#include "UISimpleButton.h"
//#include "UISimpleTexture.h"
#include "Frame.h"
#include "Texture.h"

class CheckBox;
typedef void (*CheckBoxCallback)(CheckBox* cb, bool checked);

class CheckBox : public Frame
{
public:
	CheckBox(
		IUIObject* parent,
		float					size = 0.024f,
		CheckBoxCallback		callback = NULL,
		bool* bindVar = NULL,
		const char* profileAppName = NULL,
		const char* profileKeyName = NULL,
		bool					defaultChecked = false
	);

	CheckBox(
		UISimpleFrame* parent,
		float					size = 0.024f,
		CheckBoxCallback		callback = NULL,
		bool* bindVar = NULL,
		const char* profileAppName = NULL,
		const char* profileKeyName = NULL,
		bool					defaultChecked = false
	);



	virtual ~CheckBox();

	void	setTexture(
		const char* bgTexPath = NULL,
		const char* bgDisTexPath = NULL,
		const char* bgPushTexPath = NULL,
		const char* checkTexPath = NULL,
		const char* checkDisTexPath = NULL
	);

	void	check(bool flag);
	void	toggle();
	void	executeCallback();
	bool	isChecked() const;
	void	bindCallback(CheckBoxCallback callback);
	void	bindVariable(bool* var);
	void	bindProfile(const char* appName, const char* keyName, bool defaultChecked = true);

	virtual void	setWidth(float width);
	virtual void	setHeight(float height);
	virtual void	setSize(float size);
	virtual void	setScale(float scale);

	virtual void	activate(bool flag);
	virtual void	tick();

private:
	UISimpleButton* controlButton;
	Texture* checkTexture;
	bool				bIsChecked;
	bool* boundVar;
	std::string			boundProfileApp;
	std::string			boundProfileKey;
	CheckBoxCallback	boundCallback;
	Observer* eventObserver;

	bool				bWantApplyActive;
	bool				bWantExecuteCallback;

	std::string			strBgTexPath;
	std::string			strBgDisTexPath;
	std::string			strBgPushTexPath;
	std::string			strCheckTexPath;
	std::string			strCheckDisTexPath;

	void init(float size, CheckBoxCallback callback, const char* profileAppName, const char* profileKeyName, bool* bindVar, bool defaultChecked);
};

#endif