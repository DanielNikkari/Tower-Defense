#include "missile.hpp"


Missile::Missile(std::string root_filepath, sf::Vector2f spawn, sf::Vector2f target, int damage, int speed, int soundEffectVolumeLevel) {
    spawn_ = spawn;
    target_ = target;
    damage_ = damage;
    blast_radius_ = 50;
    speed_ = speed;
    soundEffectVolumeLevel_ = soundEffectVolumeLevel;
    root_filepath_ = root_filepath;
    missile_texture_name_ = "towerDefense_tile252.png";
    explosion_texture_name_ = "towerDefense_tile021.png";
    this->setUpSprites();
    this->initSoundEffect();
    direction_ = target_ - spawn_;

    // normalizing the vector so that speed doesn't change according to distance
    direction_ = sf::Vector2f(direction_.x * (1 / sqrt(pow(direction_.x, 2) + pow(direction_.y, 2))), direction_.y * (1 / sqrt(pow(direction_.x, 2) + pow(direction_.y, 2))));
}

Missile::~Missile() {
    delete buffer;
}

void Missile::initSoundEffect()
{
    buffer = new sf::SoundBuffer;
    if (!this->buffer->loadFromFile(root_filepath_ + "/src/Audio/SoundEffects/explosion.wav"))
    {
        throw "ERROR::TOWER::UNABLE_TO_LOAD_SOUND_EFFECT_FILE";
    }

    this->sound.setBuffer(*buffer);
    this->sound.setVolume(soundEffectVolumeLevel_);
}

void Missile::setUpSprites() {
    if(!this->missile_texture_.loadFromFile(root_filepath_ + "/src/Textures/ProjectileTextures/Missiles/" + missile_texture_name_))
	{
		throw "ERROR::GAMETILE::FAILED_TO_LOAD_missile_TEXTURE";
	}

    if(!this->explosion_texture_.loadFromFile(root_filepath_ + "/src/Textures/ProjectileTextures/" + explosion_texture_name_))
	{
		throw "ERROR::GAMETILE::FAILED_TO_LOAD_explosion_TEXTURE";
	}
    
    this->missile_sprite_.setTexture(this->missile_texture_);
    this->explosion_sprite_.setTexture(this->explosion_texture_);

    this->explosion_sprite_.setOrigin(32, 32);
    this->explosion_sprite_.setScale(3, 3);
    this->missile_sprite_.setOrigin(32, 32);
    this->explosion_sprite_.setColor(sf::Color::Transparent);

    this->explosion_sprite_.setPosition(target_);
    this->missile_sprite_.setPosition(spawn_);
}

void Missile::move(const float& dt) {
    if(!this->hasExploded()) {
        double angle = atan2(target_.y - spawn_.y, target_.x - spawn_.x) * 180 / M_PI;
        this->missile_sprite_.move(speed_ * dt * direction_);
        this->missile_sprite_.setRotation(angle + 90);

        if(this->missile_sprite_.getPosition().x + 2 >= target_.x && this->missile_sprite_.getPosition().x <= target_.x + 2
        && this->missile_sprite_.getPosition().y + 2 >= target_.y && this->missile_sprite_.getPosition().y <= target_.y + 2)
        {
            this->missile_sprite_.setColor(sf::Color::Transparent);
            this->explosion_sprite_.setColor(sf::Color::Red);
            this->explosion_ = true;
            dealDamage(enemies_);
            this->clock_.restart();
        }
        
    }
    
}

void Missile::render(sf::RenderTarget* target) {
    target->draw(this->explosion_sprite_);
    target->draw(this->missile_sprite_);
}

bool Missile::hasExploded() {
    return explosion_;
}

void Missile::dealDamage(std::list<Npc*> enemies) {
    this->sound.play();
    for(auto i : enemies) {
        if(abs(sqrt(pow(this->target_.x - i->getPosition().x, 2) + pow(this->target_.y  - i->getPosition().y, 2))) <= blast_radius_) {
            i->dealDamage(damage_);
        }
    }
}

void Missile::update(std::list<Npc*> enemies) {
    enemies_ = enemies;
}