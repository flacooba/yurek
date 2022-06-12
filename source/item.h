//////////////////////////////////////////////////////////////////////
// OpenTibia - an opensource roleplaying game
//////////////////////////////////////////////////////////////////////
// Item represents an existing item.
//////////////////////////////////////////////////////////////////////
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//////////////////////////////////////////////////////////////////////


#ifndef __OTSERV_ITEM_H
#define __OTSERV_ITEM_H

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <iostream>
#include <list>
#include <vector>
#include <sstream>

#include "thing.h"
#include "items.h"
#ifdef TBB_IMBUING
#include "imbuing.h"
#endif // TBB_IMBUING


class Creature;
class Player;


#ifdef TBB_IMBUING
struct ItemAttributes {
    ItemAttributes() {
        imbuementSlots = NULL;
        imbuements = NULL;
    }
    ItemAttributes(ItemAttributes& cpy) {
        if(cpy.imbuementSlots)
            imbuementSlots = new uint8_t(*cpy.imbuementSlots);

        if(cpy.imbuements) {
            extern Imbuements g_imbuements;
            imbuements = new Imbuement*[*cpy.imbuementSlots];
            for(int i = 0; i < *cpy.imbuementSlots; i++) {
                Imbuement* imbu = cpy.imbuements[i];
                if(imbu) {
                    imbuements[i] = g_imbuements.createNewImbuement(imbu->id, imbu->level->id);
                    imbuements[i]->duration = imbu->duration;
                }
                else {
                    imbuements[i] = NULL;
                }
            }
        }
    }
    ~ItemAttributes() {

        if(imbuements) {
            for(int i = 0; i < *imbuementSlots; i++) {
                if(imbuements[i])
                    delete imbuements[i];
            }
            delete[] imbuements;
        }

        if(imbuementSlots)
            delete imbuementSlots;


    }
    void serialize(std::stringstream& ss) {
        if(imbuementSlots) {
            ss << (int32_t)ITEM_ATTRIBUTE_IMBU_SLOTS << '|' << (int32_t)*imbuementSlots << '|';
        }
        if(imbuements) {
            for(int i = 0; i < *imbuementSlots; i++) {
                if(imbuements[i]) {
                    ss << (int32_t)ITEM_ATTRIBUTE_IMBU_ID << '|' << imbuements[i]->id << '|';
                    ss << (int32_t)ITEM_ATTRIBUTE_IMBU_LVL << '|' << imbuements[i]->level->id << '|';
                    ss << (int32_t)ITEM_ATTRIBUTE_IMBU_DURATION << '|' << imbuements[i]->duration << '|';
                }
            }
        }
    }
    void unserialize(std::string str);
    Imbuement* getImbuement(int32_t id) {
        if(!imbuementSlots)
            return NULL;

        for(int i = 0; i < *imbuementSlots; i++) {
            if(imbuements[i] && imbuements[i]->id == id) {
                return imbuements[i];
            }
        }
        return NULL;
    }
    void setImbuementSlots(uint8_t count) {
        if(imbuementSlots) {
            uint8_t tmp = *imbuementSlots;
            *imbuementSlots = count;
            if(tmp < count) {
                Imbuement** tab = new Imbuement*[count];
                for(int i = 0; i < count; i++) {
                    if(i < tmp)
                        tab[i] = imbuements[i];
                    else
                        tab[i] = NULL;
                }
                delete[] imbuements;
                imbuements = tab;
            }
            else if(count < tmp) {
                for(int i = count; i < tmp; i++)
                    imbuements[i] = NULL;
            }
        }
        else {
            imbuementSlots = new uint8_t(count);
            imbuements = new Imbuement*[*imbuementSlots];
            for(int i = 0; i < count; i++)
                imbuements[i] = NULL;

        }
    }
    uint8_t* getImbuementSlots() {
        return imbuementSlots;
    }
    bool hasFreeImbuementSlots() {
        if(!imbuements)
            return false;

        for(int i = 0; i < *imbuementSlots; i++) {
            if(!imbuements[i])
                return true;
        }
        return false;
    }
    uint8_t getFirstFreeImbuementSlot() {
        if(imbuements) {
            for(int i = 0; i < *imbuementSlots; i++) {
                if(!imbuements[i])
                    return i;
            }
        }
        return 255;
    }
    bool addNewImbuement(uint8_t slot, int32_t id, int32_t level, unsigned long duration) {
        if(imbuementSlots != NULL && (*imbuementSlots) > slot) {
            extern Imbuements g_imbuements;
            Imbuement* newImbu = g_imbuements.createNewImbuement(id, level);
            if(newImbu == NULL)
                return false;
            newImbu->duration = duration;

            if(imbuements[slot] != NULL) {
                delete imbuements[slot];
                imbuements[slot] = NULL;
            }

            imbuements[slot] = newImbu;
            return true;
        }
        return false;
    }

    Imbuement** imbuements;
protected:
    uint8_t* imbuementSlots;
};
#endif // TBB_IMBUING

class Item : public Thing
{
protected:
	unsigned short id;  // the same id as in ItemType
	unsigned char count; // number of stacked items
	unsigned char chargecount; //number of charges on the item
	unsigned char fluid;
	unsigned short actionId;
	unsigned short uniqueId;
	std::string *specialDescription;
	std::string *text;	//text written

#ifdef YUR_RINGS_AMULETS
	int time;
	int charges;
#endif //YUR_RINGS_AMULETS
#ifdef YUR_READABLES
	std::string *readable;
#endif //YUR_READABLES
#ifdef TBB_IMBUING
    ItemAttributes* attributes;
#endif //TBB_IMBUING

private:
	int useCount;

public:
	static Item* CreateItem(const unsigned short _type, unsigned short _count = 0); //Factory member to create item of right type based on type
	static Items items;

	unsigned short getID() const;    // ID as in ItemType
	void setID(unsigned short newid);

	WeaponType getWeaponType() const;
	amu_t	getAmuType() const;
	subfight_t getSubfightType() const;
	virtual double getWeight() const;
	int getAttack() const;
	int getArmor() const;
	int getDefense() const;
	int getSlotPosition() const;
	int getRWInfo() const;
	int getWorth() const;

	bool isBlocking() const;
	bool isStackable() const;
	bool isFluidContainer() const;
	//bool isMultiType() const;
	bool isAlwaysOnTop() const;
	bool isGroundTile() const;
	bool isSplash() const;
	bool isNotMoveable() const;
	bool isPickupable() const;
	bool isWeapon() const;
	bool isUseable() const;

	bool floorChangeDown() const;
	bool floorChangeNorth() const;
	bool floorChangeSouth() const;
	bool floorChangeEast() const;
	bool floorChangeWest() const;

#ifdef YUR_RINGS_AMULETS
	 int getCharges() const { return charges; }
	 int getTime() const { return time; }
	 void useCharge() { --charges; }
	 void useTime(int thinkTicks) { time -= thinkTicks; }
	 void setGlimmer();
	 void removeGlimmer();
#endif //YUR_RINGS_AMULETS
#ifdef YUR_READABLES
	 void setReadable(const std::string& text) { readable = new std::string(text); }
#endif //YUR_READABLES
#ifdef TP_TRASH_BINS
	 bool isDeleter() const { return items[id].isDeleter; }
#endif //TP_TRASH_BINS
#ifdef YUR_CLEAN_MAP
	 bool decoration;
#endif //YUR_CLEAN_MAP

	virtual std::string getDescription(bool fullDescription) const;
	std::string getName() const ;
	void setSpecialDescription(std::string desc);
	std::string getSpecialDescription();
	void clearSpecialDescription();
	void setText(std::string desc);
	void clearText();
	std::string Item::getText();
#ifdef TBB_IMBUING
    ItemAttributes* getAttributes() {
        return attributes;
    }
    void setAttributes(ItemAttributes* attrs) {
        attributes = attrs;
    }
#endif // TBB_IMBUING

	virtual int unserialize(xmlNodePtr p);
	virtual xmlNodePtr serialize();

  // get the number of items
	unsigned short getItemCountOrSubtype() const;
	void setItemCountOrSubtype(unsigned char n);

	unsigned char getItemCharge() const {return chargecount;};
	void setItemCharge(unsigned char n) {chargecount = n;};

	unsigned char getFluidType() const {return fluid;};
	void setFluidType(unsigned char n) {fluid = n;};

	void setActionId(unsigned short n);
	unsigned short getActionId() const;

	void setUniqueId(unsigned short n);
	unsigned short getUniqueId() const;

	virtual long getDecayTime();
	bool canDecay();

	/**
	 * Called when the item is about to decay/transform to the next step.
	 * \returns The item to decay to.
	 */
	virtual Item* decay();
	bool isDecaying;

	bool rotate();

  // Constructor for items
	Item(const unsigned short _type);
	Item(const unsigned short _type, unsigned short _count);
	Item();
	Item(const Item &i);

	virtual ~Item();
	virtual void useThing() {
		//std::cout << "Item: useThing() " << this << std::endl;
		useCount++;
	};

	virtual void releaseThing() {
		useCount--;
		//std::cout << "Item: releaseThing() " << this << std::endl;
		//if (useCount == 0)
		if (useCount <= 0)
			delete this;
	};

	virtual bool canMovedTo(const Tile *tile) const;
};

class Teleport : public Item
{
public:
	Teleport(const unsigned short _type);
	virtual ~Teleport();
	virtual void useThing() {
		//std::cout << "Teleport: useThing() " << this << std::endl;
		useCount++;
	};

	virtual void releaseThing() {
		useCount--;
		//std::cout << "Teleport: releaseThing() " << this << std::endl;
		//if (useCount == 0)
		if (useCount <= 0)
			delete this;
	};

	void setDestPos(const Position &pos) {destPos = pos;};
	const Position& getDestPos() const {return destPos;};
private:
	int useCount;
	virtual int unserialize(xmlNodePtr p);
	virtual xmlNodePtr serialize();
	Position destPos;
};

#endif
