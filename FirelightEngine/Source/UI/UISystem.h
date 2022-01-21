#pragma once
#include"..\Events\Listener.h"
#include"..\ECS\Systems\System.h"


//inputs
#include"..\Input\MouseEvent.h"
#include"..\Input\KeyboardEvent.h"
#include"..\Input\ControllerEvent.h"

namespace Firelight::UI {
	class UISystem :public Firelight::ECS::System, public Firelight::Events::Listener
	{
		UISystem();
		~UISystem();


		virtual void Update(double dt) override;
		virtual void LateUpdate(double dt) override;
		virtual void FixedUpdate(double fixeddt) override;

		
		void HandleEvents(void* data);

		/// <summary>
		/// subscribe to events
		/// </summary>
		void Initalize();


		void UpdateDocking();
	private:

		//Event function Invoke
		void OnPress(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent);
		void OnLeave(int x, int y);
		void OnHover(int x, int y);
		void OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent);
		
		//move through items
		void OnNavergate();

		void CheckChildern();
		void CheckParent();

		void DockingSettings();
	};

}