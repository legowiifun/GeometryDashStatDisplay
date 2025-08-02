#pragma once
/**
 * Brings cocos2d and all Geode namespaces to the current scope.
 */
using namespace geode::prelude;
/**
* Various stats tracked by getStat
1: total jumps
2: total attempts
3: completed main levels
4: completed online levels
5: demons
6: stars
7: map packs
8: secret coins
9: players destroyed
10: liked/disliked levels
11: rated levels
12: user coins
13: diamonds
14: mana orbs
15: daily levels
22: total orbs
28: moons
29: diamond shards
40: gauntlets
41: list rewards
42: insane levels
Other values are unknown
100: custom stats
*/
enum class Stat {
    JUMPS = 1, ATTEMPTS, MAIN_LEVELS, ONLINE_LEVELS, DEMONS, STARS, MAP_PACKS, SECRET_COINS, DESTROYED_PLAYERS,
    LIKED_LEVELS, RATED_LEVELS, USER_COINS, DIAMONDS, MANA_ORBS, DAILY_LEVELS, TOTAL_ORBS = 22,
    MOONS = 28, DIAMOND_SHARDS, GAUNTLETS = 40, LIST_REWARDS, INSANES, OTHER = 100
};