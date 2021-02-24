
target_sources(rpg
    PRIVATE
        Creature.cpp
        Dungeon.cpp
        DungeonMaster.cpp
        Loot.cpp
        Party.cpp
        Room.cpp
        SampleBuilder.cpp
        Builder.cpp
)

target_include_directories(rpg PRIVATE ${CPP})
