# Microsoft Visual C++ generated build script - Do not modify

PROJ = INETNAV
DEBUG = 0
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG 
R_RCDEFINES = /dNDEBUG 
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = D:\APPS\INTERNET\INETNAV\
USEMFC = 1
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = /YcSTDAFX.H
CUSEPCHFLAG = 
CPPUSEPCHFLAG = /YuSTDAFX.H
FIRSTC =             
FIRSTCPP = STDAFX.CPP  
RC = rc
CFLAGS_D_WEXE = /nologo /f /G2 /Z7 /AL /Od /D "_DEBUG" /U "NDEBUG" /FR /GA 
CFLAGS_R_WEXE = /nologo /f /G2 /AL /O2 /Ob2 /D "NDEBUG" /U "_DEBUG" /Fr /GA 
LFLAGS_D_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:64 /ONERROR:NOEXE /CO  
LFLAGS_R_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:20240 /ALIGN:16 /ONERROR:NOEXE  
LIBS_D_WEXE = lafxcwd oldnames libw llibcew ctl3dv2 winsock commdlg.lib ddeml.lib shell.lib ver.lib 
LIBS_R_WEXE = lafxcw oldnames libw llibcew ctl3dv2 winsock commdlg.lib ddeml.lib shell.lib 
RCFLAGS = /nologo 
RESFLAGS = /nologo /t /k
RUNFLAGS = 
DEFFILE = INETNAV.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = STDAFX.SBR \
		DIALOGS.SBR \
		HELPINFO.SBR \
		INETNAV.SBR \
		MAINFRM.SBR \
		MAINMENU.SBR \
		PERCENT.SBR \
		PREFSDOC.SBR \
		PREFVIEW.SBR \
		PROPSHTS.SBR \
		SOCKET.SBR \
		SPLASH.SBR \
		STATBAR.SBR \
		TIPOFDAY.SBR \
		TOOLTIPB.SBR \
		WHOISDOC.SBR \
		WHOISVW.SBR \
		WHOISWND.SBR \
		WIZARDS.SBR \
		APPD.SBR \
		FINGER.SBR \
		FTP.SBR \
		GOPHER.SBR \
		MAIL.SBR \
		TEXT.SBR \
		IRCDOC.SBR \
		IRCVIEW.SBR \
		IRCWND.SBR \
		NEWSDOC.SBR \
		NEWSVIEW.SBR \
		NEWSWND.SBR \
		MAILWND.SBR \
		ASKSTAFF.SBR


STDAFX_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h


DIALOGS_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\appd.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\ftp.h


HELPINFO_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\helpinfo.h


INETNAV_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\propshts.h \
	d:\apps\internet\inetnav\finger.h \
	d:\apps\internet\inetnav\ftp.h \
	d:\apps\internet\inetnav\text.h \
	d:\apps\internet\inetnav\ircdoc.h \
	d:\apps\internet\inetnav\ircview.h \
	d:\apps\internet\inetnav\ircwnd.h \
	d:\apps\internet\inetnav\whoisdoc.h \
	d:\apps\internet\inetnav\whoiswnd.h \
	d:\apps\internet\inetnav\whoisvw.h \
	d:\apps\internet\inetnav\mainmenu.h \
	d:\apps\internet\inetnav\helpinfo.h \
	d:\apps\internet\inetnav\prefview.h \
	d:\apps\internet\inetnav\prefsdoc.h \
	d:\apps\internet\inetnav\tipofday.h \
	d:\apps\internet\inetnav\askstaff.h


MAINFRM_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\ircerrs.h \
	d:\apps\internet\inetnav\ircview.h \
	d:\apps\internet\inetnav\ircdoc.h \
	d:\apps\internet\inetnav\ircwnd.h


MAINMENU_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\mainmenu.h


PERCENT_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\ftp.h


PREFSDOC_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\prefsdoc.h \
	d:\apps\internet\inetnav\prefview.h


PREFVIEW_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\prefsdoc.h \
	d:\apps\internet\inetnav\prefview.h \
	d:\apps\internet\inetnav\propshts.h


PROPSHTS_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\propshts.h


SOCKET_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h


SPLASH_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h


STATBAR_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h


TIPOFDAY_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\tipofday.h


TOOLTIPB_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h


WHOISDOC_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\whoisdoc.h


WHOISVW_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\whoisdoc.h \
	d:\apps\internet\inetnav\whoisvw.h \
	d:\apps\internet\inetnav\percent.h


WHOISWND_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\whoisdoc.h \
	d:\apps\internet\inetnav\whoiswnd.h \
	d:\apps\internet\inetnav\whoisvw.h \
	d:\apps\internet\inetnav\percent.h


WIZARDS_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\wizards.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h


INETNAV_RCDEP = d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\res\inetnav.ico \
	d:\apps\internet\inetnav\res\fingedoc.ico \
	d:\apps\internet\inetnav\res\ftpdoc.ico \
	d:\apps\internet\inetnav\res\ircdoc.ico \
	d:\apps\internet\inetnav\res\gophdoc.ico \
	d:\apps\internet\inetnav\res\outdoc.ico \
	d:\apps\internet\inetnav\res\aol.ico \
	d:\apps\internet\inetnav\res\mainmenu.ico \
	d:\apps\internet\inetnav\res\helpinfo.ico \
	d:\apps\internet\inetnav\res\prefs.ico \
	d:\apps\internet\inetnav\res\maildoc.ico \
	d:\apps\internet\inetnav\res\newmail.ico \
	d:\apps\internet\inetnav\res\message.ico \
	d:\apps\internet\inetnav\res\nwsarea.ico \
	d:\apps\internet\inetnav\res\mailcomp.ico \
	d:\apps\internet\inetnav\res\textdoc.ico \
	d:\apps\internet\inetnav\res\tipicon.ico \
	d:\apps\internet\inetnav\res\news.ico \
	d:\apps\internet\inetnav\res\artsel.ico \
	d:\apps\internet\inetnav\res\winsock.ico \
	d:\apps\internet\inetnav\res\chol.ico \
	d:\apps\internet\inetnav\res\article.ico \
	d:\apps\internet\inetnav\res\addrbook.ico \
	d:\apps\internet\inetnav\res\earth.ico \
	d:\apps\internet\inetnav\res\newscomp.ico \
	d:\apps\internet\inetnav\res\world.ico \
	d:\apps\internet\inetnav\res\intrfile.ico \
	d:\apps\internet\inetnav\res\newgroup.ico \
	d:\apps\internet\inetnav\res\intrfold.ico \
	d:\apps\internet\inetnav\res\whoisdoc.ico \
	d:\apps\internet\inetnav\res\gopher.ico \
	d:\apps\internet\inetnav\res\helpbook.ico \
	d:\apps\internet\inetnav\res\askstaff.ico \
	d:\apps\internet\inetnav\res\psscroll.bmp \
	d:\apps\internet\inetnav\res\pofficeu.bmp \
	d:\apps\internet\inetnav\res\pofficef.bmp \
	d:\apps\internet\inetnav\res\pofficed.bmp \
	d:\apps\internet\inetnav\res\fingeru.bmp \
	d:\apps\internet\inetnav\res\fingerf.bmp \
	d:\apps\internet\inetnav\res\fingerd.bmp \
	d:\apps\internet\inetnav\res\ftpu.bmp \
	d:\apps\internet\inetnav\res\whoisu.bmp \
	d:\apps\internet\inetnav\res\chatu.bmp \
	d:\apps\internet\inetnav\res\newsu.bmp \
	d:\apps\internet\inetnav\res\gopheru.bmp \
	d:\apps\internet\inetnav\res\ftpd.bmp \
	d:\apps\internet\inetnav\res\chatd.bmp \
	d:\apps\internet\inetnav\res\newsd.bmp \
	d:\apps\internet\inetnav\res\gopherd.bmp \
	d:\apps\internet\inetnav\res\prefsd.bmp \
	d:\apps\internet\inetnav\res\prefsu.bmp \
	d:\apps\internet\inetnav\res\ftpf.bmp \
	d:\apps\internet\inetnav\res\chatf.bmp \
	d:\apps\internet\inetnav\res\newsf.bmp \
	d:\apps\internet\inetnav\res\gopherf.bmp \
	d:\apps\internet\inetnav\res\prefsf.bmp \
	d:\apps\internet\inetnav\res\whoisd.bmp \
	d:\apps\internet\inetnav\res\whoisf.bmp \
	d:\apps\internet\inetnav\res\btntri.bmp \
	d:\apps\internet\inetnav\res\chatx.bmp \
	d:\apps\internet\inetnav\res\blankx.bmp \
	d:\apps\internet\inetnav\res\fingerx.bmp \
	d:\apps\internet\inetnav\res\ftpx.bmp \
	d:\apps\internet\inetnav\res\gopherx.bmp \
	d:\apps\internet\inetnav\res\newsx.bmp \
	d:\apps\internet\inetnav\res\prefsx.bmp \
	d:\apps\internet\inetnav\res\pofficex.bmp \
	d:\apps\internet\inetnav\res\whoisx.bmp \
	d:\apps\internet\inetnav\folder.bmp \
	d:\apps\internet\inetnav\res\file.bmp \
	d:\apps\internet\inetnav\res\link.bmp \
	d:\apps\internet\inetnav\res\searchu.bmp \
	d:\apps\internet\inetnav\res\searchd.bmp \
	d:\apps\internet\inetnav\res\searchf.bmp \
	d:\apps\internet\inetnav\res\searchx.bmp \
	d:\apps\internet\inetnav\res\helpf.bmp \
	d:\apps\internet\inetnav\res\helpu.bmp \
	d:\apps\internet\inetnav\res\helpd.bmp \
	d:\apps\internet\inetnav\res\helpx.bmp \
	d:\apps\internet\inetnav\res\helpinfd.bmp \
	d:\apps\internet\inetnav\res\helpinff.bmp \
	d:\apps\internet\inetnav\res\helpinfu.bmp \
	d:\apps\internet\inetnav\res\helpinfx.bmp \
	d:\apps\internet\inetnav\res\ftpdoc.bmp \
	d:\apps\internet\inetnav\res\chat.bmp \
	d:\apps\internet\inetnav\res\blank.bmp \
	d:\apps\internet\inetnav\res\search.bmp \
	d:\apps\internet\inetnav\res\open.bmp \
	d:\apps\internet\inetnav\res\miniinfo.bmp \
	d:\apps\internet\inetnav\res\alert.bmp \
	d:\apps\internet\inetnav\res\intfold.bmp \
	d:\apps\internet\inetnav\res\intfile.bmp \
	d:\apps\internet\inetnav\res\amerol.bmp \
	d:\apps\internet\inetnav\res\cis.bmp \
	d:\apps\internet\inetnav\res\prodigy.bmp \
	d:\apps\internet\inetnav\res\gnn.bmp \
	d:\apps\internet\inetnav\res\minieart.bmp \
	d:\apps\internet\inetnav\res\grpmand.bmp \
	d:\apps\internet\inetnav\res\grpmanf.bmp \
	d:\apps\internet\inetnav\res\grpmanu.bmp \
	d:\apps\internet\inetnav\res\grpmanx.bmp \
	d:\apps\internet\inetnav\res\maddru.bmp \
	d:\apps\internet\inetnav\res\maddrf.bmp \
	d:\apps\internet\inetnav\res\maddrd.bmp \
	d:\apps\internet\inetnav\res\maddrx.bmp \
	d:\apps\internet\inetnav\hlp\world.bmp \
	d:\apps\internet\inetnav\res\gophdefx.bmp \
	d:\apps\internet\inetnav\res\gophdefu.bmp \
	d:\apps\internet\inetnav\res\gophdeff.bmp \
	d:\apps\internet\inetnav\res\gophdefd.bmp \
	d:\apps\internet\inetnav\res\backx.bmp \
	d:\apps\internet\inetnav\res\backu.bmp \
	d:\apps\internet\inetnav\res\backf.bmp \
	d:\apps\internet\inetnav\res\backd.bmp \
	d:\apps\internet\inetnav\res\toolbar.bmp \
	d:\apps\internet\inetnav\res\mailfile.bmp \
	d:\apps\internet\inetnav\res\askstaf.bmp \
	d:\apps\internet\inetnav\res\askstad.bmp \
	d:\apps\internet\inetnav\res\askstau.bmp \
	d:\apps\internet\inetnav\res\askstax.bmp \
	d:\apps\internet\inetnav\res\askstaff.bmp \
	d:\apps\internet\inetnav\res\askstafd.bmp \
	d:\apps\internet\inetnav\res\askstafu.bmp \
	d:\apps\internet\inetnav\res\inetnav.rc2


APPD_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\appd.h


FINGER_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\finger.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h


FTP_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\ftp.h \
	d:\apps\internet\inetnav\propshts.h \
	d:\apps\internet\inetnav\appd.h


GOPHER_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\propshts.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h


MAIL_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\wizards.h \
	d:\apps\internet\inetnav\propshts.h


TEXT_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\text.h \
	d:\apps\internet\inetnav\propshts.h


IRCDOC_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\ircdoc.h \
	d:\apps\internet\inetnav\ircview.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h


IRCVIEW_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\text.h \
	d:\apps\internet\inetnav\ircview.h \
	d:\apps\internet\inetnav\ircdoc.h \
	d:\apps\internet\inetnav\propshts.h


IRCWND_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\ircwnd.h \
	d:\apps\internet\inetnav\ircview.h


NEWSDOC_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\propshts.h \
	d:\apps\internet\inetnav\wizards.h


NEWSVIEW_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\propshts.h \
	d:\apps\internet\inetnav\wizards.h


NEWSWND_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\propshts.h


MAILWND_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\dialogs.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\wizards.h \
	d:\apps\internet\inetnav\propshts.h


ASKSTAFF_DEP = d:\apps\internet\inetnav\stdafx.h \
	d:\apps\internet\inetnav\waitcur.h \
	d:\msvc\include\ctl3d.h \
	d:\apps\internet\inetnav\socket.h \
	d:\msvc\include\winsock.h \
	d:\apps\internet\inetnav\inetnav.h \
	d:\apps\internet\inetnav\splash.h \
	d:\apps\internet\inetnav\mainfrm.h \
	d:\apps\internet\inetnav\statbar.h \
	d:\apps\internet\inetnav\tooltipb.h \
	d:\apps\internet\inetnav\mail.h \
	d:\apps\internet\inetnav\percent.h \
	d:\apps\internet\inetnav\news.h \
	d:\apps\internet\inetnav\gopher.h \
	d:\apps\internet\inetnav\askstaff.h


all:	$(PROJ).EXE $(PROJ).BSC

STDAFX.OBJ:	STDAFX.CPP $(STDAFX_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c STDAFX.CPP

DIALOGS.OBJ:	DIALOGS.CPP $(DIALOGS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DIALOGS.CPP

HELPINFO.OBJ:	HELPINFO.CPP $(HELPINFO_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c HELPINFO.CPP

INETNAV.OBJ:	INETNAV.CPP $(INETNAV_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c INETNAV.CPP

MAINFRM.OBJ:	MAINFRM.CPP $(MAINFRM_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MAINFRM.CPP

MAINMENU.OBJ:	MAINMENU.CPP $(MAINMENU_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MAINMENU.CPP

PERCENT.OBJ:	PERCENT.CPP $(PERCENT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PERCENT.CPP

PREFSDOC.OBJ:	PREFSDOC.CPP $(PREFSDOC_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PREFSDOC.CPP

PREFVIEW.OBJ:	PREFVIEW.CPP $(PREFVIEW_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PREFVIEW.CPP

PROPSHTS.OBJ:	PROPSHTS.CPP $(PROPSHTS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PROPSHTS.CPP

SOCKET.OBJ:	SOCKET.CPP $(SOCKET_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c SOCKET.CPP

SPLASH.OBJ:	SPLASH.CPP $(SPLASH_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c SPLASH.CPP

STATBAR.OBJ:	STATBAR.CPP $(STATBAR_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c STATBAR.CPP

TIPOFDAY.OBJ:	TIPOFDAY.CPP $(TIPOFDAY_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TIPOFDAY.CPP

TOOLTIPB.OBJ:	TOOLTIPB.CPP $(TOOLTIPB_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TOOLTIPB.CPP

WHOISDOC.OBJ:	WHOISDOC.CPP $(WHOISDOC_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c WHOISDOC.CPP

WHOISVW.OBJ:	WHOISVW.CPP $(WHOISVW_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c WHOISVW.CPP

WHOISWND.OBJ:	WHOISWND.CPP $(WHOISWND_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c WHOISWND.CPP

WIZARDS.OBJ:	WIZARDS.CPP $(WIZARDS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c WIZARDS.CPP

INETNAV.RES:	INETNAV.RC $(INETNAV_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r INETNAV.RC

APPD.OBJ:	APPD.CPP $(APPD_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c APPD.CPP

FINGER.OBJ:	FINGER.CPP $(FINGER_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FINGER.CPP

FTP.OBJ:	FTP.CPP $(FTP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FTP.CPP

GOPHER.OBJ:	GOPHER.CPP $(GOPHER_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c GOPHER.CPP

MAIL.OBJ:	MAIL.CPP $(MAIL_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MAIL.CPP

TEXT.OBJ:	TEXT.CPP $(TEXT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TEXT.CPP

IRCDOC.OBJ:	IRCDOC.CPP $(IRCDOC_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c IRCDOC.CPP

IRCVIEW.OBJ:	IRCVIEW.CPP $(IRCVIEW_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c IRCVIEW.CPP

IRCWND.OBJ:	IRCWND.CPP $(IRCWND_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c IRCWND.CPP

NEWSDOC.OBJ:	NEWSDOC.CPP $(NEWSDOC_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c NEWSDOC.CPP

NEWSVIEW.OBJ:	NEWSVIEW.CPP $(NEWSVIEW_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c NEWSVIEW.CPP

NEWSWND.OBJ:	NEWSWND.CPP $(NEWSWND_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c NEWSWND.CPP

MAILWND.OBJ:	MAILWND.CPP $(MAILWND_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MAILWND.CPP

ASKSTAFF.OBJ:	ASKSTAFF.CPP $(ASKSTAFF_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ASKSTAFF.CPP


$(PROJ).EXE::	INETNAV.RES

$(PROJ).EXE::	STDAFX.OBJ DIALOGS.OBJ HELPINFO.OBJ INETNAV.OBJ MAINFRM.OBJ MAINMENU.OBJ \
	PERCENT.OBJ PREFSDOC.OBJ PREFVIEW.OBJ PROPSHTS.OBJ SOCKET.OBJ SPLASH.OBJ STATBAR.OBJ \
	TIPOFDAY.OBJ TOOLTIPB.OBJ WHOISDOC.OBJ WHOISVW.OBJ WHOISWND.OBJ WIZARDS.OBJ APPD.OBJ \
	FINGER.OBJ FTP.OBJ GOPHER.OBJ MAIL.OBJ TEXT.OBJ IRCDOC.OBJ IRCVIEW.OBJ IRCWND.OBJ \
	NEWSDOC.OBJ NEWSVIEW.OBJ NEWSWND.OBJ MAILWND.OBJ ASKSTAFF.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
STDAFX.OBJ +
DIALOGS.OBJ +
HELPINFO.OBJ +
INETNAV.OBJ +
MAINFRM.OBJ +
MAINMENU.OBJ +
PERCENT.OBJ +
PREFSDOC.OBJ +
PREFVIEW.OBJ +
PROPSHTS.OBJ +
SOCKET.OBJ +
SPLASH.OBJ +
STATBAR.OBJ +
TIPOFDAY.OBJ +
TOOLTIPB.OBJ +
WHOISDOC.OBJ +
WHOISVW.OBJ +
WHOISWND.OBJ +
WIZARDS.OBJ +
APPD.OBJ +
FINGER.OBJ +
FTP.OBJ +
GOPHER.OBJ +
MAIL.OBJ +
TEXT.OBJ +
IRCDOC.OBJ +
IRCVIEW.OBJ +
IRCWND.OBJ +
NEWSDOC.OBJ +
NEWSVIEW.OBJ +
NEWSWND.OBJ +
MAILWND.OBJ +
ASKSTAFF.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
D:\MSVC\LIB\+
D:\MSVC\MFC\LIB\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) INETNAV.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	INETNAV.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) INETNAV.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
