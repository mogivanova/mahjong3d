[Setup]
AppName=Mahjongg 3D
AppVerName=Mahjongg 3D
AppVersion=1.0.0
MinVersion=4.1.1998, 5.0.2195
AppCopyright=Copyright © 1998-2009 Andrew Ovcharov
AppPublisher=Andrew Ovcharov (andrew.ovcharov@gmail.com)
AppPublisherURL=http://code.google.com/p/mahjong3d/
AppSupportURL=http://code.google.com/p/mahjong3d/
AppUpdatesURL=http://code.google.com/p/mahjong3d/
DefaultDirName={pf}\Mahjong 3D
DefaultGroupName=Mahjongg
OutputDir=..\_Setup\
OutputBaseFilename=mahjong3d_setup
WizardImageFile=Setup\wizard.bmp
WizardSmallImageFile=Setup\wizard_small.bmp
ShowLanguageDialog=auto

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "de"; MessagesFile: "Setup\German.isl"
Name: "fr"; MessagesFile: "Setup\French.isl"
Name: "nl"; MessagesFile: "Setup\Dutch.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Dirs]
Name: "{app}\Background Images"
Name: "{app}\Help"
Name: "{app}\Layouts"
Name: "{app}\Skins"
Name: "{app}\Tile Sets"
Name: "{app}\Languages"
Name: "{userdocs}\My Games\Mahjongg 3D"
Name: "{userdocs}\My Games\Mahjongg 3D\Background Images"
Name: "{userdocs}\My Games\Mahjongg 3D\Layouts"
Name: "{userdocs}\My Games\Mahjongg 3D\Save"
Name: "{userdocs}\My Games\Mahjongg 3D\Skins"
Name: "{userdocs}\My Games\Mahjongg 3D\Tile Sets"
Name: "{userdocs}\My Games\Mahjongg 3D\Languages"

[Files]
; Main files
Source: "..\_bin\Win32_Release\Mahjongg.exe"; DestDir: "{app}"
Source: "Resources\zlib.dll"; DestDir: "{app}"
Source: "Resources\glu.dll"; DestDir: "{app}"
Source: "Resources\opengl.dll"; DestDir: "{app}"
Source: "Resources\readme.txt"; DestDir: "{app}"

; Help files
Source: "Resources\Help\mahjongg.chm"; DestDir: "{app}\Help"; DestName: "mahjongg.chm"
Source: "Resources\Help\mahjongg_de.chm"; DestDir: "{app}\Help"; DestName: "mahjongg_de.chm"
Source: "Resources\Help\mahjongg_fr.chm"; DestDir: "{app}\Help"; DestName: "mahjongg_fr.chm"

; backgrounds
Source: "Resources\Pictures\building2.jpg"; DestDir: "{app}\Background Images"; DestName: "China.jpg"

; tile sets
Source: "Resources\TileSets\egyptian.mjt"; DestDir: "{app}\Tile Sets"
Source: "Resources\TileSets\black.mjt"; DestDir: "{app}\Tile Sets"
Source: "Resources\TileSets\steel.mjt"; DestDir: "{app}\Tile Sets"
Source: "Resources\TileSets\white.mjt"; DestDir: "{app}\Tile Sets"
Source: "Resources\TileSets\wood.mjt"; DestDir: "{app}\Tile Sets"

; skins
Source: "Resources\Skins\Blue.mji"; DestDir: "{app}\Skins"
Source: "Resources\Skins\Brown.mji"; DestDir: "{app}\Skins"

; languages
Source: "Resources\Languages\German.xml"; DestDir: "{app}\Languages"
Source: "Resources\Languages\French.xml"; DestDir: "{app}\Languages"
Source: "Resources\Languages\Dutch.xml"; DestDir: "{app}\Languages"

; Layouts
Source: "Resources\Layouts\3D Pyramid.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Arena.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Beatle.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Big Hole.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Bizarre.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Boat.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Bridging.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Castle.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Cat and Mouse.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Ceremonial.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Crown.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Cupola.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Deep Well.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Dude.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\F-15 Eagle.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Farandole.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Fish.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Five Pyramids 2.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Five Pyramids.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Full Vision 2.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Full Vision.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Gayle.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\H for Haga Traditional.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\H for Haga.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\High and Low.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Hourglass.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Inca.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Joker.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Kujaku.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Labyrinth.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Lion.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Lost.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Modern Art.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Naoki Haga Traditional.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Phoenix.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Portal.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Pyramid.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Scorpion.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Screw Up.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Seven Pyramids.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Seven.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Siam.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Space Ship.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Square.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Step Pyramid.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Stonehenge.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\SunMoon.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Temple.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Teotihucan.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\The Great Wall.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Theater.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Tile Fighter.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Tilepiles.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Tower and Walls.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Traditional Reviewed.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Traditional.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Tree of Life.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Twin Temples.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Victory Arrow.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Wavelets.mjl"; DestDir: "{app}\Layouts"
Source: "Resources\Layouts\Yummy.mjl"; DestDir: "{app}\Layouts"

[Messages]
BeveledLabel=Mahjongg 3D

[Icons]
Name: "{group}\OnLine Help"; FileName: "{app}\Help\mahjongg.chm"; Comment: "Mahjongg 3D help file"

[INI]
Filename: "{app}\mahjongg.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.mahjongg4u.com"

[Icons]
Name: "{group}\Mahjongg 3D"; Filename: "{app}\mahjongg.exe"
Name: "{group}\Mahjongg 3D on the Web"; Filename: "{app}\mahjongg.url"
Name: "{group}\Uninstall Mahjongg 3D"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Mahjongg 3D"; Filename: "{app}\mahjongg.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Mahjongg 3D"; Filename: "{app}\mahjongg.exe"; Tasks: quicklaunchicon

[Registry]
Root: HKCU; Subkey: "Software"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\Mahjongg 3D"; Flags: uninsdeletekeyifempty

Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "AutoSave"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "BackgroundColor"; ValueData: "4194304"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "BackgroundImage"; ValueData: "{app}\Background Images\China.jpg"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "BitmapBackground"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "ColoredLayers"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "EnvironmentLighting"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "FullScreen"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "GraphDetail"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "Height"; ValueData: "480"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "HideClosedTiles"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "HighQualityBackground"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "HighQualityTextures"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "Interface"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "LastGameMode"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "LastLayoutsDir"; ValueData: ""; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "LastPicturesDir"; ValueData: ""; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "LastTilesetDir"; ValueData: ""; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "Layout"; ValueData: "default"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "Maximized"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "Music"; ValueData: "{app}\Music\default.xm"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "PlayerName"; ValueData: "Player"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "PosX"; ValueData: "20"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "PosY"; ValueData: "20"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "ShowRBHints"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "SmartCursor"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "SolvableBoards"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "Sound"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "TextHints"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "TexturedSides"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "Tileset"; ValueData: "default"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "Width"; ValueData: "640"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "SmoothTiles"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "TransparentTiles"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: dword; ValueName: "UseSkin"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "Skin"; ValueData: "Brown"; Flags: uninsdeletevalue

; Language
Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "English"; Languages: en; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "German"; Languages: de; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "French"; Languages: fr; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "Dutch"; Languages: nl; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "Russian"; Languages: ru; Flags: uninsdeletevalue

[Run]
;Filename: "{app}\mahjongg.exe"; Description: "Launch Mahjongg 3D"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\mahjongg.url"
