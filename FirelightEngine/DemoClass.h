#pragma once
#include "Source/Events/EventDispatcher.h"
#include "Source/Events/DemoEvent.h"

namespace Firelight::DemoNamespace
{
	class DemoClass
	{
		int m_number;
		DemoClass() : m_number(10)
		{}

		void AFunction()
		{
			Firelight::Events::EventDispatcher::InvokeListeners<Firelight::Events::DemoNamespace::DemoEvent>((void*)&m_number);
		}
	};
}

		//DemoClass()
		//{
		//	Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::DemoNamespace::DemoEvent>(std::bind(&DemoClass::AddingTwoNumbers, this, 2, 5));
		//}

		//void AddingTwoNumbers(int num1, int num2)SS
		//{
		//	 num1 + num2;
		//}

		//void AFunction()
		//{
		//	Events::EventDispatcher::InvokeFunctions<Firelight::Events::DemoNamespace::DemoEvent>();
		//}