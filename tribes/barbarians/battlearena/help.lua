-- The Barbarian Battle Arena"

include "scripting/formatting.lua"
include "scripting/format_help.lua"

set_textdomain("tribe_barbarians")

return {
   func = function(building_description)
	-- need to get this again, so the building description will be of type "trainingsite"
	local building_description = wl.Game():get_building_description("barbarians", building_description.name)
	return

	--Lore Section
	building_help_lore_string("barbarians", building_description, _[[‘No better friend you have in battle than the enemy’s blow that misses.’]], _[[Said to originate from Neidhardt, the famous trainer.]]) ..

	--General Section TODO string design
	building_help_general_string("barbarians", building_description, "soldier",
		_"Trains soldiers in ‘Evade’ up to level %d.":bformat(building_description.max_evade+1) .. "<br>" .. _"‘Evade’ increases the soldier’s chance not to be hit by the enemy and so to remain totally unaffected.",
		_"Barbarian soldiers cannot be trained in ‘Defense’ and will remain at the level with which they came.") ..

	--Dependencies
	-- We would need to parse the production programs to aotumate the parameters here; so we do it manually
	dependencies_training("barbarians", building_description, "untrained+evade", "fulltrained-evade") ..

	rt(h3(_"Evade Training:")) ..
	dependencies_training_food("barbarians", { {"fish", "meat"}, {"strongbeer"}, {"pittabread"}}) ..

	--Workers Section
	building_help_crew_string("barbarians", building_description) ..

	--Building Section
	building_help_building_section("barbarians", building_description) ..

	--Production Section
		rt(h2(_"Production")) ..
		text_line(_"Performance:", _"If all needed wares are delivered in time, a battle arena can train %1$s for one soldier from 0 to the highest level in %2$s on average.":bformat(_"evade",_"%1$im%2$is":bformat(1,10)))
   end
}
