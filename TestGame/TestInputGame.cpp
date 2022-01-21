#include "TestInputGame.h"
#include"Source/Events/EventDispatcher.h"

#include"Source/Input/ControllerEvent.h"
#include"Source/Input/KeyboardEvent.h"
#include"Source/Input/MouseEvent.h"
#include"Source/Maths/Vec2.h"
TestInputGame::TestInputGame()
{
	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::Input::OnKeyPress>(this);
	
}

TestInputGame::~TestInputGame()
{
}

void TestInputGame::HandleEvents(void* data)
{
	//Firelight::Events::Input::ControllerState* sate = (Firelight::Events::Input::ControllerState*)data;


	//Firelight::Maths::Vec2f* d = (Firelight::Maths::Vec2f*)data;
	//unsigned char key = (unsigned char)data;
}
