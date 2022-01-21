#include "soldier.hpp"


//Constructor for tank type entities
Soldier::Soldier(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int newTileCount) : Npc(root_filepath,sLocation,eLocation,rTiles,newTileCount)
{
    this->root_filepath_ = root_filepath;
    this->spawnLocation = sLocation;
    this->exitLocation = eLocation;
    this->roadTiles = rTiles;
    this->worth_ = 1;
    //this->soldierTexture.setTexture(soldiertx_);
    initNpc();
    this->tileCount = newTileCount;
    this->nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
}

//Destructor for tank type entities
Soldier::~Soldier()
{

}

void Soldier::initNpc(){
    texture_ = new sf::Texture;
    if (!texture_->loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/FootSoldiers/towerDefense_tile248.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_SOLDIER_TEXTURE";
    }

    this->soldierTexture.setTexture(*texture_);
	this->movementSpeed = 90.f;
    this->movementspeedmemory_ = this-> movementSpeed;
    this->soldierTexture.setOrigin(32,32);
    this->soldierTexture.setPosition(spawnLocation);
    this->hitpoints = 1;
}

void Soldier::Render(sf::RenderTarget* target){
    target->draw(this->soldierTexture);
}

sf::Vector2f Soldier::getPosition() {
    return this->soldierTexture.getPosition();
}

int Soldier::getHitpoints() {
    return hitpoints;
}

void Soldier::dealDamage(float damage) {
    this->hitpoints -= damage;
}

void Soldier::slowMovement(float slow) {
    movementSpeed = movementspeedmemory_ * slow;
    //std::cout << movementSpeed << std::endl;
    slowcooldown_.restart();
}

void Soldier::MoveTo(const float& dt){
    
    int dir_x, dir_y;

    if (soldierTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
        nextLocation = exitLocation;
        //std::cout<< "Set exit!" << std::endl;
    }

    else if(sf::Vector2i(soldierTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
        tileCount++;
        nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
        //std::cout<< "Moving!" << std::endl;
    }


    
    dir_x = FindDirection(nextLocation).first;
    dir_y = FindDirection(nextLocation).second;
    


    this->soldierTexture.move(dir_x * movementSpeed * dt, dir_y * movementSpeed * dt);

    if(soldierTexture.getPosition().x + 2 >= nextLocation.x && soldierTexture.getPosition().x <= nextLocation.x + 2
    && soldierTexture.getPosition().y + 2 >= nextLocation.y && soldierTexture.getPosition().y <= nextLocation.y + 2) {
        this->soldierTexture.setPosition(nextLocation);
        if (soldierTexture.getPosition() == this->roadTiles->back()->getCenterPosition()){
            nextLocation = exitLocation;
            //std::cout<< "Set exit!" << std::endl;
        }

        else if(sf::Vector2i(soldierTexture.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition()) && nextLocation != exitLocation){
            tileCount++;
            nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
            //std::cout<< "Moving!" << std::endl;
        }
        //std::cout << "Teleport"<< std::endl;
        dir_x = FindDirection(nextLocation).first;
        dir_y = FindDirection(nextLocation).second;
        //std::cout << "Location :x " << this->soldierTexture.getPosition().x <<" :y "<< this->soldierTexture.getPosition().y << '\n' << "Next location :x "<< nextLocation.x <<" :y "<< nextLocation.y << std::endl;
    }
    
    this->Rotate(dir_x,dir_y);
    if(this->soldierTexture.getPosition() == exitLocation) {
        this->end_ = true;
    }
    
    
}

bool Soldier::hasReachedEnd() {
    return this->end_;
}

void Soldier::Rotate(int x, int y){
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
    this->soldierTexture.setRotation(degree);
}

std::pair<int,int> Soldier::FindDirection(sf::Vector2f nextLocation){
    std::pair<int,int> pos; 
    pos.first = 0;
    pos.second = 0;

    if((int)nextLocation.y == (int)soldierTexture.getPosition().y){
        pos.second = 0;
    }
    else if((int)nextLocation.y > (int)soldierTexture.getPosition().y){
        pos.second = 1;
    }
    else if((int)nextLocation.y < (int)soldierTexture.getPosition().y){
        pos.second = -1;
    }
    if((int)nextLocation.x == (int)soldierTexture.getPosition().x){
        pos.first = 0;
    }
    else if((int)nextLocation.x < (int)soldierTexture.getPosition().x){
        pos.first= -1;
    }
    else if((int)nextLocation.x > (int)soldierTexture.getPosition().x){
        pos.first = 1;
    }
    //std::cout<< "Direction x: " << pos.first << " y: " << pos.second << std::endl; 
    return pos;
}


void Soldier::Update(const float& dt) {
        this->MoveTo(dt);

        sf::Time elapsed = slowcooldown_.getElapsedTime();
        if(4 < elapsed.asSeconds()) {
            this->movementSpeed = movementspeedmemory_;
        }
}
