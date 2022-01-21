#ifndef CREATEMAPSTATE_H
#define CREATEMAPSTATE_H

#include "state.hpp"
#include "gametile.hpp"
#include "gametile.cpp"
#include "button.hpp"
#include "textbox.hpp"
#include "textbox.cpp"

#include <fstream>
#include <iostream>
#include <vector>
#include <map>

/// State that is used to display a map that can be edited
class Createmap : public State
{
public:

	/// @brief Constructor
	///
	/// @param window Window object where objects are drawn
	/// @param supportedKeys Keys that are supported
	/// @param root_filepath Root filepath
	/// @param states Stack containing all of the current states
	/// @param font Font used for texts
	Createmap(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Font* font, sf::Event* Event);

	/// @brief Destructor
	virtual ~Createmap();

	/// @brief Initialize click sound effect
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

	/// @brief Initializes some of the needed variables
	void initVariables();

	/// @brief Saves the current map to a textfile
	void SaveToFile(std::string mapname);

	/// @brief Initializes default map containing only sand tiles
	void initTiles();

	/// @brief Changes the texture on the given position
	///
	/// @param x X value of Mouse position
	/// @param y y value of Mouse position
	/// @param texturename Name of the texture that will replace the current texture
	/// @param buildable Tells if the next tile is going to be buildable tile or not
	void ReplacePosition(int x, int y, std::string texturename, bool buildable);

	/// @brief Initializes all the given textures
	void initTextures();

	/// @brief Returns the tile at that position
	///
	/// @param x X value of Mouse position
	/// @param y y value of Mouse position
	GameTile* TileAt(int x, int y);

	/// @brief Changes the texture on the given position
	///
	/// @param x X value of Mouse position
	/// @param y y value of Mouse position
	void NextTextureAt(int x, int y);

	/// @brief Changes the texture on the given position
	///
	/// @param x X value of Mouse position
	/// @param y y value of Mouse position
	void NextSpawnOrExit(int x, int y);

	/// @brief Changes the texture on the given position
	///
	/// @param x X value of Mouse position
	/// @param y y value of Mouse position
	/// @param texturename Name of the texture that will replace the current texture
	void ReplaceSpawnOrExit(int x, int y, std::string texturename);

private:

	/// Map containing all of the tiles and their x and y values
	std::map<std::pair<int, int>, GameTile *> Tiles;

	/// Map containing all of the spawn and exit tiles and their x and y values
	std::map<std::pair<int, int>, GameTile *> SpawnAndExit_;

	/// Root filepath
	std::string root_filepath_;

	/// Mouse position according to tiles starting from top left (0,0)
	sf::Vector2u mousePosGrid;

	/// Vector containing names of all the textures and bool values if they are buildable
	std::vector<std::pair<bool, std::string>> textures_;

	/// Vector containing names of all the spawn and exit textures and bool values if they are buildable
	std::vector<std::pair<bool, std::string>> spawnandexittextures_;

	/// Counts time
	sf::Clock clock;

	/// Map containing all of the buttons
	std::map<std::string, Button*> mapcreateButtons_;

	/// Timer
	sf::Clock pressTimer_;

	/// Timer max
	sf::Int32 pressTimerMax_;

	/// Window's x length
	float windowX_;

	/// Window's y length
	float windowY_;

	/// Font of the text
	sf::Font* font_;

	// Click sound effect
	sf::SoundBuffer buffer;
    sf::Sound clickSound_;
	
	/// Initializes keybinds
	void initKeyBinds();

	/// Initializes buttons
	void initButtons();

	/// Gets the presstimer
	const bool getPressTimer();

	/// Event
	sf::Event* Event_;

	/// Textbox
	Textbox* textbox_;

};

#endif