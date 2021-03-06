dirname = path.dirname(__file__)

tribes:new_productionsite_type {
   msgctxt = "atlanteans_building",
   name = "atlanteans_ironmine",
   -- TRANSLATORS: This is a building name used in lists of buildings
   descname = pgettext("atlanteans_building", "Iron Mine"),
   helptext_script = dirname .. "helptexts.lua",
   icon = dirname .. "menu.png",
   size = "mine",

   buildcost = {
      log = 7,
      planks = 4,
      spidercloth = 1
   },
   return_on_dismantle = {
      log = 2,
      planks = 2
   },

   indicate_workarea_overlaps = {
      atlanteans_ironmine = false,
   },

   animations = {
      idle = {
         pictures = path.list_files(dirname .. "idle_??.png"),
         hotspot = { 50, 56 },
      },
      working = {
         pictures = path.list_files(dirname .. "idle_??.png"), -- TODO(GunChleoc): No animation yet.
         hotspot = { 50, 56 },
      },
      empty = {
         pictures = path.list_files(dirname .. "empty_??.png"),
         hotspot = { 50, 56 },
      },
   },

   aihints = {
      mines = "iron",
      prohibited_till = 1000
   },

   working_positions = {
      atlanteans_miner = 3
   },

   inputs = {
      { name = "smoked_fish", amount = 10 },
      { name = "smoked_meat", amount = 6 },
      { name = "atlanteans_bread", amount = 10 }
   },
   outputs = {
      "iron_ore"
   },

   programs = {
      work = {
         -- TRANSLATORS: Completed/Skipped/Did not start mining iron because ...
         descname = _"mining iron",
         actions = {
            -- time total: 105 + 5 x 3.6
            "return=skipped unless economy needs iron_ore",
            "consume=smoked_fish,smoked_meat:2 atlanteans_bread:2",
            "sleep=35000",
            "call=mine_produce",
            "call=mine_produce",
            "call=mine_produce",
            "call=mine_produce",
            "call=mine_produce",
            "return=no_stats"
         },
      },
      mine_produce = {
         descname = _"mining iron",
         actions = {
            "animate=working 14000",
            "mine=iron 4 100 5 2",
            "produce=iron_ore",
         }
      },
      encyclopedia = {
         -- just a dummy program to fix encyclopedia
         descname = "encyclopedia",
         actions = {
            "consume=smoked_fish,smoked_meat:2 atlanteans_bread:2",
            "produce=iron_ore:5",
         }
      },
   },

   out_of_resource_notification = {
      -- Translators: Short for "Out of ..." for a resource
      title = _"No Iron",
      heading = _"Main Iron Vein Exhausted",
      message =
         pgettext("atlanteans_building", "This iron mine’s main vein is exhausted. Expect strongly diminished returns on investment. You should consider dismantling or destroying it."),
   },
}
