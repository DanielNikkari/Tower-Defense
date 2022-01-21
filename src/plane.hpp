#ifndef PLANE_H
#define PLANE_H

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

/// Plane class to be used as fast moving enemy
class Plane : public Npc
{
public:
	/// @brief Constructor for NPC sub-class Plane
	///
	/// @param root_filepath Root filepath
	/// @param sLocation Spawning location for Plane
    /// @param eLocation Exit location to delete Plane
	/// @param rTiles Vector of tile coordinates for Plane to follow
	/// @param round Counter for round amount
	/// @param newTileCount Used to overwrite tileCounter while spawning Plane from dying SuperPlane
	Plane(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int round, int newTileCount = 0);
	
	/// @brief Destructor
	~Plane();

	/// @brief Updates inputs
	///
	/// @param dt Delta time
	virtual void Update(const float& dt);

	/// @brief Renders Plane object
	///
	/// @param target Reference to target parameter for Gaming state to handle
	virtual void Render(sf::RenderTarget* target);

	/// @brief Handles the logic for Plane to move along road tiles
	///
	/// @param dt Delta time
	virtual void MoveTo(const float& dt);

	/// @brief Initializes Plane setting movespeed, textures, spawnpoint and hitpoints
    virtual void initNpc();

	/// @brief Finds the direction Plane has to go
	///
	/// @param nextLocation Next tile coordinate
    std::pair<int,int> FindDirection(sf::Vector2f nextLocation);

	/// @brief Rotates Plane object while it changes direction
	///
	/// @param x Gives x direction movement
	/// @param y Gives y direction movement
	virtual void Rotate(int x,int y);

	/// @brief Gets current position coordinates of Plane object
	sf::Vector2f getPosition();

	/// @brief Gets Plane's current hitpoints
	int getHitpoints();

	/// @brief Checks if Plane has reached to exit tile
	bool hasReachedEnd();

	/// @brief Calculates the difference of current Plane hitpoints and damage of tower
	///
	/// @param damage Damage parameter of projectile
	void dealDamage(float damage);

	/// @brief Slows Plane's movement speed with the product of movement speed and slowing parameter
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
	
	/// Counter to keep count on what tile Plane is on
    int tileCount = 0;
	
	/// Sprite for Plane class
	sf::Sprite planeTexture;
	
	/// Texture parameter
    sf::Texture* texture_;

	/// Clock to slow the Plane for certain amount of time
	sf::Clock slowcooldown_;

	/// Boolean for Plane to be on exit tile
	bool end_ = false;

	/// Root filepath
	std::string root_filepath_;
private:

};

#endif
