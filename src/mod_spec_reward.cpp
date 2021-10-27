/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

// By BetaYi (QQ:1518429315 ; 1518429315@qq.com) 群：719526711

#include "ScriptMgr.h"
#include "Config.h"
#include <Player.h>
#include "Group.h"
#include "GroupMgr.h"
#include "InstanceScript.h"
#include "Chat.h"
#include "WorldSession.h"
#include "Creature.h"

static bool ModuleEnable, AnnouncerEnable;
static uint32 RewardSpec,MinimalLevel,DungeonToken,RaidToken,TokenCount;
std::string HMessageText;
std::string TMessageText;
std::string DMessageText;
std::string BannerText;

enum SpecType
{
	FLAG_SPEC_HEALER = 0x00000001,
	FLAG_SPEC_DPS 	= 0x00000002,
	FLAG_SPEC_TANK 	= 0x00000004
};
  
// Add player scripts
class Spec_Reward : public PlayerScript
{
public:
    Spec_Reward() : PlayerScript("Spec_Reward") { }

    void OnLogin(Player* player) override
    {
        if (ModuleEnable && AnnouncerEnable)
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Spec Reward |rmodule.");
        }
    }

    void OnCreatureKill(Player* player, Creature* boss) override
    {
            if (ModuleEnable && boss->getLevel() > MinimalLevel &&  boss->IsDungeonBoss())
            {
                //lets get the info we want
                Map* map = player->GetMap();
                std::string p_name;
                std::string tag_colour = "7bbef7";
                std::string plr_colour = "7bbef7";
                std::string guild_colour = "00ff00";
                std::string boss_colour = "ff0000";
                std::string alive_text = "00ff00";
              bool Raid = player->GetMap()->IsRaid();

                Map::PlayerList const & playerlist = map->GetPlayers();
                std::ostringstream stream;
                uint32 i = 0;

                stream <<"["<<boss->GetNameForLocaleIdx(LOCALE_zhCN)<<" KO!]" << std::endl;

                for (Map::PlayerList::const_iterator itr = playerlist.begin(); itr != playerlist.end(); ++itr)
                {
                    if (!itr->GetSource())
                        continue;
                    
                    p_name = itr->GetSource()->GetName();

                    if (itr->GetSource()->HasHealSpec() && RewardSpec & FLAG_SPEC_HEALER)
                    {
                        ++i;
                       	itr->GetSource()->AddItem((Raid ? RaidToken : DungeonToken) , TokenCount);
                       	stream << i << ". |CFF" << tag_colour << "|r|cff" << plr_colour << p_name << "|r " << HMessageText << std::endl;
                 		continue;
					}                 	
                 	
                 	if (itr->GetSource()->HasTankSpec() && RewardSpec & FLAG_SPEC_TANK)
                    {
                        ++i;
                       	itr->GetSource()->AddItem((Raid ? RaidToken : DungeonToken) , TokenCount);
                       	stream << i << ". |CFF" << tag_colour << "|r|cff" << plr_colour << p_name << "|r " << TMessageText << std::endl;
                 		continue;
					}

					if (itr->GetSource()->HasCasterSpec() && RewardSpec & FLAG_SPEC_DPS)
                    {
                        ++i;
                       	itr->GetSource()->AddItem((Raid ? RaidToken : DungeonToken) , TokenCount);
                       	stream << i << ". |CFF" << tag_colour << "|r|cff" << plr_colour << p_name << "|r " << DMessageText << std::endl;
                 		continue;
					}
                }
                
                sWorld->SendServerMessage(SERVER_MSG_STRING,stream.str().c_str());
            }
    }

};

// Initial Conf
class Spec_Reward_Conf : public WorldScript
{
public:
	Spec_Reward_Conf() : WorldScript("Spec_Reward_Conf") { }

	void OnBeforeConfigLoad(bool reload) override
	{
		if (!reload) {
            SetInitialWorldSettings();
		}
	}
    void  SetInitialWorldSettings()
    {
        ModuleEnable = sConfigMgr->GetBoolDefault("Spec_Reward.Enable", true);
        AnnouncerEnable = sConfigMgr->GetBoolDefault("Spec_Reward.Announce", true);
        RewardSpec = sConfigMgr->GetIntDefault("Spec_Reward.Spec", 1);
        MinimalLevel = sConfigMgr->GetIntDefault("Spec_Reward.Level", 80);
		DungeonToken = sConfigMgr->GetIntDefault("Spec_Reward.DungeonToken", 38186);
		RaidToken = sConfigMgr->GetIntDefault("Spec_Reward.RaidToken", 38186);
        TokenCount = sConfigMgr->GetIntDefault("Spec_Reward.TokenCount", 1);
        HMessageText = sConfigMgr->GetStringDefault("Spec_Reward.HealText", "");
        TMessageText = sConfigMgr->GetStringDefault("Spec_Reward.TankText", "");
        DMessageText = sConfigMgr->GetStringDefault("Spec_Reward.DPSText", "");
     }
};

// Add all scripts in one
void Addmod_spec_rewardScripts()
{
    new Spec_Reward_Conf();
    new Spec_Reward();
}
