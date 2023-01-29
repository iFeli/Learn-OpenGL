#include "Renderer.h"

#include "Variables.h"

int main() 
{
	Pink::Renderer renderer = Pink::Renderer(Pink::WIDTH, Pink::HEIGHT);
	renderer.render();

	return 0;
}