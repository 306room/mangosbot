#pragma once

#include "Strategy.h"
#include "ActionFactory.h"

namespace ai
{
    class GenericNonCombatStrategy : public Strategy
    {
    public:
        GenericNonCombatStrategy(PlayerbotAIFacade* const ai) : Strategy(ai) {}

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
    };

    class FollowMasterNonCombatStrategy : public GenericNonCombatStrategy
    {
    public:
        FollowMasterNonCombatStrategy(PlayerbotAIFacade* const ai) : GenericNonCombatStrategy(ai) {}
        virtual const char* getName() { return "follow"; }
        virtual ActionNode* createAction(const char* name);
        virtual NextAction** getDefaultActions();

    };

    class GoAwayNonCombatStrategy : public GenericNonCombatStrategy
    {
    public:
        GoAwayNonCombatStrategy(PlayerbotAIFacade* const ai) : GenericNonCombatStrategy(ai) {}
        virtual const char* getName() { return "goaway"; }
        virtual ActionNode* createAction(const char* name);
        virtual NextAction** getDefaultActions();

    };

    class StayNonCombatStrategy : public GenericNonCombatStrategy
    {
    public:
        StayNonCombatStrategy(PlayerbotAIFacade* const ai) : GenericNonCombatStrategy(ai) {}
        virtual const char* getName() { return "stay"; }
        virtual ActionNode* createAction(const char* name);
        virtual NextAction** getDefaultActions();

    };

    class AssistNonCombatStrategy : public GenericNonCombatStrategy
    {
    public:
        AssistNonCombatStrategy(PlayerbotAIFacade* const ai) : GenericNonCombatStrategy(ai) {}
        virtual const char* getName() { return "assist"; }

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
    };

    class TankNonCombatStrategy : public GenericNonCombatStrategy
    {
    public:
        TankNonCombatStrategy(PlayerbotAIFacade* const ai) : GenericNonCombatStrategy(ai) {}
        virtual const char* getName() { return "tank non combat"; }

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
    };

    class LootNonCombatStrategy : public Strategy
    {
    public:
        LootNonCombatStrategy(PlayerbotAIFacade* const ai) : Strategy(ai) {}

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
        virtual const char* getName() { return "loot"; }
    };

    class RandomEmoteStrategy : public Strategy
    {
    public:
        RandomEmoteStrategy(PlayerbotAIFacade* const ai) : Strategy(ai) {}

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
        virtual const char* getName() { return "emote"; }
        virtual ActionNode* createAction(const char* name);
   };
}