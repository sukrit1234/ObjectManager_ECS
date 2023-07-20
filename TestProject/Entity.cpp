#include "Entity.h"

Entity::Entity()
{
	_mgr = std::shared_ptr<EntityActionManager>(EntityActionManager::Get());
	if (_mgr != nullptr)
		entity_id = _mgr->CreateEntity();
}
Entity::~Entity()
{
	if (_mgr != nullptr)
		_mgr->DestroyEntity(entity_id);
}