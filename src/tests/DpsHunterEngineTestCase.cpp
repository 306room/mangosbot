#include "pch.h"

#include "../game/Action.h"
#include "../game/ActionBasket.h"
#include "../game/Queue.h"
#include "../game/Trigger.h"
#include "../game/Engine.h"
#include "../game/DpsHunterStrategy.h"

#include "MockPlayerbotAIFacade.h"

using namespace ai;


class DpsHunterEngineTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( DpsHunterEngineTestCase );
  CPPUNIT_TEST( pickNewTarget );
  CPPUNIT_TEST( combatVsMelee );
  CPPUNIT_TEST( summonPet );
  CPPUNIT_TEST_SUITE_END();

protected:
    MockPlayerbotAIFacade *ai;
    Engine *engine;

public:
    void setUp()
    {
        ai = new MockPlayerbotAIFacade();

        engine = new Engine(ai, new HunterActionFactory(ai));
        engine->addStrategy("dps");
        engine->Init();
    }

    void tearDown()
    {
        if (engine)
            delete engine;
        if (ai) 
            delete ai;
    }

protected:
 	void combatVsMelee()
	{
        engine->DoNextAction(NULL); // hunter's mark
        engine->DoNextAction(NULL); // concussive shot
        engine->DoNextAction(NULL); // serpent sting
        ai->targetAuras.push_back("serpent sting");
        engine->DoNextAction(NULL); // auto shot
        
        ai->distanceToEnemy = 0.0f; // enemy too close
        engine->DoNextAction(NULL); // flee
                
        ai->distanceToEnemy = 15.0f; 
        ai->resetSpells();        
        engine->DoNextAction(NULL); // arcane shot

        ai->spellCooldowns.remove("arcane shot");        
        engine->DoNextAction(NULL); // arcane shot
        ai->spellCooldowns.remove("auto shot");        
        engine->DoNextAction(NULL); // auto shot
                
        std::cout << ai->buffer;
        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">hunter's mark>serpent sting>arcane shot>auto shot>flee>concussive shot>arcane shot>auto shot"));

	}

    void pickNewTarget()
    {
        ai->spellCooldowns.push_back("auto shot");
        ai->spellCooldowns.push_back("serpent sting");
        ai->spellCooldowns.push_back("concussive shot"); // this will not be available as we do not have any target
        ai->spellCooldowns.push_back("hunter's mark");
        ai->myAttackerCount = 0;
        engine->DoNextAction(NULL); // attack least threat
        ai->myAttackerCount = 1;
        ai->resetSpells();
        engine->DoNextAction(NULL); // concussive shot

        std::cout << ai->buffer;
        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">attack least threat>serpent sting"));

    }

    void summonPet()
    {
        ai->hasPet = FALSE;
        engine->DoNextAction(NULL);

        ai->hasPet = TRUE;
        ai->petHealth = 0;
        engine->DoNextAction(NULL);
        engine->DoNextAction(NULL);

        ai->petHealth = 1;
        engine->DoNextAction(NULL);

        std::cout << ai->buffer;
        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">call pet>revive pet>mend pet>hunter's mark"));
    }

};

CPPUNIT_TEST_SUITE_REGISTRATION( DpsHunterEngineTestCase );
