#include "tower.hpp"


Tower::Tower(std::string root_filepath, sf::Vector2f pos, float attack_speed, float damage, float radius, int soundEffectVolumeLevel, float slowing_parameter) {
    root_filepath_ = root_filepath;
    pos_ = pos;
    attack_speed_ = attack_speed;
    damage_ = damage;
    radius_ = radius;
    slowing_parameter_ = slowing_parameter;
    this->soundEffectVolumeLevel_ = soundEffectVolumeLevel;
    this->radius_shape_.setRadius(radius);
    this->radius_shape_.setOrigin(radius, radius);
    this->radius_shape_.setPosition(pos_);
    this->radius_shape_.setFillColor(sf::Color::Transparent);
    this->initSoundEffect();
}

Tower::~Tower() {

}

void Tower::setUpSprites() {
    if(!this->sftexture_gun_.loadFromFile(root_filepath_ + "/src/Textures/NPCTextures/Towers/" + texture_gun_))
	{
		throw "ERROR::GAMETILE::FAILED_TO_LOAD_GUN_TEXTURE";
	}

    if(!this->sftexture_platform_.loadFromFile(root_filepath_ + "/src/Textures/NPCTextures/Towers/" + texture_platform_))
	{
		throw "ERROR::GAMETILE::FAILED_TO_LOAD_platform_TEXTURE";
	}

    if(!this->sftexture_gunfire_.loadFromFile(root_filepath_ + "/src/Textures/ProjectileTextures/Bullet/" + texture_gunfire_))
	{
		throw "ERROR::GAMETILE::FAILED_TO_LOAD_platform_TEXTURE";
	}

    this->platform_.setTexture(this->sftexture_platform_);
    this->gun_.setTexture(this->sftexture_gun_);
    this->gunfire_.setTexture(this->sftexture_gunfire_);

    this->gun_.setOrigin(32, 32);
    this->platform_.setOrigin(32, 32);
    this->gunfire_.setOrigin(32, 32);

    this->gun_.setPosition(pos_);
    this->platform_.setPosition(pos_);
    this->gunfire_.setPosition(pos_.x, pos_.y - 40);

}

void Tower::render(sf::RenderTarget* target) {

    target->draw(this->platform_);
    target->draw(this->gun_);
    if(target_ != nullptr) {
        sf::Transform trans;
        trans.rotate(angle_ + 90, pos_);
        target->draw(this->gunfire_, trans);
    }
    target->draw(this->radius_shape_);
}

void Tower::initSoundEffect()
{
    if (!this->buffer.loadFromFile(root_filepath_ + "/src/Audio/SoundEffects/Basic_Tower_Shoot.wav"))
    {
        throw "ERROR::TOWER::UNABLE_TO_LOAD_SOUND_EFFECT_FILE";
    }

    this->sound.setBuffer(this->buffer);
    
    this->sound.setVolume(soundEffectVolumeLevel_);
}

void Tower::rotateGun(const float& dt, std::list<Npc*> enemies) {
    Npc* enemytobefaced = nullptr;
    for (auto i : enemies) {
        if(abs(sqrt(pow(pos_.x - i->getPosition().x, 2) + pow(pos_.y - i->getPosition().y, 2))) <= radius_) {
            enemytobefaced = i;
            break;
        }
    }
    if(enemytobefaced != nullptr) {
        target_ = enemytobefaced;
        angle_ = atan2(enemytobefaced->getPosition().y - pos_.y, enemytobefaced->getPosition().x - pos_.x) * 180 / M_PI;
        gun_.setRotation(angle_ + 90);
    }
    else {
        target_ = nullptr;
        //gun_.setRotation(0);
    }
}

void Tower::update(const float& dt, std::list<Npc*> enemies, const sf::Vector2f mousePos) {
    this->rotateGun(dt, enemies);
    this->attack(dt);

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
}

void Tower::attack(const float& dt) {
    if(target_ != nullptr) {
        sf::Time elapsed = clock_.getElapsedTime();
        if(elapsed.asSeconds() > attack_speed_) {
            this->sound.play();
            this->gunfire_.setColor(gunfire_color_);
            target_->dealDamage(damage_);
            target_->slowMovement(slowing_parameter_);
            clock_.restart();
            gunfire_clock_.restart();
        }
    }
    sf::Time gunfire_elapsed = gunfire_clock_.getElapsedTime();
    if(gunfire_elapsed.asSeconds() > 0.1) {
        this->gunfire_.setColor(sf::Color::Transparent);
    }
}

float Tower::getDamage() {
    return damage_;
}

float Tower::getAttackSpeed() {
    return attack_speed_;
}

int Tower::getTowerLevel() {
    return tower_level_;
}

void Tower::upgrade() {
    tower_level_ += 1;
    damage_ = damage_ * 2;
}

sf::Vector2f Tower::getPosition() {
    return pos_;
}
