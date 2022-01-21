/// State header
#ifndef STATE_H
#define STATE_H

/// Include headers
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stack>
#include <map>
#include <vector>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


class State
{
public:
	/// Constructor and destructor
	State(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states);
	virtual ~State();

	/// Functions
	virtual void checkForQuit();
	const bool& getQuit() const;
	virtual void endState() = 0;
	virtual void updateInput(const float& dt) = 0;
	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget* target = nullptr) = 0;
	virtual void updateMousePosition();
	
	bool quit_;

	

protected:
	/// Container for textures
	std::vector<sf::Texture> textures;
	/// Pointer to a window
	sf::RenderWindow* window_;
	/// Pointer to supported keys
	std::map<std::string, int>* supportedKeys_;

	/// Pointer to a stack filled with state pointers (check game.hpp private)
	std::stack<State*>* states;

	std::map<std::string, int> keybinds_;
	std::string root_filepath_;

	/// Mouse position variavles
	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	/// Functions
	virtual void initKeyBinds() = 0;
	



private:
};

#endif