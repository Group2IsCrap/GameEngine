#pragma once

#include"System.h"

#include "..\Source\Events\Listener.h"

//inputs
#include"..\Source\Input\MouseEvent.h"
#include"..\Source\Input\KeyboardEvent.h"
#include"..\Source\Input\ControllerEvent.h"


#include"..\Components\UIComponents.h"
#include"..\Components\RenderingComponents.h"
#include"..\Source\Utils\Timer.h"
#include"..\Source\Events\UIEvents.h"

namespace Firelight::UI {

	class UISystem :public Firelight::ECS::System, public Firelight::Events::Listener
	{
	public:
		UISystem();
		~UISystem();


		virtual void Update(const Utils::Time& time) override;

		void HandleEvents(const char* event,void* data);

		/// <summary>
		/// subscribe to events
		/// </summary>
		void Initalize();
		void SetSettings();
	private:

		//Event function Invoke
		void OnPress(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* widget);
		void OnLeave(int x, int y);
		void OnHover(int x, int y, ECS::UIWidget* widget);
		void OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* widget);
		
		//move through items
		void OnNavergate();
		void AnchorSettings();

		bool IsHit(int x, int y,ECS::UIWidget* widget);
		void AnchorSettings(ECS::UI_Child* widget);
		
		void AnchorSettings(ECS::UI_Canvas* widget);

	private:

		ECS::UIWidget* m_DragItem= nullptr;
		ECS::UIWidget* m_FocusedItem = nullptr;
		Maths::Vec3f m_MousePosDrag;
		Maths::Vec2f m_MouseRawCurr;
		Utils::Timer m_ClickTimer;

		bool m_DragButtionIsPressed = false;
		bool m_IsDragging = false;

		ECS::e_AnchorSettings m_CurrDragAnchor = ECS::e_AnchorSettings::None;

		Firelight::Events::Input::ControllerState m_PrevEventController;

		float m_CanvasLayer;
	};


	
}