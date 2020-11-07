#pragma once

#ifdef ET_PLATFORM_WINDOWS

extern Eternal::Application* Eternal::CreateApplication();

int main(int argc, char** argv) //argc = argument count, argv = argument vector
{
	Eternal::Log::Init();
	ET_CORE_WARN("Initialized Log!");

	auto app = Eternal::CreateApplication();
	app->Run();
	delete app;
}

#endif 
