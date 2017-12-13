-- =======================================================================
--                                 Player 3
-- =======================================================================
-- Player 3 (Vesta) will be introduced to the game when discovered
-- So, for the beginning we forbid everything and do not place any buildings

p3:forbid_buildings("all")

-- =======================================================================
--                                 Player 1
-- =======================================================================

p1:forbid_buildings("all")
p1:allow_buildings{"empire_lumberjacks_house"}

prefilled_buildings(p1, { "empire_headquarters", sf.x, sf.y,
   workers = {
      empire_builder = 10,
       empire_geologist = 1,
      },
   })

-- Lower resources to zero in starting region (to cope with default ressources)
for x=7, 42 do
   for y=190, 207 do
      local field = map:get_field(x,y)
      if field.resource == "fish" or field.resource == "water" then
	 field.resource_amount = 0
      end
   end
   for y=0, 22 do
      local field = map:get_field(x,y)
      if field.resource == "fish" or field.resource == "water" then
	 field.resource_amount = 0
      end
      end
   end

-- Place towers and fortress
place_building_in_region(p1, "empire_tower", {map:get_field(10, 196)})
place_building_in_region(p1, "empire_tower", {map:get_field(24, 190)})
place_building_in_region(p1, "empire_tower", {map:get_field(16, 18)})
place_building_in_region(p1, "empire_tower", {map:get_field(28, 24)})
place_building_in_region(p1, "empire_tower", {map:get_field(35, 202)})
place_building_in_region(p1, "empire_fortress", {map:get_field(31, 5)})


-- Place farm
place_building_in_region(p1, "empire_farm1", {map:get_field(20, 194)})
place_building_in_region(p1, "empire_farm1", sf:region(15))

-- Place fishers_house
place_building_in_region(p1, "empire_fishers_house", {map:get_field(12, 203)})
place_building_in_region(p1, "empire_fishers_house", {map:get_field(12, 15)})

-- Place well
place_building_in_region(p1, "empire_well2", sf:region(15))
place_building_in_region(p1, "empire_well2", sf:region(15))

-- Place lumberjacks
place_building_in_region(p1, "empire_lumberjacks_house2", sf:region(10))
place_building_in_region(p1, "empire_lumberjacks_house2", sf:region(10))
place_building_in_region(p1, "empire_lumberjacks_house2", sf:region(10))
place_building_in_region(p1, "empire_foresters_house2", {map:get_field(19, 190)})
place_building_in_region(p1, "empire_foresters_house2", {map:get_field(19, 198)})

-- Mines
place_building_in_region(p1, "empire_ironmine", {map:get_field(33, 194)})
place_building_in_region(p1, "empire_coalmine", {map:get_field(24, 17)})
place_building_in_region(p1, "empire_coalmine", {map:get_field(31, 20)})
place_building_in_region(p1, "empire_goldmine", sf:region(25))

-- Place quarry
place_building_in_region(p1, "empire_quarry", sf:region(9))
place_building_in_region(p1, "empire_quarry", sf:region(9))

-- Place build material infrastructure
place_building_in_region(p1, "empire_sawmill", sf:region(8))
place_building_in_region(p1, "empire_stonemasons_house", sf:region(8))

-- Place metal industry
place_building_in_region(p1, "empire_armorsmithy", sf:region(10))
place_building_in_region(p1, "empire_toolsmithy", sf:region(10))
place_building_in_region(p1, "empire_weaponsmithy", sf:region(15))
place_building_in_region(p1, "empire_smelting_works", sf:region(15))

-- Food supply
place_building_in_region(p1, "empire_bakery", sf:region(15))
place_building_in_region(p1, "empire_brewery1", sf:region(15))
place_building_in_region(p1, "empire_winery", sf:region(15))
place_building_in_region(p1, "empire_mill1", sf:region(15))
place_building_in_region(p1, "empire_tavern", sf:region(15))

-- Military training
place_building_in_region(p1, "empire_arena", sf:region(20))
place_building_in_region(p1, "empire_trainingcamp1", sf:region(20))
place_building_in_region(p1, "empire_barracks", sf:region(20), {inputs = {empire_recruit = 8}})


-- Helper function for placing a road
function try_place_road_with_carrier(x, y)
   local field = map:get_field(x,y)
   if field.immovable and field.immovable.descr.type_name == "flag" then
      local n1 = field.bln
      local n2 = n1.bln
      if n1.immovable == nil and n1:has_caps("walkable") and (n2:has_caps("flag") or (n2.immovable and n2.immovable.descr.type_name == "flag")) then
         local road = p1:place_road(field.immovable, "bl", "bl", true)
         road:set_workers('empire_carrier',1)
      end
   end
end

-- Roads
for x=7, 35 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=5, 35, 3 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=6, 35, 3 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=7, 35 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=7, 35 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=7, 35 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=7, 35 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=7, 35 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=7, 35 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=7, 35 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end

for x=7, 35 do
   for y=180, 207 do
      try_place_road_with_carrier(x, y)
   end
   for y=0, 25 do
      try_place_road_with_carrier(x, y)
   end
end


-- =======================================================================
--                                 Player 2
-- =======================================================================
p2:forbid_buildings("all")
p2:allow_buildings{
   "barbarians_bakery",
   "barbarians_barrier",
   "barbarians_farm",
   "barbarians_fishers_hut",
   "barbarians_gamekeepers_hut",
   "barbarians_hunters_hut",
   "barbarians_lime_kiln",
   "barbarians_lumberjacks_hut",
   "barbarians_micro_brewery",
   "barbarians_rangers_hut",
   "barbarians_reed_yard",
   "barbarians_sentry",
   "barbarians_tower",
   "barbarians_well",
   "barbarians_wood_hardener",
   "barbarians_quarry",
   "barbarians_coalmine",
   "barbarians_ironmine",
   "barbarians_goldmine",
   "barbarians_granitemine",
   "barbarians_coalmine_deep",
   "barbarians_ironmine_deep",
   "barbarians_goldmine_deep",
   "barbarians_coalmine_deeper",
   "barbarians_ironmine_deeper",
   "barbarians_goldmine_deeper",
   "barbarians_port",
   "barbarians_cattlefarm",
   "barbarians_charcoal_kiln",
   "barbarians_brewery",
   "barbarians_tavern",
   "barbarians_inn",
   "barbarians_smelting_works",
   "barbarians_metal_workshop",
   "barbarians_ax_workshop",
   "barbarians_warmill",
   "barbarians_helmsmithy",
   "barbarians_barracks",
   "barbarians_battlearena",
   "barbarians_trainingcamp",
   "barbarians_big_inn",
   "barbarians_scouts_hut",
   "barbarians_citadel",
   "barbarians_tower",
   "barbarians_fortress",
}

prefilled_buildings(p2,
   {"barbarians_headquarters", 85, 104,
   wares = {
      ax = 6,
      blackwood = 32,
      barbarians_bread = 8,
      bread_paddle = 2,
      cloth = 5,
      coal = 12,
      fire_tongs = 2,
      fish = 6,
      fishing_rod = 2,
      gold = 4,
      granite = 40,
      grout = 12,
      hammer = 12,
      hunting_spear = 2,
      iron = 12,
      iron_ore = 5,
      kitchen_tools = 4,
      log = 80,
      meal = 4,
      meat = 6,
      pick = 14,
      ration = 12,
      scythe = 6,
      shovel = 4,
      snack = 3,
      thatch_reed = 24,
   },
   workers = {
      barbarians_blacksmith = 2,
      barbarians_brewer = 1,
      barbarians_builder = 10,
      barbarians_carrier = 40,
      barbarians_charcoal_burner = 1,
      barbarians_gardener = 1,
      barbarians_geologist = 4,
      barbarians_lime_burner = 1,
      barbarians_lumberjack = 3,
      barbarians_miner = 4,
      barbarians_ranger = 1,
      barbarians_stonemason = 2,
   },

    soldiers = {
       [{0,0,0,0}] = 45,
    }
  }
)
