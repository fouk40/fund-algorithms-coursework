#include "EntityVector.h"

// Add an entity pointer to the collection
void EntityVector::addEntity(Entity* entity) {
    if (entity) {
        entities.push_back(entity);
    }
}

// Remove entity by ID (returns true if found and removed)
bool EntityVector::removeEntity(const string& id) {
    for (auto it = entities.begin(); it != entities.end(); ++it) {
        if ((*it)->getId() == id) {
            delete *it;               // free memory of the entity
            entities.erase(it);
            return true;
        }
    }
    return false;
}

// Find entity by ID (returns pointer or nullptr if not found)
Entity* EntityVector::getEntity(const string& id) {
    for (Entity* e : entities) {
        if (e->getId() == id) {
            return e;
        }
    }
    return nullptr;
}

// Return reference to internal vector of entities (modifiable)
vector<Entity*>& EntityVector::getAllEntities() {
    return entities;
}

// Return reference to internal vector of entities (const version)
const vector<Entity*>& EntityVector::getAllEntities() const {
    return entities;
}

// Destructor: delete all stored entities
EntityVector::~EntityVector() {
    for (Entity* e : entities) {
        delete e;
    }
    entities.clear();
}
