#include "pchdef.h"
#include "WarriorTriggers.h"
#include "WarriorMultipliers.h"
#include "DpsWarriorStrategy.h"
#include "WarriorActions.h"

using namespace ai;

NextAction** DpsWarriorStrategy::getDefaultActions()
{
    return NextAction::array(0, new NextAction("melee", 10.0f), NULL);
}

void DpsWarriorStrategy::InitTriggers(std::list<TriggerNode*> &triggers)
{
    GenericWarriorStrategy::InitTriggers(triggers);
    
    triggers.push_back(new TriggerNode(
        new EnemyOutOfMeleeTrigger(ai), 
        NextAction::array(0, new NextAction("melee", 50.0f), NULL)));

    triggers.push_back(new TriggerNode(
        new LoseAggroTrigger(ai), 
        NextAction::array(0, new NextAction("mocking blow", 30.0f), NULL)));

    triggers.push_back(new TriggerNode(
        new HeroicStrikeAvailable(ai), 
        NextAction::array(0, new NextAction("heroic strike", 20.0f), NULL)));

    triggers.push_back(new TriggerNode(
        new NoAttackersTrigger(ai), 
        NextAction::array(0, new NextAction("attack least threat", 90.0f), NULL)));
}

void DpsWarriorStrategy::InitMultipliers(std::list<Multiplier*> &multipliers)
{

}

ActionNode* DpsWarriorStrategy::createAction(const char* name)
{
    if (!strcmp("melee", name)) 
    {
        return new ActionNode (new MeleeAction(ai),  
            /*P*/ NextAction::array(0, new NextAction("charge"), NULL),
            /*A*/ NULL, 
            /*C*/ NULL);
    }
    else if (!strcmp("charge", name)) 
    {
        return new ActionNode (new CastChargeAction(ai),  
            /*P*/ NextAction::array(0, new NextAction("battle stance"), NULL),
            /*A*/ NextAction::array(0, new NextAction("reach melee"), NULL), 
            /*C*/ NULL);
    }
    else if (!strcmp("rend", name)) 
    {
        return new ActionNode (new CastRendAction(ai),  
            /*P*/ NextAction::array(0, new NextAction("battle stance"), NULL),
            /*A*/ NULL, 
            /*C*/ NextAction::array(0, new NextAction("heroic strike", 20.0f), NULL));
    }
    else if (!strcmp("mocking blow", name)) 
    {
        return new ActionNode (new CastMockingBlowAction(ai),  
            /*P*/ NextAction::array(0, new NextAction("battle stance"), NULL),
            /*A*/ NextAction::array(0, new NextAction("taunt"), NULL), 
            /*C*/ NextAction::array(0, new NextAction("heroic strike", 20.0f), NULL));
    }
    else return GenericWarriorStrategy::createAction(name);
}