#include "pch.h"

#include "../game/Action.h"
#include "../game/ActionBasket.h"
#include "../game/Queue.h"
#include "../game/Trigger.h"
#include "../game/Engine.h"

#include "MockPlayerbotAIFacade.h"

using namespace ai;


class NonCombatEngineTestCase : public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE( NonCombatEngineTestCase );
  CPPUNIT_TEST( followMaster );
  CPPUNIT_TEST( stayIfAttackers );
  CPPUNIT_TEST( stay );
  CPPUNIT_TEST( eatDrink );
  CPPUNIT_TEST( assist );
  CPPUNIT_TEST( tankNonCombat );
  CPPUNIT_TEST( loot );
  CPPUNIT_TEST( goaway );
  CPPUNIT_TEST( emote );
  CPPUNIT_TEST_SUITE_END();

protected:
    MockPlayerbotAIFacade *ai;

public:
	void setUp()
	{
	}

protected:
    void goaway() 
    {
        ai = new MockPlayerbotAIFacade();

        Engine engine(ai, new ActionFactory(ai));
        engine.addStrategy("goaway");
        engine.Init();

        ai->attackerCount = 0;
        engine.DoNextAction(NULL);
        std::cout << ai->buffer;

        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">goaway"));
    }
    void stayIfAttackers()
    {
        ai = new MockPlayerbotAIFacade();

        Engine engine(ai, new ActionFactory(ai));
        engine.addStrategy("follow");
        engine.addStrategy("assist");
        engine.Init();

        ai->attackerCount = 1;
        engine.DoNextAction(NULL);
        std::cout << ai->buffer;

        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), "")); // nothing to do
    }
    void followMaster()
    {
        ai = new MockPlayerbotAIFacade();

        Engine engine(ai, new ActionFactory(ai));
        engine.addStrategy("follow");
        engine.Init();

        ai->attackerCount = 0;
        engine.DoNextAction(NULL);
        std::cout << ai->buffer;
        
        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">follow"));
    }

    void stay()
    {
        ai = new MockPlayerbotAIFacade();

        Engine engine(ai, new ActionFactory(ai));
        engine.addStrategy("stay");
        engine.Init();

        engine.DoNextAction(NULL);
        std::cout << ai->buffer;

        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">stay"));
    }

    void assist()
    {
        ai = new MockPlayerbotAIFacade();

        Engine engine(ai, new ActionFactory(ai));
        engine.addStrategy("stay");
        engine.addStrategy("assist");
        engine.Init();

        engine.DoNextAction(NULL);
        ai->myAttackerCount = 0;
        engine.DoNextAction(NULL);
        ai->myAttackerCount = 1;

        std::cout << ai->buffer;

        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">stay>attack least threat"));
    }

    void loot()
    {
        ai = new MockPlayerbotAIFacade();

        Engine engine(ai, new ActionFactory(ai));
        engine.addStrategy("stay");
        engine.addStrategy("loot");
        engine.Init();

        ai->lootAvailable = true;
        engine.DoNextAction(NULL);
        ai->lootAvailable = false;
        engine.DoNextAction(NULL);
        ai->myAttackerCount = 1;

        std::cout << ai->buffer;

        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">loot>stay"));
    }

    void tankNonCombat()
    {
        ai = new MockPlayerbotAIFacade();

        Engine engine(ai, new ActionFactory(ai));
        engine.addStrategy("stay");
        engine.addStrategy("tank non combat");
        engine.Init();

        engine.DoNextAction(NULL);
        ai->myAttackerCount = 0;
        engine.DoNextAction(NULL);
        ai->myAttackerCount = 1;

        std::cout << ai->buffer;

        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">stay>attack bigger threat"));
    }

    void eatDrink()
    {
        ai = new MockPlayerbotAIFacade();

        Engine engine(ai, new ActionFactory(ai));
        engine.addStrategy("follow");
        engine.Init();

        ai->health = 1;
        ai->mana = 1;
        engine.DoNextAction(NULL);
        ai->auras.push_back("eat");
        engine.DoNextAction(NULL);
        ai->auras.push_back("drink");
        std::cout << ai->buffer;

        CPPUNIT_ASSERT(!strcmp(ai->buffer.c_str(), ">eat>drink"));
    }

    void emote()
    {
        ai = new MockPlayerbotAIFacade();

        Engine engine(ai, new ActionFactory(ai));
        engine.addStrategy("emote");
        engine.Init();

        for (int i=0; i<100; i++)
            engine.DoNextAction(NULL);

        std::cout << ai->buffer;
        CPPUNIT_ASSERT(strstr(ai->buffer.c_str(), ">emote"));
    }
};

CPPUNIT_TEST_SUITE_REGISTRATION( NonCombatEngineTestCase );
