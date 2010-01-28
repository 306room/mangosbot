#include "pchdef.h"
#include "PriestTriggers.h"
#include "PriestMultipliers.h"
#include "HealPriestStrategy.h"
#include "PriestActions.h"

using namespace ai;

void HealPriestStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    CombatStrategy::InitTriggers(triggers);

    triggers.push_back(new TriggerNode(
        new PowerWordFortitudeTrigger(ai), 
        NextAction::array(0, new NextAction("power word: fortitude", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        new PowerWordFortitudeOnPartyTrigger(ai), 
        NextAction::array(0, new NextAction("power word: fortitude on party", 50.0f), NULL)));


    triggers.push_back(new TriggerNode(
        new LowHealthTrigger(ai),
        NextAction::array(0, new NextAction("power word: shield", 60.0f), new NextAction("heal", 60.0f), NULL)));

    triggers.push_back(new TriggerNode(
        new PartyMemberLowHealthTrigger(ai),
        NextAction::array(0, new NextAction("power word: shield on party", 50.0f), new NextAction("heal on party", 50.0f), NULL)));

    triggers.push_back(new TriggerNode(
        new NoAttackersTrigger(ai), 
        NextAction::array(0, new NextAction("attack least threat", 20.0f), NULL)));
}

void HealPriestStrategy::InitMultipliers(std::list<Multiplier*> &multipliers)
{
    CombatStrategy::InitMultipliers(multipliers);
}

ActionNode* HealPriestStrategy::createAction(const char* name)
{
    ActionNode* node = CombatStrategy::createAction(name);
    if (node)
        return node;

    if (!strcmp("power word: fortitude", name)) 
    {
        return new ActionNode (new CastPowerWordFortitudeAction(ai),  
            /*P*/ NULL,
            /*A*/ NULL, 
            /*C*/ NULL);
    }
    else if (!strcmp("power word: fortitude on party", name)) 
    {
        return new ActionNode (new CastPowerWordFortitudeOnPartyAction(ai),  
            /*P*/ NULL,
            /*A*/ NULL, 
            /*C*/ NULL);
    }
    if (!strcmp("power word: shield", name)) 
    {
        return new ActionNode (new CastPowerWordShieldAction(ai),  
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("renew", 50.0f), NULL), 
            /*C*/ NULL);
    }
    else if (!strcmp("power word: shield on party", name)) 
    {
        return new ActionNode (new CastPowerWordShieldOnPartyAction(ai),  
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("renew on party", 50.0f), NULL), 
            /*C*/ NULL);
    }
    else if (!strcmp("renew", name)) 
    {
        return new ActionNode (new CastRenewAction(ai),  
            /*P*/ NULL,
            /*A*/ NULL, 
            /*C*/ NULL);
    }
    else if (!strcmp("renew on party", name)) 
    {
        return new ActionNode (new CastRenewOnPartyAction(ai),  
            /*P*/ NULL,
            /*A*/ NULL, 
            /*C*/ NULL);
    }
    else if (!strcmp("heal", name)) 
    {
        return new ActionNode (new CastHealAction(ai),  
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("lesser heal"), NULL), 
            /*C*/ NULL);
    }
    else if (!strcmp("heal on party", name)) 
    {
        return new ActionNode (new CastHealOnPartyAction(ai),  
            /*P*/ NULL,
            /*A*/ NextAction::array(0, new NextAction("lesser heal on party"), NULL), 
            /*C*/ NULL);
    }
    else if (!strcmp("lesser heal", name)) 
    {
        return new ActionNode (new CastLesserHealAction(ai),  
            /*P*/ NULL,
            /*A*/ NULL, 
            /*C*/ NULL);
    }
    else if (!strcmp("lesser heal on party", name)) 
    {
        return new ActionNode (new CastLesserHealOnPartyAction(ai),  
            /*P*/ NULL,
            /*A*/ NULL, 
            /*C*/ NULL);
    }
    else if (!strcmp("attack least threat", name)) 
    {
        return new ActionNode (new AttackLeastThreatAction(ai),  
            /*P*/ NULL,
            /*A*/ NULL, 
            /*C*/ NULL);
    }
    else return NULL;
}