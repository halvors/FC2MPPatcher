# FC2MPPatcher
Is an utility for patching your Far Cry 2 installation to support multiplayer properly, so that LAN play and play over various VPN services works as one would expect.

The reason for this being necessary is due to changes in the Windows API on newer versions and that Ubisoft did not implement it properly to begin with.

This resolved issues with the game or server trying to bind to the wrong network interface, and that matches doesn't show up when they should, sometimes seen as "9999" ping issue. This fix also makes it possible again to host a match directly from ingame.

<b>Note:</b> This is not a fix for Ubisoft taking down their online backend, you still have to be on the LAN network or same VPN service as your friends/peers.

# Thanks to
### Authors
<i>zenoy</i> <insert email here?> - Patching the assembly code, the time "wasted" digging into that, and patcher implementation.  
<i>halvors</i> <insert email here?> - For patcher implementation, and the interface to support it.  

### Others
Thanks to original author of PE Bliss library, rukaimi (kaimi.ru)!
Thanks to the Qt project for an amazing library to work with.
