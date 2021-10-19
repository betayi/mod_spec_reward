# ![logo](https://raw.githubusercontent.com/azerothcore/azerothcore.github.io/master/images/logo-github.png) AzerothCore
# Current features:

This module reward diffrent items to the players with spec in group while the dungeon/raid boss killed.   

## How to use?

In conf file , make the reward specs code as a mask 

### Description: Which spec player should be reward.
 1:Healer 2:DPS 4:Tank 
 Default: 5 (1+4) Healer+Tank

Spec_Reward.Spec = 5
### Description: Minimal Level of Boss killed should trigger  
Spec_Reward.Level = 80

### Reward Item if DungeonBoss  killed
Spec_Reward.DungeonToken = 38186

### Reward Item if RaidBoss  killed
Spec_Reward.RaidToken = 38186

### Reward Item Count while boss killed
Spec_Reward.TokenCount = 1

## How to install

###1) Simply place the module under the `modules` folder of your AzerothCore source folder.

###2) Re-run cmake and launch a clean build of AzerothCore

**That's it.**

### (Optional) Edit module configuration

If you need to change the module configuration, go to your server configuration folder (e.g. **etc**), copy `mod_spec_reward.conf.dist` to `mod_spec_reward.conf` and edit it as you prefer.
 
