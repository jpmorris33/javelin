#ifndef IRE_IKEYS_HPP
#define IRE_IKEYS_HPP
#include <SDL2/SDL_keycode.h>

#define IREKEY_A                 SDL_SCANCODE_A
#define IREKEY_B                 SDL_SCANCODE_B
#define IREKEY_C                 SDL_SCANCODE_C
#define IREKEY_D                 SDL_SCANCODE_D
#define IREKEY_E                 SDL_SCANCODE_E
#define IREKEY_F                 SDL_SCANCODE_F
#define IREKEY_G                 SDL_SCANCODE_G
#define IREKEY_H                 SDL_SCANCODE_H
#define IREKEY_I                 SDL_SCANCODE_I
#define IREKEY_J                 SDL_SCANCODE_J
#define IREKEY_K                 SDL_SCANCODE_K
#define IREKEY_L                 SDL_SCANCODE_L
#define IREKEY_M                 SDL_SCANCODE_M
#define IREKEY_N                 SDL_SCANCODE_N
#define IREKEY_O                 SDL_SCANCODE_O
#define IREKEY_P                 SDL_SCANCODE_P
#define IREKEY_Q                 SDL_SCANCODE_Q
#define IREKEY_R                 SDL_SCANCODE_R
#define IREKEY_S                 SDL_SCANCODE_S
#define IREKEY_T                 SDL_SCANCODE_T
#define IREKEY_U                 SDL_SCANCODE_U
#define IREKEY_V                 SDL_SCANCODE_V
#define IREKEY_W                 SDL_SCANCODE_W
#define IREKEY_X                 SDL_SCANCODE_X
#define IREKEY_Y                 SDL_SCANCODE_Y
#define IREKEY_Z                 SDL_SCANCODE_Z
#define IREKEY_0                 SDL_SCANCODE_0
#define IREKEY_1                 SDL_SCANCODE_1
#define IREKEY_2                 SDL_SCANCODE_2
#define IREKEY_3                 SDL_SCANCODE_3
#define IREKEY_4                 SDL_SCANCODE_4
#define IREKEY_5                 SDL_SCANCODE_5
#define IREKEY_6                 SDL_SCANCODE_6
#define IREKEY_7                 SDL_SCANCODE_7
#define IREKEY_8                 SDL_SCANCODE_8
#define IREKEY_9                 SDL_SCANCODE_9
#define IREKEY_0_PAD             SDL_SCANCODE_KP_0
#define IREKEY_1_PAD             SDL_SCANCODE_KP_1
#define IREKEY_2_PAD             SDL_SCANCODE_KP_2
#define IREKEY_3_PAD             SDL_SCANCODE_KP_3
#define IREKEY_4_PAD             SDL_SCANCODE_KP_4
#define IREKEY_5_PAD             SDL_SCANCODE_KP_5
#define IREKEY_6_PAD             SDL_SCANCODE_KP_6
#define IREKEY_7_PAD             SDL_SCANCODE_KP_7
#define IREKEY_8_PAD             SDL_SCANCODE_KP_8
#define IREKEY_9_PAD             SDL_SCANCODE_KP_9
#define IREKEY_F1                SDL_SCANCODE_F1
#define IREKEY_F2                SDL_SCANCODE_F2
#define IREKEY_F3                SDL_SCANCODE_F3
#define IREKEY_F4                SDL_SCANCODE_F4
#define IREKEY_F5                SDL_SCANCODE_F5
#define IREKEY_F6                SDL_SCANCODE_F6
#define IREKEY_F7                SDL_SCANCODE_F7
#define IREKEY_F8                SDL_SCANCODE_F8
#define IREKEY_F9                SDL_SCANCODE_F9
#define IREKEY_F10               SDL_SCANCODE_F10
#define IREKEY_F11               SDL_SCANCODE_F11
#define IREKEY_F12               SDL_SCANCODE_F12
#define IREKEY_ESC               SDL_SCANCODE_ESCAPE
#define IREKEY_TILDE             SDL_SCANCODE_BACKSLASH	// SDL_SCANCODE_GRAVE, apparently not
#define IREKEY_MINUS             SDL_SCANCODE_MINUS
#define IREKEY_EQUALS            SDL_SCANCODE_EQUALS
#define IREKEY_BACKSPACE         SDL_SCANCODE_BACKSPACE
#define IREKEY_TAB               SDL_SCANCODE_TAB
#define IREKEY_OPENBRACE         SDL_SCANCODE_LEFTBRACKET
#define IREKEY_CLOSEBRACE        SDL_SCANCODE_RIGHTBRACKET
#define IREKEY_ENTER             SDL_SCANCODE_RETURN
#define IREKEY_COLON             SDL_SCANCODE_SEMICOLON
#define IREKEY_QUOTE             SDL_SCANCODE_APOSTROPHE
#define IREKEY_BACKSLASH         SDL_SCANCODE_BACKSLASH
#define IREKEY_BACKSLASH2        SDL_SCANCODE_BACKSLASH	// Doubtful
#define IREKEY_COMMA             SDL_SCANCODE_COMMA
#define IREKEY_STOP              SDL_SCANCODE_PERIOD
#define IREKEY_DOT               SDL_SCANCODE_PERIOD	// Not sure that's right
#define IREKEY_SLASH             SDL_SCANCODE_SLASH
#define IREKEY_SPACE             SDL_SCANCODE_SPACE
#define IREKEY_INSERT            SDL_SCANCODE_INSERT
#define IREKEY_DEL               SDL_SCANCODE_DELETE
#define IREKEY_HOME              SDL_SCANCODE_HOME
#define IREKEY_END               SDL_SCANCODE_END
#define IREKEY_PGUP              SDL_SCANCODE_PAGEUP
#define IREKEY_PGDN              SDL_SCANCODE_PAGEDOWN
#define IREKEY_LEFT              SDL_SCANCODE_LEFT
#define IREKEY_RIGHT             SDL_SCANCODE_RIGHT
#define IREKEY_UP                SDL_SCANCODE_UP
#define IREKEY_DOWN              SDL_SCANCODE_DOWN
#define IREKEY_SLASH_PAD         SDL_SCANCODE_KP_DIVIDE
#define IREKEY_ASTERISK          SDL_SCANCODE_KP_MULTIPLY
#define IREKEY_MINUS_PAD         SDL_SCANCODE_KP_MINUS
#define IREKEY_PLUS_PAD          SDL_SCANCODE_KP_PLUS
#define IREKEY_DEL_PAD           SDL_SCANCODE_KP_PERIOD
#define IREKEY_ENTER_PAD         SDL_SCANCODE_KP_ENTER
#define IREKEY_PRTSCR            SDL_SCANCODE_PRINTSCREEN
#define IREKEY_PAUSE             SDL_SCANCODE_PAUSE
#define IREKEY_YEN               -1
#define IREKEY_YEN2              -1

#define IREKEY_MODIFIERS         -1	// ??

#define IREKEY_LSHIFT            SDL_SCANCODE_LSHIFT
#define IREKEY_RSHIFT            SDL_SCANCODE_RSHIFT
#define IREKEY_LCONTROL          SDL_SCANCODE_LCTRL
#define IREKEY_RCONTROL          SDL_SCANCODE_RCTRL
#define IREKEY_ALT               SDL_SCANCODE_LALT
#define IREKEY_ALTGR             SDL_SCANCODE_RALT
#define IREKEY_LWIN              SDL_SCANCODE_LGUI
#define IREKEY_RWIN              SDL_SCANCODE_RGUI
#define IREKEY_MENU              -100
#define IREKEY_SCRLOCK           SDL_SCANCODE_SCROLLLOCK
#define IREKEY_NUMLOCK           SDL_SCANCODE_NUMLOCKCLEAR
#define IREKEY_CAPSLOCK          SDL_SCANCODE_CAPSLOCK

//#define IREKEY_MAX               SDL_SCANCODE_LAST
#define IREKEY_MAX               4096


#define IRESHIFT_SHIFT            1
#define IRESHIFT_CONTROL          2
#define IRESHIFT_ALT              4

#endif
