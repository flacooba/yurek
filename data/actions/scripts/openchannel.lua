-- Author: The Black Boss

function checkTempValues(cid)
	local pos = getPlayerPosition(cid)
	if(getPlayerTempValue(cid, TEMP_VAL_POS_X) ~= pos.x or getPlayerTempValue(cid, TEMP_VAL_POS_Y) ~= pos.y or getPlayerTempValue(cid, TEMP_VAL_POS_Z) ~= pos.z) then
		resetTempValues(cid);
		return false;
	end
	
	local slotItem = getPlayerInventoryItem(cid, getPlayerTempValue(cid, TEMP_VAL_ITEM_SLOT));
	if(slotItem == -1) then
		resetTempValues(cid);
		return false;
	end
	
	local freeImbuSlot = getFirstFreeImbuementSlotId(slotItem.uid);
	if(slotItem.itemid ~= getPlayerTempValue(cid, TEMP_VAL_ITEM_ID) or freeImbuSlot ~= getPlayerTempValue(cid, TEMP_VAL_FREE_IMBU_SLOT)) then
		resetTempValues(cid);
		return false;
	end
	
	return true;
end

function checkConditionsForSelectedLevelChannel(cid)
	if(not checkTempValues(cid)) then
		return false;
	end
	
	local imbuId = getPlayerTempValue(cid, TEMP_VAL_IMBU_ID);
	if(imbuId == -1 or IMBUEMENTS[imbuId] == nil) then
		resetTempValues(cid);
		return false;
	end
	
	local lastChannel = getPlayerTempValue(cid, TEMP_VAL_LAST_CHANNEL);
	if(lastChannel < CHANNEL_SELECTED_IMBU[1] or lastChannel > CHANNEL_SELECTED_IMBU_LAST) then
		resetTempValues(cid);
		return false;
	end
	
	return true;
end

function onOpenChannel(cid, channelId)
	if(channelId <= CHANNEL_SHOW_IMBU_LIST) then
		return 0;
	end
	
	if(channelId >= CHANNEL_SELECTED_IMBU[1] and channelId <= CHANNEL_SELECTED_IMBU_LAST) then
		if(not checkTempValues(cid) or getPlayerTempValue(cid, TEMP_VAL_LAST_CHANNEL) ~= CHANNEL_SHOW_IMBU_LIST) then
			return 0;
		end
		local imbuId = (channelId - CHANNEL_SELECTED_IMBU[1]) + 1;
		local imbu = IMBUEMENTS[imbuId];
		if(imbu == nil) then
			return 0;
		end
		
		local levels = getPossibleImbuementLevels(cid, imbu);
		if(levels == nil) then
			doPlayerSendChannelsDialog(cid, {
				[CHANNEL_FREE[1]] = "You don't have unlocked",
				[CHANNEL_FREE[2]] = "any " .. imbu.name .. " levels"
			});
			return 0;
		end
		local channels = {}
		for i, v in pairs(levels) do

			channels[CHANNEL_SELECTED_LEVEL[i]] = IMBUEMENTS.levels[i] .. " " .. imbu.name .. " (" .. v.description .. ")";
		end
		
		setPlayerTempValue(cid, TEMP_VAL_LAST_CHANNEL, channelId)
		setPlayerTempValue(cid, TEMP_VAL_IMBU_ID, imbuId)
		
		doPlayerSendChannelsDialog(cid, channels)
		
		return 0;
	end
	
	if(channelId >= CHANNEL_SELECTED_LEVEL[1] and channelId <= CHANNEL_SELECTED_LEVEL_LAST) then
		if(not checkConditionsForSelectedLevelChannel(cid)) then
			return 0;
		end
		
		local imbu = IMBUEMENTS[getPlayerTempValue(cid, TEMP_VAL_IMBU_ID)];
		local levelId = (channelId - CHANNEL_SELECTED_LEVEL[1]) + 1;
		local level = imbu.levels[levelId];
		local channels = {
			[CHANNEL_FREE[1]] = "To imbue this item,",
			[CHANNEL_FREE[2]] = "you will need:",
		}
		local freeId = 3;
		for i,v in pairs(level.itemsNeeded) do
			channels[CHANNEL_FREE[freeId]] = v.count .. "x " .. v.name;
			freeId = freeId + 1;
		end
		channels[CHANNEL_FREE[freeId]] = " ";
		channels[CHANNEL_ACCEPT_IMBU] = "Accept.";
		channels[CHANNEL_DECLINE_IMBU] = "Decline.";
		
		doPlayerSendChannelsDialog(cid, channels);
		
		return 0;
	end
	
	if(channelId == CHANNEL_DECLINE_IMBU) then
		resetTempValues(cid)
		return 0;
	end
	
	if(channelId == CHANNEL_ACCEPT_IMBU) then
		if(not checkConditionsForSelectedLevelChannel(cid)) then
			return 0;
		end
		local imbu = IMBUEMENTS[getPlayerTempValue(cid, TEMP_VAL_IMBU_ID)];
		local levelId = (channelId - CHANNEL_SELECTED_LEVEL[1]) + 1;
		local level = imbu.levels[levelId];
		local success = true;
		
		for i,v in pairs(level.itemsNeeded) do
			--todo
			local count = getPlayerItemCount(cid, v.id);
			if(count < v.count) then
				success = false;
				break;
			end
		end
		if(not success) then
			sendPlayerTextWindow(cid, 2640, "You don't have all of the required items for this imbuement!", "Imbuing Shrine");
			resetTempValues(cid);
			return 0;
		end
		
		for i,v in pairs(level.itemsNeeded) do
			--todo
			doPlayerRemoveItem(cid, v.id, v.count);
		end
		
		--todo: szansa na udanie
		
		sendPlayerTextWindow(cid, 2640, "!", "Imbuing Shrine");
		
		return 0;
	end
	

	
	print("Channel opened: " .. channelId .. " by: " .. cid)
	doPlayerSendTextMessage(cid, 22, "xD")
   	return 0
end
