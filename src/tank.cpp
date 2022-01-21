#include "tank.hpp"


//Constructor for tank type entities
Tank::Tank(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int round, int newTileCount) : Npc(root_filepath,sLocation,eLocation,rTiles, newTileCount)
{
    this->root_filepath_ = root_filepath;
    this->spawnLocation = sLocation;
    this->exitLocation = eLocation;
    this->roadTiles = rTiles;
    this->worth_ = 2;
    initNpc();
    this->hitpoints = this->hitpoints * pow(1.05, round);
    this->tileCount = newTileCount;
    this->nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
}

//Destructor for tank type entities
Tank::~Tank()
{
    delete texture_;
}

void Tank::initNpc(){
    texture_ = new sf::Texture;
    if (!texture_->loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/Tanks/towerDefense_tileCustom.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_TANK_TEXTURE";
    }
    this->tankTexture.setTexture(*texture_);
	this->movementSpeed = 75.f;
    this->movementspeedmemory_ = this-> movementSpeed;
    this->tankTexture.setOrigin(32,32);
    this->tankTexture.setPosition(spawnLocation);
    this->hitpoints = 20;
}

void Tank::Render(sf::RenderTarget* target){
    target->draw(this->tankTexture);
}

sf::Vector2f Tank::getPosition() {
    return this->tankTexture.getPosition();
}

int Tank::getHitpoints() {
    return hitpoints;
}

void Tank::dealDamage(float damage) {
    this->hitpoints -= damage;
}

void Tank::slowMovement(float slow) {
    movementSpeed = movementspeedmemory_ * slow;
    //std::cout << movementSpeed << std::endl;
    slowcooldown_.restart();
}

void Tank::MoveTo(const float& dt){
    
    int dir_x, dir_y;

    if (tankTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
        nextLocation = exitLocation;
        //std::cout<< "Set exit!" << std::endl;
    }

    else if(sf::Vector2i(tankTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
        tileCount++;
        nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
        //std::cout<< "Moving!" << std::endl;
    }


    
    dir_x = FindDirection(nextLocation).first;
    dir_y = FindDirection(nextLocation).second;
    


    this->tankTexture.move(dir_x * movementSpeed * dt, dir_y * movementSpeed * dt);

    if(tankTexture.getPosition().x + 2 >= nextLocation.x && tankTexture.getPosition().x <= nextLocation.x + 2
    && tankTexture.getPosition().y + 2 >= nextLocation.y && tankTexture.getPosition().y <= nextLocation.y + 2) {
        this->tankTexture.setPosition(nextLocation);
        if (tankTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
            nextLocation = exitLocation;
            //std::cout<< "Set exit!" << std::endl;
        }

        else if(sf::Vector2i(tankTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
            tileCount++;
            nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
            //std::cout<< "Moving!" << std::endl;
        }
        //std::cout << "Teleport"<< std::endl;
        dir_x = FindDirection(nextLocation).first;
        dir_y = FindDirection(nextLocation).second;
        //std::cout << "Location :x " << this->tankTexture.getPosition().x <<" :y "<< this->tankTexture.getPosition().y << '\n' << "Next location :x "<< nextLocation.x <<" :y "<< nextLocation.y << std::endl;
    }
    
    this->Rotate(dir_x,dir_y);
    if(this->tankTexture.getPosition() == exitLocation) {
        this->end_ = true;
    }
    
    
}

bool Tank::hasReachedEnd() {
    return this->end_;
}

int Tank::getTileCount(){
    return tileCount;
}

void Tank::Rotate(int x, int y){
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
    this->tankTexture.setRotation(degree);
}

std::pair<int,int> Tank::FindDirection(sf::Vector2f nextLocation){
    std::pair<int,int> pos; 
    pos.first = 0;
    pos.second = 0;

    if((int)nextLocation.y == (int)tankTexture.getPosition().y){
        pos.second = 0;
    }
    else if((int)nextLocation.y > (int)tankTexture.getPosition().y){
        pos.second = 1;
    }
    else if((int)nextLocation.y < (int)tankTexture.getPosition().y){
        pos.second = -1;
    }
    if((int)nextLocation.x == (int)tankTexture.getPosition().x){
        pos.first = 0;
    }
    else if((int)nextLocation.x < (int)tankTexture.getPosition().x){
        pos.first= -1;
    }
    else if((int)nextLocation.x > (int)tankTexture.getPosition().x){
        pos.first = 1;
    }
    //std::cout<< "Direction x: " << pos.first << " y: " << pos.second << std::endl; 
    return pos;
}


void Tank::Update(const float& dt) {
        this->MoveTo(dt);

        sf::Time elapsed = slowcooldown_.getElapsedTime();
        if(4 < elapsed.asSeconds()) {
            this->movementSpeed = movementspeedmemory_;
        }
}
