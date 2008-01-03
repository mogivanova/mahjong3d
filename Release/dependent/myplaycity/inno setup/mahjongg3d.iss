[Setup]
AppName=My Free Mahjongg
AppVerName=My Free Mahjongg 2.0
AppVersion=2.0
MinVersion=4.1.1998, 5.0.2195
AppCopyright=Copyright © 2006 MyPlayCity.com and Thinking Thing Software
AppPublisher=MyPlayCity.com
AppPublisherURL=http://www.myplaycity.com
AppSupportURL=http://www.myplaycity.com
AppUpdatesURL=http://www.myplaycity.com
DefaultDirName={pf}\My Free Mahjongg
DefaultGroupName=My Free Mahjongg
OutputDir=C:\Work\My Projects\Mahjongg\Sources\Mahjongg3D\_Setup\MyPlayCity\
OutputBaseFilename=setup
WizardImageFile=Setup\wizard.bmp
WizardSmallImageFile=Setup\wizard_small.bmp
ShowLanguageDialog=auto

[Languages]
Name: "en"; MessagesFile: "compiler:Default.isl"; LicenseFile: "Setup\license.rtf"
;Name: "de"; MessagesFile: "Support\German.isl"; LicenseFile: "Support\license_de.rtf"
;Name: "fr"; MessagesFile: "Support\French.isl"; LicenseFile: "Support\license_fr.rtf"
;Name: "nl"; MessagesFile: "Support\Dutch.isl"; LicenseFile: "Support\license_nl.rtf"
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
Name: "{userdocs}\My Games\My Free Mahjongg"
Name: "{userdocs}\My Games\My Free Mahjongg\Background Images"
Name: "{userdocs}\My Games\My Free Mahjongg\Layouts"
Name: "{userdocs}\My Games\My Free Mahjongg\Save"
Name: "{userdocs}\My Games\My Free Mahjongg\Skins"
Name: "{userdocs}\My Games\My Free Mahjongg\Tile Sets"
Name: "{userdocs}\My Games\My Free Mahjongg\Languages"

[Files]
; Main files
Source: "Files\mahjongg.exe"; DestDir: "{app}"
Source: "Files\glu.dll"; DestDir: "{app}"
Source: "Files\opengl.dll"; DestDir: "{app}"
Source: "Files\readme.txt"; DestDir: "{app}"

; Help files
Source: "Files\Help\mahjongg.chm"; DestDir: "{app}\Help"; DestName: "mahjongg.chm"
Source: "Files\Help\mahjongg_de.chm"; DestDir: "{app}\Help"; DestName: "mahjongg_de.chm"
Source: "Files\Help\mahjongg_fr.chm"; DestDir: "{app}\Help"; DestName: "mahjongg_fr.chm"

; backgrounds
Source: "Files\Pictures\building2.jpg"; DestDir: "{app}\Background Images"; DestName: "China.jpg"

; tile sets
Source: "Files\TileSets\egyptian.mjt"; DestDir: "{app}\Tile Sets"

; skins
Source: "Files\Skins\Blue.mji"; DestDir: "{app}\Skins"

; music
Source: "Files\Music\default.xm"; DestDir: "{app}\Music"

; languages
Source: "Files\Languages\German.xml"; DestDir: "{app}\Languages"
Source: "Files\Languages\French.xml"; DestDir: "{app}\Languages"
Source: "Files\Languages\Dutch.xml"; DestDir: "{app}\Languages"

; Layouts
Source: "Files\Layouts\3D Pyramid.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Arena.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Beatle.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Big Hole.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Bizarre.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Boat.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Bridging.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Castle.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Cat and Mouse.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Ceremonial.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Crown.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Cupola.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Deep Well.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Dude.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\F-15 Eagle.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Farandole.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Fish.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Five Pyramids 2.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Five Pyramids.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Full Vision 2.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Full Vision.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Gayle.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\H for Haga Traditional.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\H for Haga.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\High and Low.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Hourglass.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Inca.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Joker.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Kujaku.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Labyrinth.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Lion.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Lost.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Modern Art.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Naoki Haga Traditional.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Phoenix.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Portal.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Pyramid.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Scorpion.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Screw Up.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Seven Pyramids.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Seven.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Siam.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Space Ship.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Square.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Step Pyramid.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Stonehenge.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\SunMoon.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Temple.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Teotihucan.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\The Great Wall.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Theater.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Tile Fighter.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Tilepiles.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Tower and Walls.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Traditional Reviewed.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Traditional.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Tree of Life.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Twin Temples.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Victory Arrow.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Wavelets.mjl"; DestDir: "{app}\Layouts"
Source: "Files\Layouts\Yummy.mjl"; DestDir: "{app}\Layouts"

[Messages]
BeveledLabel=My Free Mahjong

[Icons]
Name: "{group}\OnLine Help"; FileName: "{app}\Help\mahjongg.chm"; Comment: "My Free Mahjong help file"

[INI]
Filename: "{app}\mahjongg.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.myplaycity.com"

[Icons]
Name: "{group}\My Free Mahjong"; Filename: "{app}\mahjongg.exe"
Name: "{group}\My Free Mahjong on the Web"; Filename: "{app}\mahjongg.url"
Name: "{group}\Uninstall My Free Mahjong"; Filename: "{uninstallexe}"
Name: "{userdesktop}\My Free Mahjong"; Filename: "{app}\mahjongg.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\My Free Mahjong"; Filename: "{app}\mahjongg.exe"; Tasks: quicklaunchicon

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
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Skin"; ValueData: "Blue"; Flags: uninsdeletevalue

; Language
Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "English"; Languages: en; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "German"; Languages: de; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "French"; Languages: fr; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "Dutch"; Languages: nl; Flags: uninsdeletevalue
;Root: HKCU; Subkey: "Software\Thinking Thing Software\Amazing Mahjongg 3D"; ValueType: string; ValueName: "Language"; ValueData: "Russian"; Languages: ru; Flags: uninsdeletevalue

[Run]
;Filename: "{app}\mahjongg.exe"; Description: "Launch My Free Mahjong"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\mahjongg.url"
