#include "app\app.hpp"

int main()
{
	const int width = 1000, height = 700, ui_width = 300;
	App app(width, height, ui_width);
	app.loop();

	return 0;
}
