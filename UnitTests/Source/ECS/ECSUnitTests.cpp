#include "pch.h"
#include "CppUnitTest.h"
#include "Source/ECS/ECSDefines.h"
#include "Source/ECS/EntityWrappers/Entity.h"
#include "Source/ECS/EntityWrappers/Template.h"
#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/Systems/System.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


using namespace Firelight::ECS;

namespace UnitTests::ECS
{

	struct TestComponentA : BaseComponent
	{
		const char* name = "";

		TestComponentA* Clone() override
		{
			TestComponentA* clone = new TestComponentA();
			clone->name = name;

			return clone;
		}
	};

	struct TestComponentB : BaseComponent
	{
		int x = 0;
		int y = 0;

		TestComponentB* Clone() override
		{
			TestComponentB* clone = new TestComponentB();
			clone->x = x;
			clone->y = y;

			return clone;
		}
	};

	struct TestComponentC : BaseComponent
	{
		float vel = 0.0f;

		TestComponentC* Clone() override
		{
			TestComponentC* clone = new TestComponentC();
			clone->vel = vel;

			return clone;
		}
	};



	class TestEntity : public Entity
	{
	public:
		TestEntity()
		{
			AddComponent<TestComponentA>(new TestComponentA());
			AddComponent<TestComponentB>(new TestComponentB());
		}

		TestComponentA* GetTestComponentA()
		{
			return GetComponent<TestComponentA>();
		}
		TestComponentB* GetTestComponentB()
		{
			return GetComponent<TestComponentB>();
		}
	};

	class TestSystem : public System
	{
		TestSystem()
		{
			AddWhitelistComponent<TestComponentA>();
		}
	};

	TEST_CLASS(ECSUnitTests)
	{
	public:

		TEST_CLASS_INITIALIZE(ECSInitialize)
		{
		}

		/// <summary>
		/// Test to confirm that creating an Entity instance creates an entry in the EntityManager
		/// </summary>
		TEST_METHOD(EntityCreationTest)
		{
			//Arrange + Act
			Entity* player = new Entity();

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that deleting an Entity, removes the EntityManager entry
		/// </summary>
		TEST_METHOD(EntityDeletionTest)
		{
			//Arrange + Act
			Entity* player = new Entity();

			player->Destroy();
			delete player;

			//Assert
			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());
		}

		/// <summary>
		/// Test to confirm that when a component is added to an Entity, the component pointer can be retrieved
		/// </summary>
		TEST_METHOD(EntityComponentRetrievalTest)
		{
			//Arrange
			Entity* player = new Entity();
			
			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());

			//Assert
			Assert::IsNotNull(player->GetComponent<TestComponentA>());
			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that when a component is added to an Entity, the component pointer can be retrieved
		/// </summary>
		TEST_METHOD(EntityComponentAutoCreatedRetrievalTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>();

			//Assert
			Assert::IsNotNull(player->GetComponent<TestComponentA>());
			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that retrieving a non existant component returns a nullptr
		/// </summary>
		TEST_METHOD(EntityRetrieveNonExistantComponentTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());
			player->AddComponent<TestComponentB>(new TestComponentB());


			//Assert
			Assert::IsNull(player->GetComponent<TestComponentC>());
			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that two entities are equal
		/// </summary>
		TEST_METHOD(EntityEqualityOperatorFalseTest)
		{
			//Arrange
			Entity ent1 = Entity();
			Entity ent2 = Entity();

			//Act
			ent1.AddComponent<TestComponentA>(new TestComponentA());
			ent2.AddComponent<TestComponentA>(new TestComponentA());

			//Assert
			Assert::IsFalse(ent1 == ent2);
			ent1.Destroy();
			ent2.Destroy();
		}


		TEST_METHOD(EntityEqualityOperatorTrueTest)
		{
			//Arrange
			Entity ent1 = Entity();
			Entity ent2 = Entity(ent1.GetEntityID());

			//Act
			ent1.AddComponent<TestComponentA>(new TestComponentA());
			ent2.AddComponent<TestComponentA>(new TestComponentA());

			//Assert
			Assert::IsTrue(ent1 == ent2);
			ent1.Destroy();
			ent2.Destroy();
		}

		TEST_METHOD(EntityNoEqualityOperatorTrueTest)
		{
			//Arrange
			Entity ent1 = Entity();
			Entity ent2 = Entity();

			//Act
			ent1.AddComponent<TestComponentA>(new TestComponentA());
			ent2.AddComponent<TestComponentA>(new TestComponentA());

			//Assert
			Assert::IsTrue(ent1 != ent2);
			ent1.Destroy();
			ent2.Destroy();
		}


		TEST_METHOD(EntityNoEqualityOperatorFalseTest)
		{
			//Arrange
			Entity ent1 = Entity();
			Entity ent2 = Entity(ent1.GetEntityID());

			//Act
			ent1.AddComponent<TestComponentA>(new TestComponentA());
			ent2.AddComponent<TestComponentA>(new TestComponentA());

			//Assert
			Assert::IsFalse(ent1 != ent2);
			ent1.Destroy();
			ent2.Destroy();
		}

		/// <summary>
		/// Test to confirm that an Entity can accurately retrieve 2 of the same component
		/// </summary>
		TEST_METHOD(EntityComponentMultiRetrievalTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());
			player->AddComponent<TestComponentA>(new TestComponentA());

			//Assert
			Assert::IsNotNull(player->GetComponent<TestComponentA>(0));
			Assert::IsNotNull(player->GetComponent<TestComponentA>(1));
			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that an Entity can accurately retrieve 2 of the same component, removing specific ones
		/// </summary>
		TEST_METHOD(EntityComponentMultiRetrievalWithGapTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());
			player->AddComponent<TestComponentA>(new TestComponentA());
			player->AddComponent<TestComponentA>(new TestComponentA());

			//Assert
			Assert::IsNotNull(player->GetComponent<TestComponentA>(0));
			Assert::IsNotNull(player->GetComponent<TestComponentA>(1));
			Assert::IsNotNull(player->GetComponent<TestComponentA>(2));

			player->RemoveComponent<TestComponentA>(1);

			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that the value returned from HasComponent is correct
		/// </summary>
		TEST_METHOD(EntityHasComponentTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());
			player->AddComponent<TestComponentB>(new TestComponentB());

			//Assert
			Assert::AreEqual(player->GetComponent<TestComponentA>() != nullptr, player->HasComponent<TestComponentA>());
			Assert::AreEqual(player->GetComponent<TestComponentB>() != nullptr, player->HasComponent<TestComponentB>());

			player->Destroy();
			delete player;
		}


		/// <summary>
		/// Test to confirm that the Entity signature is valid.
		/// </summary>
		TEST_METHOD(EntitySignatureTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());
			player->AddComponent<TestComponentB>(new TestComponentB());
			player->AddComponent<TestComponentC>(new TestComponentC());

			//Assert
			Assert::AreEqual(3, (int)player->GetSignature().size());
			Assert::IsTrue(player->GetSignature()[0]);
			Assert::IsTrue(player->GetSignature()[1]);
			Assert::IsTrue(player->GetSignature()[2]);

			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that a component can be removed from an entity, and that the signature updates accordingly
		/// </summary>
		TEST_METHOD(EntityComponentRemovalTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());
			player->AddComponent<TestComponentB>(new TestComponentB());
			player->AddComponent<TestComponentC>(new TestComponentC());
			player->RemoveComponent<TestComponentB>();

			Assert::AreEqual(3, (int)player->GetSignature().size());
			Assert::IsTrue(player->GetSignature()[0]);
			Assert::IsFalse(player->GetSignature()[1]);
			Assert::IsTrue(player->GetSignature()[2]);

			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that if an Entity has more than one of a component the signature remains valid.
		/// </summary>
		TEST_METHOD(EntitySameComponentSignatureTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());
			player->AddComponent<TestComponentA>(new TestComponentA());

			//Assert
			Assert::AreEqual(3, (int)player->GetSignature().size());
			Assert::IsTrue(player->GetSignature()[0]);
			Assert::IsFalse(player->GetSignature()[1]);
			Assert::IsFalse(player->GetSignature()[2]);

			player->Destroy();
			delete player;
		}


		/// <summary>
		/// Test to confirm that if an Entity has more than one of a component, and one is removed, the signature remains valid.
		/// </summary>
		TEST_METHOD(EntitySameComponentRemovedSignatureTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());
			player->AddComponent<TestComponentA>(new TestComponentA());

			player->RemoveComponent<TestComponentA>();

			//Assert
			Assert::AreEqual(3, (int)player->GetSignature().size());
			Assert::IsTrue(player->GetSignature()[0]);
			Assert::IsFalse(player->GetSignature()[1]);
			Assert::IsFalse(player->GetSignature()[2]);

			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that editing a retrieved component, edits the stored data
		/// </summary>
		TEST_METHOD(EntityComponentEditingTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<TestComponentA>(new TestComponentA());
			TestComponentA* playerID = player->GetComponent<TestComponentA>();
			playerID->name = "Test";

			//Assert
			Assert::AreEqual(playerID->name, player->GetComponent<TestComponentA>()->name);

			player->Destroy();
			delete player;
		}

		/// <summary>
		/// Test to confirm that a GameEntity correctly creates two default components
		/// </summary>
		TEST_METHOD(GameEntityCreationTest)
		{
			//Arrange + Act
			TestEntity* testEntity = new TestEntity();

			//Assert
			Assert::IsNotNull(testEntity->GetTestComponentA());
			Assert::IsNotNull(testEntity->GetTestComponentB());
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			testEntity->Destroy();
			delete testEntity;
		}

		/// <summary>
		/// Test to confirm that deleting a GameEntity successfully removes it from the entitymanager
		/// </summary>
		TEST_METHOD(GameEntityDeletionTest)
		{
			//Arrange + Act
			TestEntity* testEntity = new TestEntity();

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			testEntity->Destroy();
			delete testEntity;

			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());
		}

		/// <summary>
		/// Test to confirm that you can edit a GameEntity's components
		/// </summary>
		TEST_METHOD(GameEntityEditComponentTest)
		{
			//Arrange
			TestEntity* testEntity = new TestEntity();
			
			//Act
			testEntity->GetTestComponentB()->x = 10;
			testEntity->GetTestComponentA()->name = "Rabbit";

			//Assert
			Assert::AreEqual(testEntity->GetTestComponentB()->x, EntityComponentSystem::Instance()->GetComponent<TestComponentB>(testEntity->GetEntityID())->x);
			Assert::AreEqual(testEntity->GetTestComponentA()->name, EntityComponentSystem::Instance()->GetComponent<TestComponentA>(testEntity->GetEntityID())->name);

			testEntity->Destroy();
			delete testEntity;
		}

		/// <summary>
		/// Test to confirm that you can create two GameEntities at the same time
		/// </summary>
		TEST_METHOD(MultipleGameEntityCreationTest)
		{
			//Arrange + Act
			TestEntity* testGE1 = new TestEntity();
			TestEntity* testGE2 = new TestEntity();

			//Assert
			Assert::AreEqual(2, (int)EntityComponentSystem::Instance()->GetEntities().size());

			testGE1->Destroy();
			testGE2->Destroy();
			delete testGE1;
			delete testGE2;
		}

		/// <summary>
		/// Test to confirm that GameEntities are succesfully removed when deleted
		/// </summary>
		TEST_METHOD(MultipleGameEntityDeletionTest)
		{
			//Arrange + Act
			TestEntity* testEntity1 = new TestEntity();
			TestEntity* testEntity2 = new TestEntity();

			//Assert
			Assert::AreEqual(2, (int)EntityComponentSystem::Instance()->GetEntities().size());

			testEntity1->Destroy();
			delete testEntity1;

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			testEntity2->Destroy();
			delete testEntity2;

			//Assert
			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());
		}

		/// <summary>
		/// Test to confirm that editing the data of two entities at the same time, accurately changes data.
		/// </summary>
		TEST_METHOD(MultipleGameEntityEditComponentsTest)
		{
			//Arrange
			TestEntity* testEntity1 = new TestEntity();
			TestEntity* testEntity2 = new TestEntity();

			//Act
			testEntity1->GetTestComponentB()->x = 10;
			testEntity2->GetTestComponentA()->name = "Velociraptor";
			testEntity2->GetTestComponentB()->y = -99;
			testEntity1->GetTestComponentA()->name = "Fox";

			//Assert
			Assert::AreEqual(testEntity1->GetTestComponentB()->x, EntityComponentSystem::Instance()->GetComponent<TestComponentB>(testEntity1->GetEntityID())->x);
			Assert::AreEqual(testEntity1->GetTestComponentA()->name, EntityComponentSystem::Instance()->GetComponent<TestComponentA>(testEntity1->GetEntityID())->name);
			Assert::AreEqual(testEntity2->GetTestComponentB()->y, EntityComponentSystem::Instance()->GetComponent<TestComponentB>(testEntity2->GetEntityID())->y);
			Assert::AreEqual(testEntity2->GetTestComponentA()->name, EntityComponentSystem::Instance()->GetComponent<TestComponentA>(testEntity2->GetEntityID())->name);

			testEntity1->Destroy();
			testEntity2->Destroy();
			delete testEntity1;
			delete testEntity2;
		}

		/// <summary>
		/// Test to check that the system automically tracks the added entity
		/// </summary>
		TEST_METHOD(SystemAutoUpdateOnEntityCreationTest)
		{
			System* system = new System();

			Entity* entity = new Entity();

			Assert::AreEqual(1, (int)system->GetEntities().size());

			entity->Destroy();
			delete entity;

			delete system;
		}

		/// <summary>
		/// Test to make sure that the whitelist concept accurately filters the set of entities
		/// </summary>
		TEST_METHOD(SystemAutoUpdateOnEntityCreationWhitelist1Test)
		{
			System* system = new System();
			system->AddWhitelistComponent<TestComponentA>();

			Entity* entity = new Entity();

			Assert::AreEqual(0, (int)system->GetEntities().size());

			entity->Destroy();
			delete entity;

			delete system;
		}

		/// <summary>
		/// Test to make sure that the whitelist concept accurately filters the set of entities
		/// </summary>
		TEST_METHOD(SystemAutoUpdateOnEntityCreationWhitelist2Test)
		{
			System* system = new System();
			system->AddWhitelistComponent<TestComponentA>();

			Entity* entity = new Entity();
			entity->AddComponent<TestComponentA>();

			Assert::AreEqual(1, (int)system->GetEntities().size());

			entity->Destroy();
			delete entity;

			delete system;
		}

		/// <summary>
		/// Test to make sure that the blacklist concept accurately filters the set of entities
		/// </summary>
		TEST_METHOD(SystemAutoUpdateOnEntityCreationBlacklist1Test)
		{
			System* system = new System();
			system->AddBlacklistComponent<TestComponentA>();

			Entity* entity = new Entity();
			entity->AddComponent<TestComponentA>();

			Assert::AreEqual(0, (int)system->GetEntities().size());

			entity->Destroy();
			delete entity;

			delete system;
		}

		/// <summary>
		/// Test to make sure that the blacklist concept accurately filters the set of entities
		/// </summary>
		TEST_METHOD(SystemAutoUpdateOnEntityCreationBlacklist2Test)
		{
			System* system = new System();
			system->AddBlacklistComponent<TestComponentA>();

			Entity* entity = new Entity();

			Assert::AreEqual(1, (int)system->GetEntities().size());

			entity->Destroy();
			delete entity;

			delete system;
		}

		/// <summary>
		/// Test to make sure that the blacklist concept accurately filters the set of entities
		/// </summary>
		TEST_METHOD(SystemUnsubscribeTest)
		{
			System* system1 = new System();
			System* system2 = new System();

			Entity* entity = new Entity();

			entity->Destroy();
			delete entity;

			delete system2;
			delete system1;
		}

		/// <summary>
		/// Test to make sure that the blacklist concept accurately filters the set of entities
		/// </summary>
		TEST_METHOD(SystemUnsubscribe2Test)
		{
			System* system1 = new System();
			System* system3 = new System();
			System* system2 = new System();

			Entity* entity = new Entity();

			entity->Destroy();
			delete entity;

			delete system1;
			delete system2;
			delete system3;
		}

		/// <summary>
		/// Test to make sure that the blacklist concept accurately filters the set of entities
		/// </summary>
		TEST_METHOD(MultipleSameTypeComponentsTest)
		{
			Entity* entity = new Entity();

			entity->AddComponent<TestComponentA>();
			entity->AddComponent<TestComponentA>();

			Assert::AreEqual(2, (int)entity->GetComponents<TestComponentA>().size());

			entity->Destroy();
			delete entity;
		}

		/// <summary>
		/// Test to make sure that the blacklist concept accurately filters the set of entities
		/// </summary>
		TEST_METHOD(MultipleSameTypeComponentsConsistentValuesTest)
		{
			Entity* entity = new Entity();

			TestComponentA* comp1 = new TestComponentA();
			comp1->name = "TestA";
			TestComponentA* comp2 = new TestComponentA();
			comp2->name = "TestB";
			entity->AddComponent<TestComponentA>(comp1);
			entity->AddComponent<TestComponentA>(comp2);

			std::vector<TestComponentA*> components = entity->GetComponents<TestComponentA>();
			Assert::AreEqual("TestA", components[0]->name);
			Assert::AreEqual("TestB", components[1]->name);

			entity->Destroy();
			delete entity;
		}

		/// <summary>
		/// Test to ensure that the correct amount of components are retrieved by the get all command
		/// </summary>
		TEST_METHOD(ECSGetAllComponentsOfTypeTest)
		{
			Entity* entity1 = new Entity();
			Entity* entity2 = new Entity();
			Entity* entity3 = new Entity();

			entity1->AddComponent<TestComponentA>();
			entity2->AddComponent<TestComponentA>();
			entity2->AddComponent<TestComponentA>();
			entity3->AddComponent<TestComponentA>();
			entity3->AddComponent<TestComponentA>();
			entity3->AddComponent<TestComponentA>();

			std::vector<TestComponentA*> components = EntityComponentSystem::Instance()->GetAllComponents<TestComponentA>();
			Assert::AreEqual(6, (int)components.size());

			entity1->Destroy();
			entity2->Destroy();
			entity3->Destroy();
			delete entity1;
			delete entity2;
			delete entity3;
		}

		/// <summary>
		/// Test to ensure that the correct amount of components are retrieved by the get all command
		/// </summary>
		TEST_METHOD(CreateEntityFromTemplateTest)
		{
			Template* testTemplate = new Template();

			TestComponentA* testComp = new TestComponentA();
			testComp->name = "TemplateTest";
			testTemplate->AddComponent<TestComponentA>(testComp);

			Entity* testEntity = new Entity(true, testTemplate->GetTemplateID());

			Assert::AreEqual(testEntity->GetComponent<TestComponentA>()->name, testComp->name);

			testEntity->Destroy();
			delete testEntity;
			delete testTemplate;
		}
	};
}
