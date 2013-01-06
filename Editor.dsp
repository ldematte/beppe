# Microsoft Developer Studio Project File - Name="Editor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Editor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Editor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Editor.mak" CFG="Editor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Editor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Editor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Editor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O1 /I "include\fox" /I "include\ftgl" /I "include\freetype" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "HAVE_OPENGL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "NDEBUG"
# ADD RSC /l 0x410 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib comctl32.lib shell32.lib advapi32.lib opengl32.lib glu32.lib fox.lib ws2_32.lib ftgl.lib freetype206MT.lib winmm.lib /nologo /entry:"mainCRTStartup" /subsystem:windows /machine:I386 /nodefaultlib:"libc" /out:"Editor.exe" /libpath:"lib\win32"

!ELSEIF  "$(CFG)" == "Editor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "include\fox" /I "include\ftgl" /I "include\freetype" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "HAVE_OPENGL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x410 /d "_DEBUG"
# ADD RSC /l 0x410 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib comctl32.lib shell32.lib advapi32.lib opengl32.lib glu32.lib fox.lib ws2_32.lib ftgl.lib freetype206MT.lib winmm.lib /nologo /entry:"mainCRTStartup" /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc" /out:"EditorD.exe" /pdbtype:sept /libpath:"lib\win32"

!ENDIF 

# Begin Target

# Name "Editor - Win32 Release"
# Name "Editor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\AboutWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Bezier.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Camera.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Editor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\EditorBezierPatch.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FogInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\FogMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Image.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ImageTGA.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Logger.cpp
# End Source File
# Begin Source File

SOURCE=.\src\LogWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MaterialInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MaterialMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\MatWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Mtxlib.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PatchMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Picker.cpp
# End Source File
# Begin Source File

SOURCE=.\src\RenderedBezier.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TexCached.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TexResource.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TextureMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TexWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Trackball.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Viewport.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Viewport2d.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Viewport3d.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\AboutWindow.h
# End Source File
# Begin Source File

SOURCE=.\src\Bezier.h
# End Source File
# Begin Source File

SOURCE=.\src\Camera.h
# End Source File
# Begin Source File

SOURCE=.\src\common.h
# End Source File
# Begin Source File

SOURCE=.\src\Editor.h
# End Source File
# Begin Source File

SOURCE=.\src\FogInfo.h
# End Source File
# Begin Source File

SOURCE=.\src\FogMgr.h
# End Source File
# Begin Source File

SOURCE=.\src\Icons.h
# End Source File
# Begin Source File

SOURCE=.\src\Image.h
# End Source File
# Begin Source File

SOURCE=.\src\ImageTGA.h
# End Source File
# Begin Source File

SOURCE=.\src\Logger.h
# End Source File
# Begin Source File

SOURCE=.\src\LogWindow.h
# End Source File
# Begin Source File

SOURCE=.\src\MatWindow.h
# End Source File
# Begin Source File

SOURCE=.\src\Mtxlib.h
# End Source File
# Begin Source File

SOURCE=.\src\PatchMgr.h
# End Source File
# Begin Source File

SOURCE=.\src\Picker.h
# End Source File
# Begin Source File

SOURCE=.\src\RenderedBezier.h
# End Source File
# Begin Source File

SOURCE=.\src\TexCached.h
# End Source File
# Begin Source File

SOURCE=.\src\TexResource.h
# End Source File
# Begin Source File

SOURCE=.\src\TextureMgr.h
# End Source File
# Begin Source File

SOURCE=.\src\TexWindow.h
# End Source File
# Begin Source File

SOURCE=.\src\Trackball.h
# End Source File
# Begin Source File

SOURCE=.\src\Utils.h
# End Source File
# Begin Source File

SOURCE=.\src\Viewport.h
# End Source File
# Begin Source File

SOURCE=.\src\Viewport2d.h
# End Source File
# Begin Source File

SOURCE=.\src\Viewport3d.h
# End Source File
# Begin Source File

SOURCE=.\src\zList.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
