#include "super_plane.hpp"


//Constructor for plane type entities
SuperPlane::SuperPlane(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int round) : Npc(root_filepath,sLocation,eLocation,rTiles)
{
    this->root_filepath_ = root_filepath;
    this->spawnLocation = sLocation;
    this->exitLocation = eLocation;
    this->roadTiles = rTiles;
    this->worth_ = 2;
    initNpc();
    this->hitpoints = this->hitpoints * pow(1.05, round);
    this->nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
}

//Destructor for plane type entities
SuperPlane::~SuperPlane()
{
    delete texture_;
}

void SuperPlane::initNpc(){
    texture_ = new sf::Texture;
    if (!texture_->loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/Airplanes/towerDefense_superPlane.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_SUPER_PLANE_TEXTURE";
    }

    this->superPlaneTexture.setTexture(*texture_);
    this->superPlaneTexture.setScale(1.3, 1.3);
	this->movementSpeed = 200.f;
    this->movementspeedmemory_ = this-> movementSpeed;
    this->superPlaneTexture.setOrigin(32,32);
    this->superPlaneTexture.setPosition(spawnLocation);
    this->hitpoints = 50;
}

void SuperPlane::Render(sf::RenderTarget* target){
    target->draw(this->superPlaneTexture);
}

sf::Vector2f SuperPlane::getPosition() {
    return this->superPlaneTexture.getPosition();
}

int SuperPlane::getHitpoints() {
    return hitpoints;
}

void SuperPlane::dealDamage(float damage) {
    this->hitpoints -= damage;
}

void SuperPlane::slowMovement(float slow) {
    movementSpeed = movementspeedmemory_ * slow;
    //std::cout << movementSpeed << std::endl;
    slowcooldown_.restart();
}

void SuperPlane::MoveTo(const float& dt){
    
    int dir_x, dir_y;

    if (superPlaneTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
        nextLocation = exitLocation;
        //std::cout<< "Set exit!" << std::endl;
    }

    else if(sf::Vector2i(superPlaneTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
        tileCount++;
        nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
        //std::cout<< "Moving!" << std::endl;
    }


    
    dir_x = FindDirection(nextLocation).first;
    dir_y = FindDirection(nextLocation).second;
    


    this->superPlaneTexture.move(dir_x * movementSpeed * dt, dir_y * movementSpeed * dt);

    if(superPlaneTexture.getPosition().x + 2 >= nextLocation.x && superPlaneTexture.getPosition().x <= nextLocation.x + 2
    && superPlaneTexture.getPosition().y + 2 >= nextLocation.y && superPlaneTexture.getPosition().y <= nextLocation.y + 2) {
        this->superPlaneTexture.setPosition(nextLocation);
        if (superPlaneTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
            nextLocation = exitLocation;
            //std::cout<< "Set exit!" << std::endl;
        }

        else if(sf::Vector2i(superPlaneTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
            tileCount++;
            nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
            //std::cout<< "Moving!" << std::endl;
        }
        //std::cout << "Teleport"<< std::endl;
        dir_x = FindDirection(nextLocation).first;
        dir_y = FindDirection(nextLocation).second;
        //std::cout << "Location :x " << this->superPlaneTexture.getPosition().x <<" :y "<< this->superPlaneTexture.getPosition().y << '\n' << "Next location :x "<< nextLocation.x <<" :y "<< nextLocation.y << std::endl;
    }
    
    this->Rotate(dir_x,dir_y);
    if(this->superPlaneTexture.getPosition() == exitLocation) {
        this->end_ = true;
    }
    
    
}

bool SuperPlane::hasReachedEnd() {
    return this->end_;
}

void SuperPlane::Rotate(int x, int y){
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
    this->superPlaneTexture.setRotation(degree);
}

std::pair<int,int> SuperPlane::FindDirection(sf::Vector2f nextLocation){
    std::pair<int,int> pos; 
    pos.first = 0;
    pos.second = 0;

    if((int)nextLocation.y == (int)superPlaneTexture.getPosition().y){
        pos.second = 0;
    }
    else if((int)nextLocation.y > (int)superPlaneTexture.getPosition().y){
        pos.second = 1;
    }
    else if((int)nextLocation.y < (int)superPlaneTexture.getPosition().y){
        pos.second = -1;
    }
    if((int)nextLocation.x == (int)superPlaneTexture.getPosition().x){
        pos.first = 0;
    }
    else if((int)nextLocation.x < (int)superPlaneTexture.getPosition().x){
        pos.first= -1;
    }
    else if((int)nextLocation.x > (int)superPlaneTexture.getPosition().x){
        pos.first = 1;
    }
    //std::cout<< "Direction x: " << pos.first << " y: " << pos.second << std::endl; 
    return pos;
}

int SuperPlane::getTileCount(){
    return tileCount;
}


void SuperPlane::Update(const float& dt) {
        this->MoveTo(dt);

        sf::Time elapsed = slowcooldown_.getElapsedTime();
        if(4 < elapsed.asSeconds()) {
            this->movementSpeed = movementspeedmemory_;
        }
}
