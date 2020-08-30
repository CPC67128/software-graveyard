# Microsoft Developer Studio Project File - Name="Serveur WRC" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Serveur WRC - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Serveur WRC.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Serveur WRC.mak" CFG="Serveur WRC - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Serveur WRC - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Serveur WRC - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Serveur WRC - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Serveur WRC - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "C:\TEMP\Serveur"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"C:\TEMP\Serveur\Serveur WRC.bsc"
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /pdb:"C:\TEMP\Serveur\Serveur WRC.pdb" /debug /machine:I386 /nodefaultlib:"libc" /pdbtype:sept /libpath:"jpeg-6b\Release"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Serveur WRC - Win32 Release"
# Name "Serveur WRC - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\envoi_ecran.cpp
# End Source File
# Begin Source File

SOURCE=.\exportation_image.cpp
# End Source File
# Begin Source File

SOURCE=.\jpeg.cpp
# End Source File
# Begin Source File

SOURCE=.\registre.cpp
# End Source File
# Begin Source File

SOURCE=.\rsa.cpp
# End Source File
# Begin Source File

SOURCE=.\serveur.cpp
# End Source File
# Begin Source File

SOURCE=.\sha.cpp
# End Source File
# Begin Source File

SOURCE=.\socket_serveur.cpp
# End Source File
# Begin Source File

SOURCE=.\systray.cpp
# End Source File
# Begin Source File

SOURCE=.\thread_clavier_souris.cpp
# End Source File
# Begin Source File

SOURCE=.\thread_ecran.cpp
# End Source File
# Begin Source File

SOURCE=.\thread_fichier.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\define.h
# End Source File
# Begin Source File

SOURCE=.\include.h
# End Source File
# Begin Source File

SOURCE=.\prototypes.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\types.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\resource.rc
# End Source File
# Begin Source File

SOURCE=.\wrc.ico
# End Source File
# Begin Source File

SOURCE=.\wrc_connected.ico
# End Source File
# Begin Source File

SOURCE=.\wrc_disconnected.ico
# End Source File
# Begin Source File

SOURCE=.\wrc_server_halted.ico
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=.\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=.\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=.\zlib\GZipHelper.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=.\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=.\zlib\maketree.c
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.c
# End Source File
# Begin Source File

SOURCE=.\zlib\zutil.h
# End Source File
# End Group
# Begin Group "jpeg"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\jpeg\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\jpeg\jpeg.lib
# End Source File
# End Group
# End Target
# End Project
