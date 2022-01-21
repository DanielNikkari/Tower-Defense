#ifndef NPC_H
#define NPC_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stack>
#include <map>
#include <vector>
#include "gametile.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/// Npc class as a base class fo enemies
class Npc
{
public:
	/// @brief Constructor for NPC class
	///
	/// @param root_filepath Root filepath
	/// @param sLocation Spawning location for NPC
    /// @param eLocation Exit location to delete NPC
	/// @param rTiles Vector of tile coordinates for NPC to follow
	/// @param newTileCount Used to overwrite tileCounter while spawning NPCs from dying NPC
	Npc(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int newTileCount = 0);

	/// @brief Destructor
	~Npc();

	/// @brief Updates inputs
	///
	/// @param dt Delta time
	virtual void Update(const float& dt);

	/// @brief Renders NPC object
	///
	/// @param target Reference to target parameter for Gaming state to handle
	virtual void Render(sf::RenderTarget* target);

	/// @brief Handles the logic for NPCs to move along road tiles
	///
	/// @param dt Delta time
	virtual void MoveTo(const float& dt);

	/// @brief Initializes NPCs setting movespeed, textures, spawnpoint and hitpoints
    virtual void initNpc();

	/// @brief Finds the direction NPC has to go
	///
	/// @param nextLocation Next tile coordinate
    virtual std::pair<int,int> FindDirection(sf::Vector2f nextLocation);

	/// @brief Rotates NPC object while it changes direction
	///
	/// @param x Gives x direction movement
	/// @param y Gives y direction movement
	virtual void Rotate(int x,int y);

	/// @brief Gets current position coordinates of NPC object
	virtual sf::Vector2f getPosition();

	/// @brief Gets NPCs current hitpoints
	virtual int getHitpoints();

	/// @brief Checks if NPC has reached to exit tile
	virtual bool hasReachedEnd();

	/// @brief Calculates the difference of current NPC hitpoints and damage of tower
	///
	/// @param damage Damage parameter of projectile
	virtual void dealDamage(float damage);

	/// @brief Slows NPCs movement speed with the product of movement speed and slowing parameter
	///
	/// @param slow Slowing parameter from slimeball projectile
	virtual void slowMovement(float slow);

	/// @brief Gets NPCs tile count 
	virtual int getTileCount();

	/// @brief Gets NPCs worth to give player money
	virtual int getWorth();

protected:
	/// Movement speed
	float movementSpeed;

	/// Temporary movement speed to use in slowing
	float movementspeedmemory_;

	/// Hitpoints
    float hitpoints = 1;

	/// Spawn location coordinates
    sf::Vector2f spawnLocation;

	/// Exit location coordinates
    sf::Vector2f exitLocation;

	/// Next location coordinates
    sf::Vector2f nextLocation;

	/// Vector to save road tile coordinates
    std::vector<GameTile*>* roadTiles;

	/// Counter to keep count on what tile NPC is on
    int tileCount = 0;

	/// Sprite for NPC class (debugging class)
	sf::Sprite sotilastektuuri;

	/// Texture parameter
    sf::Texture* stexture_;

	/// Amount of money NPC is worth
	int worth_ = 1;

	/// Clock to slow the NPC for certain amount of time
	sf::Clock slowcooldown_;

	/// Boolean for NPC to be on exit tile
	bool end_ = false;

	/// Root filepath
	std::string root_filepath_;
private:

};

#endif
