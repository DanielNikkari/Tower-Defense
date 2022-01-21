#include "basic_tower.hpp"

BasicTower::BasicTower(std::string root_filepath, sf::Vector2f pos, float attack_speed, int damage, float radius, int soundEffectVolumeLevel) : Tower(root_filepath, pos, attack_speed, damage, radius, soundEffectVolumeLevel) {
    this->texture_platform_ = "towerDefense_tile181.png";
    this->texture_gun_ = "towerDefense_tile250.png";
    this->texture_gunfire_ = "towerDefense_tile295.png";
    this->setUpSprites();
    this->gunfire_color_ = gunfire_.getColor();
    this->gunfire_.setColor(sf::Color::Transparent);
}

BasicTower::~BasicTower() {
    
}

void BasicTower::upgrade() {
    this->tower_level_ += 1;
    this->damage_ = this->damage_ * 1.3;
    this->attack_speed_ = this->attack_speed_ * 0.99;

}