#include "UISystem.h"

#include "..\Source\Engine.h"


namespace Firelight::UI {
	
	UISystem::UISystem()
	{
		Initalize();
		
		AddWhitelistComponent<ECS::UIWidget>();
		AddWhitelistComponent<ECS::PixelSpriteComponent>();
		AddWhitelistComponent<ECS::TransformComponent>();
		Events::EventDispatcher::SubscribeFunction<Events::UI::UpdateUIEvent>(std::bind(&UI::UISystem::SetSettings, this));
		
	}
	UISystem::~UISystem()
	{
	}
	void UISystem::Update(const Utils::Time& time)
	{
		if(m_DragItem != nullptr) {
			if (m_MouseRawCurr != Maths::Vec2f(0, 0)) {
				m_DragItem->defaultPosition = m_MousePosDrag;
				m_IsDragging = true;


				ECS::UI_Child* uIChild = dynamic_cast<ECS::UI_Child*>(m_DragItem);
				if (uIChild != nullptr) {
					AnchorSettings(m_DragID);;
				}
				AnchorSettings();
			}
		}

		//leave check
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			//checks
			ECS::UIWidget* uIComponent = m_entities[entityIndex]->GetComponent<ECS::UIWidget>();
			ECS::TransformComponent* uITansformComponent = m_entities[entityIndex]->GetComponent<ECS::TransformComponent>();
			ECS::PixelSpriteComponent* uISpriteComponent = m_entities[entityIndex]->GetComponent<ECS::PixelSpriteComponent>();
			if (uIComponent == nullptr) {
				continue;
			}

			POINT currMousePos;
			GetCursorPos(&currMousePos);
			ScreenToClient(Engine::Instance().GetWindowHandle(), &currMousePos);
			if (IsHit(currMousePos.x, currMousePos.y, uIComponent, uITansformComponent, uISpriteComponent)) {
				break;
			}
			if (m_DragItem != nullptr) {
				m_DragItem->anchorSettings = m_CurrDragAnchor;
			}
			ECS::UI_Child* uIChild = dynamic_cast<ECS::UI_Child*>(m_DragItem);
			if (uIChild != nullptr) {
				AnchorSettings(m_DragID);;
			}
			m_DragItem = nullptr;
			m_DragButtionIsPressed = false;
			m_IsDragging = false;

			AnchorSettings();

		}

	}

	void UISystem::HandleEvents(const char* event , void* data)
	{


		if (event == Events::Input::ContollerEvent::sm_descriptor) {
			Firelight::Events::Input::ControllerState* eventController = (Firelight::Events::Input::ControllerState*)data;
			for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
			{
				////checks
				//ECS::UIWidget* uIComponent = m_entities[entityIndex]->GetComponent<ECS::UIWidget>();
				//ECS::TransformComponent* uITansformComponent = m_entities[entityIndex]->GetComponent<ECS::TransformComponent>();
				//ECS::PixelSpriteComponent* uISpriteComponent = m_entities[entityIndex]->GetComponent<ECS::PixelSpriteComponent>();
				//if (uITansformComponent == nullptr || uISpriteComponent == nullptr)
				//{
				//	continue;
				//}
				//if (uIComponent->isDrag && eventController->m_A ) 
				//{

				//	OnDrag(m_MousePosDrag.x, m_MousePosDrag.y, Events::Input::e_MouseEventType::LPress, uIComponent);

				//}
				//if (!eventController->m_A && m_PrevEventController.m_A) 
				//{
				//	
				//	if (m_DragItem == uIComponent) 
				//	{
				//		OnDrag(m_MousePosDrag.x, m_MousePosDrag.y, Events::Input::e_MouseEventType::LRelease, uIComponent);
				//	}
				//	if (uIComponent->isPressable && m_DragItem == nullptr) {
				//		OnPress(m_MousePosDrag.x, m_MousePosDrag.y, Events::Input::e_MouseEventType::LRelease, uIComponent);

				//	}
				//}
				//if (eventController->m_LeftStick.x > 0.5 || eventController->m_LeftStick.x < -0.5 || eventController->m_LeftStick.y > 0.5 || eventController->m_LeftStick.y < -0.5) 
				//{
				//	m_MouseRawCurr = Maths::Vec2f(1, 1);
				//}
				//if (eventController->m_A != m_PrevEventController.m_A) 
				//{
				//	m_MouseRawCurr = Maths::Vec2f(0, 0);	
				//}
			}
			m_PrevEventController = *eventController;
			return;
		}
		else if (event == Events::Input::OnKeyPress::sm_descriptor) 
		{
			unsigned char eventKey = (unsigned char)data;

			return;
		}
		else 
		{
			Firelight::Events::Input::MouseEvent* eventMouse = static_cast<Firelight::Events::Input::MouseEvent*>(data);

			if (eventMouse->GetType() == Events::Input::e_MouseEventType::Move) 
			{
				m_MousePosDrag = Maths::Vec3f(eventMouse->GetMouseX(), eventMouse->GetMouseY(), 0);
				
			}
			if (eventMouse->GetType() == Events::Input::e_MouseEventType::RawMove) 
			{

				m_MouseRawCurr = Maths::Vec2f(eventMouse->GetMouseX(), eventMouse->GetMouseY());

			}

			for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
			{
				//checks
				ECS::UIWidget* uIComponent = m_entities[entityIndex]->GetComponent<ECS::UIWidget>();
				ECS::TransformComponent* uITansformComponent = m_entities[entityIndex]->GetComponent<ECS::TransformComponent>();
				ECS::PixelSpriteComponent* uISpriteComponent = m_entities[entityIndex]->GetComponent<ECS::PixelSpriteComponent>();
				if (uITansformComponent == nullptr || uISpriteComponent == nullptr)
				{
					continue;
				}

				if (eventMouse->GetType() != Events::Input::e_MouseEventType::RawMove)
				{
					if (m_DragItem == nullptr) {
						if (eventMouse->GetType() != Events::Input::e_MouseEventType::Move) 
						{
							if (uIComponent->isPressable) {

								OnPress(eventMouse->GetMouseX(), eventMouse->GetMouseY(), eventMouse->GetType(), m_entities[entityIndex]);

							}
						}
						if (uIComponent->isHover) 
						{
							OnHover(eventMouse->GetMouseX(), eventMouse->GetMouseY(), m_entities[entityIndex]);
						}
					}
					if (uIComponent->isDrag) 
					{

						OnDrag(eventMouse->GetMouseX(), eventMouse->GetMouseY(), eventMouse->GetType(), m_entities[entityIndex]);

					}


				}
				if (eventMouse->GetType() == Events::Input::e_MouseEventType::Move && m_FocusedItem == uIComponent) {
					OnLeave(eventMouse->GetMouseX(), eventMouse->GetMouseY(), m_entities[entityIndex]);
				}
			}
		}
		
		
		
	}
	void UISystem::Initalize()
	{
		Events::EventDispatcher::AddListener<Events::Input::OnKeyPress>(this);
		Events::EventDispatcher::AddListener<Events::Input::MouseButtonPressEvent>(this);
		Events::EventDispatcher::AddListener<Events::Input::MouseButtonReleaseEvent>(this);
		Events::EventDispatcher::AddListener<Events::Input::MouseMoveEvent>(this);
		Events::EventDispatcher::AddListener<Events::Input::MouseMoveRawEvent>(this);
		Events::EventDispatcher::AddListener<Events::Input::ContollerEvent>(this);
	}
	
	void UISystem::OnPress(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::Entity* entity)
	{
			ECS::UIWidget* uIComponent = entity->GetComponent<ECS::UIWidget>();
			ECS::TransformComponent* uITansformComponent = entity->GetComponent<ECS::TransformComponent>();
			ECS::PixelSpriteComponent* uISpriteComponent = entity->GetComponent<ECS::PixelSpriteComponent>();

			if (IsHit(x, y, uIComponent, uITansformComponent, uISpriteComponent))
			{
				switch (mouseEvent)
				{
				case Firelight::Events::Input::e_MouseEventType::LRelease:
				{
					
					for (auto&& Event : uIComponent->onLeftPressFunctions)
					{
						Event();
					}
					
				}
				break;
				case Firelight::Events::Input::e_MouseEventType::RRelease:
					
					for (auto&& Event : uIComponent->onRightPressFunctions)
					{
						Event();
					}
					
					break;
				case Firelight::Events::Input::e_MouseEventType::MRelease:
					
					for (auto&& Event : uIComponent->onMiddlePressFunctions)
					{
						Event();
					}
					break;
		
				default:
					break;
				}

			}

	}
	void UISystem::OnLeave(int x, int y, ECS::Entity* entity)
	{

		if (!IsHit(x, y, m_FocusedItem, m_FocusedTransform,m_FocusedSprite))
		{

			m_DragButtionIsPressed = false;
			

			ECS::UI_Button* Button = dynamic_cast<ECS::UI_Button*>(m_FocusedItem);
			if (Button != nullptr) {
				if (Button->isChangeOfTex) {
					m_FocusedSprite->colour = Button->colour[0];
				}
			}
			m_FocusedItem = nullptr;
			m_FocusedTransform = nullptr;
			m_FocusedSprite = nullptr;

		}

	}
	void UISystem::OnHover(int x, int y, ECS::Entity* entity)
	{
		ECS::UIWidget* uIComponent = entity->GetComponent<ECS::UIWidget>();
		ECS::TransformComponent* uITansformComponent = entity->GetComponent<ECS::TransformComponent>();
		ECS::PixelSpriteComponent* uISpriteComponent = entity->GetComponent<ECS::PixelSpriteComponent>();
		if (IsHit(x, y, uIComponent, uITansformComponent, uISpriteComponent))
		{
			if (m_FocusedItem != uIComponent) {
				ECS::UI_Button* uIButton = dynamic_cast<ECS::UI_Button*>(m_FocusedItem);
				if (uIButton != nullptr) {
					if (uIButton->isChangeOfTex) {
						m_FocusedSprite->colour = uIButton->colour[0];
					}
				}
			}


			m_FocusedItem = uIComponent;
			m_FocusedTransform = uITansformComponent;
			m_FocusedSprite = uISpriteComponent;
			//do hover
			for (auto&& Event : m_FocusedItem->onHoverFunctions)
			{
				Event();
			}

			ECS::UI_Button* Button = dynamic_cast<ECS::UI_Button*>(m_FocusedItem);
			if (Button == nullptr) {
				return;
			}
			if (Button->isChangeOfTex) {
				m_FocusedSprite->colour = Button->colour[1];
			}
		}
	}
	
	void UISystem::OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::Entity* entity)
	{
		ECS::UIWidget* uIComponent = entity->GetComponent<ECS::UIWidget>();
		ECS::TransformComponent* uITansformComponent = entity->GetComponent<ECS::TransformComponent>();
		ECS::PixelSpriteComponent* uISpriteComponent = entity->GetComponent<ECS::PixelSpriteComponent>();

		m_ClickTimer.Stop();
		if (IsHit(x, y, uIComponent, uITansformComponent, uISpriteComponent))
		{

			if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LPress && m_DragItem == nullptr) {

				//say item is being draged

				m_ClickTimer.Start();
				m_DragButtionIsPressed = true;

			}
			if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LRelease)
			{
				if (m_DragItem != nullptr) {
					m_DragItem->anchorSettings = m_CurrDragAnchor;
				}
				ECS::UI_Child* uIChild = dynamic_cast<ECS::UI_Child*>(m_DragItem);
				if (uIChild != nullptr) {
					AnchorSettings(entity);;
				}

				m_DragButtionIsPressed = false;
				m_IsDragging = false;

				AnchorSettings();
			}

			if (m_DragButtionIsPressed && m_ClickTimer.GetDurationSeconds() > 0.1f) {
				if (m_DragItem == ECS::EntityComponentSystem::Instance()->GetComponents<ECS::UIWidget>(dynamic_cast<ECS::UI_Child*>(uIComponent)->parent)[0] && !m_IsDragging)
				{
					if (m_DragItem != nullptr) {
						m_DragItem->anchorSettings = m_CurrDragAnchor;
					}
					m_DragID= entity;
					m_DragItem = uIComponent;
					m_DragSprite = uISpriteComponent;
					m_DragTransform = uITansformComponent;

					m_FocusedItem = m_DragItem;
					m_FocusedTransform = uITansformComponent;
					m_FocusedSprite = uISpriteComponent;

					m_CurrDragAnchor = m_DragItem->anchorSettings;
					m_DragItem->anchorSettings = ECS::e_AnchorSettings::None;
				}
				else if (m_DragItem == nullptr)
				{
					if (m_DragItem != nullptr) {
						m_DragItem->anchorSettings = m_CurrDragAnchor;
					}
					m_DragID = entity;
					m_DragItem = uIComponent;
					m_DragSprite = uISpriteComponent;
					m_DragTransform = uITansformComponent;

					m_FocusedItem = m_DragItem;
					m_FocusedTransform = uITansformComponent;
					m_FocusedSprite = uISpriteComponent;

					m_CurrDragAnchor = m_DragItem->anchorSettings;
					m_DragItem->anchorSettings = ECS::e_AnchorSettings::None;
				}


			}

		}
		if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LRelease && m_DragItem == uIComponent) {
			m_DragID = nullptr;
			m_DragItem = nullptr;
			m_DragSprite = nullptr;
			m_DragTransform = nullptr;
			m_IsDragging = false;
		}
		
	
	}
	void UISystem::OnNavergate()
	{
		//movethrough on focused
	}
	
	void UISystem::AnchorSettings()
	{
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			AnchorSettings(m_entities[entityIndex]);
		}
	}
	bool UISystem::IsHit(int x, int y, ECS::UIWidget* widget,ECS::TransformComponent* transform,ECS::PixelSpriteComponent* sprite)
	{

		float width = Engine::Instance().GetWindowDimensionsFloat().x* transform->scale.x;
		float hight = Engine::Instance().GetWindowDimensionsFloat().y* transform->scale.y;
		Maths::Rectf rectPixel(
			transform->position.x - (width * 0.5f),
			transform->position.y - (hight * 0.5f),
			width, hight);
		Maths::Rectf rectNDC = rectPixel.CreateNDCRectFromPixelRect(Engine::Instance().GetWindowDimensionsFloat());
		
		RECT rect;
		float widthScreen=0;
		float heightSCreen=0;
		if (GetClientRect(Engine::Instance().GetWindowHandle(), &rect))
		{
			 widthScreen = rect.right - rect.left;
			 heightSCreen = rect.bottom - rect.top;
		}

		float pointNDCX = ((x / widthScreen) - 0.5) * 2;
		float pointNDCY = -((y / heightSCreen) - 0.5) * 2;

		if (pointNDCX >= rectNDC.x &&
			pointNDCX <= (rectNDC.x + rectNDC.w) &&
			pointNDCY >= rectNDC.y &&
			pointNDCY <= (rectNDC.y + rectNDC.h))
		{
			return true;
		}
		return false;
	}

	void UISystem::AnchorSettings(ECS::Entity* entity)
	{
		ECS::UIWidget* uIComponent = entity->GetComponent<ECS::UIWidget>();
		ECS::TransformComponent* uITansformComponent = entity->GetComponent<ECS::TransformComponent>();
		ECS::PixelSpriteComponent* uISpriteComponent = entity->GetComponent<ECS::PixelSpriteComponent>();

		ECS::UI_Canvas* uICanvas = entity->GetComponent<ECS::UIWidget, ECS::UI_Canvas>();
		

		ECS::UI_Child* uIChild = entity->GetComponent<ECS::UIWidget, ECS::UI_Child>();
		
		
		Maths::Vec2f screen = Engine::Instance().GetWindowDimensionsFloat();
		if (uIChild == nullptr) {

			if (uICanvas != nullptr) {
				uISpriteComponent->layer = uICanvas->layer;
				m_CanvasLayer = uICanvas->layer + 1;
			}
			else {
				uISpriteComponent->layer = m_CanvasLayer;
			}
			uITansformComponent->scale = uIComponent->defaultScale;
			float width = Engine::Instance().GetWindowDimensionsFloat().x * uITansformComponent->scale.x;
			float hight = Engine::Instance().GetWindowDimensionsFloat().y * uITansformComponent->scale.y;

			switch (uIComponent->anchorSettings)
			{
			case Firelight::ECS::e_AnchorSettings::TopLeft:
				uITansformComponent->position.y = 0 + (hight * 0.5f);
				uITansformComponent->position.x = 0 + (width * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Top:
				uITansformComponent->position.y = 0 + (hight * 0.5f);
				uITansformComponent->position.x = (screen.x * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::TopRight:
				uITansformComponent->position.y = 0 + (hight * 0.5f);
				uITansformComponent->position.x = screen.x - (width * 0.5f);
				break;

			case Firelight::ECS::e_AnchorSettings::BottomLeft:
				uITansformComponent->position.y = screen.y - (hight * 0.5f);
				uITansformComponent->position.x = 0 + (width * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Bottom:
				uITansformComponent->position.y = screen.y - (hight * 0.5f);
				uITansformComponent->position.x = (screen.x * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::BottomRight:
				uITansformComponent->position.y = screen.y - (hight * 0.5f);
				uITansformComponent->position.x = screen.x - (width * 0.5f);
				break;

			case Firelight::ECS::e_AnchorSettings::Left:
				uITansformComponent->position.y = (screen.y * 0.5f) + (hight * 0.5f);
				uITansformComponent->position.x = 0 + (width * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Center:
				uITansformComponent->position.y = (screen.y * 0.5f);
				uITansformComponent->position.x = (screen.x * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Right:
				uITansformComponent->position.y = (screen.y * 0.5f) + (hight * 0.5f);
				uITansformComponent->position.x = screen.x - (width * 0.5f);
				break;

			case Firelight::ECS::e_AnchorSettings::None:
				uITansformComponent->position = uIComponent->defaultPosition;
				return;
				break;
			default:
				uITansformComponent->position = uIComponent->defaultPosition;
				break;
			}
		}
		else
		{
			std::vector< ECS::UIWidget*>currWidgetParent = ECS::EntityComponentSystem::Instance()->GetComponents<ECS::UIWidget>(uIChild->parent);
			std::vector< ECS::TransformComponent*>currTransform = ECS::EntityComponentSystem::Instance()->GetComponents<ECS::TransformComponent>(uIChild->parent);
			std::vector< ECS::PixelSpriteComponent*>currSprite = ECS::EntityComponentSystem::Instance()->GetComponents<ECS::PixelSpriteComponent>(uIChild->parent);

			uIChild->currentScale = uIComponent->defaultScale * currWidgetParent[0]->currentScale;
			uITansformComponent->scale = uIComponent->currentScale;

			float width = screen.x * currTransform[0]->scale.x;
			float hight = screen.y * currTransform[0]->scale.y;
			float childWidth = screen.x * uIComponent->currentScale.x;
			float childHight = screen.y * uIComponent->currentScale.y;

			switch (uIComponent->anchorSettings)
			{
				case Firelight::ECS::e_AnchorSettings::TopLeft:
					uITansformComponent->position.y = (currTransform[0]->position.y- (hight*0.5f)) + (childHight * 0.5f);
					uITansformComponent->position.x = (currTransform[0]->position.x - (width * 0.5f)) + (childWidth * 0.5f);
					break;
				case Firelight::ECS::e_AnchorSettings::Top:
					uITansformComponent->position.y = (currTransform[0]->position.y - (hight * 0.5f)) + (childHight * 0.5f);
					uITansformComponent->position.x = currTransform[0]->position.x;
					break;
				case Firelight::ECS::e_AnchorSettings::TopRight:
					uITansformComponent->position.y = (currTransform[0]->position.y - (hight * 0.5f)) + (childHight * 0.5f);
					uITansformComponent->position.x = (currTransform[0]->position.x + (width * 0.5f))- (childWidth * 0.5f);
					break;

				case Firelight::ECS::e_AnchorSettings::BottomLeft:
					uITansformComponent->position.y = (currTransform[0]->position.y + (hight * 0.5f)) - (childHight * 0.5f);
					uITansformComponent->position.x = (currTransform[0]->position.x - (width * 0.5f)) + (childWidth * 0.5f);
					break;
				case Firelight::ECS::e_AnchorSettings::Bottom:
					uITansformComponent->position.y = (currTransform[0]->position.y + (hight * 0.5f)) - (childHight * 0.5f);
					uITansformComponent->position.x = currTransform[0]->position.x;
					break;
				case Firelight::ECS::e_AnchorSettings::BottomRight:
					uITansformComponent->position.y = (currTransform[0]->position.y + (hight * 0.5f)) - (childHight * 0.5f);
					uITansformComponent->position.x = (currTransform[0]->position.x + (width * 0.5f)) - (childWidth * 0.5f);
					break;

				case Firelight::ECS::e_AnchorSettings::Left:
					uITansformComponent->position.y = currTransform[0]->position.y;
					uITansformComponent->position.x = (currTransform[0]->position.x - (width * 0.5f)) + (childWidth * 0.5f);
					break;
				case Firelight::ECS::e_AnchorSettings::Center:
					uITansformComponent->position.y = currTransform[0]->position.y;
					uITansformComponent->position.x = currTransform[0]->position.x ;
					break;
				case Firelight::ECS::e_AnchorSettings::Right:
					uITansformComponent->position.y = currTransform[0]->position.y;
					uITansformComponent->position.x = (currTransform[0]->position.x + (width * 0.5f)) - (childWidth * 0.5f);
					break;

				case Firelight::ECS::e_AnchorSettings::None: {
					uITansformComponent->position = uIComponent->defaultPosition;

					ECS::Entity* currentParent = nullptr;
					//ECS::UI_Canvas* currentCanvas = nullptr;
					for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
					{
						//checks
						ECS::UI_Canvas* uICanvas = m_entities[entityIndex]->GetComponent<ECS::UIWidget,ECS::UI_Canvas>();
						

						ECS::UI_Panel* uIPanel = m_entities[entityIndex]->GetComponent<ECS::UIWidget, ECS::UI_Panel>();


						if (uICanvas == nullptr && uIPanel==nullptr) {
							continue;
						}
						else if (uIPanel == nullptr) {
							ECS::TransformComponent* uICanvasTansformComponent = m_entities[entityIndex]->GetComponent<ECS::TransformComponent>();
							ECS::PixelSpriteComponent* uICanvasSpriteComponent = m_entities[entityIndex]->GetComponent<ECS::PixelSpriteComponent>();
							if (IsHit(uIChild->defaultPosition.x, uIChild->defaultPosition.y, uICanvas, uICanvasTansformComponent, uICanvasSpriteComponent)) {
								
									currentParent = m_entities[entityIndex];
								
								//currentCanvas = m_entities[entityIndex];
							}
						}
						else
						{
							
							ECS::TransformComponent* uIPanelTansformComponent = m_entities[entityIndex]->GetComponent<ECS::TransformComponent>();
							ECS::PixelSpriteComponent* uIPanelSpriteComponent = m_entities[entityIndex]->GetComponent<ECS::PixelSpriteComponent>();
							if (uIPanel != uIChild) {

									if (IsHit(uIChild->defaultPosition.x, uIChild->defaultPosition.y, uIPanel, uIPanelTansformComponent, uIPanelSpriteComponent)) {
									
										if (currentParent->GetComponent<ECS::PixelSpriteComponent>()->layer < m_entities[entityIndex]->GetComponent<ECS::PixelSpriteComponent>()->layer) {
												currentParent = m_entities[entityIndex];
												continue;
										}	
									}
							}
						}

					}

					if (currentParent == nullptr) {

						return;
					}
					uIChild->parent = currentParent->GetEntityID();
					std::vector< ECS::TransformComponent*>t= ECS::EntityComponentSystem::Instance()->GetComponents<ECS::TransformComponent>(uIChild->parent);
					std::vector< ECS::PixelSpriteComponent*>Ks = ECS::EntityComponentSystem::Instance()->GetComponents<ECS::PixelSpriteComponent>(uIChild->parent);

					uIComponent->currentScale= uIComponent->defaultScale* t[0]->scale;
					uITansformComponent->scale = uIChild->currentScale;
					uISpriteComponent->layer = Ks[0]->layer+1;
					return;
				}
					break;
			default:
				break;
			}
			std::vector< ECS::TransformComponent*>t = ECS::EntityComponentSystem::Instance()->GetComponents<ECS::TransformComponent>(uIChild->parent);
			std::vector< ECS::PixelSpriteComponent*>Ks = ECS::EntityComponentSystem::Instance()->GetComponents<ECS::PixelSpriteComponent>(uIChild->parent);

			uIComponent->currentScale = uIComponent->defaultScale * t[0]->scale;
			uITansformComponent->scale = uIChild->currentScale;
			uISpriteComponent->layer = Ks[0]->layer + 1;
		}
		

	/*	widget->transform->position.x += widget->offSet.x;
		widget->transform->position.y += widget->offSet.y;*/
	}

	void UISystem::SetSettings()
	{
		AnchorSettings();

	}
	

		
}
