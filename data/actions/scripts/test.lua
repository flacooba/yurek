
function onUse(cid, item, frompos, itemEx, topos)
	if(item.itemid == 2120 and itemEx.uid ~= 0) then
		local itemExPos = getThingPos(itemEx.uid);
		resetTempValues(cid)
		local isSlotItem = itemExPos.x == 65535 and itemExPos.y >= SLOT_HEAD and itemExPos.y <= SLOT_AMMO and itemExPos.z == 0;
		if(not isSlotItem) then
			doPlayerSendTextMessage(cid, 22, "You can imbue only items in your inventory!");
			return 1;
		end
		
		local imbuements = getImbuementsForItem(itemEx.itemid);
		if(not imbuements) then
			doPlayerSendTextMessage(cid, 22, "You cannot imbue this item!");
			return 1;
		end
		
		local freeImbuSlot = getFirstFreeImbuementSlotId(itemEx.uid);
		if(freeImbuSlot == IMBU_SLOT_INVALID) then
			doPlayerSendTextMessage(cid, 22, "You cannot add more imbuements to this item!");
			return 1;
		end
		
		local imbuChannels = nil;
		for i,v in pairs(imbuements) do
			print("Has: " .. hasItemImbuement(itemEx.uid, i))
			if(hasItemImbuement(itemEx.uid, i) == 0) then
				if(imbuChannels == nil) then
					imbuChannels = {};
				end
				local name = v.name;
				if(v.description) then
					name = name .. " (" .. v.description .. ")";
				end
				imbuChannels[CHANNEL_SELECTED_IMBU[i]] = name;
			end
		end
		
		if(imbuChannels == nil) then
			doPlayerSendTextMessage(cid, 22, "There are no imbuements available for this item!");
			return 1;
		end
		
		local playerPos = getPlayerPosition(cid);
		setPlayerTempValue(cid, TEMP_VAL_POS_X, playerPos.x)
		setPlayerTempValue(cid, TEMP_VAL_POS_Y, playerPos.y)
		setPlayerTempValue(cid, TEMP_VAL_POS_Z, playerPos.z)
		setPlayerTempValue(cid, TEMP_VAL_ITEM_SLOT, itemExPos.y)
		setPlayerTempValue(cid, TEMP_VAL_ITEM_ID, itemEx.itemid)
		setPlayerTempValue(cid, TEMP_VAL_FREE_IMBU_SLOT, freeImbuSlot)
		setPlayerTempValue(cid, TEMP_VAL_LAST_CHANNEL, CHANNEL_SHOW_IMBU_LIST)
		doPlayerSendChannelsDialog(cid, imbuChannels);
		return 0;
	end
	
	if(item.itemid == 2640) then
		doPlayerSendTextMessage(cid, 22, "Temp value: set")
		doPlayerSendChannelsDialog(cid, {
			[320] = "Witaj, czlowieku",
			[321] = "milo cie widziec"
		})
	else
		setItemAttribute(item.uid, 3, 3)
		setItemAttribute(item.uid, 4, "1,1,3,3600000")
		print(setItemAttribute(item.uid, 4, "2,2,2,3600000"));
	end
	return 0
end
