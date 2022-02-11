#pragma once

#include "../Source/ECS/Components/TilemapComponent.h"
#include "../ECSDefines.h"
#include "../Systems/System.h"

namespace Firelight::ECS
{
	class TilemapSystem : public System
	{
	public:
		TilemapSystem();

	private:
		void RenderTilemaps();
		void RenderTilemap(Firelight::ECS::TilemapComponent* tilemap);
	};
}