It is easy enough to make your own tile set for Mahjong 3D. First, you have to make images for all of tiles. Each image must be in separate file. Better, but not necessary, if all of images have same dimensions. Tiles are shown most correctly when width of tile image is 75% of its height (for example 100 pixels wide and 75 pixels high). Images must be in [.JPG](http://en.wikipedia.org/wiki/JPEG) format.

Afterwards you should set names for image files:

  * Empty tile used for tile sides and backwards is named **00.jpg**
  * Tiles of dots (circles) suit have names from **01.jpg** to **09.jpg**
  * Tiles of bamboo (sticks) suit have names from **11.jpg** to **19.jpg**
  * Tiles of characters suit have names from **21.jpg** to **29.jpg**
  * Winds are named from **31.jpg** to **34.jpg** (North, South, East, West)
  * Dragons are named **10.jpg** for White, **20.jpg** for Green and **30.jpg** for Red one.
  * Season tiles have names **35.jpg**, **36.jpg**, **37.jpg**, **38.jpg**
  * Flower tiles have names **39.jpg**, **40.jpg**, **41.jpg**, **42.jpg**

Therefore you will got set of 43 files named from **00.jpg** to **42.jpg**.

If you want to place additional information about this tileset and your name, you have to create text file named **tileset.ini** (make it with Notepad.exe for example).

It must contain several lines in form 'name = value'. Valid names are 'Name' and 'Author'. For example, your tileset.ini might look like

```
Name=My first great tile set
Author=Great tile set creator
```

Now you compress all those files (00.jpg - 42.jpg and tileset.ini) with any [.ZIP](http://en.wikipedia.org/wiki/ZIP_(file_format)) compression utility (WinZip, PkZip, etc.). It's up to you to leave extension of this file as .zip or to change it to .mjt.

Your tile set is ready. Place it under **Amazing Mahjongg\Tile sets** directory.