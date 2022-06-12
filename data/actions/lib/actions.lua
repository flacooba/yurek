-- Nothing --
 
--slots
SLOT_HEAD=1
SLOT_NECKLACE=2
SLOT_BACKPACK=3
SLOT_ARMOR=4
SLOT_RIGHT=5
SLOT_LEFT=6
SLOT_LEGS=7
SLOT_FEET=8
SLOT_RING=9
SLOT_AMMO=10

--channels
CHANNEL_SHOW_IMBU_LIST = 300

--[imbu_id] = channel_id
CHANNEL_FREE = {[1] = 282}
CHANNEL_FREE[2] = CHANNEL_FREE[1] + 1
CHANNEL_FREE[3] = CHANNEL_FREE[2] + 1
CHANNEL_FREE[4] = CHANNEL_FREE[3] + 1
CHANNEL_FREE[5] = CHANNEL_FREE[4] + 1
CHANNEL_FREE[6] = CHANNEL_FREE[5] + 1
CHANNEL_FREE[7] = CHANNEL_FREE[6] + 1
CHANNEL_FREE[8] = CHANNEL_FREE[7] + 1
CHANNEL_FREE[9] = CHANNEL_FREE[8] + 1
CHANNEL_FREE[10] = CHANNEL_FREE[9] + 1
CHANNEL_FREE[11] = CHANNEL_FREE[10] + 1
CHANNEL_FREE[12] = CHANNEL_FREE[11] + 1

CHANNEL_SELECTED_IMBU = {
	[1] = 310,
	[2] = 311,
	[3] = 312,
	[4] = 313,
	[5] = 314,
	[6] = 315
}
CHANNEL_SELECTED_IMBU_LAST = CHANNEL_SELECTED_IMBU[6]

CHANNEL_SELECTED_LEVEL = {
	[1] = 340,
	[2] = 341,
	[3] = 342,
	[4] = 343
}
CHANNEL_SELECTED_LEVEL_LAST = CHANNEL_SELECTED_LEVEL[4]

CHANNEL_ACCEPT_IMBU = 350
CHANNEL_DECLINE_IMBU = 351

--temp values
TEMP_VAL_POS_X = 100
TEMP_VAL_POS_Y = 101
TEMP_VAL_POS_Z = 102
TEMP_VAL_ITEM_SLOT = 103
TEMP_VAL_ITEM_ID = 104
TEMP_VAL_FREE_IMBU_SLOT = 105
TEMP_VAL_LAST_CHANNEL = 106
TEMP_VAL_IMBU_ID = 107

IMBUEMENTS = {
	levels = {
		[1] = "Basic",
		[2] = "Intricate",
		[3] = "Powerful"
	},
	[1] = {
		name = "Void",
		description = "Mana Leech",
		acceptedItems = {2471},
		levels = {
			[1] = {
				description = "2% leech, 80% chance",
				itemsNeeded = {{id = 2640, name = "soft boots", count = 1}, {id = 2641, name = "traper boots", count = 1}},
				req = {
					storages = {
						{id = 4444, value = 1}
					}
				},
				chance = 90,
			},
			[2] = {
				description = "4% leech, 70% chance",
				itemsNeeded = {{id = 2640, name = "soft boots", count = 3}, {id = 2641, name = "traper boots", count = 1}},
				req = {
					storages = {
						{id = 4444, value = 1},
						{id = 4445, value = 1}
					}
				},
				chance = 90,
			}
		}
	},
	[2] = {
		name = "Vampirism",
		description = "Life Leech",
		acceptedItems = {2471},
		levels = {
			[1] = {
				description = "3% leech, 80% chance",
				itemsNeeded = {{id = 2640, name = "soft boots", count = 1}, {id = 2641, name = "traper boots", count = 1}},
				req = {
					storages = {
						{id = 4444, value = 1}
					}
				},
				chance = 90,
			},
			[2] = {
				description = "5% leech, 80% chance",
				itemsNeeded = {{id = 2640, name = "soft boots", count = 3}, {id = 2641, name = "traper boots", count = 1}},
				req = {
					storages = {
						{id = 4444, value = 1},
						{id = 4445, value = 1}
					}
				},
				chance = 90,
			}
		}
	}
}

IMBU_ID_LAST = 3
IMBU_LEVEL_LAST = 3
IMBU_SLOT_INVALID = 255


--help functions
if(not table) then
	table = {}
end

function table.find(t, value, lowercase)
  for k,v in pairs(t) do
    if lowercase and type(value) == 'string' and type(v) == 'string' then
      if v:lower() == value:lower() then return k end
    end
    if v == value then return k end
  end
end

function table.contains(t, value, lowercase)
  return table.find(t, value, lowercase) ~= nil
end

function getImbuementsForItem(itemId)
	local ret = nil;
	for i = 1, IMBU_ID_LAST do
		local imbu = IMBUEMENTS[i];
		if(imbu ~= nil) then
			if(table.contains(imbu.acceptedItems, itemId)) then
				if(ret == nil) then
					ret = {}
				end
				ret[i] = imbu;
			end
		end
	end
	return ret;
end

function getPossibleImbuementLevels(player, imbu)
	local ret = nil;

	for x = 1, IMBU_LEVEL_LAST do 
		local level = imbu.levels[x];
		if(level ~= nil) then
			local meetsReq = true;
			if(level.req.storages) then
				for _, storage in pairs(level.req.storages) do
					if(getPlayerStorageValue(player, storage.id) ~= storage.value) then
						meetsReq = false;
						break;
					end
				end
			end
			if(meetsReq) then
				if(ret == nil) then
					ret = {};
				end
				ret[x] = level;
			end
		end
	end
	return ret;
end

function resetTempValues(cid)
	setPlayerTempValue(cid, TEMP_VAL_POS_X, -1)
	setPlayerTempValue(cid, TEMP_VAL_POS_Y, -1)
	setPlayerTempValue(cid, TEMP_VAL_POS_Z, -1)
	setPlayerTempValue(cid, TEMP_VAL_ITEM_SLOT, -1)
	setPlayerTempValue(cid, TEMP_VAL_ITEM_ID, -1)
	setPlayerTempValue(cid, TEMP_VAL_FREE_IMBU_SLOT, -1)
	setPlayerTempValue(cid, TEMP_VAL_LAST_CHANNEL, -1)
	setPlayerTempValue(cid, TEMP_VAL_IMBU_ID, -1)
end
