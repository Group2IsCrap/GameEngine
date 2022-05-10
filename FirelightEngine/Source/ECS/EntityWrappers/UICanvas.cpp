#include "UICanvas.h"

namespace Firelight::ECS
{

	UICanvas::UICanvas()
	{
		AddComponent<UICanvasComponent>();
	}

	UICanvas::UICanvas(Firelight::Maths::Vec3f dimensions, int layer, bool draw, Firelight::ECS::e_AnchorSettings anchorSettings)
	{
		AddComponent<UICanvasComponent>();
		GetSpriteComponent()->toDraw = draw;
		SetAnchorSettings(anchorSettings);
		GetCanvasComponent()->XScreenSize = dimensions.x;
		GetCanvasComponent()->YScreenSize = dimensions.y;
		GetCanvasComponent()->layer = layer;
	}

	UICanvasComponent* UICanvas::GetCanvasComponent()
	{
		return GetComponent<UICanvasComponent>();
	}

}