#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx    = 2;        /* border pixel of windows */
static const unsigned int snap        = 32;       /* snap pixel */
static const unsigned int gappx       = 6;        /* pixel gap between clients */
static const int swallowfloating      = 1;        /* 1 means swallow floating windows by default */
static const int showbar              = 1;        /* 0 means no bar */
static const int topbar               = 1;        /* 0 means bottom bar */
static const int horizpadbar          = 4;        /* horizontal padding for statusbar */
static const int vertpadbar           = 4;        /* vertical padding for statusbar */
static const int bargapx              = 6;        /* horizontal gap for statusbar */
static const int bargapy              = 6;        /* vertical gap for statusbar */
static int stupidgaps                 = 0;

/* Mononoki Nerd Font must be installed from AUR nerd-fonts-complete.
 * Otherwise, your default font will be Hack which is found in the standard
 * Arch repos and is listed as a dependency for this build. JoyPixels is also
 * a hard dependency and makes colored fonts and emojis possible.
 */
static const char *fonts[]            = {
	"Source Code Pro Medium:style=Medium,Regular:size=11",
	//"Mononoki Nerd Font:size=11:antialias=true:autohint=true",
	//"Inconsolata:pixelsize=16:lcdfilter=lcddefault:hintstyle=hintnone:rgba=rgb:antialias=true:autohint=false",
                                         "Hack:size=14:antialias=true:autohint=true",
                                         "JoyPixels:size=14:antialias=true:autohint=true",
					 "Noto Color Emoji:style=Regular:size=10",
                                         
										};
static const char col_gray1[]         = "#222222";
static const char col_gray2[]         = "#444444"; /* border color unfocused windows */
static const char col_gray3[]         = "#bbbbbb";
static const char col_gray4[]         = "#eeeeee";
static const char col_cyan[]          = "#00394f"; /* border color focused windows and tags */
/* bar opacity 
 * 0xff is no transparency.
 * 0xee adds wee bit of transparency.
 * Play with the value to get desired transparency.
 */
static const unsigned int baralpha    = 0xee; //0xad; 
static const unsigned int borderalpha = OPAQUE;
static const char *colors[][3]        = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray4, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static const unsigned int alphas[][3] = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
//static const char *tags[] = { "", "", "", "", "", "", "", "", "" };


static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox", NULL,     NULL,           0,         0,          0,          -1,        -1 },
	{ "st",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */


};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "layouts.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "HHH",      grid },
	{ "TTT",      bstack },
	{ NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define CMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]    = { "j4-dmenu-desktop", "--dmenu='/usr/local/bin/dmenu -h 24'", NULL };
/* An alternative way to launch st along with the fish shell */
/* static const char *termcmd[]     = { "st", "-e fish", NULL }; */
static const char *termcmd[]     = { "st", NULL };
static const char *tabtermcmd[]  = { "tabbed", "-r 2", "st", "-w", "''", NULL };

static Key keys[] = {
	/* modifier             key        function        argument */
	{ MODKEY|ShiftMask,     XK_Return, spawn,          CMD("j4-dmenu-desktop --dmenu='/usr/local/bin/dmenu -i -h 24'") },
	{ MODKEY,               XK_Return, spawn,          {.v = termcmd } },
	{ Mod1Mask,             XK_Return, spawn,          {.v = tabtermcmd } },
	{ MODKEY,               XK_b,      togglebar,      {0} },
	{ MODKEY|ControlMask,   XK_g,      togglestupidgaps,{0} },
	{ MODKEY|ShiftMask,     XK_j,      rotatestack,    {.i = +1 } },
	{ MODKEY|ShiftMask,     XK_k,      rotatestack,    {.i = -1 } },
	{ MODKEY,               XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,               XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,               XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,               XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,               XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,               XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ControlMask,   XK_Return, zoom,           {0} },
	{ MODKEY,               XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,     XK_c,      killclient,     {0} },

    /* Layout manipulation */
	//{ MODKEY,               XK_Tab,    cyclelayout,    {.i = -1 } },
	{ MODKEY|ShiftMask,     XK_Tab,    cyclelayout,    {.i = +1 } },
	{ MODKEY,               XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,     XK_space,  togglefloating, {0} },

    /* Switch to specific layouts */
	{ MODKEY,               XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,               XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,               XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,               XK_g,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,               XK_u,      setlayout,      {.v = &layouts[4]} },

	{ MODKEY,               XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,     XK_0,      tag,            {.ui = ~0 } },

    /* Switching between monitors */
	{ MODKEY,               XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,               XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,     XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,     XK_period, tagmon,         {.i = +1 } },

 	{ MODKEY,               XK_n,      spawn,          CMD("sudo networkmanager_dmenu") },
	{ MODKEY,               XK_p,      spawn,          CMD("bash -c 'gopass-dmenu'") },
	{ MODKEY|Mod1Mask,      XK_x,      spawn,          CMD("/home/mark/poweroptions") },
	
    /* Apps Launched with SUPER + ALT + KEY */
	{ MODKEY|Mod1Mask,      XK_h,    	spawn,          CMD("st -e htop") },
	{ MODKEY,         	XK_Escape,    	spawn,          CMD("slock") },

    
	TAGKEYS(                  XK_1,    0)
	TAGKEYS(                  XK_2,    1)
	TAGKEYS(                  XK_3,    2)
	TAGKEYS(                  XK_4,    3)
	TAGKEYS(                  XK_5,    4)
	TAGKEYS(                  XK_6,    5)
	TAGKEYS(                  XK_7,    6)
	TAGKEYS(                  XK_8,    7)
	TAGKEYS(                  XK_9,    8)

	/* logout and reset */
	{ MODKEY|ShiftMask,       XK_q,    quit,    {0} },
    	{ MODKEY|ShiftMask,       XK_r,    quit,    {1} }, 

	/* brigthness control */
	{ 0,   XF86XK_MonBrightnessUp,    spawn,    CMD("sudo brightnessctl set 10%+") },
	{ 0,   XF86XK_MonBrightnessDown,  spawn,    CMD("sudo brightnessctl set 10%-") },
	/* volume control */
	{ 0,   XF86XK_AudioLowerVolume,    spawn,    CMD("amixer -D pulse sset Master 20%-") },
	{ 0,   XF86XK_AudioRaiseVolume,    spawn,    CMD("amixer -D pulse sset Master 20%+") },
	{ 0,   XF86XK_AudioMute,    spawn,    CMD("pactl set-sink-mute 1 toggle") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click           event mask   button          function        argument */
	{ ClkLtSymbol,     0,           Button1,        setlayout,      {0} },
	{ ClkLtSymbol,     0,           Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,     0,           Button2,        zoom,           {0} },
	{ ClkStatusText,   0,           Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,    MODKEY,      Button1,        movemouse,      {0} },
	{ ClkClientWin,    MODKEY,      Button2,        togglefloating, {0} },
	{ ClkClientWin,    MODKEY,      Button3,        resizemouse,    {0} },
	{ ClkTagBar,       0,           Button1,        view,           {0} },
	{ ClkTagBar,       0,           Button3,        toggleview,     {0} },
	{ ClkTagBar,       MODKEY,      Button1,        tag,            {0} },
	{ ClkTagBar,       MODKEY,      Button3,        toggletag,      {0} },
};


