/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Boss_Mandokir
SD%Complete: 90
SDComment: Ohgan function needs improvements.
SDCategory: Zul'Gurub
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "zulgurub.h"

enum Says
{
    SAY_AGGRO                 = 0,
    SAY_DING_KILL             = 1,
    SAY_WATCH                 = 2,
    SAY_WATCH_WHISPER         = 3,
    SAY_OHGAN_DEAD            = 4,
    SAY_GRATS_JINDO           = 0,
};

enum Spells
{
    SPELL_CHARGE              = 24408, // seen
    SPELL_OVERPOWER           = 24407, // Seen
    SPELL_FEAR                = 29321,
    SPELL_WHIRLWIND           = 13736, // Triggers 15589
    SPELL_MORTAL_STRIKE       = 16856, // Seen
    SPELL_FRENZY              = 24318, // seen
    SPELL_WATCH               = 24314, // seen 24315, 24316
    SPELL_WATCH_CHARGE        = 24315, // Triggers 24316
    SPELL_LEVEL_UP            = 24312  //
};

enum Events
{
    EVENT_CHECK_START         = 1,
    EVENT_STARTED             = 2,
    EVENT_OVERPOWER           = 3,
    EVENT_MORTAL_STRIKE       = 4,
    EVENT_WHIRLWIND           = 5,
    EVENT_CHECK_OHGAN         = 6,
    EVENT_WATCH_PLAYER        = 7
};

enum Misc
{
    MODEL_OHGAN_MOUNT         = 15271,
    PATH_MANDOKIR             = 492861,
    CHAINED_SPIRT_COUNT       = 20
};

Position const PosSummonChainedSpirits[CHAINED_SPIRT_COUNT] =
{
    { -12167.17f, -1979.330f, 133.0992f, 2.268928f },
    { -12262.74f, -1953.394f, 133.5496f, 0.593412f },
    { -12176.89f, -1983.068f, 133.7841f, 2.129302f },
    { -12226.45f, -1977.933f, 132.7982f, 1.466077f },
    { -12204.74f, -1890.431f, 135.7569f, 4.415683f },
    { -12216.70f, -1891.806f, 136.3496f, 4.677482f },
    { -12236.19f, -1892.034f, 134.1041f, 5.044002f },
    { -12248.24f, -1893.424f, 134.1182f, 5.270895f },
    { -12257.36f, -1897.663f, 133.1484f, 5.462881f },
    { -12265.84f, -1903.077f, 133.1649f, 5.654867f },
    { -12158.69f, -1972.707f, 133.8751f, 2.408554f },
    { -12178.82f, -1891.974f, 134.1786f, 3.944444f },
    { -12193.36f, -1890.039f, 135.1441f, 4.188790f },
    { -12275.59f, -1932.845f, 134.9017f, 0.174533f },
    { -12273.51f, -1941.539f, 136.1262f, 0.314159f },
    { -12247.02f, -1963.497f, 133.9476f, 0.872665f },
    { -12238.68f, -1969.574f, 133.6273f, 1.134464f },
    { -12192.78f, -1982.116f, 132.6966f, 1.919862f },
    { -12210.81f, -1979.316f, 133.8700f, 1.797689f },
    { -12283.51f, -1924.839f, 133.5170f, 0.069813f }
};

class boss_mandokir : public CreatureScript
{
    public: boss_mandokir() : CreatureScript("boss_mandokir") {}

        struct boss_mandokirAI : public BossAI
        {
            boss_mandokirAI(Creature* creature) : BossAI(creature, DATA_MANDOKIR) {}

            uint8 killCount;

            void Reset()
            {
                _Reset();
                killCount = 0;
                me->Mount(MODEL_OHGAN_MOUNT);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_IMMUNE_TO_NPC);
                events.ScheduleEvent(EVENT_CHECK_START, 1000);
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
            }

            void Evade(Unit* /*killer*/)
            {

            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
                events.ScheduleEvent(EVENT_OVERPOWER, urand(7000, 9000));
                events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(12000, 18000));
                events.ScheduleEvent(EVENT_WHIRLWIND, urand(24000, 30000));
                events.ScheduleEvent(EVENT_CHECK_OHGAN, 1000);
                events.ScheduleEvent(EVENT_WATCH_PLAYER, urand(13000, 15000));
                me->SetSpeed(MOVE_RUN, false);
                me->GetMotionMaster()->Clear(true);
                Talk(SAY_AGGRO);
                me->Dismount();
                // Summon Ohgan (Spell missing) TEMP HACK
                me->SummonCreature(NPC_OHGAN, me->getVictim()->GetPositionX(), me->getVictim()->GetPositionY(), me->getVictim()->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 35000);
                for (int i = 0; i < CHAINED_SPIRT_COUNT; ++i)
                    me->SummonCreature(NPC_CHAINED_SPIRT, PosSummonChainedSpirits[i], TEMPSUMMON_CORPSE_DESPAWN);
            }

            void KilledUnit(Unit* victim)
            {
                if (victim->GetTypeId() == TYPEID_PLAYER)
                {
                    if (++killCount == 3 && instance)
                    {
                        Talk(SAY_DING_KILL);
                        if (uint64 JindoGUID = instance->GetData64(DATA_JINDO))
                            if (Creature* jTemp = Creature::GetCreature(*me, JindoGUID))
                                if (jTemp->isAlive())
                                    jTemp->AI()->Talk(SAY_GRATS_JINDO);
                        DoCast(me, SPELL_LEVEL_UP, true);
                        killCount = 0;
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {

                events.Update(diff);

                if (!UpdateVictim())
                    if (instance->GetBossState(DATA_MANDOKIR) == NOT_STARTED || instance->GetBossState(DATA_MANDOKIR) == SPECIAL)
                    {
                        while (uint32 eventId = events.ExecuteEvent())
                        {
                            switch (eventId)
                            {
                                case EVENT_CHECK_START:
                                    if(instance)
                                        if (instance->GetBossState(DATA_MANDOKIR) == SPECIAL)
                                        {
                                            me->SetSpeed(MOVE_RUN, true);
                                            me->GetMotionMaster()->MovePoint(0, -12197.86f, -1949.392f, 130.2745f);
                                            events.ScheduleEvent(EVENT_STARTED, 3000);
                                        }
                                        else
                                            events.ScheduleEvent(EVENT_CHECK_START, 1000);
                                    break;
                                case EVENT_STARTED:
                                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_IMMUNE_TO_NPC);
                                    me->SetReactState(REACT_AGGRESSIVE);
                                    me->GetMotionMaster()->MovePath(PATH_MANDOKIR,false);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_OVERPOWER:
                            DoCastVictim(SPELL_OVERPOWER, true);
                            events.ScheduleEvent(EVENT_OVERPOWER, urand(6000, 12000));
                            break;
                        case EVENT_MORTAL_STRIKE:
                            if (me->getVictim() && me->getVictim()->HealthBelowPct(50))
                                DoCastVictim(SPELL_MORTAL_STRIKE, true);
                            events.ScheduleEvent(EVENT_MORTAL_STRIKE, urand(12000, 18000));
                            break;
                        case EVENT_WHIRLWIND:
                            DoCast(me, SPELL_WHIRLWIND);
                            events.ScheduleEvent(EVENT_WHIRLWIND, urand(22000, 26000));
                            break;
                        case EVENT_CHECK_OHGAN:
                            if (instance)
                                if (instance->GetBossState(DATA_OHGAN) == DONE)
                                {
                                    DoCast(me, SPELL_FRENZY);
                                    Talk(SAY_OHGAN_DEAD);
                                }
                                else
                                    events.ScheduleEvent(EVENT_CHECK_OHGAN, 1000);
                            break;
                        case EVENT_WATCH_PLAYER:
                            if (Unit* player = SelectTarget(SELECT_TARGET_RANDOM, 0, 40, true))
                            {
                                DoCast(player, SPELL_WATCH);
                                Talk(SAY_WATCH, player->GetGUID());
                                // Not complete
                            }
                            events.ScheduleEvent(EVENT_WATCH_PLAYER, urand(12000, 15000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_mandokirAI(creature);
        }
};

// Ohgan

enum O_Spells
{
    SPELL_SUNDERARMOR         = 24317
};

class mob_ohgan : public CreatureScript
{
    public: mob_ohgan() : CreatureScript("mob_ohgan") {}

        struct mob_ohganAI : public ScriptedAI
        {
            mob_ohganAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
            }

            uint32 SunderArmor_Timer;
            InstanceScript* instance;

            void Reset()
            {
                SunderArmor_Timer = 5000;
            }

            void EnterCombat(Unit* /*who*/) {}

            void JustDied(Unit* /*killer*/)
            {
                if (instance)
                    instance->SetBossState(DATA_OHGAN, DONE);
            }

            void UpdateAI (const uint32 diff)
            {
                //Return since we have no target
                if (!UpdateVictim())
                    return;

                //SunderArmor_Timer
                if (SunderArmor_Timer <= diff)
                {
                    DoCastVictim(SPELL_SUNDERARMOR, true);
                    SunderArmor_Timer = urand(10000, 15000);
                } else SunderArmor_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ohganAI(creature);
        }
};

// Vilebranch Speaker

enum VS_Spells
{
    SPELL_DEMORALIZING_SHOUT  = 13730,
    SPELL_CLEAVE              = 15284
};

class mob_vilebranch_speaker : public CreatureScript
{
    public: mob_vilebranch_speaker() : CreatureScript("mob_vilebranch_speaker") {}

        struct mob_vilebranch_speakerAI : public ScriptedAI
        {
            mob_vilebranch_speakerAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = me->GetInstanceScript();
            }

            uint32 demoralizing_Shout_Timer;
            uint32 cleave_Timer;
            InstanceScript* instance;

            void Reset()
            {
                demoralizing_Shout_Timer = urand (2000, 4000);
                cleave_Timer = urand (5000, 8000);
            }

            void EnterCombat(Unit* /*who*/) {}

            void JustDied(Unit* /*killer*/)
            {
                if (instance)
                    instance->SetBossState(DATA_MANDOKIR, SPECIAL);
            }

            void UpdateAI (const uint32 diff)
            {
                // Return since we have no target
                if (!UpdateVictim())
                    return;

                // demoralizing_Shout_Timer
                if (demoralizing_Shout_Timer <= diff)
                {
                    DoCast(me, SPELL_DEMORALIZING_SHOUT);
                    demoralizing_Shout_Timer = urand(22000, 30000);
                } else demoralizing_Shout_Timer -= diff;

                // cleave_Timer
                if (cleave_Timer <= diff)
                {
                    DoCastVictim(SPELL_CLEAVE, true);
                    cleave_Timer = urand(6000, 9000);
                } else cleave_Timer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_vilebranch_speakerAI(creature);
        }
};

void AddSC_boss_mandokir()
{
    new boss_mandokir();
    new mob_ohgan();
    new mob_vilebranch_speaker();
}