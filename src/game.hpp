// Game header

#ifndef GAME_H
#define GAME_H

// Include needed headers
#include "main_menu_state.hpp"
#include "main_menu_state.cpp"

/// Base class for the game
class Game
{
public:
	/// @brief Constructor
	///
	/// @param Root filepath
	Game(std::string& root_filepath);

	/// @brief Destructor
	virtual ~Game();

	/// @brief Function dealing with ending application
	void endApplication();

	/// @brief Measures the refresh time of the game
	void updateDt();

	/// @brief Updates SFML pollevents
	void updateSFMLEvents();

	/// @brief Updates functions
	void update();

	/// @brief Renders objects on screen
	void render();

	/// @brief Core function for running the game
	void run();

private:
	// Initializing

	/// @brief Initializes variables
	void initVariables();

	/// @brief Initializes window
	void initWindow();

	/// @brief Initializes states stack
	void initStates();

	/// @brief Initializes keys
	void initKeys();

	// Variables

	/// Root filepath
	std::string root_filepath_;

	/// Window pointer object
	sf::RenderWindow *window;

	/// SFML event object
	sf::Event sfEvent;

	/// Vector of available video modes
	std::vector<sf::VideoMode> videoModes_;

	/// Window settings object
	sf::ContextSettings windowSettings_;

	/// Boolean for the fullscreen mode
	bool fullscreen_;

	/// dt variable to measure refresh time
	float dt;

	/// Clock object to measure elapsed time
	sf::Clock dtClock;

	/// Clock for timing the call for quit
	sf::Clock quitclock_;

	/// States stack
	std::stack<State*> states; 

	/// Map of supported keys
	std::map<std::string, int> supportedKeys; 

	/// Game icon
	sf::Image icon_;
};

#endif