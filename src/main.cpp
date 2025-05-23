#include <Application.h>
#include <sol/sol.hpp>

int main()
{
    std::cout << "=== opening a state ===" << std::endl;

	sol::state lua;
	// open some common libraries
	lua.open_libraries(sol::lib::base, sol::lib::package);
	lua.script("print('bark bark bark!')");

	std::cout << std::endl;

    Application app;
    int result = app.Run();

    return result;
}