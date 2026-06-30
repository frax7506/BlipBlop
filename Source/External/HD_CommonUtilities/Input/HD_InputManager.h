#pragma once

#include "HD_Bitset.h"
#include "HD_Singleton.h"
#include "HD_Vector2.h"

#include "Windows.h"

class HD_InputManager : public HD_Singleton<HD_InputManager>
{
public:
	void Init(HWND aWindowHandle);

	void Update();
	bool UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam);

	bool GetIsKeyHeld(int aKeyCode) const;
	bool GetIsKeyPressed(int aKeyCode) const;
	bool GetIsKeyReleased(int aKeyCode) const;

	HD_Vector2f GetMouseDelta() const;
	HD_Vector2f GetMousePosition() const;

	void ShowMouse() const;
	void HideMouse() const;

	void CaptureMouse() const;
	void ReleaseMouse() const;

private:
	// The "tentative" state holds the state that is
	// being updated by the message pump thread. It
	// can be used in place of the current state but
	// depending on how the game is threaded it might
	// be nice to keep them separate.
	HD_Bitset<256> myTentativeState;
	HD_Bitset<256> myCurrentState;
	HD_Bitset<256> myPreviousState;

	HWND myOwnerHWND;

	HD_Vector2i myTentativeMousePosition;
	HD_Vector2i myCurrentMousePosition;
	HD_Vector2i myPreviousMousePosition;

	HD_Vector2i myTentativeMouseDelta;
	HD_Vector2i myMouseDelta;

	float myTentativeMouseWheelDelta;
	float myMouseWheelDelta;
};
