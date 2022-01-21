#ifndef TANK_H
#define TANK_H

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

/// Tank class which spawns 3 Soldiers
class Tank : public Npc
{
public:
	/// @brief Constructor for NPC sub-class Tank
	///
	/// @param root_filepath Root filepath
	/// @param sLocation Spawning location for Tank
    /// @param eLocation Exit location to delete Tank
	/// @param rTiles Vector of tile coordinates for Tank to follow
	/// @param round Counter for round amount
	/// @param newTileCount Used to overwrite tileCounter while spawning Tank from dying SuperTank
	Tank(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int round, int newTileCount = 0);
	
	/// @brief Destructor
	~Tank();

	/// @brief Updates inputs
	///
	/// @param dt Delta time
	virtual void Update(const float& dt);
		
	/// @brief Renders Tank object
	///
	/// @param target Reference to target parameter for Gaming state to handle
	virtual void Render(sf::RenderTarget* target);
		
	/// @brief Handles the logic for Tank to move along road tiles
	///
	/// @param dt Delta time
	virtual void MoveTo(const float& dt);

	/// @brief Initializes Tank setting movespeed, textures, spawnpoint and hitpoints
    virtual void initNpc();
		
	/// @brief Finds the direction Tank has to go
	///
	/// @param nextLocation Next tile coordinate
    std::pair<int,int> FindDirection(sf::Vector2f nextLocation);
		
	/// @brief Rotates Tank object while it changes direction
	///
	/// @param x Gives x direction movement
	/// @param y Gives y direction movement
	virtual void Rotate(int x,int y);
		
	/// @brief Gets current position coordinates of Tank object
	sf::Vector2f getPosition();
		
	/// @brief Gets Tank's current hitpoints
	int getHitpoints();
		
	/// @brief Checks if Tank has reached to exit tile
	bool hasReachedEnd();
		
	/// @brief Calculates the difference of current Tank hitpoints and damage of tower
	///
	/// @param damage Damage parameter of projectile
	void dealDamage(float damage);
		
	/// @brief Slows Tank's movement speed with the product of movement speed and slowing parameter
	///
	/// @param slow Slowing parameter from slimeball projectile
	void slowMovement(float slow);
	
	/// @brief Gets Tank's tile count 
	int getTileCount();

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
			
	/// Counter to keep count on what tile Tank is on
    int tileCount = 0;
			
	/// Sprite for Tank class
	sf::Sprite tankTexture;
			
	/// Texture parameter
    sf::Texture* texture_;

	/// Clock to slow the Tank for certain amount of time
	sf::Clock slowcooldown_;
		
	/// Boolean for Tank to be on exit tile
	bool end_ = false;

	/// Root filepath
	std::string root_filepath_;
private:

};

#endif
