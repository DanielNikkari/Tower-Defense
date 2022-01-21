#include "gametile.hpp"

//Constructor of GameTile class
GameTile::GameTile(std::string root_filepath, std::string texturename, sf::Vector2f pos, bool buildable, int squaresize) {
    this->buildable_ = buildable;
    this->root_filepath_ = root_filepath + "/src/Textures/WorldTextures/";
    this->texturename_ = texturename;
    this->pos_ = pos;
    this->squaresize_ = squaresize;
    this->setUpSprite();
}

//Destructor of GameTile class
GameTile::~GameTile() {
    
}

//Checks if you can build on top of the given tile
bool GameTile::isBuildable() {
    return this->buildable_;
}

//Checks if the given tile is a road
bool GameTile::isRoad() {
    return !buildable_;
}

//Gets the name of the texture used in the given tile
std::string GameTile::CurrentTexture() {
    return texturename_;
}

//Gets the squaresize of the given tile
int GameTile::getSquareSize() {
    return squaresize_;
}

//Checks if this tile is so called exittile
bool GameTile::isExit() {
    if(texturename_ == "towerDefense_tile017.png") {
        return true;
    }
    else {
        return false;
    }
}

//Checks if this tile is so called spawnertile
bool GameTile::isSpawn() {
    if(texturename_ == "towerDefense_tile016.png") {
        return true;
    }
    else {
        return false;
    }
}

//Gets the center coordinates of the given tile
sf::Vector2f GameTile::getCenterPosition() {
    sf::Vector2f pos(pos_.x + 32, pos_.y + 32);
    return pos;
}

//Gets the coordinates of the given tile. Upper left corner
sf::Vector2f GameTile::getPosition() {
    return pos_;
}

//Loads the texture and initializes the texture as sprite
void GameTile::setUpSprite() {

    if(!this->texture_.loadFromFile(root_filepath_ + texturename_))
	{
		throw "ERROR::GAMETILE::FAILED_TO_LOAD_TILE_TEXTURE";
	}
    this->sprite_.setTexture(texture_);
    this->sprite_.setTextureRect(sf::IntRect(0, 0, squaresize_, squaresize_));
    this->sprite_.setPosition(pos_);

}

//Renders the sprite to a given target window
void GameTile::render(sf::RenderTarget* target)
{
	// Draw the tile
	target->draw(this->sprite_);
    
}
