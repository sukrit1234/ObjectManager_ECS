## ObjectManager V2 and Entity Component System
This is simple and test version of Entity Component System (ECS) combine with Object Manager pattern.
It's compose some parts of module.

## Object Manager Module
 + [**Object**](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/Object.h) : Base class of Object that can create and manage ObjectManager
 + [**ObjectManager**](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/ObjectManager.h) : Base class of ObjectManager that used in this project. Can create/access and manage objects.
 + [**GenericObjectManager**](https://github.com/sukrit1234/ObjectManager_ECS/blob/e15481f2b6f09ee35788ff3d5419ba7b73784eec/TestProject/ObjectManager.h#L57) : It's generic version of ObjectManager take advantage of C++ template and also can iterate through object in manage on  for(auto _iter : Manager)

### Iterate objects in GenericObjectManager
```
    //Note : EntityActionManager derive from GeneraicObjectManager<EntityAction> class
    EntityActionManager* Manager = EntityActionManager::Get();
    for(auto _iter : (*Manager)){
        //Print out object id.
        std::cout << _iter->GetObjectID();
    }
```
