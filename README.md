## ObjectManager V2 and Entity Component System
This is simple and test version of Entity Component System (ECS) combine with Object Manager pattern.
It's compose some parts of module.

## Object Manager Module
 + [**Object**](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/Object.h) : Base class of Object that can create and manage ObjectManager
 + [**ObjectManager**](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/ObjectManager.h) : Base class of ObjectManager that used in this project. Can create/access and manage objects.
 + [**GenericObjectManager**](https://github.com/sukrit1234/ObjectManager_ECS/blob/e15481f2b6f09ee35788ff3d5419ba7b73784eec/TestProject/ObjectManager.h#L57) : It's generic version of ObjectManager take advantage of C++ template and also can iterate through object.

### Iterate objects in GenericObjectManager
```
    //Note : EntityActionManager derive from GeneraicObjectManager<EntityAction> class
    EntityActionManager* Manager = EntityActionManager::Get();
    if(Manager != nullptr)
    {
        for(auto _iter : (*Manager)){
            //Print out object id.
            std::cout << _iter->GetObjectID();
        }
    }
```
### No need to cleanup object
ObjectManager has method ObjectManager::CheckAndDeleteUnuseObjects() to check reference count and delete object automatically just call this function in game loop (or application loop) at interval of time. It's simple garbage collector just use for this project. 

```
    EntityActionManager* Manager = EntityActionManager::Get();
    if(Manager != nullptr)
       Manager->CheckAndDeleteUnuseObjects();
```

## Entity Component System
 + [**Entity**](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/Entity.h) : contain entity id and reference to EntityActionManager for internall call.
 + [**EntityAction**] (https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/EntityAction.h) : Base class of enitity's action - the logic class that apply to all of entity that match with component types combination.
 + [**EntityActionManager**](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/EntityActionManager.h) : The ObjectManager that create and manage EntityAction object and also create/destroy/manage component data of entities.

### Create Entity

```
Entity player, goblin;
```
### Assign component data to entity
component in example define at [Example Component](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/ExampleComponent.h)
```
player.Assign(Position{ 234.0f, 456.0f });
player.Assign(Velocity{ 3.4f , 6.7f });
player.Assign(Angle{ 0.0f });
player.Assign(RotateSpeed{ 40.0f });
player.Assign(CharacterTraits{ "Bob", "Human" });

goblin.Assign(Position{ 567.0f , 876.0f });
goblin.Assign(Angle{ 0.0f });
goblin.Assign(RotateSpeed{ 0.5f });
goblin.Assign(CharacterTraits{ "Griblub", "Goblin" });
```
