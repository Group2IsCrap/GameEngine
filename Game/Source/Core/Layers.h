#pragma once

// The layer of which the entity's layer component is set to
enum class GameLayer : int
{
	Player = 0,
	Items = 1,
	Enemy = 2,
	Resource = 3
};

// The layer of which the entity should render on
enum class RenderLayer : int
{
	World = 0,
	Player = 999,
	Enemy = 998,
	Items = 997,
	UI = 1000
};