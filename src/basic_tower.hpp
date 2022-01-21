#ifndef BASIC_TOWER_H
#define BASIC_TOWER_H

#include "tower.hpp"
#include "tower.cpp"

/// Basic tower object that shoots
class BasicTower : public Tower {
public:

    /// @brief Constructor
	///
	/// @param root_filepath Root filepath
	/// @param pos Coordinates for tower location
	/// @param attack_speed Attack speed
	/// @param damage Damage in integers
	/// @param radius Radius of the attack reach
	/// @param soundEffectVolumeLevel Sound level (0-100)
    BasicTower(std::string root_filepath, sf::Vector2f pos, float attack_speed, int damage, float radius, int soundEffectVolumeLevel);

    /// @brief Destructor
    virtual ~BasicTower();

    /// @brief Upgrades the tower to next level
    void upgrade();

private:

};















#endif