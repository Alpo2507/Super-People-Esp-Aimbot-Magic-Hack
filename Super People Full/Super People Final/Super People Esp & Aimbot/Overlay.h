#pragma once
class Overlay
{
public:
	Overlay();
	~Overlay();

	sf::RenderWindow window;
	int width{ 800 };
	int height{ 600 };
	int left{ 10 };
	int top{ 30 };
	sf::ContextSettings settings;
	MARGINS margin{ -1 };

	void createWindow();
};

extern Overlay* g_pOverlay;