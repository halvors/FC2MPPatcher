# FC2MPPatcher
An utility for patching your Far Cry 2 installation to support multiplayer agian. 

This resolves the following issues for the game and dedicated server:
 * Binding to the wrong network interface, resolving issues with online multiplayer. 
 * Annoncing wrong IP address in multiplayer, showing 9999 ping for matches in lobby. 
 * Broadcasting to wrong address, causing issues with LAN multiplayer (including over VPN).
 * Dedicated Server not starting in online mode.
 * Joining matches hosted ingame.
 * Steam and Uplay version being unable to login to online accounts.
 * Custom map download from host not ever working properly on online servers.

If you experience diffculties patching the game, first make sure you got the 1.03 patch installed, if your version of the game isn't supported (only Retail, Steam, Uplay and GOG is tested), please report on the issue tracker, it's a quick fix.

### Requirements
 * The game files itself. (We do not ship any game files as they are copyrighted by Ubisoft). 
 * The newest version of the game from either Steam, Uplay, GOG or the Retail edition (Disc), your game needs to be version 1.03.

### Installation
<b>NOTE:</b> As mentioned in license, we're not responsible for breaking your game or getting you banned.

* Download the latest version of the patcher, if you got an old version installed, uninstall it first.
* Run the patcher and make sure to select the network interface that your internet connection is on (Usually Ethernet or Wifi).
* Press the patch button and you are good to go!

To play ranked matches you need PunkBuster which does no longer work out-of-the-box, you can follow these instructions to manually update your PunkBuster files: http://ned.theoldergamers.com/static.php?page=farcry2-punkbuster

### Upgrading
If you are upgrading from an old version of the patch uninstall the old one and install the new version of the patch.

### Antivirus
This patch will probably trigger a false positive in your antivirus software because it alters the game files of Far Cry 2, this is normal.  
The patch contains no malware or virueses, you're also welcome to read throught our code!

### Dedicated Server
If hosting a server to be used by players thru VPN, you have to select your VPN adapter before patching!

Also please be aware that IP addresses shown in server logs may be misleading, no matter what address is shown the server always listens on 0.0.0.0 (any), which means it's reachable on any network adapter.

### Technical information
The reason for this being necessary is due to changes in the Windows API on newer versions, that is not compatible with Ubisoft's somewhat strange implementation.

## Discord
Please join us at Discord for support, and to find other people to play with!
https://discord.gg/82G9Fh5

## Thanks to
### Contributors
<i>zenoy and halvors</i> - for patching the assembly code and creating the patcher application.  
<i>svinsen, FEARIV, Gabor and Lasercar</i> - For help testing and community support.

### Others
Thanks to original author of PE Bliss library, rukaimi (kaimi.ru)!  
Thanks to the Qt project for an amazing library to work with.  
