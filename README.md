# FC2MPPatcher
An utility for patching your Far Cry 2 installation to support multiplayer agian. 
Currently fixes Ranked (see install instructions) and Unranked online multiplayer, LAN multiplayer (also over various VPN services).

This resolves for Game and dedicated server:
 * Binding to the wrong network interface, resolving issues with unranked online multiplayer. 
 * Annoncing wrong IP address in multiplayer, showing 9999 ping for mathces in lobby. 
 * Broadcasting to wrong address, causing issues with LAN multiplayer (including over VPN). 
 * Joining matches hosted ingame.

If you experience diffculties patching the game, first make sure you got the 1.03 patch installed, if your version of the game isn't supported (only Retail, Steam and GOG is tested), please report on the issue tracker, it's a quick fix.

The reason for this being necessary is due to changes in the Windows API on newer versions, that is not compatible with Ubisoft's somewhat strange implementation.

## Installation
<b>NOTE:</b> As mentioned in license, we're not responsible for breaking your game or getting you banned.

You will be able to play on ranked servers if you manually update your PunkBuster files.
To do so follow the instructions at: http://ned.theoldergamers.com/static.php?page=farcry2-punkbuster

### Requirements
 * The game files itself. (We do not ship any game files as they are copyrighted by Ubisoft). 
 * The newest version of the game from (Steam, GOG or Uplay) or Retail version with Patch 1.03 applied. 

### Antivirus
This patch wil probably trigger a false positive in your antivirus software because it alters the gme files of Far Cry 2, this is normal.  
You're welcome to read thrught our code!

## Technical information

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
