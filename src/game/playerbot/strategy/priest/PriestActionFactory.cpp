#include "../../../pchdef.h"
#include "../../playerbot.h"
#include "PriestActions.h"
#include "PriestActionFactory.h"
#include "HealPriestNonCombatStrategy.h"
#include "DpsPriestStrategy.h"
#include "../PullStrategy.h"

using namespace ai;

Strategy* PriestActionFactory::createStrategy(const char* name)
{
    if (!strcmp("heal", name))
        return new HealPriestStrategy(ai);

    if (!strcmp("dps", name))
        return new DpsPriestStrategy(ai);

    if (!strcmp("nc", name))
        return new HealPriestNonCombatStrategy(ai);

    if (!strcmp("pull", name))
        return new PullStrategy(ai, "shoot");

    Strategy* strategy = ActionFactory::createStrategy(name);
    if (strategy)
        return strategy;
    return new HealPriestStrategy(ai);
}
