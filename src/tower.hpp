#ifndef TOWER_H
#define TOWER_H

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <list>
#include <cmath>

enum tower_states{TOWER_IDLE = 0, TOWER_HOVER};

/// Tower class for all kinds of towers
class Tower {
public:
    /// @brief Constructor
	///
	/// @param root_filepath Root filepath
	/// @param pos Position for the tower placement
	/// @param attack_speed Attack speed (seconds)
	/// @param damage Damage that tower deals per attack to a target
	/// @param radius Attack radius
	/// @param slowing_parameter Possible parameter if you want this tower to slow the targeted enemy
    Tower(std::string root_filepath, sf::Vector2f pos, float attack_speed, float damage, float radius, int soundEffectVolumeLevel, float slowing_parameter = 1);

    /// @brief Destructor
    virtual ~Tower();

    /// @brief Render tower on target (window)
	///
	/// @param target Rendering target, i.e. game window
    virtual void render(sf::RenderTarget* target);

    /// @brief Update tower with new information
	///
    /// @param dt Delta time
    /// @param enemies List of every enemy on the board currently
	/// @param mousePos Mouse position on screen
    virtual void update(const float& dt, std::list<Npc*> enemies, const sf::Vector2f mousePos);

    /// @brief Initializes shooting sound effect
    void initSoundEffect();

    /// @brief Loads required textures and sets up sprites
    void setUpSprites();

    /// @brief Rotates the gun part of the tower towards first enemy and targets that enemy for the next attack
	///
    /// @param dt Delta time
    /// @param enemies List of every enemy on the board currently
    void rotateGun(const float& dt, std::list<Npc*> enemies);

    /// @brief Attacks a target if the tower has one. Dealing damage and possible slowing the enemy
	///
    /// @param dt Delta time
    virtual void attack(const float& dt);

    /// @brief Upgardes tower to the next level
    virtual void upgrade();

    /// @brief Returns the damage of the tower
    float getDamage();

    /// @brief Returns the attack speed of the tower
    float getAttackSpeed();

    /// @brief Returns the level of the tower
    int getTowerLevel();

    /// @brief Returns the position of the tower
    sf::Vector2f getPosition();

protected:

    /// Sprite for the platform part of the tower
    sf::Sprite platform_;

    /// Sprite for the gun part of the tower
    sf::Sprite gun_;

    /// Texture for the platform part of the tower
    sf::Texture sftexture_platform_;

    /// Texture for the gun part of the tower
    sf::Texture sftexture_gun_;

    /// Texture for the gunfire part of the tower
    sf::Texture sftexture_gunfire_;

    /// Name of the gunfire texture
    std::string texture_gunfire_;

    /// Sprite for the gunfire part of the tower
    sf::Sprite gunfire_;

    /// Target for the next attack
    Npc* target_;

    /// Measures time
    sf::Clock clock_;

    /// Position of this tower
    sf::Vector2f pos_;

    /// Root filepath
    std::string root_filepath_;

    /// Name of the platform texture
    std::string texture_platform_;

    /// Name of the gun texture
    std::string texture_gun_;

    /// CircleShape to show the radius of the tower
    sf::CircleShape radius_shape_;

    /// Attack speed (seconds)
    float attack_speed_;

    /// Damage 
    float damage_;

    /// Tower level
    int tower_level_ = 1;

    /// Attack radius
    float radius_;

    /// Slowing parameter
    float slowing_parameter_;

    /// Checks if the mouse is hovering the tower
    short unsigned towerState_;

    /// Transform
    sf::Transform transform_;

    /// Angle towards target
    double angle_;

    /// Timer for gunfire
    sf::Clock gunfire_clock_;

    /// Color for gunfire
    sf::Color gunfire_color_;

    /// Shoot sound effect buffer
    sf::SoundBuffer buffer;

    /// Shoot sound effect
    sf::Sound sound;

    /// Volume level
    int soundEffectVolumeLevel_;
    

};


#endif