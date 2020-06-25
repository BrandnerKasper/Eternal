#include <Eternal.h>

class Sandbox : public Eternal::Application
{
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Eternal::Application* Eternal::CreateApplication()
{
	return new Sandbox();
}