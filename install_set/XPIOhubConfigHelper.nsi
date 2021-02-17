;NSIS Modern User Interface
;NSIS Modern User Interface

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"

;--------------------------------XPIOHubConfigHelper-0.3.0
;General

  ;Name and file
  Name "XPIOHubConfigHelper-0.3.0"
  OutFile "XPIOHubConfigHelper-0.3.0.exe"

  ;Default installation folder
  InstallDir "C:\Program Files (x86)\XPIOHubConfigHelper"
  
 

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Variables

  Var StartMenuFolder

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

;  !insertmacro MUI_PAGE_LICENSE "${NSISDIR}\Docs\Modern UI\License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  
  ;Start Menu Folder Page Configuration
  
  !insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
  
  !insertmacro MUI_PAGE_INSTFILES
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
;Installer Sections

Section "XPIOHubConfigHelper" SecDummy

  SetOutPath "$INSTDIR"
  
  ;ADD YOUR OWN FILES HERE... 


file cfghelper.exe
file wxmsw30u_core_gcc510TDM.dll
file wxbase30u_gcc510TDM.dll
file wxmsw30u_adv_gcc510TDM.dll   
  ;Store installation folder
  
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\Uninstall.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
    
    ;Create shortcuts
    CreateDirectory "$SMPROGRAMS\$StartMenuFolder"
    CreateShortCut "$SMPROGRAMS\$StartMenuFolder\XPIO Hub Config Helper.lnk" "$INSTDIR\cfghelper.exe"
  
  !insertmacro MUI_STARTMENU_WRITE_END

sectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecDummy ${LANG_ENGLISH} "XPIOHub config helper"
  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDummy} $(DESC_SecDummy)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END
 
;--------------------------------
;Uninstaller Section

Section "Uninstall"

  ;ADD YOUR OWN FILES HERE...


Delete "$INSTDIR\cfghelper.exe"
Delete "$INSTDIR\wxmsw30u_core_gcc510TDM.dll"
Delete "$INSTDIR\wxbase30u_gcc510TDM.dll"
Delete "$INSTDIR\wxmsw30u_adv_gcc510TDM.dll"

  RMDir "$INSTDIR"
  
  !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
    
  Delete "$SMPROGRAMS\$StartMenuFolder\Uninstall.lnk"
  Delete "$SMPROGRAMS\$StartMenuFolder\XPIO Hub Config Helper.lnk"
  RMDir "$SMPROGRAMS\$StartMenuFolder"
  


SectionEnd