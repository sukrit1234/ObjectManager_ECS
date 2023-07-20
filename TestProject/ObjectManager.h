#pragma once
#include <memory>
#include <atomic>
#include <mutex>
#include <algorithm> 
#include <unordered_map>
#include "Object.h"

typedef std::unordered_map<ObjectID, std::shared_ptr<Object>> ObjectBuffer;

/*
	Generic Object Manager;
	 + Create object and register , assign object id and object type id automatically.
	 + Access object with ObjectID
	 + No need to remove or destroy object. It's will take advantage of std::shared_ptr
	   to check and delete object by reference count (just call CheckAndDeleteUnuseObjects) at interval of time
	   on application or game loop. see TestProject.cpp file
*/

class ObjectManager
{
public :

	/*Get Object*/
	virtual std::shared_ptr<Object> GetObject(ObjectID ID) const { return objects.at(ID); }
	
	/*Check object exists by ID*/
	virtual bool HasObject(ObjectID ID) const { return objects.find(ID) != objects.end(); }
	
	/*Check object exists by TypeID*/
	virtual bool HasObjectByType(ObjectTypeID TypeID) const;

	/*
		Simple garbage collector check reference count of object and delete if it's unuse.
	*/
	void CheckAndDeleteUnuseObjects();

protected:
	
	ObjectManager() :next_object_id(1) {}

	virtual void OnObjectCreated(std::shared_ptr<Object> NewObject, ObjectTypeID ObjectTypeID);
	const std::vector<ObjectID>& get_object_ids() const { return object_ids; }

private:

	void remove_object_id(ObjectID _id);

	ObjectID GetNextObjectID();
	ObjectID next_object_id;

	std::vector<ObjectID> to_delete_object_ids;
	std::vector<ObjectID> object_ids;
	ObjectBuffer objects;
};

/*
	GenericObjectManager 
	 + It's generic and template version of ObjectManager.
	 + Can be iterate through each object
	      for(auto _iter : (*Manager)){
			
		  }
*/
template<class T = Object>
class GenericObjectManager : public ObjectManager
{

public:

	template<class O = T, class... TArgs>
	std::shared_ptr<O> Create(TArgs... Args);

	template<class... TArgs>
	std::shared_ptr<T> Create(TArgs... Args);

	GenericObjectManager() : ObjectManager() {}

	std::shared_ptr<T> GetTypedObject(ObjectID ID) const
	{
		return std::dynamic_pointer_cast<T>(GetObject(ID));
	}

	struct Iterator
	{
		std::shared_ptr<T> operator*() const
		{
			return ((_mgr != nullptr) && _mgr->HasObject(*_iter)) ? _mgr->GetTypedObject(*_iter) : nullptr;
		}
		Iterator(GenericObjectManager* mgr, std::vector<ObjectID>::const_iterator iter)
			: _mgr(mgr), _iter(iter){}
		
		Iterator& operator++()
		{
			_iter++;
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator tmp = *this; ++(*this);
			return tmp;
		}
		friend bool operator== (const Iterator& a, const Iterator& b)
		{
			return (a._mgr == b._mgr) && (a._iter == b._iter);
		};
		friend bool operator!= (const Iterator& a, const Iterator& b)
		{
			return (a._mgr != b._mgr) || (a._iter != b._iter);
		};

	private:

		GenericObjectManager* _mgr;
		std::vector<ObjectID>::const_iterator _iter;
	};

	Iterator begin()
	{
		return Iterator(this, get_object_ids().begin());
	}
	Iterator end()
	{
		return Iterator(this, get_object_ids().end());
	}
protected:
	virtual bool AllowToCreateObject(ObjectTypeID TypeID) const { return true; }
};

template<class T>
template<class... TArgs>
std::shared_ptr<T> GenericObjectManager<T>::Create(TArgs... Args)
{
	ObjectTypeID ObjectType = GetObjectTypeId<T>();
	if (!AllowToCreateObject(ObjectType))
		return nullptr;

	std::shared_ptr<T> obj_ptr = std::make_shared<T>(TArgs(Args)...);
	OnObjectCreated(obj_ptr, ObjectType);
	return obj_ptr;
}

template<class T>
template<class O, class... TArgs>
std::shared_ptr<O> GenericObjectManager<T>::Create(TArgs... Args)
{
	ObjectTypeID ObjectType = GetObjectTypeId<O>();
	if (!AllowToCreateObject(ObjectType))
		return nullptr;

	std::shared_ptr<O> obj_ptr = std::make_shared<O>(TArgs(Args)...);
	OnObjectCreated(obj_ptr, ObjectType);
	return obj_ptr;
}