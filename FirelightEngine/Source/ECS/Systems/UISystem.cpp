#include "UISystem.h"

#include "..\Source\Engine.h"


namespace Firelight::UI {
	
	UISystem::UISystem()
	{
		Initalize();
		
		AddWhitelistComponent<ECS::UIWidget>();

		Events::EventDispatcher::SubscribeFunction<Events::UI::UpdateUIEvent>(std::bind(&UI::UISystem::SetSettings, this));
		
	}
	UISystem::~UISystem()
	{
	}
	void UISystem::Update(const Utils::Time& time)
	{
		if (m_DragItem != nullptr) {
			if (m_MouseRawCurr != Maths::Vec2f(0,0)) {
				m_DragItem->defaultPosition = m_MousePosDrag;
				m_IsDragging = true;
				

				ECS::UI_Child* uIChild = dynamic_cast<ECS::UI_Child*>(m_DragItem);
				if (uIChild != nullptr) {
					AnchorSettings(uIChild);;
				}
				AnchorSettings();
			}
		}
		//AnchorSettings();
	}

	void UISystem::HandleEvents(const char* event , void* data)
	{


		if (event == Events::Input::ContollerEvent::sm_descriptor) {
			Firelight::Events::Input::ControllerState* eventController = (Firelight::Events::Input::ControllerState*)data;
			for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
			{
				//checks
				ECS::UIWidget* uIComponent = m_entities[entityIndex]->GetComponent<ECS::UIWidget>();

				if (uIComponent->texture == nullptr || uIComponent->transform == nullptr) 
				{
					continue;
				}
				if (uIComponent->isDrag && eventController->m_A ) 
				{

					OnDrag(m_MousePosDrag.x, m_MousePosDrag.y, Events::Input::e_MouseEventType::LPress, uIComponent);

				}
				if (!eventController->m_A && m_PrevEventController.m_A) 
				{
					
					if (m_DragItem == uIComponent) 
					{
						OnDrag(m_MousePosDrag.x, m_MousePosDrag.y, Events::Input::e_MouseEventType::LRelease, uIComponent);
					}
					if (uIComponent->isPressable && m_DragItem == nullptr) {
						OnPress(m_MousePosDrag.x, m_MousePosDrag.y, Events::Input::e_MouseEventType::LRelease, uIComponent);

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

				if (uIComponent->texture == nullptr || uIComponent->transform == nullptr) {
					continue;
				}

				if (eventMouse->GetType() != Events::Input::e_MouseEventType::RawMove)
				{
					if (m_DragItem == nullptr) {
						if (eventMouse->GetType() != Events::Input::e_MouseEventType::Move) 
						{
							if (uIComponent->isPressable) {

								OnPress(eventMouse->GetMouseX(), eventMouse->GetMouseY(), eventMouse->GetType(), uIComponent);

							}
						}
						if (uIComponent->isHover) 
						{
							OnHover(eventMouse->GetMouseX(), eventMouse->GetMouseY(), uIComponent);
						}
					}
					if (uIComponent->isDrag) 
					{

						OnDrag(eventMouse->GetMouseX(), eventMouse->GetMouseY(), eventMouse->GetType(), uIComponent);

					}


				}
				if (eventMouse->GetType() == Events::Input::e_MouseEventType::Move && m_FocusedItem == uIComponent) {
					OnLeave(eventMouse->GetMouseX(), eventMouse->GetMouseY());
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
	
	void UISystem::OnPress(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* widget)
	{
		
			if (IsHit(x, y, widget))
			{
				switch (mouseEvent)
				{
				case Firelight::Events::Input::e_MouseEventType::LRelease:
				{
					
					for (auto&& Event : widget->onLeftPressFunctions)
					{
						Event();
					}
					
				}
				break;
				case Firelight::Events::Input::e_MouseEventType::RRelease:
					
					for (auto&& Event : widget->onRightPressFunctions)
					{
						Event();
					}
					
					break;
				case Firelight::Events::Input::e_MouseEventType::MRelease:
					
					for (auto&& Event : widget->onMiddlePressFunctions)
					{
						Event();
					}
					break;
		
				default:
					break;
				}

			}

	}
	void UISystem::OnLeave(int x, int y)
	{
		
		if (!IsHit(x,y, m_FocusedItem))
		{
			ECS::UI_Button* Button = dynamic_cast<ECS::UI_Button*>(m_FocusedItem);
			if (Button == nullptr) {
				return;
			}
			if (Button->isChangeOfTex) {
				Button->texture->colour = Button->colour[0];
			}
			m_FocusedItem = nullptr;
			
		}

	}
	void UISystem::OnHover(int x, int y, ECS::UIWidget* widget)
	{

		if (IsHit(x, y, widget))
		{
			if (m_FocusedItem != widget) {
				ECS::UI_Button* uIButton = dynamic_cast<ECS::UI_Button*>(m_FocusedItem);
				if (uIButton != nullptr) {
					if (uIButton->isChangeOfTex) {
						uIButton->texture->colour = uIButton->colour[0];
					}
				}
			}

			m_FocusedItem = widget;
			//do hover
			for (auto&& Event : widget->onHoverFunctions)
			{
				Event();
			}

			ECS::UI_Button* Button = dynamic_cast<ECS::UI_Button*>(widget);
			if (Button == nullptr) {
				return;
			}
			if (Button->isChangeOfTex) {
				Button->texture->colour = Button->colour[1];
			}
		}
	}
	
	void UISystem::OnDrag(int x, int y, Firelight::Events::Input::e_MouseEventType mouseEvent, ECS::UIWidget* widget)
	{
		m_ClickTimer.Stop();
		if (IsHit(x, y, widget))
		{
			
			if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LPress && m_DragItem == nullptr) {
				
					//say item is being draged
					
					m_ClickTimer.Start();
					m_DragButtionIsPressed = true;
			}
			else if (mouseEvent == Firelight::Events::Input::e_MouseEventType::LRelease )
			{
				if (m_DragItem != nullptr) {
					m_DragItem->anchorSettings = m_CurrDragAnchor;
				}
				ECS::UI_Child* uIChild = dynamic_cast<ECS::UI_Child*>(m_DragItem);
				if (uIChild != nullptr) {
					AnchorSettings(uIChild);;
				}
				m_DragItem = nullptr;
				m_DragButtionIsPressed = false;
				m_IsDragging = false;
				
				AnchorSettings();
			}
			 
			if (m_DragButtionIsPressed && m_ClickTimer.GetDurationSeconds() > 0.1f) {
				if(m_DragItem == dynamic_cast<ECS::UI_Child*>(widget)->parent && !m_IsDragging)
				{
					if (m_DragItem != nullptr) {
						m_DragItem->anchorSettings = m_CurrDragAnchor;
					}
					m_DragItem = widget;


					
					m_CurrDragAnchor = m_DragItem->anchorSettings;
					m_DragItem->anchorSettings = ECS::e_AnchorSettings::None;
				}
				else if(m_DragItem == nullptr)
				{
					if (m_DragItem != nullptr) {
						m_DragItem->anchorSettings = m_CurrDragAnchor;
					}
					m_DragItem = widget;
					
					m_CurrDragAnchor = m_DragItem->anchorSettings;
					m_DragItem->anchorSettings = ECS::e_AnchorSettings::None;
				}

				
			}
		
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
			ECS::UI_Canvas* uICanvas = m_entities[entityIndex]->GetComponent<ECS::UIWidget,ECS::UI_Canvas>();
			if (uICanvas != nullptr) {
				AnchorSettings(uICanvas);
			}
			
			ECS::UI_Child* uIChild = m_entities[entityIndex]->GetComponent<ECS::UIWidget, ECS::UI_Child>();
			if (uIChild == nullptr) {
				continue;
			}
			if (uIChild == m_DragItem) {
				continue;
			}
			AnchorSettings(uIChild);
		}
	}
	bool UISystem::IsHit(int x, int y, ECS::UIWidget* widget)
	{

		float width = Engine::Instance().GetWindowDimensionsFloat().x* widget->transform->scale.x;
		float hight = Engine::Instance().GetWindowDimensionsFloat().y* widget->transform->scale.y;
		Maths::Rectf rectPixel(
			widget->transform->position.x - (width * 0.5f),
			widget->transform->position.y - (hight * 0.5f),
			width, hight);
		Maths::Rectf rectNDC = rectPixel.CreateNDCRectFromPixelRect(Engine::Instance().GetWindowDimensionsFloat());
		
		RECT rect;
		float widthScreen;
		float heightSCreen;
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

	void UISystem::AnchorSettings(ECS::UI_Child* widget)
	{
		
		//widget->texture->layer = m_CanvasLayer;
		Maths::Vec2f screen = Engine::Instance().GetWindowDimensionsFloat();
		if (widget->parent == nullptr) {
			float width = screen.x * widget->transform->scale.x;
			float hight = screen.y * widget->transform->scale.y;
			
			switch (widget->anchorSettings)
			{
			case Firelight::ECS::e_AnchorSettings::TopLeft:
				widget->transform->position.y = 0 + (hight * 0.5f);
				widget->transform->position.x = 0 + (width * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Top:
				widget->transform->position.y = 0 + (hight * 0.5f);
				widget->transform->position.x = (screen.x * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::TopRight:
				widget->transform->position.y = 0 + (hight * 0.5f);
				widget->transform->position.x = screen.x - (width * 0.5f);
				break;

			case Firelight::ECS::e_AnchorSettings::BottomLeft:
				widget->transform->position.y = screen.y - (hight * 0.5f);
				widget->transform->position.x = 0 + (width * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Bottom:
				widget->transform->position.y = screen.y - (hight * 0.5f);
				widget->transform->position.x = (screen.x * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::BottomRight:
				widget->transform->position.y = screen.y - (hight * 0.5f);
				widget->transform->position.x = screen.x - (width * 0.5f);
				break;

			case Firelight::ECS::e_AnchorSettings::Left:
				widget->transform->position.y = (screen.y * 0.5f) + (hight * 0.5f);
				widget->transform->position.x = 0 + (width * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Center:
				widget->transform->position.y = (screen.y * 0.5f);
				widget->transform->position.x = (screen.x * 0.5f);
				break;
			case Firelight::ECS::e_AnchorSettings::Right:
				widget->transform->position.y = (screen.y * 0.5f) + (hight * 0.5f);
				widget->transform->position.x = screen.x - (width * 0.5f);
				break;

			case Firelight::ECS::e_AnchorSettings::None:
				widget->transform->position = widget->defaultPosition;
				return;
				break;
			default:
				widget->transform->position = widget->defaultPosition;
				break;
			}
		}
		else
		{

			widget->currentScale = widget->defaultScale * widget->parent->currentScale;
			widget->transform->scale = widget->currentScale;

			float width = screen.x * widget->parent->transform->scale.x;
			float hight = screen.y * widget->parent->transform->scale.y;
			float childWidth = screen.x * widget->currentScale.x;
			float childHight = screen.y * widget->currentScale.y;

			switch (widget->anchorSettings)
			{
				case Firelight::ECS::e_AnchorSettings::TopLeft:
					widget->transform->position.y = (widget->parent->transform->position.y- (hight*0.5f)) + (childHight * 0.5f);
					widget->transform->position.x = (widget->parent->transform->position.x - (width * 0.5f)) + (childWidth * 0.5f);
					break;
				case Firelight::ECS::e_AnchorSettings::Top:
					widget->transform->position.y = (widget->parent->transform->position.y - (hight * 0.5f)) + (childHight * 0.5f);
					widget->transform->position.x = widget->parent->transform->position.x;
					break;
				case Firelight::ECS::e_AnchorSettings::TopRight:
					widget->transform->position.y = (widget->parent->transform->position.y - (hight * 0.5f)) + (childHight * 0.5f);
					widget->transform->position.x = (widget->parent->transform->position.x + (width * 0.5f))- (childWidth * 0.5f);
					break;

				case Firelight::ECS::e_AnchorSettings::BottomLeft:
					widget->transform->position.y = (widget->parent->transform->position.y + (hight * 0.5f)) - (childHight * 0.5f);
					widget->transform->position.x = (widget->parent->transform->position.x - (width * 0.5f)) + (childWidth * 0.5f);
					break;
				case Firelight::ECS::e_AnchorSettings::Bottom:
					widget->transform->position.y = (widget->parent->transform->position.y + (hight * 0.5f)) - (childHight * 0.5f);
					widget->transform->position.x = widget->parent->transform->position.x;
					break;
				case Firelight::ECS::e_AnchorSettings::BottomRight:
					widget->transform->position.y = (widget->parent->transform->position.y + (hight * 0.5f)) - (childHight * 0.5f);
					widget->transform->position.x = (widget->parent->transform->position.x + (width * 0.5f)) - (childWidth * 0.5f);
					break;

				case Firelight::ECS::e_AnchorSettings::Left:
					widget->transform->position.y = widget->parent->transform->position.y;
					widget->transform->position.x = (widget->parent->transform->position.x - (width * 0.5f)) + (childWidth * 0.5f);
					break;
				case Firelight::ECS::e_AnchorSettings::Center:
					widget->transform->position.y = widget->parent->transform->position.y;
					widget->transform->position.x = widget->parent->transform->position.x ;
					break;
				case Firelight::ECS::e_AnchorSettings::Right:
					widget->transform->position.y = widget->parent->transform->position.y;
					widget->transform->position.x = (widget->parent->transform->position.x + (width * 0.5f)) - (childWidth * 0.5f);
					break;

				case Firelight::ECS::e_AnchorSettings::None: {
					widget->transform->position = widget->defaultPosition;

					ECS::UIWidget* currentParent = nullptr;
					ECS::UI_Canvas* currentCanvas = nullptr;
					for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
					{
						//checks
						ECS::UI_Canvas* uICanvas = m_entities[entityIndex]->GetComponent<ECS::UIWidget,ECS::UI_Canvas>();
						ECS::UI_Panel* uIPanel = m_entities[entityIndex]->GetComponent<ECS::UIWidget, ECS::UI_Panel>();
						if (uICanvas == nullptr && uIPanel==nullptr) {
							continue;
						}
						else if (uIPanel == nullptr) {
							if (IsHit(widget->defaultPosition.x, widget->defaultPosition.y, uICanvas)) {
								currentParent = uICanvas;
								currentCanvas = uICanvas;
							}
						}
						else
						{
							if (uIPanel != widget) {

									if (IsHit(widget->defaultPosition.x, widget->defaultPosition.y, uIPanel)) {
									
										if (currentParent->texture->layer < uIPanel->texture->layer) {
												currentParent = uIPanel;
												continue;
										}	
									}
							}
						}

					}

					if (currentParent == nullptr) {

						return;
					}
					widget->parent = currentParent;
					
					widget->currentScale= widget->defaultScale* widget->parent->transform->scale;
					widget->transform->scale = widget->currentScale;
					widget->texture->layer = widget->parent->texture->layer+1;
					return;
				}
					break;
			default:
				break;
			}
			widget->currentScale = widget->defaultScale* widget->parent->transform->scale;
			widget->transform->scale = widget->currentScale;
			widget->texture->layer = widget->parent->texture->layer + 1;
		}
		
		widget->transform->position.x += widget->offSet.x;
		widget->transform->position.y += widget->offSet.y;
	}

	void UISystem::AnchorSettings(ECS::UI_Canvas* widget)
	{
		widget->transform->scale = widget->defaultScale;
		float width = Engine::Instance().GetWindowDimensionsFloat().x * widget->transform->scale.x;
		float hight = Engine::Instance().GetWindowDimensionsFloat().y * widget->transform->scale.y;
		Maths::Vec2f screen = Engine::Instance().GetWindowDimensionsFloat();
		widget->texture->layer = widget->layer;

		m_CanvasLayer = widget->layer +1;
		switch (widget->anchorSettings)
		{
		case Firelight::ECS::e_AnchorSettings::TopLeft:
			widget->transform->position.y = 0 + (hight*0.5f);
			widget->transform->position.x = 0 + (width*0.5f);
			break;
		case Firelight::ECS::e_AnchorSettings::Top:
			widget->transform->position.y = 0 + (hight * 0.5f);
			widget->transform->position.x = (screen.x * 0.5f);
			break;
		case Firelight::ECS::e_AnchorSettings::TopRight:
			widget->transform->position.y = 0 + (hight * 0.5f);
			widget->transform->position.x = screen.x - (width * 0.5f);
			break;

		case Firelight::ECS::e_AnchorSettings::BottomLeft:
			widget->transform->position.y = screen.y - (hight * 0.5f);
			widget->transform->position.x = 0 + (width * 0.5f);
			break;
		case Firelight::ECS::e_AnchorSettings::Bottom:
			widget->transform->position.y = screen.y - (hight * 0.5f);
			widget->transform->position.x = (screen.x * 0.5f);
			break;
		case Firelight::ECS::e_AnchorSettings::BottomRight:
			widget->transform->position.y = screen.y - (hight * 0.5f);
			widget->transform->position.x = screen.x - (width * 0.5f);
			break;

		case Firelight::ECS::e_AnchorSettings::Left:
			widget->transform->position.y = (screen.y * 0.5f) + (hight * 0.5f);
			widget->transform->position.x = 0 + (width * 0.5f);
			break;
		case Firelight::ECS::e_AnchorSettings::Center:
			widget->transform->position.y = (screen.y * 0.5f);
			widget->transform->position.x = (screen.x* 0.5f);
			break;
		case Firelight::ECS::e_AnchorSettings::Right:
			widget->transform->position.y = (screen.y * 0.5f) + (hight * 0.5f);
			widget->transform->position.x = screen.x  - (width * 0.5f);
			break;

		case Firelight::ECS::e_AnchorSettings::None:
			widget->transform->position = widget->defaultPosition;
			return;
			break;
		default:
			widget->transform->position = widget->defaultPosition;
			break;
		}

		widget->transform->position.x += widget->offSet.x;
		widget->transform->position.y += widget->offSet.y;
		
	}

	void UISystem::SetSettings()
	{
		AnchorSettings();

	}
	

		
}
