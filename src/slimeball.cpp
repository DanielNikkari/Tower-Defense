#include "slimeball.hpp"


Slimeball::Slimeball(std::string root_filepath, sf::Vector2f spawn, sf::Vector2f target, int damage, int speed, float slowing_parameter) {
    spawn_ = spawn;
    target_ = target;
    damage_ = damage;
    slowing_parameter_ = slowing_parameter;
    blast_radius_ = 50;
    speed_ = speed;
    root_filepath_ = root_filepath;
    missile_texture_name_ = "slimeball.png";
    explosion_texture_name_ = "slimesplash.png";
    this->setUpSprites();
    direction_ = target_ - spawn_;

    // normalizing the vector so that speed doesn't change according to distance
    direction_ = sf::Vector2f(direction_.x * (1 / sqrt(pow(direction_.x, 2) + pow(direction_.y, 2))), direction_.y * (1 / sqrt(pow(direction_.x, 2) + pow(direction_.y, 2))));
}

Slimeball::~Slimeball() {

}

void Slimeball::setUpSprites() {
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
    this->explosion_sprite_.setScale(1.5, 1.5);
    this->missile_sprite_.setScale(0.25, 0.25);

    this->missile_sprite_.setOrigin(32, 32);
    this->explosion_sprite_.setColor(sf::Color::Transparent);

    this->explosion_sprite_.setPosition(target_);
    this->missile_sprite_.setPosition(spawn_);
}

void Slimeball::move(const float& dt) {
    if(!this->hasExploded()) {
        double angle = atan2(target_.y - spawn_.y, target_.x - spawn_.x) * 180 / M_PI;
        this->missile_sprite_.move(speed_ * dt * direction_);
        this->missile_sprite_.setRotation(angle + 90);

        if(this->missile_sprite_.getPosition().x + 2 >= target_.x && this->missile_sprite_.getPosition().x <= target_.x + 2
        && this->missile_sprite_.getPosition().y + 2 >= target_.y && this->missile_sprite_.getPosition().y <= target_.y + 2)
        {
            this->missile_sprite_.setColor(sf::Color::Transparent);
            this->explosion_sprite_.setColor(sf::Color(0, 200, 0, 50));
            this->explosion_ = true;
            dealDamage(enemies_);
            this->clock_.restart();
        }
        
    }
    
}

void Slimeball::render(sf::RenderTarget* target) {
    target->draw(this->explosion_sprite_);
    target->draw(this->missile_sprite_);
}

bool Slimeball::hasExploded() {
    return explosion_;
}

void Slimeball::dealDamage(std::list<Npc*> enemies) {
    for(auto i : enemies) {
        if(abs(sqrt(pow(this->target_.x - i->getPosition().x, 2) + pow(this->target_.y  - i->getPosition().y, 2))) <= blast_radius_) {
            i->dealDamage(damage_);
            i->slowMovement(slowing_parameter_);
        }
    }
}

void Slimeball::update(std::list<Npc*> enemies) {
    enemies_ = enemies;
}