#include "UICanvas.h"

namespace Firelight::ECS
{

	UICanvas::UICanvas()
	{
		AddComponent<UICanvasComponent>();
	}

	UICanvasComponent* UICanvas::GetCanvasComponent()
	{
		return GetComponent<UICanvasComponent>();
	}

}