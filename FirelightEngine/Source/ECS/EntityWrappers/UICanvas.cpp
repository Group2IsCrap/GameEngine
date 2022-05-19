#include "UICanvas.h"

namespace Firelight::ECS
{

	UICanvas::UICanvas() : UIEntity()
	{
		AddComponent<UICanvasComponent>();
		this->GetIDComponent()->name = "UI Canvas";
	}


	UICanvas::UICanvas(std::string name) : UICanvas()
	{
		this->GetIDComponent()->name = name;
	}

	UICanvas::UICanvas(Firelight::Maths::Vec3f dimensions, int layer, bool draw, Firelight::ECS::e_AnchorSettings anchorSettings) : UIEntity()
	{
		AddComponent<UICanvasComponent>();
		GetSpriteComponent()->toDraw = draw;
		SetAnchorSettings(anchorSettings);
		GetCanvasComponent()->XScreenSize = dimensions.x;
		GetCanvasComponent()->YScreenSize = dimensions.y;
		GetCanvasComponent()->layer = layer;
		this->GetIDComponent()->name = "UI Canvas";
	}

	UICanvasComponent* UICanvas::GetCanvasComponent()
	{
		return GetComponent<UICanvasComponent>();
	}

}