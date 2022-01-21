#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include "state.hpp"
#include "button.hpp"

#include <iostream>
#include <fstream>
#include <list>
#include <utility>
#include <map>

/// State that shows all of the highscores on the screen
class Highscorestate : public State
{
public:
	/// @brief Constructor
	///
	/// @param window Window object where objects are drawn
	/// @param supportedKeys Keys that are supported
	/// @param root_filepath Root filepath
	/// @param states Stack containing all of the current states
	/// @param font Font used for texts
	Highscorestate(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Font* font);

	/// @brief Destructor
	virtual ~Highscorestate();

	/// @brief Initialize sound effects
	void initSoundEffect();

	/// @brief Updates inputs
	///
	/// @param dt Delta time
	virtual void updateInput(const float& dt);

	/// @brief Called when ending this state
	virtual void endState();

	/// @brief Updates objects
	///
	/// @param dt Delta time
	virtual void update(const float& dt);

	/// @brief Renders objects
	///
	/// @param target Rendering target, i.e. game window
	virtual void render(sf::RenderTarget* target = nullptr);

	/// @brief Updates buttons
	void updateButtons();

	/// @brief Renders all of the buttons
	///
	/// @param target Rendering target, i.e. game window
	void renderButtons(sf::RenderTarget* target = nullptr);

	/// @brief Adds a new highscore to the textfile
	///
	/// @param name Name of the person
	/// @param highscore Score achieved
	static void Addhighscore(const std::string name, int highscore);

	/// @brief Reads the file containing the highscores
	void Readhighscores();

private:
	/// Root filepath
	std::string root_filepath_;

	/// List containing all of the highscores and names
	std::map<std::string, std::list<std::pair<int, std::string>>> highscores_;

	/// Rectangle background
	sf::RectangleShape background_;

	/// Font of the text
	sf::Font* font_;

	/// Vector containing all of the texts to be drawn
	std::vector<sf::Text> Texts_;

	/// Map containing all of the buttons
	std::map<std::string, Button*> highscroeButtons_;

	/// Timer
	sf::Clock pressTimer_;

	/// Timer max
	sf::Int32 pressTimerMax_;

	/// Click sound effect
	sf::SoundBuffer buffer;
    sf::Sound clickSound_;

	/// Window's x length
	float windowX_;

	/// Window's y length
	float windowY_;

	/// Initializes some of the needed variables
	void initVariables();

	/// Initializes keybinds
	void initKeyBinds();

	/// Initializes the highscores
	void initHighscores();

	/// Initializes the buttons
	void initButtons();

	/// Gets the presstimer
	const bool getPressTimer();

	int scrollUpperBoundary_;
	int scrollLowerBoundary_;

	
};

#endif
