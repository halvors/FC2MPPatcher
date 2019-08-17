# FC2MPPatcher
An utility for patching your Far Cry 2 installation to support multiplayer agian. 
Currently fixes Unranked online multiplayer, LAN multiplayer (also over various VPN services).

The reason for this being necessary is due to changes in the Windows API on newer versions and that Ubisoft did not implement it properly to begin with.

This resolved issues with the game or server trying to bind to the wrong network interface, that matches doesn't show up when they should, and sometimes seen as "9999" ping issue. And also makes it possible again to host a match directly from the game itself.

If you experience diffculties patching the game, first make sure you got the 1.03 patch installed, if your version of the game isn't supported (only Retail, Steam and GOG is tested), please report on the issue tracker, it's a quick fix.

<b>Note:</b> This is not a fix for Ubisoft taking down their online backend, you still have to be on the LAN network or same VPN service as your friends/peers.

## Technical information

## Thanks to
### Authors
<i>zenoy</i> - Patching the assembly code, the time "wasted" figuring out.  
<i>halvors</i> - For patcher implementation, and the interface to support it.  
<i>svinsen</i> - For testing thru gameplay.

### Others
Thanks to original author of PE Bliss library, rukaimi (kaimi.ru)!  
Thanks to the Qt project for an amazing library to work with.  
