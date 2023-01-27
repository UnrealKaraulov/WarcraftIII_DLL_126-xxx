#ifndef TOOLTIP_H_
#define TOOLTIP_H_

#include "UISimpleFrame.h"
#include "UISimpleFontString.h"
class Button;

class ToolTip {
public:
	static ToolTip* Create(
		UISimpleFrame* parent,
		float width,
		float height,
		const char* formattedContent,
		bool dontStore = false
	);

	static void Destroy(ToolTip* tooltip);

	void bindButton(Button* button);
	void applyPosition();

	void show();
	void hide();

private:
	ToolTip();
	UISimpleFrame* _frame;
	UISimpleFontString* _text;
};

void ToolTip_Init();
void ToolTip_Cleanup();

#endif