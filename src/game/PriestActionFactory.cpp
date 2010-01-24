#include "pchdef.h"
#include "PriestActions.h"
#include "PriestActionFactory.h"
#include "HealPriestStrategy.h"
#include "DpsPriestStrategy.h"

using namespace ai;

Strategy* PriestActionFactory::createStrategy(const char* name)
{
    if (!strcmp("heal", name))
        return new HealPriestStrategy(ai);

    if (!strcmp("dps", name))
        return new DpsPriestStrategy(ai);

    if (!strcmp("nc", name))
        return new HealPriestStrategy(ai);

    Strategy* strategy = ActionFactory::createStrategy(name);
    if (strategy)
        return strategy;
    return new HealPriestStrategy(ai);
}
