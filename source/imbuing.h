#ifdef TBB_IMBUING
#ifndef IMBUING_H
#define IMBUING_H

#include "const76.h"
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

struct ImbuementLevel {
    int32_t id;
    std::string name;
};

struct Imbuement {
    Imbuement() {
        duration = 3600;
    }
    Imbuement(Imbuement& cpy) {
        id = cpy.id;
        name = cpy.name;
        value = cpy.value;
        level = cpy.level;
        duration = cpy.duration;
    }
    int32_t id;
    std::string name;
    const ImbuementLevel* level;
    int32_t value;
    int32_t duration;
};

typedef std::map<int32_t, std::vector<Imbuement*> > ImbuementsMap;
typedef std::map<int32_t, ImbuementLevel*> ImbuementLevelsMap;

class Imbuements {
    ImbuementsMap imbuements;
    ImbuementLevelsMap imbuementLevels;

public:
    bool loadFromXml(const std::string& datadir);
    ImbuementsMap& getImbuements() { return imbuements; }
    Imbuement* getImbuement(int32_t id, int32_t level) {
        if(imbuements.find(id) != imbuements.end()) {
            std::vector<Imbuement*>& vec = imbuements[id];

            if(level < 0 || level >= vec.size()) {
                return NULL;
            } else {
                Imbuement* found = vec[level];
                return found;
            }
        }
        return NULL;
    }

    Imbuement* createNewImbuement(int32_t id, int32_t level) {
        Imbuement* found = getImbuement(id, level);
        if(found) {
            return new Imbuement(*found);
        }
        return NULL;
    }
};



#endif //IMBUING_H
#endif // TBB_IMBUING
