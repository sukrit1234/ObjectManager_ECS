#include "ObjectManager.h"
#include <iostream>


ObjectID ObjectManager::GetNextObjectID()
{
	ObjectID object_id = next_object_id++;
	return object_id;
}
void ObjectManager::OnObjectCreated(std::shared_ptr<Object> NewObject, ObjectTypeID ObjectTypeID)
{
	ObjectID object_Id = GetNextObjectID();
	NewObject->object_id = object_Id;
	NewObject->object_type_id = ObjectTypeID;
	object_ids.push_back(object_Id);
	objects.insert({ object_Id ,NewObject });
}
void ObjectManager::remove_object_id(ObjectID _id)
{
	object_ids.erase(std::remove(object_ids.begin(), object_ids.end(), _id), object_ids.end());
}
void ObjectManager::CheckAndDeleteUnuseObjects()
{
	to_delete_object_ids.reserve(objects.size());
	for (std::pair<ObjectID, std::shared_ptr<Object>> pair : objects)
	{
		if ((pair.second != nullptr) && (pair.second.use_count() <= 1))
			to_delete_object_ids.push_back(pair.first);
	}
	for (ObjectID _id : to_delete_object_ids)
	{
		objects.erase(_id);
		remove_object_id(_id);
	}
	to_delete_object_ids.clear();
}
bool ObjectManager::HasObjectByType(ObjectTypeID TypeID) const
{
	for (std::pair<ObjectID, std::shared_ptr<Object>> pair : objects)
	{
		if (pair.second->GetObjectTypeID() == TypeID)
			return true;
	}
	return false;
}