#include "pchdef.h"
#include "GenericTriggers.h"
#include "Unit.h"
#include "PlayerbotAIFacade.h"

using namespace ai;

BOOL EnemyTooCloseTrigger::IsActive()
{
    float distance = ai->GetDistanceToEnemy(ATTACK_DISTANCE + 1);
    return distance <= ATTACK_DISTANCE;
}

BOOL EnemyOutOfMeleeTrigger::IsActive()
{
    float distance = ai->GetDistanceToEnemy();
    return distance > ATTACK_DISTANCE;
}

BOOL EnemyOutOfSpellRangeTrigger::IsActive()
{
    float distance = ai->GetDistanceToEnemy();
    return distance > SPELL_DISTANCE; // TODO: externalize
}

BOOL RageAvailable::IsActive()
{
    return ai->GetRage() >= amount;
}

BOOL LowHealthTrigger::IsActive()
{
    return ai->GetHealthPercent() < EAT_DRINK_PERCENT;
}

BOOL PartyMemberLowHealthTrigger::IsActive()
{
    return ai->GetPartyMinHealthPercent() < EAT_DRINK_PERCENT;
}

BOOL LowManaTrigger::IsActive()
{
    return ai->GetManaPercent() != 0 && ai->GetManaPercent() < EAT_DRINK_PERCENT;
}

BOOL LoseAggroTrigger::IsActive()
{
    return !ai->HasAggro();
}

BOOL AttackerCountTrigger::IsActive()
{
    return ai->GetAttackerCount() >= amount;
}


BOOL PanicTrigger::IsActive()
{
    return ai->GetHealthPercent() < 25 && ai->GetManaPercent() != 0 && ai->GetManaPercent() < 25;
}

BOOL BuffTrigger::IsActive()
{
    return !ai->HasAura(spell);
}


BOOL BuffOnPartyTrigger::IsActive()
{
    return !ai->IsAllPartyHasAura(spell);
}

BOOL NoAttackersTrigger::IsActive()
{
    return !ai->HaveTarget() && (ai->GetAttackerCount() > 0 && ai->GetMyAttackerCount() == 0);
}

BOOL DebuffTrigger::IsActive()
{
    return !ai->TargetHasAura(spell) && ai->canCastSpell(spell);
}

BOOL SpellAvailableTrigger::IsActive()
{
    return ai->canCastSpell(spell);
}

BOOL LootAvailableTrigger::IsActive()
{   
    return ai->CanLoot();
}