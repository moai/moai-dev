To build moaiext-untz:

Visual Studio 2008
1) Install the latest Direct X SDK
2) Click on the menu command Options->Projects and Solutions->VC++ Directories
3) Edit the value of Include files to include C:\Program Files\Microsoft DirectX SDK (June 2010)\Include
4) Edit the value of Library files to include C:\Program Files\Microsoft DirectX SDK (June 2010)\Lib\x86

Visual Studio 2010
1) Install the latest Direct X SDK
2) Click on the menu command View->Other Windows->Property Manager
3) In Property Manager, open box2d->Debug | Win32->Microsoft.Cpp.Win32.user
4) In the resulting pop-up window, click Common Properties->VC++ Directories
5) Edit the value of Include Directories to include C:\Program Files\Microsoft DirectX SDK (June 2010)\Include
6) Edit the value of Library Directories to include C:\Program Files\Microsoft DirectX SDK (June 2010)\Lib\x86