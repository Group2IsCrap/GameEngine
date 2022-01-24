#pragma once

#include"System.h"

#include "..\Source\Events\Listener.h"

//inputs
#include"..\Source\Input\MouseEvent.h"
#include"..\Source\Input\KeyboardEvent.h"
#include"..\Source\Input\ControllerEvent.h"


#include"..\Components\UIComponents.h"
#include"..\Components\RenderingComponents.h"


namespace Firelight::UI {
	class UISystem :public Firelight::ECS::System, public Firelight::Events::Listener
	{
	public:
		UISystem();
		~UISystem();


		virtual void Update(double dt) override;


		
		void HandleEvents(void* data);

		/// <summary>
		/// subscribe to events
		/// </summary>
		void Initalize();


		void UpdateDocking();




		
		


	private:

		//Event function Invoke
		void OnPress(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* widget);
		void OnLeave(int x, int y);
		void OnHover(int x, int y, ECS::UIWidget* widget);
		void OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* widget);
		
		//move through items
		void OnNavergate();

		void CheckChildern();
		
		void DockingSettings();


	private:

		ECS::UIWidget* DagItem= nullptr;
		ECS::UIWidget* FocusedItem = nullptr;
		Maths::Vec3f MousePosDrag;
		Maths::Vec2f mouseRawCurr;
		
	};


	
}