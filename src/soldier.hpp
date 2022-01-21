#ifndef SOLDIER_H
#define SOLDIER_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <stack>
#include <map>
#include <vector>
#include "gametile.hpp"
#include "npc.hpp"

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

/// Soldier class to be used as one hit enemy
class Soldier : public Npc
{
public:
	/// @brief Constructor for NPC sub-class Soldier
	///
	/// @param root_filepath Root filepath
	/// @param sLocation Spawning location for Soldier
    /// @param eLocation Exit location to delete Soldier
	/// @param rTiles Vector of tile coordinates for Soldier to follow
	/// @param newTileCount Used to overwrite tileCounter while spawning Soldier from dying Tank
	Soldier(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int newTileCount = 0);
		
	/// @brief Destructor
	~Soldier();

	/// @brief Updates inputs
	///
	/// @param dt Delta time
	virtual void Update(const float& dt);
	
	/// @brief Renders Soldier object
	///
	/// @param target Reference to target parameter for Gaming state to handle
	virtual void Render(sf::RenderTarget* target);
	
	/// @brief Handles the logic for Soldier to move along road tiles
	///
	/// @param dt Delta time
	virtual void MoveTo(const float& dt);

	/// @brief Initializes Soldier setting movespeed, textures, spawnpoint and hitpoints
    virtual void initNpc();
	
	/// @brief Finds the direction Soldier has to go
	///
	/// @param nextLocation Next tile coordinate
    std::pair<int,int> FindDirection(sf::Vector2f nextLocation);
	
	/// @brief Rotates Soldier object while it changes direction
	///
	/// @param x Gives x direction movement
	/// @param y Gives y direction movement
	virtual void Rotate(int x,int y);
	
	/// @brief Gets current position coordinates of Soldier object
	sf::Vector2f getPosition();
	
	/// @brief Gets Soldier's current hitpoints
	int getHitpoints();
	
	/// @brief Checks if Soldier has reached to exit tile
	bool hasReachedEnd();
	
	/// @brief Calculates the difference of current Soldier hitpoints and damage of tower
	///
	/// @param damage Damage parameter of projectile
	void dealDamage(float damage);
	
	/// @brief Slows Soldier's movement speed with the product of movement speed and slowing parameter
	///
	/// @param slow Slowing parameter from slimeball projectile
	void slowMovement(float slow);

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
		
	/// Counter to keep count on what tile Soldier is on
    int tileCount = 0;
		
	/// Sprite for Soldier class
	sf::Sprite soldierTexture;
		
	/// Texture parameter
    sf::Texture* texture_;

	/// Clock to slow the Soldier for certain amount of time
	sf::Clock slowcooldown_;
	
	/// Boolean for Soldier to be on exit tile
	bool end_ = false;

	/// Root filepath
	std::string root_filepath_;
private:

};

#endif
