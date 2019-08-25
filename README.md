# FC2MPPatcher
An utility for patching your Far Cry 2 installation to support multiplayer agian. 

This resolves for Game and dedicated server:
 * Binding to the wrong network interface, resolving issues with online multiplayer. 
 * Annoncing wrong IP address in multiplayer, showing 9999 ping for mathces in lobby. 
 * Broadcasting to wrong address, causing issues with LAN multiplayer (including over VPN).
 * Joining matches hosted ingame.

If you experience diffculties patching the game, first make sure you got the 1.03 patch installed, if your version of the game isn't supported (only Retail, Steam and GOG is tested), please report on the issue tracker, it's a quick fix.

### Requirements
 * The game files itself. (We do not ship any game files as they are copyrighted by Ubisoft). 
 * The newest version of the game from (Steam, GOG or Uplay) or Retail version, that mean patch 1.03.

### Installation
<b>NOTE:</b> As mentioned in license, we're not responsible for breaking your game or getting you banned.

* Download the latest version of the patcher, if you got an old version installed, uninstall it first.
* Run the patcher and make sure to select the network interface that your internet connection is on (Usually Ethernet or Wifi).
* Press the patch button and you are good to go!

You will be able to play on ranked servers if you manually update your PunkBuster files.
To do so follow the instructions at: http://ned.theoldergamers.com/static.php?page=farcry2-punkbuster

### Upgrading
If you are upgrading from an old version of the patch uninstall the old one and follow the installation instructions.

### Antivirus
This patch will probably trigger a false positive in your antivirus software because it alters the game files of Far Cry 2, this is normal.  
The patch contains no malware or virueses, you're also welcome to read throught our code!

### Technical information
The reason for this being necessary is due to changes in the Windows API on newer versions, that is not compatible with Ubisoft's somewhat strange implementation.

## Discord
Please join us at Discord for support, and to find other people to play with!
https://discord.gg/82G9Fh5

## Thanks to
### Contributors
<i>zenoy</i> - Patching the assembly code, the time "wasted" figuring out.  
<i>halvors</i> - For patcher implementation, and the interface to support it.  

### Others
Thanks to original author of PE Bliss library, rukaimi (kaimi.ru)!  
Thanks to the Qt project for an amazing library to work with.  
