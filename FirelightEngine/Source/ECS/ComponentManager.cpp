#include "ComponentManager.h"

namespace Firelight::ECS
{
	ComponentTypeID ComponentManager::sm_nextComponentID = 0;

	int ComponentManager::GetRegisteredComponentTypeCount()
	{
		return (int)m_componentTypeHash.size();
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
	/// Returns the name of the given component ID
	/// </summary>
	/// <param name="typeID"></param>
	/// <returns></returns>
	const char* ComponentManager::GetComponentName(ComponentTypeID typeID)
	{
		return m_componentHashNames[m_componentTypeHash[typeID]];
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

	void ComponentManager::SerializeAllComponents(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
	{
		//Create Component Group
		writer.Key("Components");
		writer.StartArray();
		for (auto& it : m_componentData)
		{
			writer.StartObject();
			//Create ComponentType group
			writer.Key("ComponentID");
			writer.Uint(it.first);
			writer.Key("Components");
			writer.StartArray();
			for (auto component : it.second)
			{
				//Create component object
				writer.StartObject();
				component->Serialise(writer);
				//close object
				writer.EndObject();
			}
			//close group
			writer.EndArray();
			writer.EndObject();
		}
		//Close component group
		writer.EndArray();

		//Create ComponentType group
		writer.Key("ComponentMap");
		writer.StartArray();
		for (auto &it : m_componentMap)
		{
			//Create entity group

			writer.StartObject();
			writer.Key("ComponentID");
			writer.Uint(it.first);
			writer.Key("EntityIDs");
			writer.StartArray();
			for (auto& it2 : m_componentMap[it.first])
			{
				//Create entityIndices
				writer.StartObject();
				writer.Key("EntityID");
				writer.Uint(it2.first);
				writer.Key("Indices");

				writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatSingleLineArray);
				writer.StartArray();
				for (auto index : m_componentMap[it.first][it2.first])
				{
					//write index
					writer.Int(index);
				}
				//close entityIndices
				writer.EndArray();

				writer.SetFormatOptions(rapidjson::PrettyFormatOptions::kFormatDefault);
				writer.EndObject();
			}
			//close entity group
			writer.EndArray();
			writer.EndObject();
		}
		//close componenttype group
		writer.EndArray();


		//create type group
		writer.Key("ComponentTypeReference");
		writer.StartArray();
		for(auto& it : m_componentHashTypes)
		{
			writer.StartObject();
			//write name
			writer.Key("ComponentTypeName");
			writer.String(m_componentHashNames[it.first]);

			//write id
			writer.Key("ComponentTypeID");
			writer.Uint(m_componentHashTypes[it.first]);

			writer.EndObject();
		}
		writer.EndArray();
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