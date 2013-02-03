#include "Global.h"

#include "ChatDialog.hpp"
#include "DialogType.hpp"
#include "Network.hpp"

#include "..\RGE\EditBox.hpp"
#include "..\RGE\ListBox.hpp"
#include "..\RGE\ScrollBar.hpp"

#define NOGDI
#include "Windows.hpp"

ChatDialog::ChatDialog(){
	SetID(DLG_CHAT_BOX);
}

ChatDialog::~ChatDialog(){
}

ChatDialog* ChatDialog::Create(){
	ChatDialog* dlg = new ChatDialog();
	dlg->CreateFromXML("3DDATA\\CONTROL\\XML\\DlgChat.xml");
	return dlg;
}

void ChatDialog::OnCreated(){
	mEditBox = (EditBox*)GetControlByID(15);

	mListBoxes[CHAT_TAB_ALL] = (ListBox*)GetControlByID(20);
	mListBoxes[CHAT_TAB_CLAN] = (ListBox*)GetControlByID(40);
	mListBoxes[CHAT_TAB_TRADE] = (ListBox*)GetControlByID(30);
	mListBoxes[CHAT_TAB_PARTY] = (ListBox*)GetControlByID(35);
	mListBoxes[CHAT_TAB_ALLIED] = (ListBox*)GetControlByID(45);
	mListBoxes[CHAT_TAB_WHISPER] = (ListBox*)GetControlByID(25);

	mScrollBars[CHAT_TAB_ALL] = (ScrollBar*)GetControlByID(21);
	mScrollBars[CHAT_TAB_CLAN] = (ScrollBar*)GetControlByID(41);
	mScrollBars[CHAT_TAB_TRADE] = (ScrollBar*)GetControlByID(31);
	mScrollBars[CHAT_TAB_PARTY] = (ScrollBar*)GetControlByID(36);
	mScrollBars[CHAT_TAB_ALLIED] = (ScrollBar*)GetControlByID(46);
	mScrollBars[CHAT_TAB_WHISPER] = (ScrollBar*)GetControlByID(26);

	mListBoxes[CHAT_TAB_CLAN]->SetVisible(false);
	mListBoxes[CHAT_TAB_PARTY]->SetVisible(false);
	mListBoxes[CHAT_TAB_TRADE]->SetVisible(false);
	mListBoxes[CHAT_TAB_ALLIED]->SetVisible(false);
	mListBoxes[CHAT_TAB_WHISPER]->SetVisible(false);

	mScrollBars[CHAT_TAB_CLAN]->SetVisible(false);
	mScrollBars[CHAT_TAB_PARTY]->SetVisible(false);
	mScrollBars[CHAT_TAB_TRADE]->SetVisible(false);
	mScrollBars[CHAT_TAB_ALLIED]->SetVisible(false);
	mScrollBars[CHAT_TAB_WHISPER]->SetVisible(false);
}

int ChatDialog::HandleEvent(KeyboardEvent* evt){
	int ret = Dialog::HandleEvent(evt);
	if(ret == mEditBox->ID() && evt->type == KEY_UP && evt->key == VK_RETURN)
		SendChatMessage();

	return ret;
}

void ChatDialog::AddChatMessage(const char* message, ChatType type, Colour force_colour){
	static Colour gChatColours[] = {
		Colour(255, 255, 255, 255),
		Colour(189, 250, 255, 255),
		Colour(255, 237, 140, 255),
		Colour(201, 255, 144, 255),
		Colour(255, 188, 172, 255),
		Colour(255, 224, 229, 255),
		Colour(255, 0, 0, 255),
		Colour(151, 221, 241, 255),
		Colour(224, 162, 209, 255),
		Colour(255, 228, 122, 255),
		Colour(255, 255, 255, 255),
		Colour(255, 228, 122, 255),
		Colour(255, 228, 122, 255),
	};

	static ChatFilter gChatFilters[] = {
		CHAT_FILTER_NORMAL,
		CHAT_FILTER_NORMAL,
		CHAT_FILTER_PARTY,
		CHAT_FILTER_WHISPER,
		CHAT_FILTER_SYSTEM,
		CHAT_FILTER_SYSTEM,
		CHAT_FILTER_SYSTEM,
		CHAT_FILTER_SYSTEM,
		CHAT_FILTER_SYSTEM,
		CHAT_FILTER_CLAN,
		CHAT_FILTER_TRADE,
		CHAT_FILTER_CLAN,
		CHAT_FILTER_SYSTEM,
	};

	static bool gTabFilters[CHAT_TAB_MAX][CHAT_FILTER_MAX] = {
		{ true, true, true, true, true, true, true },
		{ false, true, false, false, false, false, true },
		{ false, true, true, false, false, false, true },
		{ false, true, false, true, false, false, true },
		{ false, true, false, false, true, false, true },
		{ false, true, false, false, false, true, true }
	};

	ChatFilter filter = CHAT_FILTER_NORMAL;
	Colour colour = Colour();
	if(force_colour != 0)
		colour = force_colour;
	else
		colour = gChatColours[type];

	for(unsigned int i = 0; i < CHAT_TAB_MAX; ++i){
		if(!gTabFilters[i][filter]) continue;
		ListBox* lst = mListBoxes[i];
		bool scroll = lst->GetValue() == lst->GetMax();
		lst->AddItem(message, colour);

		if(scroll){
			ScrollBar* bar = mScrollBars[i];
			bar->ScrollToBottom();
		}
	}
}

void ChatDialog::SendChatMessage(){
	const char* text = mEditBox->GetText();
	switch(text[0]){
		case '!':
			break;
		case '$':
			break;
		case '@':
			break;
		case '&':
			break;
		case '#':
			break;
		case '~':
			break;
		default:
			Packet pakout(0x783);
			pakout.Add(text);
			gNetwork->SendPacket(GS, &pakout);
			break;
	};

	mEditBox->ClearText();
}
