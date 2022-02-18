#include "Template.h"

namespace Firelight::ECS
{
	Template::Template()
	{
		m_templateID = EntityComponentSystem::Instance()->CreateTemplate();
	}

	Template::~Template()
	{
	}

	void Template::Destroy()
	{
		EntityComponentSystem::Instance()->RemoveEntity(m_templateID);
	}

    EntityID Template::GetTemplateID()
    {
        return m_templateID;
    }
}