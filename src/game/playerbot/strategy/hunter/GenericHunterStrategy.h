#pragma once

#include "../Strategy.h"

namespace ai
{
    class ActionFactory;

    class GenericHunterStrategy : public CombatStrategy
    {
    public:
        GenericHunterStrategy(AiManagerRegistry* const ai) : CombatStrategy(ai) {}

    public:
        virtual void InitTriggers(std::list<TriggerNode*> &triggers);
        virtual ActionFactory* createActionFactory();
        virtual const char* getName() { return "hunter"; }
        virtual ActionNode* createAction(const char* name);
    };
}

