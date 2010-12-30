#include "pch.h"

#include "aitest.h"
#include "../game/playerbot/strategy/paladin/PaladinActionFactory.h"

using namespace ai;


class PaladinNonCombatTestCase : public EngineTestBase
{
    CPPUNIT_TEST_SUITE( PaladinNonCombatTestCase );
        CPPUNIT_TEST( buff );
		CPPUNIT_TEST( resurrect );
		CPPUNIT_TEST( resistances );
    CPPUNIT_TEST_SUITE_END();


public:
    virtual void setUp()
    {
		EngineTestBase::setUp();
		setupEngine(new PaladinActionFactory(ai), "nc", NULL);

        statsManager->attackerCount = 0;
    }

protected:
    void buff()
    {
        tick(); 
		statsManager->mounted = true;
		tick();

		assertActions(">P:blessing of kings>S:crusader aura");
    }

	void resurrect()
	{
		tickWithDeadPartyMember();
		assertActions(">P:redemption");
	}

	void resistances()
	{
		engine->addStrategy("rshadow");
		tick();
	
		engine->addStrategy("rfrost");
		tick();
	
		engine->addStrategy("rfire");
		tick();

		assertActions(">S:shadow resistance aura>S:frost resistance aura>S:fire resistance aura");
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( PaladinNonCombatTestCase );
