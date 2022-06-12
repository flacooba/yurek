#ifdef TBB_IMBUING

#include "definitions.h"
#include "imbuing.h"
#include <iostream>

bool Imbuements::loadFromXml(const std::string& datadir)
{
    std::string filename = datadir + "imbuements.xml";
    std::transform(filename.begin(), filename.end(), filename.begin(), tolower);
    xmlDocPtr doc = xmlParseFile(filename.c_str());

    if(doc)
    {
        xmlNodePtr root, p, tmp;
        root = xmlDocGetRootElement(doc);
        char* nodeValue = NULL;

        if (xmlStrcmp(root->name,(const xmlChar*) "imbuements"))
        {
            //TODO: use exceptions here
            std::cerr << "Malformed XML" << std::endl;
        }

        p = root->children;

        while (p)
        {
            const char* str = (char*)p->name;

            if (strcmp(str, "levels") == 0)
            {
                tmp = p->children;
                while(tmp)
                {
                    if (strcmp((const char*)tmp->name, "level") == 0)
                    {
                        ImbuementLevel* level = new ImbuementLevel();
                        nodeValue = (char*)xmlGetProp(tmp, (const xmlChar *)"id");
                        if(nodeValue)
                        {
                            level->id = atoi(nodeValue);
                            xmlFreeOTSERV(nodeValue);
                        }
                        nodeValue = (char*)xmlGetProp(tmp, (const xmlChar *)"name");
                        if(nodeValue)
                        {
                            level->name = std::string(nodeValue);
                            xmlFreeOTSERV(nodeValue);
                        }
                        imbuementLevels[level->id] = level;
                    }
                    tmp = tmp->next;
                }
            }
            else if (strcmp(str, "imbuement") == 0)
            {
                int32_t id;
                std::string name;
                nodeValue = (char*)xmlGetProp(p, (const xmlChar *)"id");
                if(nodeValue)
                {
                    id = atoi(nodeValue);
                    xmlFreeOTSERV(nodeValue);
                }

                nodeValue = (char*)xmlGetProp(p, (const xmlChar *)"name");
                if(nodeValue)
                {
                    name = std::string(nodeValue);
                    xmlFreeOTSERV(nodeValue);
                }

                tmp = p->children;
                while(tmp)
                {
                    if (strcmp((const char*)tmp->name, "power") == 0)
                    {
                        Imbuement* imbuement = new Imbuement();
                        imbuement->id = id;
                        imbuement->name = name;
                        int32_t level;
                        int32_t value;
                        nodeValue = (char*)xmlGetProp(tmp, (const xmlChar *)"level");
                        if(nodeValue)
                        {
                            level = atoi(nodeValue);
                            xmlFreeOTSERV(nodeValue);
                        }
                        nodeValue = (char*)xmlGetProp(tmp, (const xmlChar *)"value");
                        if(nodeValue)
                        {
                            value = atoi(nodeValue);
                            xmlFreeOTSERV(nodeValue);
                        }
                        imbuement->level = imbuementLevels[level];
                        imbuement->value = value;
                        imbuements[imbuement->id].push_back(imbuement);
                    }
                    tmp = tmp->next;
                }
            }
            p = p->next;
        }
    }

    return true;
}

#endif //TBB_IMBUING
