#ifndef SLIMEBALL_H
#define SLIMEBALL_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>

/// Slimeball object that is initialized from slowing tower
class Slimeball {
public:

    /// @brief Constructor
	///
	/// @param root_filepath Root filepath
	/// @param spawn Coordinates for spawn location
	/// @param target Coordinates to target location
	/// @param damage Damage in integers
	/// @param speed Speed of the missile
	/// @param slowing_parameter Slowing parameter
    Slimeball(std::string root_filepath, sf::Vector2f spawn, sf::Vector2f target, int damage, int speed, float slowing_parameter);

    /// @brief Destructor
    ~Slimeball();

    /// @brief Moves the missile towards target location
	///
	/// @param dt Delta time
    void move(const float& dt);

    /// @brief Sets up all the sprites
    void setUpSprites();

    /// @brief Renders the slimeball on target (window)
	///
	/// @param target Rendering target, i.e. game window
    void render(sf::RenderTarget* target);

    /// @brief Checks if explosion has happened
    bool hasExploded();

    /// @brief Slows all enemies in slimeball radius
    ///
	/// @param enemies List of all the current enemies
    void dealDamage(std::list<Npc*> enemies);

    /// @brief Updates this object
    ///
	/// @param enemies List of all the current enemies
    void update(std::list<Npc*> enemies);

    /// @brief Timer
    sf::Clock clock_;

private:

    /// Name of the missile texture
    std::string missile_texture_name_;

    /// Name of the explosion texture
    std::string explosion_texture_name_;

    /// Missile texture
    sf::Texture missile_texture_;

    /// Explosion texture
    sf::Texture explosion_texture_;

    /// Missile sprite
    sf::Sprite missile_sprite_;

    /// Explosion sprite
    sf::Sprite explosion_sprite_;

    /// Spawn location
    sf::Vector2f spawn_;

    /// Target location
    sf::Vector2f target_;

    /// List of current enemies
    std::list<Npc*> enemies_;

    /// Damage
    int damage_;

    /// Blast radius
    int blast_radius_;

    /// Speed of the missile
    int speed_;

    /// Slowing parameter
    float slowing_parameter_;

    /// Direction for the missile
    sf::Vector2f direction_;

    /// Checks if explosion has happened
    bool explosion_ = false;

    /// Root filepath
    std::string root_filepath_;

};

#endif
