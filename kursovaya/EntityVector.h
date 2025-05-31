#pragma once
#include <vector>
#include <string>
#include "Entity.h"

using std::vector;
using std::string;

// Container class to manage a collection of Entity pointers
class EntityVector {
private:
    vector<Entity*> entities;
public:
    void addEntity(Entity* entity);
    bool removeEntity(const string& id);
    Entity* getEntity(const string& id);
    vector<Entity*>& getAllEntities();
    const vector<Entity*>& getAllEntities() const;
    ~EntityVector();
};
