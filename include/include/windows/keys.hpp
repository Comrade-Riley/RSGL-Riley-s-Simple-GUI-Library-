#pragma once

/*** Virtual key codes ***/
#define RK_LBUTTON             0x01
#define RK_RBUTTON             0x02
#define RK_CANCEL              0x03
#define RK_MBUTTON             0x04
#define RK_XBUTTON1            0x05
#define RK_XBUTTON2            0x06
/*                             0x07  Undefined */
#define RK_BACK                0x08
#define RK_TAB                 0x09
/*                             0x0A-0x0B  Undefined */
#define RK_CLEAR               0x0C
#define RK_RETURN              0x0D
/*                             0x0E-0x0F  Undefined */
#define RK_SHIFT               0x10
#define RK_CONTROL             0x11
#define RK_MENU                0x12
#define RK_PAUSE               0x13
#define RK_CAPITAL             0x14

#define RK_KANA                0x15
#define RK_HANGEUL             RK_KANA
#define RK_HANGUL              RK_KANA
/*                             0x16  Undefined */
#define RK_JUNJA               0x17
#define RK_FINAL               0x18
#define RK_HANJA               0x19
#define RK_KANJI               RK_HANJA

/*                             0x1A       Undefined */
#define RK_ESCAPE              0x1B

#define RK_CONVERT             0x1C
#define RK_NONCONVERT          0x1D
#define RK_ACCEPT              0x1E
#define RK_MODECHANGE          0x1F

#define RK_SPACE               0x20
#define RK_PRIOR               0x21
#define RK_NEXT                0x22
#define RK_END                 0x23
#define RK_HOME                0x24
#define RK_LEFT                0x25
#define RK_UP                  0x26
#define RK_RIGHT               0x27
#define RK_DOWN                0x28
#define RK_SELECT              0x29
#define RK_PRINT               0x2A /* OEM specific in Windows 3.1 SDK */
#define RK_EXECUTE             0x2B
#define RK_SNAPSHOT            0x2C
#define RK_INSERT              0x2D
#define RK_DELETE              0x2E
#define RK_HELP                0x2F
/* RK_0 - VK-9                 0x30-0x39  Use ASCII instead */
/*                             0x3A-0x40  Undefined */
/* RK_A - RK_Z                 0x41-0x5A  Use ASCII instead */
#define RK_LWIN                0x5B
#define RK_RWIN                0x5C
#define RK_APPS                0x5D
/*                             0x5E Unassigned */
#define RK_SLEEP               0x5F
#define RK_NUMPAD0             0x60
#define RK_NUMPAD1             0x61
#define RK_NUMPAD2             0x62
#define RK_NUMPAD3             0x63
#define RK_NUMPAD4             0x64
#define RK_NUMPAD5             0x65
#define RK_NUMPAD6             0x66
#define RK_NUMPAD7             0x67
#define RK_NUMPAD8             0x68
#define RK_NUMPAD9             0x69
#define RK_MULTIPLY            0x6A
#define RK_ADD                 0x6B
#define RK_SEPARATOR           0x6C
#define RK_SUBTRACT            0x6D
#define RK_DECIMAL             0x6E
#define RK_DIVIDE              0x6F
#define RK_F1                  0x70
#define RK_F2                  0x71
#define RK_F3                  0x72
#define RK_F4                  0x73
#define RK_F5                  0x74
#define RK_F6                  0x75
#define RK_F7                  0x76
#define RK_F8                  0x77
#define RK_F9                  0x78
#define RK_F10                 0x79
#define RK_F11                 0x7A
#define RK_F12                 0x7B
#define RK_F13                 0x7C
#define RK_F14                 0x7D
#define RK_F15                 0x7E
#define RK_F16                 0x7F
#define RK_F17                 0x80
#define RK_F18                 0x81
#define RK_F19                 0x82
#define RK_F20                 0x83
#define RK_F21                 0x84
#define RK_F22                 0x85
#define RK_F23                 0x86
#define RK_F24                 0x87
#define RK_NAVIGATION_VIEW     0x88
#define RK_NAVIGATION_MENU     0x89
#define RK_NAVIGATION_UP       0x8A
#define RK_NAVIGATION_DOWN     0x8B
#define RK_NAVIGATION_LEFT     0x8C
#define RK_NAVIGATION_RIGHT    0x8D
#define RK_NAVIGATION_ACCEPT   0x8E
#define RK_NAVIGATION_CANCEL   0x8F
#define RK_NUMLOCK             0x90
#define RK_SCROLL              0x91
#define RK_OEM_NEC_EQUAL       0x92
#define RK_OEM_FJ_JISHO        0x92
#define RK_OEM_FJ_MASSHOU      0x93
#define RK_OEM_FJ_TOUROKU      0x94
#define RK_OEM_FJ_LOYA         0x95
#define RK_OEM_FJ_ROYA         0x96
/*                             0x97-0x9F  Unassigned */
/*
 * differencing between right and left shift/control/alt key.
 * Used only by GetAsyncKeyState() and GetKeyState().
 */
#define RK_LSHIFT              0xA0
#define RK_RSHIFT              0xA1
#define RK_LCONTROL            0xA2
#define RK_RCONTROL            0xA3
#define RK_LMENU               0xA4
#define RK_RMENU               0xA5

#define RK_BROWSER_BACK        0xA6
#define RK_BROWSER_FORWARD     0xA7
#define RK_BROWSER_REFRESH     0xA8
#define RK_BROWSER_STOP        0xA9
#define RK_BROWSER_SEARCH      0xAA
#define RK_BROWSER_FAVORITES   0xAB
#define RK_BROWSER_HOME        0xAC
#define RK_VOLUME_MUTE         0xAD
#define RK_VOLUME_DOWN         0xAE
#define RK_VOLUME_UP           0xAF
#define RK_MEDIA_NEXT_TRACK    0xB0
#define RK_MEDIA_PREV_TRACK    0xB1
#define RK_MEDIA_STOP          0xB2
#define RK_MEDIA_PLAY_PAUSE    0xB3
#define RK_LAUNCH_MAIL         0xB4
#define RK_LAUNCH_MEDIA_SELECT 0xB5
#define RK_LAUNCH_APP1         0xB6
#define RK_LAUNCH_APP2         0xB7

/*                             0xB8-0xB9  Unassigned */
#define RK_OEM_1               0xBA
#define RK_OEM_PLUS            0xBB
#define RK_OEM_COMMA           0xBC
#define RK_OEM_MINUS           0xBD
#define RK_OEM_PERIOD          0xBE
#define RK_OEM_2               0xBF
#define RK_OEM_3               0xC0
/*                             0xC1-0xC2  Unassigned */
#define RK_GAMEPAD_A           0xC3
#define RK_GAMEPAD_B           0xC4
#define RK_GAMEPAD_X           0xC5
#define RK_GAMEPAD_Y           0xC6
#define RK_GAMEPAD_RIGHT_SHOULDER 0xC7
#define RK_GAMEPAD_LEFT_SHOULDER 0xC8
#define RK_GAMEPAD_LEFT_TRIGGER 0xC9
#define RK_GAMEPAD_RIGHT_TRIGGER 0xCA
#define RK_GAMEPAD_DPAD_UP     0xCB
#define RK_GAMEPAD_DPAD_DOWN   0xCC
#define RK_GAMEPAD_DPAD_LEFT   0xCD
#define RK_GAMEPAD_DPAD_RIGHT  0xCE
#define RK_GAMEPAD_MENU        0xCF
#define RK_GAMEPAD_VIEW        0xD0
#define RK_GAMEPAD_LEFT_THUMBSTICK_BUTTON 0xD1
#define RK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON 0xD2
#define RK_GAMEPAD_LEFT_THUMBSTICK_UP 0xD3
#define RK_GAMEPAD_LEFT_THUMBSTICK_DOWN 0xD4
#define RK_GAMEPAD_LEFT_THUMBSTICK_RIGHT 0xD5
#define RK_GAMEPAD_LEFT_THUMBSTICK_LEFT 0xD6
#define RK_GAMEPAD_RIGHT_THUMBSTICK_UP 0xD7
#define RK_GAMEPAD_RIGHT_THUMBSTICK_DOWN 0xD8
#define RK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT 0xD9
#define RK_GAMEPAD_RIGHT_THUMBSTICK_LEFT 0xDA
#define RK_OEM_4               0xDB
#define RK_OEM_5               0xDC
#define RK_OEM_6               0xDD
#define RK_OEM_7               0xDE
#define RK_OEM_8               0xDF
/*                             0xE0       OEM specific */
#define RK_OEM_AX              0xE1  /* "AX" key on Japanese AX keyboard */
#define RK_OEM_102             0xE2  /* "<>" or "\|" on RT 102-key keyboard */
#define RK_ICO_HELP            0xE3  /* Help key on ICO */
#define RK_ICO_00              0xE4  /* 00 key on ICO */
#define RK_PROCESSKEY          0xE5
#define RK_ICO_CLEAR           0xE6

#define RK_PACKET              0xE7
/*                             0xE8       Unassigned */

#define RK_OEM_RESET           0xE9
#define RK_OEM_JUMP            0xEA
#define RK_OEM_PA1             0xEB
#define RK_OEM_PA2             0xEC
#define RK_OEM_PA3             0xED
#define RK_OEM_WSCTRL          0xEE
#define RK_OEM_CUSEL           0xEF
#define RK_OEM_ATTN            0xF0
#define RK_OEM_FINISH          0xF1
#define RK_OEM_COPY            0xF2
#define RK_OEM_AUTO            0xF3
#define RK_OEM_ENLW            0xF4
#define RK_OEM_BACKTAB         0xF5
#define RK_ATTN                0xF6
#define RK_CRSEL               0xF7
#define RK_EXSEL               0xF8
#define RK_EREOF               0xF9
#define RK_PLAY                0xFA
#define RK_ZOOM                0xFB
#define RK_NONAME              0xFC
#define RK_PA1                 0xFD
#define RK_OEM_CLEAR           0xFE
/*                             0xFF       Unassigned */

#define RK_0 0x30
#define RK_1 0x31
#define RK_2 0x32
#define RK_3 0x33
#define RK_4 0x34
#define RK_5 0x35
#define RK_6 0x36
#define RK_7 0x37
#define RK_8 0x38
#define RK_9 0x39

#define RK_A 0x41
#define RK_B 0x42
#define RK_C 0x43
#define RK_D 0x44
#define RK_E 0x45
#define RK_F 0x46
#define RK_G 0x47
#define RK_H 0x48
#define RK_I 0x49
#define RK_J 0x4A
#define RK_K 0x4B
#define RK_L 0x4C
#define RK_M 0x4D
#define RK_N 0x4E
#define RK_O 0x4F
#define RK_P 0x50
#define RK_Q 0x51
#define RK_R 0x52
#define RK_S 0x53
#define RK_T 0x54
#define RK_U 0x55
#define RK_V 0x56
#define RK_W 0x57
#define RK_X 0x58
#define RK_Y 0x59
#define RK_Z 0x5A