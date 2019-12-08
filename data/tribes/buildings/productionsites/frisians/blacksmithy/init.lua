dirname = path.dirname (__file__)

tribes:new_productionsite_type {
   msgctxt = "frisians_building",
   name = "frisians_blacksmithy",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = pgettext ("frisians_building", "Blacksmithy"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   size = "small",

   buildcost = {
      brick = 1,
      granite = 1,
      log = 1,
      reed = 2
   },
   return_on_dismantle = {
      brick = 1,
      granite = 1,
      reed = 1
   },

   spritesheets = {
      idle = {
         directory = dirname,
         basename = "idle",
         hotspot = {50, 73},
         frames = 10,
         columns = 5,
         rows = 2,
         fps = 10
      },
      working = {
         directory = dirname,
         basename = "working",
         hotspot = {50, 72}, -- one pixel higher
         frames = 10,
         columns = 5,
         rows = 2,
         fps = 10
      }
   },
   animations = {
      unoccupied = {
         directory = dirname,
         basename = "unoccupied",
         hotspot = {50, 58}
      }
   },

   aihints = {
      prohibited_till = 400,
      basic_amount = 1,
      weak_ai_limit = 2,
      very_weak_ai_limit = 1,
   },

   working_positions = {
      frisians_blacksmith = 1
   },

   inputs = {
      { name = "iron", amount = 7 },
      { name = "log", amount = 7 },
      { name = "reed", amount = 7 }
   },
   outputs = {
      "felling_ax",
      "pick",
      "scythe",
      "shovel",
      "basket",
      "hunting_spear",
      "fishing_net",
      "bread_paddle",
      "kitchen_tools",
      "hammer",
      "fire_tongs",
      "needles",
   },

   programs = {
      work = {
         -- TRANSLATORS: Completed/Skipped/Did not start working because ...
         descname = _"working",
         actions = {
            "call=produce_4",
            "call=produce_2",
            "call=produce_10",
            "call=produce_11",
            "call=produce_1",
            "call=produce_3",
            "call=produce_12",
            "call=produce_5",
            "call=produce_9",
            "call=produce_8",
            "call=produce_7",
            "call=produce_6",
            "return=no_stats",
         },
      },
      produce_1 = {
         -- TRANSLATORS: Completed/Skipped/Did not start forging a felling ax because ...
         descname = _"forging a felling ax",
         actions = {
            "return=skipped unless economy needs felling_ax",
            "consume=log iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=felling_ax"
         },
      },
      produce_2 = {
         -- TRANSLATORS: Completed/Skipped/Did not start forging a pick because ...
         descname = _"forging a pick",
         actions = {
            "return=skipped unless economy needs pick",
            "consume=log iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=pick"
         },
      },
      produce_3 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making a scythe because ...
         descname = _"making a scythe",
         actions = {
            "return=skipped unless economy needs scythe",
            "consume=log iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=scythe"
         },
      },
      produce_4 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making a shovel because ...
         descname = _"making a shovel",
         actions = {
            "return=skipped unless economy needs shovel",
            "consume=log iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=shovel"
         },
      },
      produce_5 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making a basket because ...
         descname = _"making a basket",
         actions = {
            "return=skipped unless economy needs basket",
            "consume=reed log",
            "sleep=32000",
            "animate=working 35000",
            "produce=basket"
         },
      },
      produce_6 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making a hunting spear because ...
         descname = _"making a hunting spear",
         actions = {
            "return=skipped unless economy needs hunting_spear",
            "consume=log iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=hunting_spear"
         },
      },
      produce_7 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making a fishing net because ...
         descname = _"making a fishing net",
         actions = {
            "return=skipped unless economy needs fishing_net",
            "consume=reed:2",
            "sleep=32000",
            "animate=working 35000",
            "produce=fishing_net"
         },
      },
      produce_8 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making a bread paddle because ...
         descname = _"making a bread paddle",
         actions = {
            "return=skipped unless economy needs bread_paddle",
            "consume=log iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=bread_paddle"
         },
      },
      produce_9 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making kitchen tools because ...
         descname = _"making kitchen tools",
         actions = {
            "return=skipped unless economy needs kitchen_tools",
            "consume=log iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=kitchen_tools"
         },
      },
      produce_10 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making a hammer because ...
         descname = _"making a hammer",
         actions = {
            "return=skipped unless economy needs hammer",
            "consume=log iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=hammer"
         },
      },
      produce_11 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making fire tongs because ...
         descname = _"making fire tongs",
         actions = {
            "return=skipped unless economy needs fire_tongs",
            "consume=iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=fire_tongs"
         },
      },
      produce_12 = {
         -- TRANSLATORS: Completed/Skipped/Did not start making needles because ...
         descname = _"making needles",
         actions = {
            "return=skipped unless economy needs needles",
            "consume=iron",
            "sleep=32000",
            "animate=working 35000",
            "produce=needles:2"
         },
      },
   },
}
