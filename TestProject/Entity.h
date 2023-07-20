#pragma once
#include "Types.h"
#include <memory>
#include "EntityActionManager.h"

/*
	Entity.
	 + contain entity_id to identify which entity in manager.
	 + assign component data by calling Assign<Component>(Component{v1,v2,v3})
	 + calling EnitityActionManager internally.
*/
class Entity
{
public:

	inline EntityID GetEntityID() const { return entity_id; }

	Entity();
	virtual ~Entity();

	/*Add component data to entity*/
	template<typename C>
	void Assign(const C& Data);
	
private:

	EntityID entity_id;
	std::shared_ptr<EntityActionManager> _mgr;
};

template<typename C>
void Entity::Assign(const C& Data)
{
	if (_mgr != nullptr)
		_mgr->AddComponentToEntity<C>(entity_id, Data);
}
