/* See LICENSE file for copyright and license details... */

/* constants */ #define TERMINAL "st"
#define TERMCLASS "St"
#define BROWSER $BROWSER

/* appearance */
static const double activeopacity   = 1.0f;     /* Window opacity when it's focused (0 <= opacity <= 1) */
static const double inactiveopacity = 0.9f;   /* Window opacity when it's inactive (0 <= opacity <= 1) */
static       Bool bUseOpacity       = True;     /* Starts with opacity on any unfocused windows */

static const unsigned int borderpx  = 4;        /* border pixel of windows       3*/
static const unsigned int snap      = 16;       /* snap pixel */
static const unsigned int gappih    = 16;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 16;       /* vert inner gap between windows */
static const unsigned int gappoh    = 16;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 16;       /* vert outer gap between windows and screen edge */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar                  = 1;        /* 0 means no bar */
static const int topbar                   = 1;        /* 0 means bottom bar */
static const char font[]                  = "mononoki:pixelsize=15";
static const char dmenufont[]             = "mononoki:pixelsize=15";
static const char *fonts[]          = { "mononoki:pixelsize=15", "monospace:pixelsize=15" };
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#000000";
static const char col_gray3[]       = "#ffffff";
static const char col_gray4[]       = "#000000";
static const char col_cyan[]        = "#ffffff";
static const char *colors[][3]      = {
        /*               fg         bg         border   */
        [SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
        [SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9"};

static const Rule rules[] = {
        /* xprop(1):
         *      WM_CLASS(STRING) = instance, class
         *      WM_NAME(STRING) = title
         */
        /* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
        { "Gimp",    NULL,     NULL,           1 << 9,    0,          0,           0,        -1 },
        { TERMCLASS, NULL,     NULL,           0,         0,          1,           0,        -1 },
        { NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
        { "mpv",     NULL,     "Webcam",       1 << 8,    1,          0,           0,        -1 }, /* xev */
        { "zotero",  NULL,     "Quick Format Citation",1 << 8, 1,     0,           0,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

/*#include "fibonacci.c"*/
static const Layout layouts[] = {
        /* symbol     arrange function */
        { "[]=",      tile },    /* first entry is default *//*0 */
        { "[M]",      monocle },
        { "[@]",      spiral },
        { "[\\]",     dwindle },
        { "H[]",      deck },
        { "TTT",      bstack },
        { "===",      bstackhoriz },
        { "HHH",      grid },
        { "###",      nrowgrid },
        { "---",      horizgrid },
        { ":::",      gaplessgrid },
        { "|M|",      centeredmaster },
        { ">M>",      centeredfloatingmaster },
        { "><>",      NULL }, /*13*/   /* no layout function means floating behavior */
        { NULL,       NULL },

};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
        { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
        { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
        { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

// xmodmap -pk can be used to get the names of key codes
// xev keys can be intersepted by the window manager, so getting mappings bay be hard if they are already used for something

/* i added */
/*
#define XK_micMute              0xffc1
#define XK_volumeDown           0xffbf
#define XK_mute                 0xffbe
#define XK_volumeUp             0xffc0
*/

//trying something
#include <X11/XF86keysym.h>
/*
#define XK_mute                 0xff12
#define XK_volumeDown           0x1008ff11
#define XK_volumeUp             0x1008ff13
#define XK_micMute              0x1008ffb2
#define XK_brightDown           0x1008ff03
#define XK_brightUp             0x1008ff02
*/

/* commands */
static const char *termcmd[]  = { "st", NULL };
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
/*
ResourcePref resources[] = {
        { "color4",             STRING,  &normfgcolor },
        { "color0",             STRING,  &normbgcolor },
        { "color0",             STRING,  &normbordercolor },
        { "color0",             STRING,  &selfgcolor },
        { "color4",             STRING,  &selbgcolor },
        { "color7",             STRING,  &selbordercolor },
};*/
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-l", "10",  NULL };

static const char dmenu_sudo_prompt[] = "THIS IS WILL BE RUN AS SUDO: ";

static const char *sudo_dmenucmd[] = { "sudo", "dmenu_run", "-m", dmenumon, "-p", dmenu_sudo_prompt, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-l", "10",  NULL };

static const char *browser[]  = { "/usr/bin/firefox", "-P", "arkenfox" };
static const char *browser2[]  = { "/usr/bin/firefox", "-P", "default-release" };

static Key keys[] = {
        /* modifier                     key        function        argument */
        /* thinkpad things */
    { 0,                            XF86XK_AudioMute,spawn,          {.v = (const char*[]){"pulsemixer", "--toggle-mute", NULL } } },
    //{ 0,                            XK_mute,        spawn,          {.v = (const char*[]){"pulsemixer", "--toggle-mute", NULL } } },
    { 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = (const char*[]){"pulsemixer", "--change-volume", "-2", NULL } } },
    { 0,                            XF86XK_AudioRaiseVolume,    spawn,          {.v = (const char*[]){"pulsemixer", "--change-volume", "+2", NULL } } },
    { 0,                            XF86XK_AudioMicMute,     spawn,          {.v = (const char*[]){"pulsemixer", "--id", "source-1", "--toggle-mute", NULL } } },
    { 0,                            XF86XK_MonBrightnessDown,  spawn,          {.v = (const char*[]){"brightnessctl" , "s", "50-", NULL } } },
    { 0,                            XF86XK_MonBrightnessUp,    spawn,          {.v = (const char*[]){"brightnessctl" , "s", "50+", NULL } } },

    /* espeak clipboard */
        { MODKEY,                       XK_e,           spawn,          {.v = (const char*[]){"read_clipboard", NULL } } },
        { MODKEY|ShiftMask,             XK_e,           spawn,          {.v = (const char*[]){"stop_read_clipboard", NULL } } },

        /* other things */
        { MODKEY|ControlMask|ShiftMask, XK_s,           spawn,          SHCMD("screenshotOptions") },
        { MODKEY,                       XK_o,           spawn,          {.v = (const char*[]){ TERMINAL, "-e", "lfub", NULL } } },
        { MODKEY|ShiftMask,             XK_h,           spawn,          {.v = (const char*[]){ TERMINAL, "-e", "htop", NULL } } },
        { MODKEY,                       XK_a,           spawn,          {.v = (const char*[]){ TERMINAL, "-e", "pulsemixer", NULL } } },
        { MODKEY,                       XK_r,           spawn,          {.v = dmenucmd } },
        { MODKEY|ShiftMask,             XK_r,           spawn,          {.v = sudo_dmenucmd } },

        { MODKEY,                       XK_x,           spawn,          {.v = termcmd } },
        { MODKEY|ShiftMask,             XK_x,           spawn,          SHCMD("sd") },

        { MODKEY,                       XK_w,           spawn,          {.v = browser } },
        { MODKEY|ShiftMask,             XK_w,           spawn,          SHCMD("notify-send \"STOP\"") },
        { MODKEY|ControlMask,           XK_w,           spawn,          {.v = browser2 } },
        { MODKEY,                       XK_j,           focusstack,     {.i = +1 } },
        { MODKEY,                       XK_k,           focusstack,     {.i = -1 } },
        { MODKEY,                       XK_i,           incnmaster,     {.i = +1 } },
        { MODKEY|ShiftMask,             XK_i,           incnmaster,     {.i = -1 } },
        { MODKEY,                       XK_h,           setmfact,       {.f = -0.05} },
        { MODKEY,                       XK_l,           setmfact,       {.f = +0.05} },
        { MODKEY|ShiftMask,             XK_o,           toggleopacity,  {0} },
        { MODKEY,                       XK_Return,      zoom,           {0} },

        { MODKEY,                       XK_b,           togglebar,      {0} },
        { MODKEY|ShiftMask,             XK_b,           togglegaps,     {0} },
        { MODKEY|ControlMask,           XK_b,           defaultgaps,    {0} },
        { MODKEY|ShiftMask|ControlMask, XK_b,           spawn,          SHCMD("setbg") },

        { MODKEY|ShiftMask,             XK_q,           killclient,     {0} },

        { MODKEY,                       XK_v,           setlayout,      {.v = &layouts[0]} },
        { MODKEY|ShiftMask,             XK_v,           setlayout,      {.v = &layouts[5]} },
        { MODKEY|ControlMask,           XK_v,           setlayout,      {.v = &layouts[6]} },

        { MODKEY|ShiftMask,             XK_f,           setlayout,      {.v = &layouts[13]} },
        { MODKEY,                       XK_f,           setlayout,      {.v = &layouts[1]} },

        { MODKEY,                       XK_s,           setlayout,      {.v = &layouts[2]} },
        { MODKEY|ShiftMask,             XK_s,           setlayout,      {.v = &layouts[3]} },

        { MODKEY,                       XK_g,           setlayout,      {.v = &layouts[10]} },
        { MODKEY|ShiftMask,             XK_g,           setlayout,      {.v = &layouts[7]} },
        { MODKEY|ControlMask,           XK_g,           setlayout,      {.v = &layouts[9]} },

        { MODKEY,                       XK_m,           spawn,          SHCMD("mounter") },
        { MODKEY|ShiftMask,             XK_m,           spawn,          SHCMD("unmounter") },
        { MODKEY|ControlMask,           XK_m,           setlayout,      {.v = &layouts[11]} },
        { MODKEY|ShiftMask|ControlMask, XK_m,           setlayout,      {.v = &layouts[12]} },

        { MODKEY,                       XK_d,           spawn,          SHCMD("displayselect") },
        { MODKEY|ShiftMask,             XK_d,           spawn,          SHCMD("fixdisplay") },

        { MODKEY|ShiftMask,             XK_space,       setlayout,      {0} },
        { MODKEY,                       XK_space,       togglefloating, {0} },
        { MODKEY,                       XK_0,           view,           {.ui = ~0 } },
        { MODKEY|ShiftMask,             XK_0,           tag,            {.ui = ~0 } },
        { MODKEY,                       XK_comma,       focusmon,       {.i = -1 } },
        { MODKEY,                       XK_period,      focusmon,       {.i = +1 } },
        { MODKEY|ShiftMask,             XK_comma,       tagmon,         {.i = -1 } },
        { MODKEY|ShiftMask,             XK_period,      tagmon,         {.i = +1 } },
        TAGKEYS(                        XK_1,                           0)
        TAGKEYS(                        XK_2,                           1)
        TAGKEYS(                        XK_3,                           2)
        TAGKEYS(                        XK_4,                           3)
        TAGKEYS(                        XK_5,                           4)
        TAGKEYS(                        XK_6,                           5)
        TAGKEYS(                        XK_7,                           6)
        TAGKEYS(                        XK_8,                           7)
        TAGKEYS(                        XK_9,                           8)
        { MODKEY|ShiftMask,             XK_p,           spawn,          SHCMD("dmenuPowerOptions") },
        { MODKEY|ControlMask|ShiftMask, XK_r,           quit,           {0} },

    /*Password things*/
        { MODKEY|ControlMask,           XK_p,           spawn,          SHCMD("dmenupass") },
        { MODKEY|ControlMask|ShiftMask, XK_p,           spawn,          {.v = (const char*[]){"gpg-connect-agent", "reloadagent", "/bye", NULL } } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
        /* click                event mask      button          function        argument */
        { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
        { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
        { ClkWinTitle,          0,              Button2,        zoom,           {0} },
        { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
        { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
        { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
        { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
        { ClkTagBar,            0,              Button1,        view,           {0} },
        { ClkTagBar,            0,              Button3,        toggleview,     {0} },
        { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
        { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

