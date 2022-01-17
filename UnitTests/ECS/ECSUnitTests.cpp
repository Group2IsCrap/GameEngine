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
		
		TEST_CLASS_INITIALIZE(ECSInitialize)
		{
			EntityComponentSystem::Instance()->RegisterComponent<IdentificationComponent>();
			EntityComponentSystem::Instance()->RegisterComponent<TransformComponent>();
			EntityComponentSystem::Instance()->RegisterComponent<PhysicsComponent>();
		}

		TEST_METHOD(EntityCreationTest)
		{
			//Arrange + Act
			Entity* player = new Entity();

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete player;
		}

		TEST_METHOD(EntityDeletionTest)
		{
			//Arrange + Act
			Entity* player = new Entity();

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete player;

			//Assert
			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());
		}
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

		TEST_METHOD(GameEntityCreationTest)
		{
			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());

			//Arrange + Act
			GameEntity* testGE = new GameEntity();

			//Assert
			Assert::IsNotNull(testGE->GetComponent<IdentificationComponent>());
			Assert::IsNotNull(testGE->GetComponent<TransformComponent>());
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete testGE;
		}

		TEST_METHOD(GameEntityDeletionTest)
		{
			//Arrange + Act
			GameEntity* testGE = new GameEntity();

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete testGE;

			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());
		}

		TEST_METHOD(GameEntityDeletion2Test)
		{
			//Arrange + Act
			GameEntity* testGE = new GameEntity();

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete testGE;

			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());

			//Arrange + Act
			GameEntity* testGE2 = new GameEntity();

			//Assert
			Assert::AreEqual(1, (int)EntityComponentSystem::Instance()->GetEntities().size());

			delete testGE2;

			Assert::AreEqual(0, (int)EntityComponentSystem::Instance()->GetEntities().size());
		}
		TEST_METHOD(GameEntityEditComponentTest)
		{
			//Arrange
			GameEntity* testGE = new GameEntity();
			
			//Act
			testGE->GetTransform()->posX = 10;
			testGE->GetID()->name = "Rabbit";

			//Assert
			Assert::AreEqual(testGE->GetTransform()->posX, EntityComponentSystem::Instance()->GetComponent<TransformComponent>(testGE->GetEntityID())->posX);
			Assert::AreEqual(testGE->GetID()->name, EntityComponentSystem::Instance()->GetComponent<IdentificationComponent>(testGE->GetEntityID())->name);

			delete testGE;
		}

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

		TEST_METHOD(MultipleGameEntityEditComponentsTest)
		{
			//Arrange
			GameEntity* testGE1 = new GameEntity();
			GameEntity* testGE2 = new GameEntity();

			//Act
			testGE1->GetTransform()->posX = 10;
			testGE2->GetID()->name = "Velociraptor";
			testGE2->GetTransform()->posY = -99;
			testGE1->GetID()->name = "Fox";

			//Assert
			Assert::AreEqual(testGE1->GetTransform()->posX, EntityComponentSystem::Instance()->GetComponent<TransformComponent>(testGE1->GetEntityID())->posX);
			Assert::AreEqual(testGE1->GetID()->name, EntityComponentSystem::Instance()->GetComponent<IdentificationComponent>(testGE1->GetEntityID())->name);
			Assert::AreEqual(testGE2->GetTransform()->posY, EntityComponentSystem::Instance()->GetComponent<TransformComponent>(testGE2->GetEntityID())->posY);
			Assert::AreEqual(testGE2->GetID()->name, EntityComponentSystem::Instance()->GetComponent<IdentificationComponent>(testGE2->GetEntityID())->name);

			delete testGE1;
			delete testGE2;
		}

	};
}
