TweakWnd.dll:
	cl /LD user32.lib Toggle.c TweakAlpha.c /o TweakWnd.dll

clean: 
	del *.obj
	del *.dll
	del *.exp
	del *.lib
