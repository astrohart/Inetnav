@ECHO OFF
ECHO Welcome to the MAKEHMAP program!
ECHO Making the Internet Navigator help map file... Please wait.
REM -- First make map file from App Studio generated resource.h
echo // MAKEHMAP.BAT generated Help Map file.  Used by INETNAV.HPJ. >inetnav.hm
echo. >>inetnav.hm
ECHO Now building Commands section...  Please wait.
ECHO.
echo // Commands (ID_* and IDM_*) >>inetnav.hm
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 c:\apps\internet\inetnav\resource.h >>inetnav.hm
echo. >>inetnav.hm
ECHO Now building Prompts section...  Please wait.
ECHO.
echo // Prompts (IDP_*) >>inetnav.hm
makehm IDP_,HIDP_,0x30000 c:\apps\internet\inetnav\resource.h >>inetnav.hm
echo. >>inetnav.hm
ECHO Now building the Resources section...  Please wait.
ECHO.
echo // Resources (IDR_*) >>inetnav.hm
makehm IDR_,HIDR_,0x20000 c:\apps\internet\inetnav\resource.h >>inetnav.hm
echo. >>inetnav.hm
ECHO Now building the Dialogs section...  Please wait.
ECHO.
echo // Dialogs (IDD_*) >>inetnav.hm
makehm IDD_,HIDD_,0x20000 c:\apps\internet\inetnav\resource.h >>inetnav.hm
echo. >>inetnav.hm
ECHO Now building the Frame Controls section...  Please wait.
ECHO.
echo // Frame Controls (IDW_*) >>inetnav.hm
makehm IDW_,HIDW_,0x50000 c:\apps\internet\inetnav\resource.h >>inetnav.hm
ECHO Help map file build is complete!  Thank you for your patience!
ECHO Help map filename: C:\APPS\INTERNET\INETNAV\INETNAV.HM
ECHO.
ECHO Thank you for using the MAKEHMAP program!
ECHO.
