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
 + [**EntityAction**](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/EntityAction.h) : Base class of enitity's action - the logic class that apply to all of entity that match with component types combination.
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
### Create Entity Actions
action class in example define here : [[Header]](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/ExampleActions.h)  [[Source]](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/TestProject/ExampleActions.cpp)
```
EntityActionManager* EntityActionMgrPtr = EntityActionManager::Get();
EntityActionManager& EntityActionMgr = (*EntityActionMgrPtr);

EntityActionMgr.CreateAction<MovementAction>();
EntityActionMgr.CreateAction<RotateAction>();
EntityActionMgr.CreateAction<RenderAction>();
```

## Put everything all together.
Let create entity , define and create actions and update on the simple game loop. This example we lock game loop at 30 frame per seconds and do garbage collector check every 2 seconds.
```
int main()
{
	EntityActionManager* EntityActionMgrPtr = EntityActionManager::Get();
	EntityActionManager& EntityActionMgr = (*EntityActionMgrPtr);

	/*Define Action for all of entities*/
	/*1. Movement Action  move enitity position with linear velocity*/
	EntityActionMgr.CreateAction<MovementAction>();
	/*2. Rotate Action rotate enitity angle with rotate speed*/
	EntityActionMgr.CreateAction<RotateAction>();
	/*3. Render enitity position and angle to screen*/
	EntityActionMgr.CreateAction<RenderAction>();

	/*Create two entities*/
	/*For Example just player and goblin*/
	Entity player, goblin;

	/*
		Add support component for player entity
		Position , Velocity , Angle , Rotate Speed, and Trait Info.
		so Player will move and rotate
	*/
	player.Assign(Position{ 234.0f, 456.0f });
	player.Assign(Velocity{ 3.4f , 6.7f });
	player.Assign(Angle{ 0.0f });
	player.Assign(RotateSpeed{ 40.0f });
	player.Assign(CharacterTraits{ "Bob", "Human" });

	/*
		Add Support component for goblin entity
		Position , Angle , Rotate Speed , and Trait Info.
		so Goblin will only rotate.
	*/
	goblin.Assign(Position{ 567.0f , 876.0f });
	goblin.Assign(Angle{ 0.0f });
	goblin.Assign(RotateSpeed{ 0.5f });
	goblin.Assign(CharacterTraits{ "Griblub", "Goblin" });


	/*Simple game loop lock at 30 frame per seconds*/
	using clock = std::chrono::high_resolution_clock;
	auto time_start = clock::now();
	bool quit_game = false;

	
	float acc_deltatime = 0.0f;
	float acc_check_reference = 0.0f;
	const float lock_30fps_deltatime = 0.034f;
	const float check_reference_ThesholdTime = 2.0f;

	std::chrono::duration<float> delta_time;
	while (!quit_game) {
		delta_time = clock::now() - time_start;
		time_start = clock::now();

		acc_deltatime += delta_time.count();
		if (acc_deltatime > lock_30fps_deltatime)
		{
			//Process all logic of all processors.
			EntityActionMgr.Execute(acc_deltatime);
			acc_deltatime = 0.0f;
		}

		/*
			Do check reference and delete object.
			in some interval of time.
		*/
		acc_check_reference += delta_time.count();
		if (acc_check_reference > check_reference_ThesholdTime)
		{
			EntityActionMgr.CheckAndDeleteUnuseObjects();
			acc_check_reference = 0.0f;
		}

	}
}
```
### Output
![plot](https://github.com/sukrit1234/ObjectManager_ECS/blob/main/Output_ECS.png)

## Limitations or encountered problems
We have limitation and many space to improve from this.
  + Should update Entity class inherit from Object class and create EntityManager inherit from GenericObjectManager<Entity>
to take advantage from ObjectManager to control Entity instance references.
  + Should add more utitlity to access data pointer reference to modify entity's component data outside EntityAction
  + Right now Archetype directly contain array of entity's component data. For very optimized performance. I should implement Archetype Chuck to contain entity's component data
    and then assign those chucks reference to Archetype and update entity action execute action for each chuck, It will reduce cache miss and a lot better performance.
  + Garbage collector should be use more complex approach. Right now just simple check all of object reference count each interval of time. I must improve by defered check, queue object and check some of object in list. Spread computation to several frames.
  + Maybe many more bugs that I not found it right now because limit time to test. But on real working must do test much much more.
