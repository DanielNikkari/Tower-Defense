#include "bomb_tower.hpp"

BombTower::BombTower(std::string root_filepath, sf::Vector2f pos, float attack_speed, int damage, float radius, int soundEffectVolumeLevel) : Tower(root_filepath, pos, attack_speed, damage, radius, soundEffectVolumeLevel) {
    this->texture_platform_ = "towerDefense_tile182.png";
    this->texture_gun_ = "towerDefense_tile206.png";
    this->texture_gunfire_ = "towerDefense_tile295.png";
    this->soundEffectVolumeLevel_ = soundEffectVolumeLevel;
    this->setUpSprites();
}

BombTower::~BombTower() {
    std::cout << "Bombtower destructor called" << "\n";
    for(auto i : missiles_) {
        delete i;
    }
}

void BombTower::attack(const float& dt) {
    if(target_ != nullptr) {
        sf::Time elapsed = clock_.getElapsedTime();
        if(elapsed.asSeconds() > attack_speed_) {
            missiles_.push_back(new Missile(root_filepath_, pos_ , target_->getPosition(), damage_, 130, this->soundEffectVolumeLevel_));
            clock_.restart();
        }
    }
}

/*void BombTower::deleteMissile()
{
    if (missiles_.size() > 0) {
        for (auto i : missiles_) {
            std::cout << "missiles_ vector size:" << missiles_.size() << "\n";
            if (i->hasExploded()) {
                std::cout << "Missile destroyed" << "\n";
                //missiles_.remove(i);
                delete i;
                std::cout << "missiles_ vector size after delete:" << missiles_.size() << "\n";
                }
            }
        }
    
}*/

void BombTower::update(const float& dt, std::list<Npc*> enemies, const sf::Vector2f mousePos) {
    this->attack(dt);
    this->rotateGun(dt, enemies);

    this->towerState_ = TOWER_IDLE;

	// Hover
	if (this->platform_.getGlobalBounds().contains(mousePos))
	{
		this->towerState_ = TOWER_HOVER;

	}

	// Change button color according to the button state
	switch (this->towerState_)
	{
	case TOWER_IDLE:
        this->radius_shape_.setFillColor(sf::Color::Transparent);
		break;
	case TOWER_HOVER:
        this->radius_shape_.setFillColor(sf::Color(30, 30, 30, 30));
		break;
    }

    std::vector<Missile*> tobedeleted;
    for(auto i : missiles_) {
        i->move(dt);
        i->update(enemies);
        sf::Time elapsed = i->clock_.getElapsedTime();
        if(i->hasExploded() && elapsed.asSeconds() > 1) {
            tobedeleted.push_back(i);
        }
    }
     for (auto i : tobedeleted) {
        missiles_.remove(i);
    }
    for (auto i : tobedeleted) {
        // std::cout << "Missile destroyed" << "\n";
        delete(i);
    }
    tobedeleted.clear();
}

void BombTower::render(sf::RenderTarget* target) {
    target->draw(this->platform_);
    target->draw(this->gun_);
    target->draw(this->radius_shape_);

    for (auto i : missiles_) {
        i->render(target);
    }
}

void BombTower::upgrade() {
    this->tower_level_ += 1;
    this->damage_ = this->damage_ * 1.2;
}
