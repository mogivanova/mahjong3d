[Setup]
AppName=Amazing Mahjongg 3D
AppVerName=Amazing Mahjongg 3D
AppVersion=1.3.0
MinVersion=4.1.1998, 5.0.2195
AppCopyright=Copyright © 1998-2006 Thinking Thing Software
AppPublisher=Thinking Thing Software
AppPublisherURL=http://www.mahjongg4u.com
AppSupportURL=http://www.mahjongg4u.com
AppUpdatesURL=http://www.mahjongg4u.com
DefaultDirName={pf}\Amazing Mahjongg 3D
DefaultGroupName=Amazing Mahjongg
OutputDir=C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Setup\Default\
OutputBaseFilename=setup
WizardImageFile=wizard.bmp
WizardSmallImageFile=wizard_small.bmp
ShowLanguageDialog=auto

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"; LicenseFile: "license.rtf"
;Name: "de"; MessagesFile: "German.isl";
;Name: "fr"; MessagesFile: "French.isl";
;Name: "nl"; MessagesFile: "Dutch.isl";
;Name: "ru"; MessagesFile: "Support\Russian.isl"; LicenseFile: "Support\license_ru.rtf"

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
Name: "{userdocs}\My Games\Amazing Mahjongg 3D"
Name: "{userdocs}\My Games\Amazing Mahjongg 3D\Background Images"
Name: "{userdocs}\My Games\Amazing Mahjongg 3D\Layouts"
Name: "{userdocs}\My Games\Amazing Mahjongg 3D\Save"
Name: "{userdocs}\My Games\Amazing Mahjongg 3D\Skins"
Name: "{userdocs}\My Games\Amazing Mahjongg 3D\Tile Sets"
Name: "{userdocs}\My Games\Amazing Mahjongg 3D\Languages"

[Files]
; Main files
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\mahjongg.exe"; DestDir: "{app}"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\glu.dll"; DestDir: "{app}"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\opengl.dll"; DestDir: "{app}"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\readme.txt"; DestDir: "{app}"

; Help files
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Help\mahjongg.chm"; DestDir: "{app}\Help"; DestName: "mahjongg.chm"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Help\mahjongg_de.chm"; DestDir: "{app}\Help"; DestName: "mahjongg_de.chm"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Help\mahjongg_fr.chm"; DestDir: "{app}\Help"; DestName: "mahjongg_fr.chm"

; backgrounds
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Pictures\building2.jpg"; DestDir: "{app}\Background Images"; DestName: "China.jpg"

; tile sets
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\TileSets\egyptian.mjt"; DestDir: "{app}\Tile Sets"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\TileSets\black.mjt"; DestDir: "{app}\Tile Sets"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\TileSets\steel.mjt"; DestDir: "{app}\Tile Sets"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\TileSets\white.mjt"; DestDir: "{app}\Tile Sets"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\TileSets\wood.mjt"; DestDir: "{app}\Tile Sets"

; skins
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Skins\Blue.mji"; DestDir: "{app}\Skins"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Skins\Brown.mji"; DestDir: "{app}\Skins"

; languages
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Languages\German.xml"; DestDir: "{app}\Languages"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Languages\French.xml"; DestDir: "{app}\Languages"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Languages\Dutch.xml"; DestDir: "{app}\Languages"

; Layouts
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\3D Pyramid.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Arena.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Beatle.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Big Hole.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Bizarre.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Boat.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Bridging.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Castle.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Cat and Mouse.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Ceremonial.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Crown.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Cupola.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Deep Well.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Dude.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\F-15 Eagle.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Farandole.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Fish.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Five Pyramids 2.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Five Pyramids.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Full Vision 2.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Full Vision.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Gayle.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\H for Haga Traditional.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\H for Haga.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\High and Low.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Hourglass.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Inca.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Joker.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Kujaku.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Labyrinth.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Lion.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Lost.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Modern Art.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Naoki Haga Traditional.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Phoenix.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Portal.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Pyramid.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Scorpion.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Screw Up.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Seven Pyramids.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Seven.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Siam.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Space Ship.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Square.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Step Pyramid.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Stonehenge.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\SunMoon.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Temple.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Teotihucan.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\The Great Wall.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Theater.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Tile Fighter.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Tilepiles.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Tower and Walls.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Traditional Reviewed.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Traditional.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Tree of Life.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Twin Temples.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Victory Arrow.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Wavelets.mjl"; DestDir: "{app}\Layouts"
Source: "C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Temp\Setup\Layouts\Yummy.mjl"; DestDir: "{app}\Layouts"

[Messages]
BeveledLabel=Amazing Mahjongg 3D

[Icons]
Name: "{group}\OnLine Help"; FileName: "{app}\Help\mahjongg.chm"; Comment: "Amazing Mahjongg 3D help file"

[INI]
Filename: "{app}\mahjongg.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.mahjongg4u.com"

[Icons]
Name: "{group}\Amazing Mahjongg 3D"; Filename: "{app}\mahjongg.exe"
Name: "{group}\Amazing Mahjongg 3D on the Web"; Filename: "{app}\mahjongg.url"
Name: "{group}\Uninstall Amazing Mahjongg 3D"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Amazing Mahjongg 3D"; Filename: "{app}\mahjongg.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Amazing Mahjongg 3D"; Filename: "{app}\mahjongg.exe"; Tasks: quicklaunchicon

[Registry]
Root: HKCU; Subkey: "Software\Thinking Thing Software"; Flags: uninsdeletekeyifempty
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; Flags: uninsdeletekeyifempty

Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "AutoSave"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "BackgroundColor"; ValueData: "4194304"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "BackgroundImage"; ValueData: "{app}\Background Images\China.jpg"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "BitmapBackground"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "ColoredLayers"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "EnvironmentLighting"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "FullScreen"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "GraphDetail"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "Height"; ValueData: "480"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "HideClosedTiles"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "HighQualityBackground"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "HighQualityTextures"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "Interface"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "LastGameMode"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "LastLayoutsDir"; ValueData: ""; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "LastPicturesDir"; ValueData: ""; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "LastTilesetDir"; ValueData: ""; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Layout"; ValueData: "default"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "Maximized"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Music"; ValueData: "{app}\Music\\default.xm"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "PlayerName"; ValueData: "Player"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "PosX"; ValueData: "20"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "PosY"; ValueData: "20"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "ShowRBHints"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "SmartCursor"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "SolvableBoards"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "Sound"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "TextHints"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "TexturedSides"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Tileset"; ValueData: "default"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "Width"; ValueData: "640"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "SmoothTiles"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "TransparentTiles"; ValueData: "0"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: dword; ValueName: "UseSkin"; ValueData: "1"; Flags: uninsdeletevalue
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Skin"; ValueData: "Brown"; Flags: uninsdeletevalue

; Language
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "English"; Languages: en; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "German"; Languages: de; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "French"; Languages: fr; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "Dutch"; Languages: nl; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "Russian"; Languages: ru; Flags: uninsdeletevalue

[Run]
;Filename: "{app}\mahjongg.exe"; Description: "Launch Amazing Mahjongg 3D"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\mahjongg.url"
