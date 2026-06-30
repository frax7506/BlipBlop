#include "HD_InputManager.h"

#include "HD_Math.h"
#include "HD_Unused.h"

#include "Windowsx.h"

void HD_InputManager::Init(HWND aWindowHandle)
{
	myOwnerHWND = aWindowHandle;

#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC ((USHORT) 0x01)
#endif

#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE ((USHORT) 0x02)
#endif

	RAWINPUTDEVICE rawInputDevice[1];
	rawInputDevice[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	rawInputDevice[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	rawInputDevice[0].dwFlags = RIDEV_INPUTSINK;
	rawInputDevice[0].hwndTarget = myOwnerHWND;
	RegisterRawInputDevices(rawInputDevice, 1, sizeof(rawInputDevice[0]));

	myTentativeMousePosition.Set(0, 0);
	myCurrentMousePosition.Set(0, 0);
	myPreviousMousePosition.Set(0, 0);

	myTentativeMouseDelta.Set(0, 0);
	myMouseDelta.Set(0, 0);

	myTentativeMouseWheelDelta = 0.f;
	myMouseWheelDelta = 0.f;
}

void HD_InputManager::Update()
{
	myPreviousMousePosition = myCurrentMousePosition;
	myCurrentMousePosition = myTentativeMousePosition;

	myMouseDelta = myTentativeMouseDelta;
	myTentativeMouseDelta.Set(0, 0);

	myMouseWheelDelta = myTentativeMouseWheelDelta / HD_Abs(myTentativeMouseWheelDelta);
	myTentativeMouseWheelDelta = 0.f;

	myPreviousState = myCurrentState;
	myCurrentState = myTentativeState;
}

bool HD_InputManager::UpdateEvents(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		myTentativeState[static_cast<int>(wParam)] = true;
		return true;

	case WM_KEYUP:
		myTentativeState[static_cast<int>(wParam)] = false;
		return true;

	case WM_LBUTTONDOWN:
		myTentativeState[VK_LBUTTON] = true;
		return true;

	case WM_LBUTTONUP:
		myTentativeState[VK_LBUTTON] = false;
		return true;

	case WM_RBUTTONDOWN:
		myTentativeState[VK_RBUTTON] = true;
		return true;

	case WM_RBUTTONUP:
		myTentativeState[VK_RBUTTON] = false;
		return true;

	case WM_MBUTTONDOWN:
		myTentativeState[VK_MBUTTON] = true;
		return true;

	case WM_MBUTTONUP:
		myTentativeState[VK_MBUTTON] = false;
		return true;

	case WM_SYSKEYDOWN:
		myTentativeState[static_cast<int>(wParam)] = true;
		return true;

	case WM_SYSKEYUP:
		myTentativeState[static_cast<int>(wParam)] = false;
		return true;

	case WM_XBUTTONDOWN:
	{
		int xButton = GET_XBUTTON_WPARAM(wParam);
		if (xButton == 1)
			myTentativeState[VK_XBUTTON1] = true;
		else
			myTentativeState[VK_XBUTTON2] = true;

		return true;
	}

	case WM_XBUTTONUP:
	{
		int xButton = GET_XBUTTON_WPARAM(wParam);
		if (xButton == 1)
			myTentativeState[VK_XBUTTON1] = false;
		else
			myTentativeState[VK_XBUTTON2] = false;

		return true;
	}

	case WM_MOUSEWHEEL:
		myTentativeMouseWheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		return true;

	// This is only used when you want X/Y coordinates.
	// The reason is that it's clunky to rely on delta
	// movements of the mouse. ClipRect and SetMousePos both
	// cause their own problems for input which are easily
	// solved by registering for the raw HID data and listening
	// for WM_INPUT instead.
	case WM_MOUSEMOVE:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		myTentativeMousePosition.myX = xPos;
		myTentativeMousePosition.myY = yPos;

		return true;
	}

	// Handles mouse delta, used in 3D navigation etc.
	case WM_INPUT:
	{
		UINT dwSize = sizeof(RAWINPUT);
		static BYTE lpb[sizeof(RAWINPUT)];

		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

		RAWINPUT* raw = (RAWINPUT*)lpb;

		if (raw->header.dwType == RIM_TYPEMOUSE)
		{
			myTentativeMouseDelta.myX += raw->data.mouse.lLastX;
			myTentativeMouseDelta.myY += raw->data.mouse.lLastY;
		}

		return true;
	}
	}

	return false;
}

bool HD_InputManager::GetIsKeyHeld(int aKeyCode) const
{
	return myCurrentState[aKeyCode] && myPreviousState[aKeyCode];
}

bool HD_InputManager::GetIsKeyPressed(int aKeyCode) const
{
	return myCurrentState[aKeyCode] && !myPreviousState[aKeyCode];
}

bool HD_InputManager::GetIsKeyReleased(int aKeyCode) const
{
	return !myCurrentState[aKeyCode] && myPreviousState[aKeyCode];
}

HD_Vector2f HD_InputManager::GetMouseDelta() const
{
	return HD_Vector2f(static_cast<float>(myMouseDelta.myX), static_cast<float>(myMouseDelta.myY));
}

HD_Vector2f HD_InputManager::GetMousePosition() const
{
	return HD_Vector2f(static_cast<float>(myCurrentMousePosition.myX), static_cast<float>(myCurrentMousePosition.myY));
}

void HD_InputManager::ShowMouse() const
{
	ShowCursor(true);
}

void HD_InputManager::HideMouse() const
{
	ShowCursor(false);
}

void HD_InputManager::CaptureMouse() const
{
	RECT clipRect;

	GetClientRect(myOwnerHWND, &clipRect);

	POINT upperLeft;
	upperLeft.x = clipRect.left;
	upperLeft.y = clipRect.top;

	POINT lowerRight;
	lowerRight.x = clipRect.right;
	lowerRight.y = clipRect.bottom;

	MapWindowPoints(myOwnerHWND, nullptr, &upperLeft, 1);
	MapWindowPoints(myOwnerHWND, nullptr, &lowerRight, 1);

	clipRect.left = upperLeft.x;
	clipRect.top = upperLeft.y;
	clipRect.right = lowerRight.x;
	clipRect.bottom = lowerRight.y;

	ClipCursor(&clipRect);
}

void HD_InputManager::ReleaseMouse() const
{
	ClipCursor(nullptr);
}
