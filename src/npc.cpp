#include "npc.hpp"


//Constructor for Npc type entities
Npc::Npc(std::string root_filepath,sf::Vector2f sLocation,sf::Vector2f eLocation,std::vector<GameTile*>* rTiles, int newTileCount)
{
    this->root_filepath_ = root_filepath;
    this->spawnLocation = sLocation;
    this->exitLocation = eLocation;
    this->roadTiles = rTiles;
    initNpc();
    this->tileCount = newTileCount;
    this->nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
}

//Destructor for npc type entities
Npc::~Npc()
{
    std::cout << "~Npc called!" << "\n";
    delete stexture_;
}

void Npc::initNpc(){
    stexture_ = new sf::Texture;
    if (!stexture_->loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/FootSoldiers/towerDefense_tile245.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_SOLDIER_TEXTURE";
    }
    this->sotilastektuuri.setTexture(*stexture_);
	this->movementSpeed = 100.f;
    this->movementspeedmemory_ = this-> movementSpeed;
    this->sotilastektuuri.setOrigin(32,32);
    this->sotilastektuuri.setPosition(spawnLocation);
    this->hitpoints = 1;
}

void Npc::Render(sf::RenderTarget* target){
    target->draw(this->sotilastektuuri);
}

sf::Vector2f Npc::getPosition() {
    return this->sotilastektuuri.getPosition();
}

int Npc::getHitpoints() {
    return hitpoints;
}

void Npc::dealDamage(float damage) {
    this->hitpoints -= damage;
}

void Npc::slowMovement(float slow) {
    if(movementspeedmemory_ - slow <= movementSpeed) {
        //movementSpeed = movementspeedmemory_ - slow;
        std::cout << movementSpeed << std::endl;
        slowcooldown_.restart();
    }
}

void Npc::MoveTo(const float& dt){
    
    int dir_x, dir_y;

    if (sotilastektuuri.getPosition() == this->roadTiles->back()->getCenterPosition()){
        nextLocation = exitLocation;
        //std::cout<< "Set exit!" << std::endl;
    }

    else if(sf::Vector2i(sotilastektuuri.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition())){
        tileCount++;
        nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
        //std::cout<< "Moving!" << std::endl;
    }


    
    dir_x = FindDirection(nextLocation).first;
    dir_y = FindDirection(nextLocation).second;
    


    this->sotilastektuuri.move(dir_x * movementSpeed * dt, dir_y * movementSpeed * dt);

    if(sotilastektuuri.getPosition().x + 2 >= nextLocation.x && sotilastektuuri.getPosition().x <= nextLocation.x + 2
    && sotilastektuuri.getPosition().y + 2 >= nextLocation.y && sotilastektuuri.getPosition().y <= nextLocation.y + 2) {
        this->sotilastektuuri.setPosition(nextLocation);
        if (sotilastektuuri.getPosition() == this->roadTiles->back()->getCenterPosition()){
            nextLocation = exitLocation;
            //std::cout<< "Set exit!" << std::endl;
        }

        else if(sf::Vector2i(sotilastektuuri.getPosition()) == sf::Vector2i(this->roadTiles->operator[](tileCount)->getCenterPosition())){
            tileCount++;
            nextLocation = this->roadTiles->operator[](tileCount)->getCenterPosition();
            //std::cout<< "Moving!" << std::endl;
        }
        //std::cout << "Teleport"<< std::endl;
        dir_x = FindDirection(nextLocation).first;
        dir_y = FindDirection(nextLocation).second;
        //std::cout << "Location :x " << this->sotilastektuuri.getPosition().x <<" :y "<< this->sotilastektuuri.getPosition().y << '\n' << "Next location :x "<< nextLocation.x <<" :y "<< nextLocation.y << std::endl;
    }
    
    this->Rotate(dir_x,dir_y);
    if(this->sotilastektuuri.getPosition() == exitLocation) {
        this->end_ = true;
    }
    
    
}

bool Npc::hasReachedEnd() {
    return this->end_;
}

void Npc::Rotate(int x, int y){
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
    this->sotilastektuuri.setRotation(degree);
}

std::pair<int,int> Npc::FindDirection(sf::Vector2f nextLocation){
    std::pair<int,int> pos; 
    pos.first = 0;
    pos.second = 0;

    if((int)nextLocation.y == (int)sotilastektuuri.getPosition().y){
        pos.second = 0;
    }
    else if((int)nextLocation.y > (int)sotilastektuuri.getPosition().y){
        pos.second = 1;
    }
    else if((int)nextLocation.y < (int)sotilastektuuri.getPosition().y){
        pos.second = -1;
    }
    if((int)nextLocation.x == (int)sotilastektuuri.getPosition().x){
        pos.first = 0;
    }
    else if((int)nextLocation.x < (int)sotilastektuuri.getPosition().x){
        pos.first= -1;
    }
    else if((int)nextLocation.x > (int)sotilastektuuri.getPosition().x){
        pos.first = 1;
    }
    //std::cout<< "Direction x: " << pos.first << " y: " << pos.second << std::endl; 
    return pos;
}

int Npc::getTileCount(){
    return tileCount;
}


void Npc::Update(const float& dt) {
        this->MoveTo(dt);

        sf::Time elapsed = slowcooldown_.getElapsedTime();
        if(4 < elapsed.asSeconds()) {
            this->movementSpeed = movementspeedmemory_;
        }
}
int Npc::getWorth() {
    return worth_;
}
