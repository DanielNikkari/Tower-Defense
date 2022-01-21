#include "super_tank.hpp"


//Constructor for tank type entities
SuperTank::SuperTank(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int round) : Npc(root_filepath,sLocation,eLocation,rTiles)
{
    this->root_filepath_ = root_filepath;
    this->spawnLocation = sLocation;
    this->exitLocation = eLocation;
    this->roadTiles = rTiles;
    this->worth_ = 5;
    initNpc();
    this->hitpoints = this->hitpoints * pow(1.05, round);
    this->nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
}

//Destructor for tank type entities
SuperTank::~SuperTank()
{
    delete texture_;
}

void SuperTank::initNpc(){
    texture_ = new sf::Texture;
    if (!texture_->loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/Tanks/towerDefense_BossTank.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_SUPER_TANK_TEXTURE";
    }

    this->superTankTexture.setTexture(*texture_);
    this->superTankTexture.setScale(1.5, 1.5);
	this->movementSpeed = 50.f;
    this->movementspeedmemory_ = this-> movementSpeed;
    this->superTankTexture.setOrigin(32,32);
    this->superTankTexture.setPosition(spawnLocation);
    this->hitpoints = 150;
}

void SuperTank::Render(sf::RenderTarget* target){
    target->draw(this->superTankTexture);
}

sf::Vector2f SuperTank::getPosition() {
    return this->superTankTexture.getPosition();
}

int SuperTank::getHitpoints() {
    return hitpoints;
}

void SuperTank::dealDamage(float damage) {
    this->hitpoints -= damage;
}

void SuperTank::slowMovement(float slow) {
    movementSpeed = movementspeedmemory_ * slow;
    //std::cout << movementSpeed << std::endl;
    slowcooldown_.restart();
}

void SuperTank::MoveTo(const float& dt){
    
    int dir_x, dir_y;

    if (superTankTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
        nextLocation = exitLocation;
        //std::cout<< "Set exit!" << std::endl;
    }

    else if(sf::Vector2i(superTankTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
        tileCount++;
        nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
        //std::cout<< "Moving!" << std::endl;
    }


    
    dir_x = FindDirection(nextLocation).first;
    dir_y = FindDirection(nextLocation).second;
    


    this->superTankTexture.move(dir_x * movementSpeed * dt, dir_y * movementSpeed * dt);

    if(superTankTexture.getPosition().x + 2 >= nextLocation.x && superTankTexture.getPosition().x <= nextLocation.x + 2
    && superTankTexture.getPosition().y + 2 >= nextLocation.y && superTankTexture.getPosition().y <= nextLocation.y + 2) {
        this->superTankTexture.setPosition(nextLocation);
        if (superTankTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
            nextLocation = exitLocation;
            //std::cout<< "Set exit!" << std::endl;
        }

        else if(sf::Vector2i(superTankTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
            tileCount++;
            nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
            //std::cout<< "Moving!" << std::endl;
        }
        //std::cout << "Teleport"<< std::endl;
        dir_x = FindDirection(nextLocation).first;
        dir_y = FindDirection(nextLocation).second;
        //std::cout << "Location :x " << this->superTankTexture.getPosition().x <<" :y "<< this->superTankTexture.getPosition().y << '\n' << "Next location :x "<< nextLocation.x <<" :y "<< nextLocation.y << std::endl;
    }
    
    this->Rotate(dir_x,dir_y);
    if(this->superTankTexture.getPosition() == exitLocation) {
        this->end_ = true;
    }
    
    
}

bool SuperTank::hasReachedEnd() {
    return this->end_;
}

int SuperTank::getTileCount(){
    return tileCount;
}

void SuperTank::Rotate(int x, int y){
    float degree = 0;
    if (x == 1){
        degree = 0;
    }
    else if (x == -1){
        degree = 180;
    } 
    if (y == 1){
        degree = 90;
    }
    else if (y == -1){
        degree = 270;
    } 
    //std::cout<< "Rotation " << degree << std::endl;
    this->superTankTexture.setRotation(degree);
}

std::pair<int,int> SuperTank::FindDirection(sf::Vector2f nextLocation){
    std::pair<int,int> pos; 
    pos.first = 0;
    pos.second = 0;

    if((int)nextLocation.y == (int)superTankTexture.getPosition().y){
        pos.second = 0;
    }
    else if((int)nextLocation.y > (int)superTankTexture.getPosition().y){
        pos.second = 1;
    }
    else if((int)nextLocation.y < (int)superTankTexture.getPosition().y){
        pos.second = -1;
    }
    if((int)nextLocation.x == (int)superTankTexture.getPosition().x){
        pos.first = 0;
    }
    else if((int)nextLocation.x < (int)superTankTexture.getPosition().x){
        pos.first= -1;
    }
    else if((int)nextLocation.x > (int)superTankTexture.getPosition().x){
        pos.first = 1;
    }
    //std::cout<< "Direction x: " << pos.first << " y: " << pos.second << std::endl; 
    return pos;
}


void SuperTank::Update(const float& dt) {
        this->MoveTo(dt);

        sf::Time elapsed = slowcooldown_.getElapsedTime();
        if(4 < elapsed.asSeconds()) {
            this->movementSpeed = movementspeedmemory_;
        }
}
