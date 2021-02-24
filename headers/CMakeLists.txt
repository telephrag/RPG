
target_sources(rpg
    PUBLIC
        Creature.hh
        Dungeon.hh
        DungeonMaster.hh
        Loot.hh      
        Party.hh
        Room.hh    
        SampleBuilder.hh
        Builder.hh
)

target_include_directories(rpg PUBLIC ${HEADERS})
