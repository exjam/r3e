#ifndef UI_STATE_HPP
#define UI_STATE_HPP

class IControl;

struct UiState {
	static IControl* mFocus;
	static IControl* mModal;
};

#endif
