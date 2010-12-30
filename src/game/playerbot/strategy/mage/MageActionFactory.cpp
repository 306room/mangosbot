#include "../../../pchdef.h"
#include "../../playerbot.h"
#include "MageActions.h"
#include "MageActionFactory.h"
#include "FrostMageStrategy.h"
#include "GenericMageNonCombatStrategy.h"
#include "FireMageStrategy.h"
#include "../PullStrategy.h"

using namespace ai;

Strategy* MageActionFactory::createStrategy(const char* name)
{
    if (!strcmp("frost", name))
        return new FrostMageStrategy(ai);

    if (!strcmp("fire", name))
        return new FireMageStrategy(ai);

    if (!strcmp("nc", name))
        return new GenericMageNonCombatStrategy(ai);

    if (!strcmp("pull", name))
        return new PullStrategy(ai, "shoot");

    Strategy* strategy = ActionFactory::createStrategy(name);
    if (strategy)
        return strategy;
    return new GenericMageStrategy(ai);
}
