# StatDisplay
Displays various statistics on the UILayer

This mod supports a dispatch event-based API. To use it, first include in your #includes #include <Geode/loader/Dispatch.hpp>. 
Next, store the possible start and end strings in std::strings. For this example, I will call them start and end. 
You also will want to store a string with the id of the stat. Make sure there is a resource in the mod resources folder with a name matching the ID, and append the ID with _spr.

Next, hook GJBaseGameLayer::createTextLayers, and after calling the original method, call the following line of code: 

`DispatchEvent<std::string, std::string, std::string, int>("legowiifun.stat_display/addStat", start,end, id, int value).post();`
with int value being replaced by the int variable where you are storing the stat to display. The mod does not support non-int stats.

This will add the stat to the stat display. However, it will only update when exiting and reentering the level, unlike the base stats. 