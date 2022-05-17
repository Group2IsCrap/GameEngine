#include "AIAttackingBehaviour.h"

AIAttackingBehaviour::AIAttackingBehaviour(RigidBodyComponent* rigidbodyComponent, AIComponent* targetAIComponent, float speed, float attackInterval) : m_rigidBodyComponent(rigidbodyComponent), m_targetAIComponent(targetAIComponent), m_speed(speed)
{

}

void AIAttackingBehaviour::HandleState(const Firelight::Utils::Time& time)
{
	if (m_targetAIComponent == nullptr || m_targetAIComponent->m_Target == nullptr)
		return;

	Firelight::Maths::Vec3f dir = m_targetAIComponent->m_Target->GetComponent<RigidBodyComponent>()->nextPos - m_rigidBodyComponent->nextPos;
	dir.Normalise();

	m_rigidBodyComponent->velocity += dir * m_speed * time.GetDeltaTime();
}
