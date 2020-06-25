#pragma once

#ifdef ET_PLATFORM_WINDOWS

extern Eternal::Application* Eternal::CreateApplication();

int main(int argc, char** argv) //argc = argument count, argv = argument vector
{
	auto app = Eternal::CreateApplication();
	app->Run();
	delete app;
}

#endif 
