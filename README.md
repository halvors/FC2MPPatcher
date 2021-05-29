# FC2MPPatcher
An utility for patching your Far Cry 2 installation to support multiplayer agian. 

## Features
This resolves the following issues for the game and dedicated server.

### Both
* Binding to the wrong network interface, resolving issues with online multiplayer. 
* Broadcasting to wrong address, causing issues with LAN multiplayer (including over VPN).

### Game
* Steam and Uplay version now being able to login to online accounts.
* Joining matches hosted ingame.

### Dedicated Server
* Dedicated Server now able to start in online mode.
* Annoncing wrong IP address in multiplayer, showing 9999 ping for matches in lobby.
* Custom map download from host now working properly for online servers.
* Possible to disable PunkBuster even for ranked matches.

If you experience diffculties patching the game, first make sure you got the 1.03 patch installed, if your version of the game isn't supported (only Retail, Steam, Uplay and GOG is tested), please report on the issue tracker, it's a quick fix.

## Requirements
 * The game files itself. (We do not ship any game files as they are copyrighted by Ubisoft). 
 * The newest version of the game from either Steam, Uplay, GOG or the Retail edition (Disc), your game needs to be version 1.03.

## Installation
<b>NOTE:</b> As mentioned in license, we're not responsible for breaking your game or getting you banned.

* Download the latest version of the patcher, if you got an old version installed, uninstall it first.
* Run the patcher and make sure to select the network interface that your internet connection is on (Usually Ethernet or Wifi).
* Press the patch button and you are good to go!

To play ranked matches you need PunkBuster installed, even though it is not in use by the server itself.

### Antivirus
This patch will probably trigger a false positive in your antivirus software because it alters the game files of Far Cry 2, this is normal.  
The patch contains no malware or virueses, you're also welcome to read throught our code!

### Running a Dedicated Server
If hosting a server to be used by players thru VPN, you have to select your VPN adapter before patching!

Also please be aware that IP addresses shown in server logs may be misleading, no matter what address is shown the server always listens on 0.0.0.0 (any), which means it's reachable on any network adapter.

## Discord
### Getting suppport for the patch
https://discord.gg/82G9Fh5

### Getting in touch with other players
https://discord.link/FC2

## Thanks to
### Contributors
<i>zenoy and halvors</i> - for patching the assembly code and creating the patcher application.  
<i>svinsen, FEARIV, Gabor and Lasercar</i> - for help testing and with community support.

### Others
Thanks to original author of PE Bliss library, rukaimi (kaimi.ru)!  
Thanks to the Qt project for an amazing library to work with.  
