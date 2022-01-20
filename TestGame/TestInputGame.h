#pragma once
#include"Source/Events/Listener.h"
#include<vector>
class TestInputGame: public Firelight::Events::Listener
{
public:
	TestInputGame();
	~TestInputGame();
	void HandleEvents(void* data);

	std::vector<unsigned char> inputs;
};

