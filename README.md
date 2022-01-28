# FC2MPPatcher
A community-made utility for patching Far Cry 2 to yet again support multiplayer online. 

## Howto / Instructions
By following the these instructions you will be able to play multiplayer online within 2 minutes!

### Account registration
In order to play online you have to register for an online account, Ubisoft services shutdown on June 1st 2021 ([Source](https://forums.ubisoft.com/showthread.php/1241617-Online-Services-Update-Masterlist-Forums?s=4e28173932bf67c5c4b122e6e2bd10eb)).

* Create an online account by registering at [farcry2.online](https://farcry2.online/register.html).
    * Verify your account by clicking on the link in the received e-mail.
    * NOTE: You do not need to login to your account on the website, only ingame.
* Add an online profile (or edit existing one) in the ingame multiplayer menu, use exactly the same account details as registered on [farcry2.online](https://farcry2.online) website.
* Enjoy your multiplayer online experience!

### Installation
* Download the latest version of the patch [here](https://github.com/halvors/FC2MPPatcher/releases) (make sure to download the .zip file), if you have a previous version installed, select upgrade instead of install.
* Run the patcher application (fc2mppatcher.exe) and make sure to select the network interface that your internet connection is on (Usually Ethernet or Wifi).
* Press the patch button and you are good to go!

Currently to play ranked matches you need PunkBuster installed, even though it is not in use by the server itself.

**NOTE:** As mentioned in our license, we are liable for bricking your game or even computer. 

### Requirements
 * A legal copy of the Far Cry 2 game (We do not ship any game files as they are intellectual property of Ubisoft Montreal). 
 * Game from one of these sources: Steam, Uplay, GOG or the Retail edition (CD/Disc).
 * Your game needs to be patched to version 1.03.

### Antivirus
This patch could trigger a false positive in your antivirus software, that is because it alters Far Cry 2 game files, this is to be expected and can be ignored. This software contains no code, you're welcome to read throught our source code ;-)

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

## Hosting a server
To host a server online you have to make the proper ports is open/forwarded. Usually port TCP/UDP 9000 to 9100 should do, everything else should work out of the box!

Please be aware that IP addresses shown in server logs may be misleading, no matter what address is shown the server actually listens to address 0.0.0.0 (any), which makes it reachable on any network adapter.

If hosting a server to be used by players on LAN or thru VPN, you have to select the proper network adapter (if VPN select that) before patching!

## Discord
[FC2MPPatcher](https://discord.gg/82G9Fh5)  
[Far Cry 2 Community](https://discord.link/FC2)
[Far Cry 2 Multiplayer](https://discord.com/invite/YRQhc39)

## Thanks to
### Contributors
<i>zenoy, halvors and FEARIV</i> - for patching the assembly code, creating the patcher application, and providing community backend services.  
<i>svinsen, Gabor and Lasercar</i> - for help testing and community support.

### Others
Thanks to original author of PE Bliss library, rukaimi (kaimi.ru)!  
Thanks to the Qt project for an amazing library to work with.  
