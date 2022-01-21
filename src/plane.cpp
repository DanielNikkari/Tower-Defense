#include "plane.hpp"


//Constructor for plane type entities
Plane::Plane(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int round, int newTileCount) : Npc(root_filepath,sLocation,eLocation,rTiles,newTileCount)
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

//Destructor for plane type entities
Plane::~Plane()
{
    delete texture_;
}

void Plane::initNpc(){
    texture_ = new sf::Texture;
    if (!texture_->loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/Airplanes/towerDefense_tile271.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_PLANE_TEXTURE";
    }

    this->planeTexture.setTexture(*texture_);
	this->movementSpeed = 120.f;
    this->movementspeedmemory_ = this-> movementSpeed;
    this->planeTexture.setOrigin(32,32);
    this->planeTexture.setPosition(spawnLocation);
    this->hitpoints = 10;
}

void Plane::Render(sf::RenderTarget* target){
    target->draw(this->planeTexture);
}

sf::Vector2f Plane::getPosition() {
    return this->planeTexture.getPosition();
}

int Plane::getHitpoints() {
    return hitpoints;
}

void Plane::dealDamage(float damage) {
    this->hitpoints -= damage;
}

void Plane::slowMovement(float slow) {
    movementSpeed = movementspeedmemory_ * slow;
    //std::cout << movementSpeed << std::endl;
    slowcooldown_.restart();
}

void Plane::MoveTo(const float& dt){
    
    int dir_x, dir_y;

    if (planeTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
        nextLocation = exitLocation;
        //std::cout<< "Set exit!" << std::endl;
    }

    else if(sf::Vector2i(planeTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
        tileCount++;
        nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
        //std::cout<< "Moving!" << std::endl;
    }


    
    dir_x = FindDirection(nextLocation).first;
    dir_y = FindDirection(nextLocation).second;
    


    this->planeTexture.move(dir_x * movementSpeed * dt, dir_y * movementSpeed * dt);

    if(planeTexture.getPosition().x + 2 >= nextLocation.x && planeTexture.getPosition().x <= nextLocation.x + 2
    && planeTexture.getPosition().y + 2 >= nextLocation.y && planeTexture.getPosition().y <= nextLocation.y + 2) {
        this->planeTexture.setPosition(nextLocation);
        if (planeTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
            nextLocation = exitLocation;
            //std::cout<< "Set exit!" << std::endl;
        }

        else if(sf::Vector2i(planeTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
            tileCount++;
            nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
            //std::cout<< "Moving!" << std::endl;
        }
        //std::cout << "Teleport"<< std::endl;
        dir_x = FindDirection(nextLocation).first;
        dir_y = FindDirection(nextLocation).second;
        //std::cout << "Location :x " << this->planeTexture.getPosition().x <<" :y "<< this->planeTexture.getPosition().y << '\n' << "Next location :x "<< nextLocation.x <<" :y "<< nextLocation.y << std::endl;
    }
    
    this->Rotate(dir_x,dir_y);
    if(this->planeTexture.getPosition() == exitLocation) {
        this->end_ = true;
    }
    
    
}

bool Plane::hasReachedEnd() {
    return this->end_;
}

void Plane::Rotate(int x, int y){
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
    this->planeTexture.setRotation(degree);
}

std::pair<int,int> Plane::FindDirection(sf::Vector2f nextLocation){
    std::pair<int,int> pos; 
    pos.first = 0;
    pos.second = 0;

    if((int)nextLocation.y == (int)planeTexture.getPosition().y){
        pos.second = 0;
    }
    else if((int)nextLocation.y > (int)planeTexture.getPosition().y){
        pos.second = 1;
    }
    else if((int)nextLocation.y < (int)planeTexture.getPosition().y){
        pos.second = -1;
    }
    if((int)nextLocation.x == (int)planeTexture.getPosition().x){
        pos.first = 0;
    }
    else if((int)nextLocation.x < (int)planeTexture.getPosition().x){
        pos.first= -1;
    }
    else if((int)nextLocation.x > (int)planeTexture.getPosition().x){
        pos.first = 1;
    }
    //std::cout<< "Direction x: " << pos.first << " y: " << pos.second << std::endl; 
    return pos;
}


void Plane::Update(const float& dt) {
        this->MoveTo(dt);

        sf::Time elapsed = slowcooldown_.getElapsedTime();
        if(4 < elapsed.asSeconds()) {
            this->movementSpeed = movementspeedmemory_;
        }
}
