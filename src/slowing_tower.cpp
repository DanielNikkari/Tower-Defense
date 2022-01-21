#include "slowing_tower.hpp"

SlowingTower::SlowingTower(std::string root_filepath, sf::Vector2f pos, float attack_speed, int damage, float radius, int soundEffectVolumeLevel) : Tower(root_filepath, pos, attack_speed, damage, radius, soundEffectVolumeLevel, 0.4) {
    this->texture_platform_ = "towerDefense_tile183.png";
    this->texture_gun_ = "towerDefense_tile249.png";
    this->texture_gunfire_ = "towerDefense_tile295.png";
    this->soundEffectVolumeLevel_ = soundEffectVolumeLevel;
    this->setUpSprites();
    this->gunfire_.setColor(sf::Color::Transparent);
    this->gunfire_color_ = sf::Color::Green;
    this->initSoundEffect();
}

SlowingTower::~SlowingTower() {
    for(auto i : missiles_) {
        delete i;
    }
}

void SlowingTower::initSoundEffect()
{
    if (!this->buffer.loadFromFile(root_filepath_ + "/src/Audio/SoundEffects/sticky_tower_shoot.wav"))
    {
        throw "ERROR::TOWER::UNABLE_TO_LOAD_SOUND_EFFECT_FILE";
    }

    this->sound.setBuffer(this->buffer);
    this->sound.setVolume(this->soundEffectVolumeLevel_);
}


void SlowingTower::attack(const float& dt) {
    if(target_ != nullptr) {
        sf::Time elapsed = clock_.getElapsedTime();
        if(elapsed.asSeconds() > attack_speed_) {
            this->sound.play();
            missiles_.push_back(new Slimeball(root_filepath_, pos_ , target_->getPosition(), damage_, 130, slowing_parameter_));
            clock_.restart();
        }
    }
}

/*void SlowingTower::deleteMissile()
{
    
    for (auto i : missiles_) {
        if (i->hasExploded()) {
            std::cout << "Slime destroyed" << "\n";
            missiles_.remove(i);
            //delete i;
        }
    }
}*/

void SlowingTower::update(const float& dt, std::list<Npc*> enemies, const sf::Vector2f mousePos) {
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

    std::vector<Slimeball*> tobedeleted;
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

void SlowingTower::render(sf::RenderTarget* target) {
    target->draw(this->platform_);
    target->draw(this->gun_);
    target->draw(this->radius_shape_);

    for (auto i : missiles_) {
        i->render(target);
    }
}

void SlowingTower::upgrade() {
    this->tower_level_ += 1;
    this->attack_speed_ = this->attack_speed_ * 0.9;
}
