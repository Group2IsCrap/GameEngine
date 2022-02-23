#include "ComponentManager.h"
#include "../Serialisation/Serialiser.h"

using namespace Firelight::Serialisation;

namespace Firelight::ECS
{
	ComponentTypeID ComponentManager::sm_nextComponentID = 0;

	std::unordered_map<TypeHash, ComponentTypeID> ComponentManager::sm_componentHashTypes;
	std::unordered_map<ComponentTypeID, TypeHash> ComponentManager::sm_componentTypeHash;
	std::unordered_map<TypeHash, const char*> ComponentManager::sm_componentHashNames;


	int ComponentManager::GetRegisteredComponentTypeCount()
	{
		return (int)sm_componentTypeHash.size();
	}

	/// <summary>
	/// Returns the entire map of component data
	/// </summary>
	/// <returns></returns>
	std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> ComponentManager::GetComponentData()
	{
		return m_componentData;
	}

	/// <summary>
	/// Returns the entire map of component data
	/// </summary>
	/// <returns></returns>
	std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> ComponentManager::GetComponentDataCopy(EntityID id)
	{
		std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> associatedComponents = std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>>();
		for (auto& typeComponentListPair : m_componentMap)
		{
			associatedComponents.insert(std::make_pair(typeComponentListPair.first, std::vector<BaseComponent*>()));

			for (auto element : m_componentMap[typeComponentListPair.first][id])
			{
				associatedComponents[typeComponentListPair.first].push_back(m_componentData[typeComponentListPair.first][element]->Clone());
			}
		}

		return associatedComponents;
	}

	/// <summary>
	/// Returns the name of the given component ID
	/// </summary>
	/// <param name="typeID"></param>
	/// <returns></returns>
	const char* ComponentManager::GetComponentName(ComponentTypeID typeID)
	{
		return sm_componentHashNames[sm_componentTypeHash[typeID]];
	}	

	bool ComponentManager::HasComponent(ComponentTypeID typeID, EntityID entity)
	{
		return (m_componentMap[typeID].find(entity) != m_componentMap[typeID].end());
	}

	/// <summary>
	/// When a component is removed, the position of other components is shifted. 
	/// This needs to be reflected in the component map, else the map will become invalidated
	/// </summary>
	/// <param name="componentType"></param>
	/// <param name="removedIndex">The index of the removed component</param>
	void ComponentManager::UpdateComponentMap(ComponentTypeID componentType, int removedIndex)
	{
		for (auto& map : m_componentMap[componentType])
		{
			for (auto& index : map.second)
			{
				if (index > removedIndex)
				{
					index--;
				}
			}
		}
	}

	void ComponentManager::SerialiseAllComponents()
	{
		//Create Component Group
		Serialiser::StartArray("ComponentTypes");
		for (auto& it : m_componentData)
		{
			Serialiser::StartObject();
			//Create ComponentType group
			Serialiser::Serialise("ComponentID", it.first);

			Serialiser::StartArray("Components");
			for (auto component : it.second)
			{
				//Create component object
				Serialiser::StartObject();
				component->Serialise();
				//close object
				Serialiser::EndObject();
			}
			//close group
			Serialiser::EndArray();
			Serialiser::EndObject();
		}
		//Close component group
		Serialiser::EndArray();

		//Create ComponentType group
		Serialiser::StartArray("ComponentMap");
		for (auto &it : m_componentMap)
		{
			//Create entity group

			Serialiser::StartObject();
			Serialiser::Serialise("ComponentID", it.first);
			Serialiser::StartArray("EntityIDs");
			for (auto& it2 : m_componentMap[it.first])
			{
				//Create entityIndices
				Serialiser::StartObject();
				Serialiser::Serialise("EntityID", it2.first);

				Serialiser::SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
				Serialiser::StartArray("Indices");
				for (auto index : m_componentMap[it.first][it2.first])
				{
					//write index
					Serialiser::Serialise(index);
				}
				//close entityIndices
				Serialiser::EndArray();

				Serialiser::SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);
				Serialiser::EndObject();
			}
			//close entity group
			Serialiser::EndArray();
			Serialiser::EndObject();
		}
		//close componenttype group
		Serialiser::EndArray();


		//create type group
		Serialiser::StartArray("ComponentTypeReference");
		for(auto& it : sm_componentHashTypes)
		{
			Serialiser::StartObject();
			//write name
			Serialiser::Serialise("ComponentTypeName", sm_componentHashNames[it.first]);

			//write id
			Serialiser::Serialise("ComponentTypeID", sm_componentHashTypes[it.first]);

			Serialiser::EndObject();
		}
		Serialiser::EndArray();
	}

	/// <summary>
	/// Removes all components associated with the given entity
	/// </summary>
	/// <param name="entity"></param>
	void ComponentManager::RemoveEntity(EntityID entity)
	{
		for (auto& componentType : m_componentData)
		{
			if (m_componentMap[componentType.first].find(entity) != m_componentMap[componentType.first].end())
			{
				std::vector<int> componentIndexesToRemove = m_componentMap[componentType.first][entity];
				std::sort(componentIndexesToRemove.begin(), componentIndexesToRemove.end());
				for (auto& index : componentIndexesToRemove)
				{
					// Delete the component
					delete m_componentData[componentType.first][index];
					// Remove ptr form list
					m_componentData[componentType.first].erase(m_componentData[componentType.first].begin() + index);
					UpdateComponentMap(componentType.first, index);
					//Decrement indices to reflect erased index's removal
					for (auto& index2 : componentIndexesToRemove)
					{
						index2--;
					}
				}
				m_componentMap[componentType.first].erase(entity);
			}
		}
	}
}