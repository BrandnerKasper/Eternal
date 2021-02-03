#pragma once

//Own wrapper for key codes (if you change to sth. else than glfw it wouldn't break, for now it uses the same keycodes as glfw3.h!!

/* Printable keys */
#define ET_KEY_SPACE              32
#define ET_KEY_APOSTROPHE         39  /* ' */
#define ET_KEY_COMMA              44  /* , */
#define ET_KEY_MINUS              45  /* - */
#define ET_KEY_PERIOD             46  /* . */
#define ET_KEY_SLASH              47  /* / */
#define ET_KEY_0                  48
#define ET_KEY_1                  49
#define ET_KEY_2                  50
#define ET_KEY_3                  51
#define ET_KEY_4                  52
#define ET_KEY_5                  53
#define ET_KEY_6                  54
#define ET_KEY_7                  55
#define ET_KEY_8                  56
#define ET_KEY_9                  57
#define ET_KEY_SEMICOLON          59  /* ; */
#define ET_KEY_EQUAL              61  /* = */
#define ET_KEY_A                  65
#define ET_KEY_B                  66
#define ET_KEY_C                  67
#define ET_KEY_D                  68
#define ET_KEY_E                  69
#define ET_KEY_F                  70
#define ET_KEY_G                  71
#define ET_KEY_H                  72
#define ET_KEY_I                  73
#define ET_KEY_J                  74
#define ET_KEY_K                  75
#define ET_KEY_L                  76
#define ET_KEY_M                  77
#define ET_KEY_N                  78
#define ET_KEY_O                  79
#define ET_KEY_P                  80
#define ET_KEY_Q                  81
#define ET_KEY_R                  82
#define ET_KEY_S                  83
#define ET_KEY_T                  84
#define ET_KEY_U                  85
#define ET_KEY_V                  86
#define ET_KEY_W                  87
#define ET_KEY_X                  88
#define ET_KEY_Y                  89
#define ET_KEY_Z                  90
#define ET_KEY_LEFT_BRACKET       91  /* [ */
#define ET_KEY_BACKSLASH          92  /* \ */
#define ET_KEY_RIGHT_BRACKET      93  /* ] */
#define ET_KEY_GRAVE_ACCENT       96  /* ` */
#define ET_KEY_WORLD_1            161 /* non-US #1 */
#define ET_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define ET_KEY_ESCAPE             256
#define ET_KEY_ENTER              257
#define ET_KEY_TAB                258
#define ET_KEY_BACKSPACE          259
#define ET_KEY_INSERT             260
#define ET_KEY_DELETE             261
#define ET_KEY_RIGHT              262
#define ET_KEY_LEFT               263
#define ET_KEY_DOWN               264
#define ET_KEY_UP                 265
#define ET_KEY_PAGE_UP            266
#define ET_KEY_PAGE_DOWN          267
#define ET_KEY_HOME               268
#define ET_KEY_END                269
#define ET_KEY_CAPS_LOCK          280
#define ET_KEY_SCROLL_LOCK        281
#define ET_KEY_NUM_LOCK           282
#define ET_KEY_PRINT_SCREEN       283
#define ET_KEY_PAUSE              284
#define ET_KEY_F1                 290
#define ET_KEY_F2                 291
#define ET_KEY_F3                 292
#define ET_KEY_F4                 293
#define ET_KEY_F5                 294
#define ET_KEY_F6                 295
#define ET_KEY_F7                 296
#define ET_KEY_F8                 297
#define ET_KEY_F9                 298
#define ET_KEY_F10                299
#define ET_KEY_F11                300
#define ET_KEY_F12                301
#define ET_KEY_F13                302
#define ET_KEY_F14                303
#define ET_KEY_F15                304
#define ET_KEY_F16                305
#define ET_KEY_F17                306
#define ET_KEY_F18                307
#define ET_KEY_F19                308
#define ET_KEY_F20                309
#define ET_KEY_F21                310
#define ET_KEY_F22                311
#define ET_KEY_F23                312
#define ET_KEY_F24                313
#define ET_KEY_F25                314
#define ET_KEY_KP_0               320
#define ET_KEY_KP_1               321
#define ET_KEY_KP_2               322
#define ET_KEY_KP_3               323
#define ET_KEY_KP_4               324
#define ET_KEY_KP_5               325
#define ET_KEY_KP_6               326
#define ET_KEY_KP_7               327
#define ET_KEY_KP_8               328
#define ET_KEY_KP_9               329
#define ET_KEY_KP_DECIMAL         330
#define ET_KEY_KP_DIVIDE          331
#define ET_KEY_KP_MULTIPLY        332
#define ET_KEY_KP_SUBTRACT        333
#define ET_KEY_KP_ADD             334
#define ET_KEY_KP_ENTER           335
#define ET_KEY_KP_EQUAL           336
#define ET_KEY_LEFT_SHIFT         340
#define ET_KEY_LEFT_CONTROL       341
#define ET_KEY_LEFT_ALT           342
#define ET_KEY_LEFT_SUPER         343
#define ET_KEY_RIGHT_SHIFT        344
#define ET_KEY_RIGHT_CONTROL      345
#define ET_KEY_RIGHT_ALT          346
#define ET_KEY_RIGHT_SUPER        347
#define ET_KEY_MENU               348