#pragma once

/*Forward declaration*/
class Object;

using ObjectTypeID = unsigned int;
using ObjectID = unsigned long long;

extern ObjectTypeID S_ObjectTypeIDCounter;
template <class T=Object>
ObjectTypeID GetObjectTypeId()
{
	static ObjectTypeID S_ObjectTypeId = S_ObjectTypeIDCounter++;
	return S_ObjectTypeId;
}

/*
	Base object of this project.
	derived with Object class will take advantage from ObjectManager.
*/
class Object
{
	friend class ObjectManager;

public:

	/*Get Object ID*/
	inline ObjectID GetObjectID() const { return object_id; }
	
	/*Get Object Type ID*/
	inline ObjectTypeID GetObjectTypeID() const { return object_type_id; }

	virtual ~Object() {}

private:

	/*These property will internal assigned by ObjectManager*/
	
	/*Object ID*/
	ObjectID object_id;

	/*Object Type ID*/
	ObjectTypeID object_type_id;
};