#pragma once

#include "Action.h"
#include "GenericSpellActions.h"
#include "ReachTargetActions.h"
#include "ChooseTargetActions.h"

namespace ai
{
    class FleeAction : public Action {
    public:
        FleeAction(AiManagerRegistry* const ai, float distance = SPELL_DISTANCE) : Action(ai, "flee") {
			this->distance = distance;
		}

        virtual bool ExecuteResult() 
		{
            return ai->GetMoveManager()->Flee(ai->GetTargetManager()->GetCurrentTarget(), distance); 
        }

	private:
		float distance;
    };

    class FollowAction : public Action {
    public:
        FollowAction(AiManagerRegistry* const ai) : Action(ai, "follow") {}
        virtual void Execute();
    };

    class StayAction : public Action {
    public:
        StayAction(AiManagerRegistry* const ai) : Action(ai, "stay") {}
        virtual void Execute();
    };

    class GoAwayAction : public Action {
    public:
        GoAwayAction(AiManagerRegistry* const ai) : Action(ai, "goaway") {}
        virtual void Execute() 
		{
			ai->GetMoveManager()->Flee(ai->GetTargetManager()->GetMaster()); 
        }
    };

    class MeleeAction : public Action {
    public:
        MeleeAction(AiManagerRegistry* const ai) : Action(ai, "melee") {}
        virtual void Execute() {
            ai->GetMoveManager()->Attack(ai->GetTargetManager()->GetCurrentTarget());
        }
    };

    class UseHealingPotion : public Action {
    public:
        UseHealingPotion(AiManagerRegistry* const ai) : Action(ai, "healing potion") {}
        virtual void Execute() {
            ai->GetInventoryManager()->UseHealingPotion(); 
        }
        virtual bool isPossible() {
            return ai->GetInventoryManager()->HasHealingPotion();
        }
    };

    class UseManaPotion : public Action {
    public:
        UseManaPotion(AiManagerRegistry* const ai) : Action(ai, "mana potion") {}
        virtual void Execute() {
            ai->GetInventoryManager()->UseManaPotion(); 
        }
        virtual bool isPossible() {
            return ai->GetInventoryManager()->HasManaPotion();
        }
    };

    class UsePanicPotion : public Action {
    public:
        UsePanicPotion(AiManagerRegistry* const ai) : Action(ai, "panic potion") {}
        virtual void Execute() {
            ai->GetInventoryManager()->UsePanicPotion(); 
        }
        virtual bool isPossible() {
            return ai->GetInventoryManager()->HasPanicPotion();
        }
    };

	class UseItemAction : public Action {
	public:
		UseItemAction(AiManagerRegistry* const ai, const char* name) : Action(ai, name) {}
		virtual void Execute() {
			ai->GetInventoryManager()->FindAndUse(getName()); 
		}
        virtual bool isUseful();
		virtual bool isPossible() {
			return ai->GetInventoryManager()->GetItemCount(getName()) > 0;
		}
	};

    class LootAction : public Action {
    public:
        LootAction(AiManagerRegistry* const ai) : Action(ai, "loot") {}
        virtual void Execute() {
            ai->GetInventoryManager()->DoLoot();
        }
    };

    class EmoteAction : public Action
    {
    public:
        EmoteAction(AiManagerRegistry* const ai, uint32 type) : Action(ai, "emote") {
            this->type = type;
        }

        virtual void Execute() {
            ai->GetSocialManager()->Emote(type ? type : rand() % 450);
        }

    protected:
        uint32 type;
    };
    
    class ChangeCombatStrategyAction : public Action {
    public:
        ChangeCombatStrategyAction(AiManagerRegistry* const ai, const char* name);
        virtual void Execute();

    private:
        string name;
    };
}