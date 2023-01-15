#ifndef INFOPANEL_ROW_H_
#define INFOPANEL_ROW_H_

class UISimpleFrame;
class UISimpleTexture;
class UISimpleFontString;
class UILayoutFrame;
class Label;

namespace InfoPanel {
	//keyΪ��Ŀ��(0-based)
	typedef std::map< unsigned int, UISimpleFontString* > TextContentType;

	//����һ�У�����˹̶�Ϊ�������ͼ�� + ��Һ�(1-based)
	//��ʽ��һ��ϸ�߿��ޱ�����UISimpleFrame
	//��С��ָ�����¿�ȣ�ָ����Ŀ��ȣ����ݾ�����λ��
	//�ɰ��� obj[0], obj[1]����access��Ŀ��[0]Ϊ��һ����Ŀ
	class Row {
		unsigned int					itemCount;			//��Ŀ����

		UISimpleFrame* frame;				//������λ�ͱ����Ļ�����
		UISimpleFrame* playerIconFrame;
		float						height;
		float						itemWidth;
		float						width; //��Ϊ0ʱǿ��Ϊ���
		bool						hidden;

		UISimpleTexture* playerColorBg;		//�����ɫ
		UISimpleTexture* playerRaceIcon;		//�������ͼ��
		Label* playerIdLabel;
		int							playerId;			//��Һ�(0-based)	 TODO���������
		bool						playerWantUpdate;	//�Ƿ���update�и���

		TextContentType				contentSimpleFontStrings;
		std::map<unsigned int, std::string>	contentText;

	public:

		Row(UISimpleFrame* parent, float inHeight, float inItemWidth, unsigned int inItemCount);
		~Row();

		void	setPlayerId(int inPlayerId);
		void	setText(unsigned int index, char* format, ...);
		void	setWidth(float width);

		UILayoutFrame* getFrame();
		float	getHeight() { return height; }

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

		void	show(bool flag = true);
		bool	isHidden();
		void	update();						//�����������ݸ���
	};

}

#endif