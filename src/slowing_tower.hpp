#ifndef SLOWING_TOWER_H
#define SLOWING_TOWER_H

#include "tower.hpp"
#include "slimeball.hpp"
#include "slimeball.cpp"

/// Slowing tower object that shoots slimeballs
class SlowingTower : public Tower {
public:

    /// @brief Constructor
	///
	/// @param root_filepath Root filepath
	/// @param pos Coordinates for tower location
	/// @param attack_speed Attack speed
	/// @param damage Damage in integers
	/// @param radius Radius of the attack reach
	/// @param soundEffectVolumeLevel Sound level (0-100)
    SlowingTower(std::string root_filepath, sf::Vector2f pos, float attack_speed, int damage, float radius, int soundEffectVolumeLevel);

    /// @brief Destructor
    virtual ~SlowingTower();

    /// @brief Tower attacks a target
	///
	/// @param dt Delta time
    void attack(const float& dt);

    /// @brief Updates the tower
	///
	/// @param dt Delta time
    /// @param enemies List of all the enemies
    /// @param mousePos Current mouse position
    void update(const float& dt, std::list<Npc*> enemies, const sf::Vector2f mousePos);

    /// @brief Renders the tower on target (window)
	///
	/// @param target Rendering target, i.e. game window
    void render(sf::RenderTarget* target);

    /// @brief Upgrades the tower to next level
    void upgrade();

    /// @brief Initializes shooting sound effect
    void initSoundEffect();

    /// @brief Deletes the missile after exploding
    void deleteMissile();

private:

    /// List of all slimeballs 
    std::list<Slimeball*> missiles_;

    /// Shoot sound effect buffer
    sf::SoundBuffer buffer;

    /// Shoot sound effect
    sf::Sound sound;

    /// Volume level
    int soundEffectVolumeLevel_;

};















#endif