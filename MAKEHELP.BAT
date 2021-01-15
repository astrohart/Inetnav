@echo off
REM -- First make map file from App Studio generated resource.h
echo // MAKEHELP.BAT generated Help Map file.  Used by INETNAV.HPJ. >hlp\inetnav.hm
echo. >>hlp\inetnav.hm
echo // Commands (ID_* and IDM_*) >>hlp\inetnav.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >>hlp\inetnav.hm
echo. >>hlp\inetnav.hm
echo // Prompts (IDP_*) >>hlp\inetnav.hm
makehm IDP_,HIDP_,0x30000 resource.h >>hlp\inetnav.hm
echo. >>hlp\inetnav.hm
echo // Resources (IDR_*) >>hlp\inetnav.hm
makehm IDR_,HIDR_,0x20000 resource.h >>hlp\inetnav.hm
echo. >>hlp\inetnav.hm
echo // Dialogs (IDD_*) >>hlp\inetnav.hm
makehm IDD_,HIDD_,0x20000 resource.h >>hlp\inetnav.hm
echo. >>hlp\inetnav.hm
echo // Frame Controls (IDW_*) >>hlp\inetnav.hm
makehm IDW_,HIDW_,0x50000 resource.h >>hlp\inetnav.hm
REM -- Make help for Project INETNAV
call hc31 inetnav.hpj
echo.
