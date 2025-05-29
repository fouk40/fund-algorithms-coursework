#pragma once
#include <string>

using std::string;

// Abstract base class for any entity with an ID
class Entity {
protected:
    string id;
public:
    Entity(const string& id): id(id) {}
    virtual string getId() const { return id; }
    // Make destructor pure virtual to enforce abstract class
    virtual ~Entity() = 0;
};

// Definition of pure virtual destructor (must have a body)
inline Entity::~Entity() {}
