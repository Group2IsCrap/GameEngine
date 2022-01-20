#pragma once
#include"..\Maths\Vec2.h"
#include"..\Events\Event.h"
namespace Firelight::Events::Input {

	//More Readable format 
	struct ControllerState
	{
		bool A = false;
		bool Y = false;
		bool X = false;
		bool B = false;

		bool RightBumber = false;
		bool LeftBumber = false;

		bool StickLeftPress = false;
		bool StickRightPress = false;
		bool Back = false;
		bool Start = false;

		bool DPADUp = false;
		bool DPADDown = false;
		bool DPADLeft = false;
		bool DPADRight = false;
		Maths::Vec2f LeftStick = Maths::Vec2f();
		Maths::Vec2f  RightStick = Maths::Vec2f();
		double LeftTrigger = 0;
		double RightTrigger = 0;

	};

	class ContollerEvent : public Events::Event
	{
	public:
		ContollerEvent() {}
		~ContollerEvent() {}

		static constexpr DescriptorType sm_Description = "ControllerState";

		virtual DescriptorType Type() const override
		{
			return sm_Description;
		}


	};
}