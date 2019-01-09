# TweakWnd
Tweak Window


dllexport:
/*Toggle current GUI process main window full screen*/
ToggleFullScreen(LPSTR lpszChildWndClassName)

/*Set main window transparency value*/
SetAlpha(LONG nTrans)

/* enable or disable main window style WS_CAPTION*/
EnableCaption(LONG bCaption)
EnableMaximize(LONG bEnable)
EnableTopMost(LONG bEnable)

