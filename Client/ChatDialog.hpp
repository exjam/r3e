#ifndef CHAT_DIALOG_HPP
#define CHAT_DIALOG_HPP

#include "Colour.hpp"
#include "..\RGE\Dialog.hpp"

enum ChatFilter {
	CHAT_FILTER_NORMAL,
	CHAT_FILTER_SYSTEM,
	CHAT_FILTER_TRADE,
	CHAT_FILTER_PARTY,
	CHAT_FILTER_CLAN,
	CHAT_FILTER_ALLIED,
	CHAT_FILTER_WHISPER,
	CHAT_FILTER_MAX,
};

enum ChatTab {
	CHAT_TAB_ALL,
	CHAT_TAB_WHISPER,
	CHAT_TAB_TRADE,
	CHAT_TAB_PARTY,
	CHAT_TAB_CLAN,
	CHAT_TAB_ALLIED,
	CHAT_TAB_MAX,
};

enum ChatType {
	CHAT_TYPE_ALL,
	CHAT_TYPE_SHOUT,
	CHAT_TYPE_PARTY,
	CHAT_TYPE_WHISPER,
	CHAT_TYPE_NOTICE,
	CHAT_TYPE_SYSTEM,
	CHAT_TYPE_ERROR,
	CHAT_TYPE_QUEST,
	CHAT_TYPE_QUESTREWARD,
	CHAT_TYPE_CLAN,
	CHAT_TYPE_TRADE,
	CHAT_TYPE_ALLIED,
	CHAT_TYPE_FRIEND,
	CHAT_TYPE_MAX,
};

class EditBox;
class ListBox;
class ScrollBar;

class ChatDialog : public Dialog {
public:
	ChatDialog();
	~ChatDialog();

	static ChatDialog* Create();
	virtual void OnCreated();
	virtual int HandleEvent(KeyboardEvent* evt);

	void AddChatMessage(const char* message, ChatType type, Colour force_colour = 0);

private:
	void SendChatMessage();

private:
	EditBox* mEditBox;
	ListBox* mListBoxes[CHAT_TAB_MAX];
	ScrollBar* mScrollBars[CHAT_TAB_MAX];
};

#endif