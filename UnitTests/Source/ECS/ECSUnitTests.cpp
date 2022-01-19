#include "pch.h"
#include "CppUnitTest.h"
#include "Source/ECS/GameEntity.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace Firelight::ECS;

namespace UnitTests::ECS
{
	TEST_CLASS(ECSUnitTests)
	{
	public:
		
		/// <summary>
		/// Register the 3 test components 
		/// </summary>
		/// <returns></returns>
		TEST_CLASS_INITIALIZE(ECSInitialize)
		{
			EntityComponentSystem::Instance()->RegisterComponent<IdentificationComponent>();
			EntityComponentSystem::Instance()->RegisterComponent<TransformComponent>();
			EntityComponentSystem::Instance()->RegisterComponent<PhysicsComponent>();
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

			delete player;
		}

		/// <summary>
		/// Test to confirm that deleting an Entity, removes the EntityManager entry
		/// </summary>
		TEST_METHOD(EntityDeletionTest)
		{
			//Arrange + Act
			Entity* player = new Entity();

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
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());

			//Assert
			Assert::IsNotNull(player->GetComponent<IdentificationComponent>());
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
			player->AddComponent<IdentificationComponent>();

			//Assert
			Assert::IsNotNull(player->GetComponent<IdentificationComponent>());
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
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			player->AddComponent<TransformComponent>(new TransformComponent());

			//Assert
			Assert::IsNull(player->GetComponent<PhysicsComponent>());
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
			ent1.AddComponent<IdentificationComponent>(new IdentificationComponent());
			ent2.AddComponent<IdentificationComponent>(new IdentificationComponent());

			//Assert
			Assert::IsFalse(ent1 == ent2);
		}


		TEST_METHOD(EntityEqualityOperatorTrueTest)
		{
			//Arrange
			Entity ent1 = Entity();
			Entity ent2 = Entity(ent1.GetEntityID());

			//Act
			ent1.AddComponent<IdentificationComponent>(new IdentificationComponent());
			ent2.AddComponent<IdentificationComponent>(new IdentificationComponent());

			//Assert
			Assert::IsTrue(ent1 == ent2);
		}

		TEST_METHOD(EntityNoEqualityOperatorTrueTest)
		{
			//Arrange
			Entity ent1 = Entity();
			Entity ent2 = Entity();

			//Act
			ent1.AddComponent<IdentificationComponent>(new IdentificationComponent());
			ent2.AddComponent<IdentificationComponent>(new IdentificationComponent());

			//Assert
			Assert::IsTrue(ent1 != ent2);
		}


		TEST_METHOD(EntityNoEqualityOperatorFalseTest)
		{
			//Arrange
			Entity ent1 = Entity();
			Entity ent2 = Entity(ent1.GetEntityID());

			//Act
			ent1.AddComponent<IdentificationComponent>(new IdentificationComponent());
			ent2.AddComponent<IdentificationComponent>(new IdentificationComponent());

			//Assert
			Assert::IsFalse(ent1 != ent2);
		}

		/// <summary>
		/// Test to confirm that an Entity can accurately retrieve 2 of the same component
		/// </summary>
		TEST_METHOD(EntityComponentMultiRetrievalTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());

			//Assert
			Assert::IsNotNull(player->GetComponent<IdentificationComponent>(0));
			Assert::IsNotNull(player->GetComponent<IdentificationComponent>(1));
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
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());

			//Assert
			Assert::IsNotNull(player->GetComponent<IdentificationComponent>(0));
			Assert::IsNotNull(player->GetComponent<IdentificationComponent>(1));
			Assert::IsNotNull(player->GetComponent<IdentificationComponent>(2));

			player->RemoveComponent<IdentificationComponent>(1);

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
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			player->AddComponent<TransformComponent>(new TransformComponent());

			//Assert
			Assert::AreEqual(player->GetComponent<IdentificationComponent>() != nullptr, player->HasComponent<IdentificationComponent>());
			Assert::AreEqual(player->GetComponent<TransformComponent>() != nullptr, player->HasComponent<TransformComponent>());
			Assert::AreEqual(player->GetComponent<PhysicsComponent>() != nullptr, player->HasComponent<PhysicsComponent>());

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
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			player->AddComponent<TransformComponent>(new TransformComponent());
			player->AddComponent<PhysicsComponent>(new PhysicsComponent());

			//Assert
			Assert::AreEqual(3, (int)player->GetSignature().size());
			Assert::IsTrue(player->GetSignature()[0]);
			Assert::IsTrue(player->GetSignature()[1]);
			Assert::IsTrue(player->GetSignature()[2]);

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
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			player->AddComponent<TransformComponent>(new TransformComponent());
			player->AddComponent<PhysicsComponent>(new PhysicsComponent());
			player->RemoveComponent<TransformComponent>();

			Assert::AreEqual(3, (int)player->GetSignature().size());
			Assert::IsTrue(player->GetSignature()[0]);
			Assert::IsFalse(player->GetSignature()[1]);
			Assert::IsTrue(player->GetSignature()[2]);

			delete player;

			EntityComponentSystem::Instance()->DebugEntities();
		}

		/// <summary>
		/// Test to confirm that if an Entity has more than one of a component the signature remains valid.
		/// </summary>
		TEST_METHOD(EntitySameComponentSignatureTest)
		{
			//Arrange
			Entity* player = new Entity();

			//Act
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());

			//Assert
			Assert::AreEqual(3, (int)player->GetSignature().size());
			Assert::IsTrue(player->GetSignature()[0]);
			Assert::IsFalse(player->GetSignature()[1]);
			Assert::IsFalse(player->GetSignature()[2]);

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
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());

			player->RemoveComponent<IdentificationComponent>();

			//Assert
			Assert::AreEqual(3, (int)player->GetSignature().size());
			Assert::IsTrue(player->GetSignature()[0]);
			Assert::IsFalse(player->GetSignature()[1]);
			Assert::IsFalse(player->GetSignature()[2]);

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
			player->AddComponent<IdentificationComponent>(new IdentificationComponent());
			IdentificationComponent* playerID = player->GetComponent<IdentificationComponent>();
			playerID->name = "Test";

			//Assert
			Assert::AreEqual(playerID->name, player->GetComponent<IdentificationComponent>()->name);

			delete player;
		}

		/// <summary>
		/// Test to confirm that a GameEntity correctly creates two default components
		/// </summary>
		TEST_METHOD(GameEntityCreationTest)
		{
			//Arrange + Act
			GameEntity* testGE = new GameEntity();

			//Assert
			Assert::IsNotNull(testGE->GetComponent<IdentificationComponent>());
			Assert::IsNotNull(testGE->GetComponent<TransformComponent>());
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete testGE;
		}

		/// <summary>
		/// Test to confirm that deleting a GameEntity successfully removes it from the entitymanager
		/// </summary>
		TEST_METHOD(GameEntityDeletionTest)
		{
			//Arrange + Act
			GameEntity* testGE = new GameEntity();

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete testGE;

			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());
		}

		/// <summary>
		/// Test to confirm that you can edit a GameEntity's components
		/// </summary>
		TEST_METHOD(GameEntityEditComponentTest)
		{
			//Arrange
			GameEntity* testGE = new GameEntity();
			
			//Act
			testGE->GetTransform()->position.x = 10.0f;
			testGE->GetID()->name = "Rabbit";

			//Assert
			Assert::AreEqual(testGE->GetTransform()->position.x, EntityComponentSystem::Instance()->GetComponent<TransformComponent>(testGE->GetEntityID())->position.x);
			Assert::AreEqual(testGE->GetID()->name, EntityComponentSystem::Instance()->GetComponent<IdentificationComponent>(testGE->GetEntityID())->name);

			delete testGE;
		}

		/// <summary>
		/// Test to confirm that you can create two GameEntities at the same time
		/// </summary>
		TEST_METHOD(MultipleGameEntityCreationTest)
		{
			//Arrange + Act
			GameEntity* testGE1 = new GameEntity();
			GameEntity* testGE2 = new GameEntity();

			//Assert
			Assert::AreEqual(2, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete testGE1;
			delete testGE2;
		}

		/// <summary>
		/// Test to confirm that GameEntities are succesfully removed when deleted
		/// </summary>
		TEST_METHOD(MultipleGameEntityDeletionTest)
		{
			//Arrange + Act
			GameEntity* testGE1 = new GameEntity();
			GameEntity* testGE2 = new GameEntity();

			//Assert
			Assert::AreEqual(2, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete testGE1;

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete testGE2;

			//Assert
			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());
		}

		/// <summary>
		/// Test to confirm that editing the data of two entities at the same time, accurately changes data.
		/// </summary>
		TEST_METHOD(MultipleGameEntityEditComponentsTest)
		{
			//Arrange
			GameEntity* testGE1 = new GameEntity();
			GameEntity* testGE2 = new GameEntity();

			//Act
			testGE1->GetTransform()->position.x = 10.0f;
			testGE2->GetID()->name = "Velociraptor";
			testGE2->GetTransform()->position.y = -99.0f;
			testGE1->GetID()->name = "Fox";

			//Assert
			Assert::AreEqual(testGE1->GetTransform()->position.x, EntityComponentSystem::Instance()->GetComponent<TransformComponent>(testGE1->GetEntityID())->position.x);
			Assert::AreEqual(testGE1->GetID()->name, EntityComponentSystem::Instance()->GetComponent<IdentificationComponent>(testGE1->GetEntityID())->name);
			Assert::AreEqual(testGE2->GetTransform()->position.y, EntityComponentSystem::Instance()->GetComponent<TransformComponent>(testGE2->GetEntityID())->position.y);
			Assert::AreEqual(testGE2->GetID()->name, EntityComponentSystem::Instance()->GetComponent<IdentificationComponent>(testGE2->GetEntityID())->name);

			delete testGE1;
			delete testGE2;
		}

	};
}
