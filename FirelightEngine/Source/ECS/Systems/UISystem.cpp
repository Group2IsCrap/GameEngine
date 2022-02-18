#include "UISystem.h"

#include "..\Source\Engine.h"


namespace Firelight::UI {
	
	UISystem::UISystem()
	{
		Initalize();
		
		AddWhitelistComponent<ECS::UIBaseWidgetComponent>();
		AddWhitelistComponent<ECS::PixelSpriteComponent>();
		AddWhitelistComponent<ECS::TransformComponent>();
		Events::EventDispatcher::SubscribeFunction<Events::UI::UpdateUIEvent>(std::bind(&UI::UISystem::SetSettings, this));
		
	}
	UISystem::~UISystem()
	{
	}
	void UISystem::Update(const Utils::Time& time)
	{
		UNREFERENCED_PARAMETER(time);

		if (m_dragWidget != nullptr) 
		{
			if (m_MouseRawCurr != Maths::Vec2f(0,0)) 
			{
				m_dragWidget->defaultPosition = m_MousePosDrag;
				m_isDragging = true;

				if (m_dragWidget->hasParent)
				{
					AnchorSettings(m_dragEntity);
				}
				AnchorSettings();
			}
		}

		//leave check
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			//checks
			ECS::UIBaseWidgetComponent* UIComponent = m_entities[entityIndex]->GetComponent<ECS::UIBaseWidgetComponent>();
			ECS::TransformComponent* UITransformComponent = m_entities[entityIndex]->GetComponent<ECS::TransformComponent>();
			ECS::PixelSpriteComponent* UISpriteComponent = m_entities[entityIndex]->GetComponent<ECS::PixelSpriteComponent>();

			POINT currMousePos;
			GetCursorPos(&currMousePos);
			ScreenToClient(Engine::Instance().GetWindowHandle(), &currMousePos);
			if (IsHit(currMousePos.x, currMousePos.y, UIComponent, UITransformComponent)) 
			{
				break;
			}
			if (m_dragWidget != nullptr)
			{
				m_dragWidget->anchorSettings = m_CurrDragAnchor;
				if (m_dragWidget->hasParent)
				{
					AnchorSettings(m_dragEntity);
				}
			}
			m_dragEntity = nullptr;
			m_dragWidget = nullptr;
			m_dragButtonIsPressed = false;
			m_isDragging = false;

			AnchorSettings();

		}

	}

	void UISystem::HandleEvents(const char* event , void* data)
	{
		if (event == Events::Input::ContollerEvent::sm_descriptor) 
		{
			Firelight::Events::Input::ControllerState* eventController = (Firelight::Events::Input::ControllerState*)data;
			for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
			{
				ECS::UIBaseWidgetComponent* UIComponent = m_entities[entityIndex]->GetComponent<ECS::UIBaseWidgetComponent>();
				if (m_entities[entityIndex]->HasComponent<ECS::UIDraggableComponent>() && eventController->m_A)
				{

					OnDrag(m_MousePosDrag.x, m_MousePosDrag.y, Events::Input::e_MouseEventType::LPress, m_entities[entityIndex]);

				}
				if (!eventController->m_A && m_PrevEventController.m_A) 
				{
					if (m_dragWidget == UIComponent) 
					{
						OnDrag(m_MousePosDrag.x, m_MousePosDrag.y, Events::Input::e_MouseEventType::LRelease, m_entities[entityIndex]);
					}
					if (m_entities[entityIndex]->HasComponent<ECS::UIPressableComponent>() && m_dragWidget == nullptr) 
					{
						OnPress(m_MousePosDrag.x, m_MousePosDrag.y, Events::Input::e_MouseEventType::LRelease, m_entities[entityIndex]);
					}
				}
				if (eventController->m_LeftStick.x > 0.5 || eventController->m_LeftStick.x < -0.5 || eventController->m_LeftStick.y > 0.5 || eventController->m_LeftStick.y < -0.5) 
				{
					m_MouseRawCurr = Maths::Vec2f(1, 1);
				}
				if (eventController->m_A != m_PrevEventController.m_A) 
				{
					m_MouseRawCurr = Maths::Vec2f(0, 0);	
				}
			}
			m_PrevEventController = *eventController;
			return;
		}
		else if (event == Events::Input::OnKeyPress::sm_descriptor) 
		{
			unsigned char eventKey = reinterpret_cast<unsigned char>(data);

			return;
		}
		else 
		{
			Firelight::Events::Input::MouseEvent* eventMouse = static_cast<Firelight::Events::Input::MouseEvent*>(data);

			if (eventMouse->GetType() == Events::Input::e_MouseEventType::Move) 
			{
				m_MousePosDrag = Maths::Vec3f((float)eventMouse->GetMouseX(), (float)eventMouse->GetMouseY(), 0.0f);
				
			}
			if (eventMouse->GetType() == Events::Input::e_MouseEventType::RawMove) 
			{

				m_MouseRawCurr = Maths::Vec2f((float)eventMouse->GetMouseX(), (float)eventMouse->GetMouseY());

			}

			for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
			{
				ECS::UIBaseWidgetComponent* UIComponent = m_entities[entityIndex]->GetComponent<ECS::UIBaseWidgetComponent>();

				if (eventMouse->GetType() != Events::Input::e_MouseEventType::RawMove)
				{
					if (m_dragWidget == nullptr) {
						if (eventMouse->GetType() != Events::Input::e_MouseEventType::Move) 
						{
							if (m_entities[entityIndex]->HasComponent<ECS::UIPressableComponent>()) 
							{
								OnPress(eventMouse->GetMouseX(), eventMouse->GetMouseY(), eventMouse->GetType(), m_entities[entityIndex]);
							}
						}
						if (m_entities[entityIndex]->HasComponent<ECS::UIHoverableComponent>())
						{
							OnHover(eventMouse->GetMouseX(), eventMouse->GetMouseY(), m_entities[entityIndex]);
						}
					}
					if (m_entities[entityIndex]->HasComponent<ECS::UIDraggableComponent>())
					{
						OnDrag(eventMouse->GetMouseX(), eventMouse->GetMouseY(), eventMouse->GetType(), m_entities[entityIndex]);
					}
				}
				if (eventMouse->GetType() == Events::Input::e_MouseEventType::Move && m_focusedWidget == UIComponent)
				{
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
			ECS::UIBaseWidgetComponent* UIComponent = entity->GetComponent<ECS::UIBaseWidgetComponent>();
			ECS::TransformComponent* UITransformComponent = entity->GetComponent<ECS::TransformComponent>();
			ECS::UIPressableComponent* UIPressableComponet = entity->GetComponent<ECS::UIPressableComponent>();

			if (IsHit(x, y, UIComponent, UITransformComponent))
			{
				switch (mouseEvent)
				{
				case Firelight::Events::Input::e_MouseEventType::LRelease:
				{
					
					for (auto&& Event : UIPressableComponet->onLeftPressFunctions)
					{
						Event();
					}
					
				}
				break;
				case Firelight::Events::Input::e_MouseEventType::RRelease:
					
					for (auto&& Event : UIPressableComponet->onRightPressFunctions)
					{
						Event();
					}
					
					break;
				case Firelight::Events::Input::e_MouseEventType::MRelease:
					
					for (auto&& Event : UIPressableComponet->onMiddlePressFunctions)
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
		if (!IsHit(x, y, m_focusedWidget, m_focusedTransform))
		{
			m_dragButtonIsPressed = false;
			
			ECS::UIButtonComponent* button = m_focusedEntity->GetComponent<ECS::UIButtonComponent>();
			if (button != nullptr)
			{
				if (button->isChangeOfTex)
				{
					m_focusedSprite->colour = button->colour[0];
				}
			}

			m_focusedEntity = nullptr;
			m_focusedWidget = nullptr;
			m_focusedTransform = nullptr;
			m_focusedSprite = nullptr;

		}

	}
	void UISystem::OnHover(int x, int y, ECS::Entity* entity)
	{
		ECS::UIBaseWidgetComponent* UIComponent = entity->GetComponent<ECS::UIBaseWidgetComponent>();
		ECS::TransformComponent* UITransformComponent = entity->GetComponent<ECS::TransformComponent>();
		ECS::PixelSpriteComponent* UISpriteComponent = entity->GetComponent<ECS::PixelSpriteComponent>();
		ECS::UIHoverableComponent* UIHoverComponent = entity->GetComponent<ECS::UIHoverableComponent>();

		if (IsHit(x, y, UIComponent, UITransformComponent))
		{
			if (m_focusedWidget != UIComponent && m_focusedSprite != nullptr)
			{
				ECS::UIButtonComponent* button = m_focusedEntity->GetComponent<ECS::UIButtonComponent>();
				if (button != nullptr)
				{
					if (button->isChangeOfTex)
					{
						m_focusedSprite->colour = button->colour[0];
						
					}
				}
				OnLeave(x, y, m_focusedEntity);
			}

			m_focusedEntity = entity;
			m_focusedWidget = UIComponent;
			m_focusedTransform = UITransformComponent;
			m_focusedSprite = UISpriteComponent;

			//do hover
			for (auto&& Event : UIHoverComponent->onHoverFunctions)
			{
				Event();
			}

			ECS::UIButtonComponent* button = m_focusedEntity->GetComponent<ECS::UIButtonComponent>();
			if (button != nullptr) 
			{
				if (button->isChangeOfTex)
				{
					m_focusedSprite->colour = button->colour[1];
				}
			}
		}
	}
	
	void UISystem::OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::Entity* entity)
	{
		ECS::UIBaseWidgetComponent* UIComponent = entity->GetComponent<ECS::UIBaseWidgetComponent>();
		ECS::TransformComponent* UITransformComponent = entity->GetComponent<ECS::TransformComponent>();
		ECS::PixelSpriteComponent* UISpriteComponent = entity->GetComponent<ECS::PixelSpriteComponent>();

		m_ClickTimer.Stop();
		if (IsHit(x, y, UIComponent, UITransformComponent))
		{

			if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LPress && m_dragWidget == nullptr) {

				//say item is being draged

				m_ClickTimer.Start();
				m_dragButtonIsPressed = true;

			}
			if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LRelease)
			{
				if (m_dragWidget != nullptr)
				{
					m_dragWidget->anchorSettings = m_CurrDragAnchor;

					if (m_dragWidget->hasParent)
					{
						AnchorSettings(entity);
					}
				}

				m_dragButtonIsPressed = false;
				m_isDragging = false;

				AnchorSettings();
			}

			if (m_dragButtonIsPressed && m_ClickTimer.GetDurationSeconds() > 0.1f) 
			{
				auto parentComponent = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(UIComponent->parentID);
				if (UIComponent->hasParent && parentComponent != nullptr && m_dragWidget == parentComponent && !m_isDragging)
				{
					if (m_dragWidget != nullptr) 
					{
						m_dragWidget->anchorSettings = m_CurrDragAnchor;
					}
					m_dragEntity = entity;
					m_dragWidget = UIComponent;
					m_dragSprite = UISpriteComponent;
					m_dragTransform = UITransformComponent;

					m_focusedEntity = entity;
					m_focusedWidget = m_dragWidget;
					m_focusedTransform = UITransformComponent;
					m_focusedSprite = UISpriteComponent;

					m_CurrDragAnchor = m_dragWidget->anchorSettings;
					m_dragWidget->anchorSettings = ECS::e_AnchorSettings::None;
				}
				else if (m_dragWidget == nullptr)
				{
					if (m_dragWidget != nullptr) 
					{
						m_dragWidget->anchorSettings = m_CurrDragAnchor;
					}

					m_dragEntity = entity;
					m_dragWidget = UIComponent;
					m_dragSprite = UISpriteComponent;
					m_dragTransform = UITransformComponent;

					m_focusedEntity = entity;
					m_focusedWidget = m_dragWidget;
					m_focusedTransform = UITransformComponent;
					m_focusedSprite = UISpriteComponent;

					m_CurrDragAnchor = m_dragWidget->anchorSettings;
					m_dragWidget->anchorSettings = ECS::e_AnchorSettings::None;
				}

			}

		}
		if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LRelease && m_dragWidget == UIComponent) 
		{
			m_dragEntity = nullptr;
			m_dragWidget = nullptr;
			m_dragSprite = nullptr;
			m_dragTransform = nullptr;
			m_isDragging = false;
		}
		
	
	}

	void UISystem::OnNavigate()
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

	bool UISystem::IsHit(int x, int y, ECS::UIBaseWidgetComponent* widget, ECS::TransformComponent* transform)
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
		ECS::UICanvasComponent* currCanvas = nullptr;
		ECS::UIBaseWidgetComponent* UIComponent = entity->GetComponent<ECS::UIBaseWidgetComponent>();
		ECS::TransformComponent* UITransformComponent = entity->GetComponent<ECS::TransformComponent>();
		ECS::PixelSpriteComponent* UISpriteComponent = entity->GetComponent<ECS::PixelSpriteComponent>();
		if (m_Canvas != nullptr) 
		{
			currCanvas = m_Canvas->GetComponent<ECS::UICanvasComponent>();
		}

		ECS::UICanvasComponent* UICanvas = entity->GetComponent<ECS::UICanvasComponent>();
		
		
		Maths::Vec2f screen = Engine::Instance().GetWindowDimensionsFloat();
		if (!UIComponent->hasParent)
		{

			if (UICanvas != nullptr) 
			{
				UISpriteComponent->layer = UICanvas->layer;
				m_Canvas = entity;
				m_CanvasLayer = UICanvas->layer + 1;
			}
			else 
			{
				UISpriteComponent->layer = m_CanvasLayer;
			}
			UITransformComponent->scale = UIComponent->defaultScale;
			float width = Engine::Instance().GetWindowDimensionsFloat().x * UITransformComponent->scale.x;
			float hight = Engine::Instance().GetWindowDimensionsFloat().y * UITransformComponent->scale.y;

			switch (UIComponent->anchorSettings)
			{
			case Firelight::ECS::e_AnchorSettings::TopLeft:
				UITransformComponent->position.y = 0 + (hight * 0.5f);
				UITransformComponent->position.x = 0 + (width * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Top:
				UITransformComponent->position.y = 0 + (hight * 0.5f);
				UITransformComponent->position.x = (screen.x * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::TopRight:
				UITransformComponent->position.y = 0 + (hight * 0.5f);
				UITransformComponent->position.x = screen.x - (width * 0.5f);
				break;

			case Firelight::ECS::e_AnchorSettings::BottomLeft:
				UITransformComponent->position.y = screen.y - (hight * 0.5f);
				UITransformComponent->position.x = 0 + (width * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Bottom:
				UITransformComponent->position.y = screen.y - (hight * 0.5f);
				UITransformComponent->position.x = (screen.x * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::BottomRight:
				UITransformComponent->position.y = screen.y - (hight * 0.5f);
				UITransformComponent->position.x = screen.x - (width * 0.5f);
				break;

			case Firelight::ECS::e_AnchorSettings::Left:
				UITransformComponent->position.y = (screen.y * 0.5f) + (hight * 0.5f);
				UITransformComponent->position.x = 0 + (width * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Center:
				UITransformComponent->position.y = (screen.y * 0.5f);
				UITransformComponent->position.x = (screen.x * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Right:
				UITransformComponent->position.y = (screen.y * 0.5f) + (hight * 0.5f);
				UITransformComponent->position.x = screen.x - (width * 0.5f);
				break;

			case Firelight::ECS::e_AnchorSettings::None:
				UITransformComponent->position = UIComponent->defaultPosition;
				return;
				break;
			default:
				UITransformComponent->position = UIComponent->defaultPosition;
				break;
			}
		}
		else
		{
			ECS::UIBaseWidgetComponent* currWidgetParent = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(UIComponent->parentID);
			ECS::TransformComponent* currTransform = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TransformComponent>(UIComponent->parentID);
			ECS::PixelSpriteComponent* currSprite = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(UIComponent->parentID);

			if (currCanvas != nullptr) 
			{
				UIComponent->defaultScale.x = UIComponent->defaultDimensions.x / currCanvas->XScreenSize;
				UIComponent->defaultScale.y = UIComponent->defaultDimensions.y / currCanvas->YScreenSize;
			}
			UIComponent->currentScale = UIComponent->defaultScale * currWidgetParent->currentScale;
			UITransformComponent->scale = UIComponent->currentScale;

			float width = screen.x * currTransform->scale.x;
			float hight = screen.y * currTransform->scale.y;
			float childWidth = screen.x * UIComponent->currentScale.x;
			float childHight = screen.y * UIComponent->currentScale.y;

			switch (UIComponent->anchorSettings)
			{
				case Firelight::ECS::e_AnchorSettings::TopLeft:
					UITransformComponent->position.y = (currTransform->position.y- (hight*0.5f)) + (childHight * 0.5f);
					UITransformComponent->position.x = (currTransform->position.x - (width * 0.5f)) + (childWidth * 0.5f);
					break;
				case Firelight::ECS::e_AnchorSettings::Top:
					UITransformComponent->position.y = (currTransform->position.y - (hight * 0.5f)) + (childHight * 0.5f);
					UITransformComponent->position.x = currTransform->position.x;
					break;
				case Firelight::ECS::e_AnchorSettings::TopRight:
					UITransformComponent->position.y = (currTransform->position.y - (hight * 0.5f)) + (childHight * 0.5f);
					UITransformComponent->position.x = (currTransform->position.x + (width * 0.5f))- (childWidth * 0.5f);
					break;

				case Firelight::ECS::e_AnchorSettings::BottomLeft:
					UITransformComponent->position.y = (currTransform->position.y + (hight * 0.5f)) - (childHight * 0.5f);
					UITransformComponent->position.x = (currTransform->position.x - (width * 0.5f)) + (childWidth * 0.5f);
					break;
				case Firelight::ECS::e_AnchorSettings::Bottom:
					UITransformComponent->position.y = (currTransform->position.y + (hight * 0.5f)) - (childHight * 0.5f);
					UITransformComponent->position.x = currTransform->position.x;
					break;
				case Firelight::ECS::e_AnchorSettings::BottomRight:
					UITransformComponent->position.y = (currTransform->position.y + (hight * 0.5f)) - (childHight * 0.5f);
					UITransformComponent->position.x = (currTransform->position.x + (width * 0.5f)) - (childWidth * 0.5f);
					break;

				case Firelight::ECS::e_AnchorSettings::Left:
					UITransformComponent->position.y = currTransform->position.y;
					UITransformComponent->position.x = (currTransform->position.x - (width * 0.5f)) + (childWidth * 0.5f);
					break;
				case Firelight::ECS::e_AnchorSettings::Center:
					UITransformComponent->position.y = currTransform->position.y;
					UITransformComponent->position.x = currTransform->position.x ;
					break;
				case Firelight::ECS::e_AnchorSettings::Right:
					UITransformComponent->position.y = currTransform->position.y;
					UITransformComponent->position.x = (currTransform->position.x + (width * 0.5f)) - (childWidth * 0.5f);
					break;

				case Firelight::ECS::e_AnchorSettings::None: {
					UITransformComponent->position = UIComponent->defaultPosition;

					ECS::Entity* currentParent = m_Canvas;
					
					for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
					{
						//checks
						ECS::UICanvasComponent* UICanvas = m_entities[entityIndex]->GetComponent<ECS::UICanvasComponent>();
						ECS::UIPanelComponent* UIPanel = m_entities[entityIndex]->GetComponent<ECS::UIPanelComponent>();

						if (UICanvas == nullptr && UIPanel==nullptr) 
						{
							continue;
						}
						else if (UIPanel == nullptr) 
						{
							if (IsHit(UIComponent->defaultPosition.x, UIComponent->defaultPosition.y, UIComponent, UITransformComponent))
							{								
								currentParent = m_entities[entityIndex];
							}
						}
						else
						{
							ECS::UIBaseWidgetComponent* UIPanelComponent = m_entities[entityIndex]->GetComponent<ECS::UIBaseWidgetComponent>();
							ECS::TransformComponent* UIPanelTansformComponent = m_entities[entityIndex]->GetComponent<ECS::TransformComponent>();
							if (m_entities[entityIndex] != entity) 
							{
								if (IsHit(UIComponent->defaultPosition.x, UIComponent->defaultPosition.y, UIPanelComponent, UIPanelTansformComponent)) 
								{
									if (currentParent->GetComponent<ECS::PixelSpriteComponent>()->layer < m_entities[entityIndex]->GetComponent<ECS::PixelSpriteComponent>()->layer) {
										currentParent = m_entities[entityIndex];
										continue;
									}
									
								}
							}
						}

					}

					if (currentParent == nullptr) 
					{
						return;
					}

					UIComponent->parentID = currentParent->GetEntityID();
					ECS::TransformComponent* newParentTransform = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TransformComponent>(UIComponent->parentID);
					ECS::PixelSpriteComponent* newParentSprite = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(UIComponent->parentID);

					UIComponent->currentScale= UIComponent->defaultScale * newParentTransform->scale;
					UITransformComponent->scale = UIComponent->currentScale;
					UISpriteComponent->layer = newParentSprite->layer+1;
					return;
				}
					break;
			default:
				break;
			}
		

			UIComponent->currentScale = UIComponent->defaultScale * currTransform->scale;
			UITransformComponent->scale = UIComponent->currentScale;
			UISpriteComponent->layer = currSprite->layer + 1;
		}
		
		if (currCanvas != nullptr) {
			//offset(pix) = size(pix) * scale factor(%)
			float offX = Engine::Instance().GetWindowDimensionsFloat().x * (UIComponent->offSet.x / currCanvas->XScreenSize);
			float offY = Engine::Instance().GetWindowDimensionsFloat().y * (UIComponent->offSet.y / currCanvas->YScreenSize);
			UITransformComponent->position.x += offX;
			UITransformComponent->position.y += offY;
		}
	}

	void UISystem::SetSettings()
	{
		AnchorSettings();
	}
	

		
}
