#ifndef SVR_LST_DIALOG_HPP
#define SVR_LST_DIALOG_HPP

#include "..\RGE\Dialog.hpp"

class Button;
class ListBox;
struct WorldServer;
struct GameChannel;

class ServerListDialog : public Dialog {
public:
	ServerListDialog();
	~ServerListDialog();

	static ServerListDialog* Create();

	virtual void OnCreated();
	virtual int HandleEvent(MouseEvent* evt);

	void ClearServers();
	void ClearChannels();

	void AddServer(WorldServer* svr);
	void AddChannel(int server, GameChannel* channel);

public:
	ListBox* mServerList;
	ListBox* mChannelList;

private:
	Button* mButtonOK;
	Button* mButtonCancel;

	//ID Mapping
	Array<int, 5> mServers; 
	Array<int, 5> mChannels;
};

#endif