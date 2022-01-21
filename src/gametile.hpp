#ifndef GAMETILE_H
#define GAMETILE_H

#include "state.hpp"

/// GameTile object that is used for a construction of a map
class GameTile 
{
public:
    /// @brief Constructor
	///
	/// @param root_filepath Root filepath
	/// @param texturename Name of the texture used for the tile
	/// @param pos Position of the tile
    /// @param buildable Value telling if the tile is buildable
    /// @param squaresize Size of the tile
    GameTile(std::string root_filepath, std::string texturename, sf::Vector2f pos, bool buildable, int squaresize); 

    /// @brief Destructor
    ~GameTile();

    /// @brief Sets up the sprite
    void setUpSprite();

    /// @brief Returns the name of the texture
    std::string CurrentTexture();

    /// @brief Returns the center position of the tile
    sf::Vector2f getCenterPosition();

    /// @brief Returns the top left corner position of this tile
    sf::Vector2f getPosition();

    /// @brief Returns the squaresize of the tile
    int getSquareSize();
    
    /// @brief Checks if the tile is exit tile
    bool isExit();

    /// @brief Checks if the tile is spawn tile
    bool isSpawn();

    /// @brief Checks if the tile is a road tile
    bool isRoad();

    /// @brief Checks if the tile is a buildable tile
    bool isBuildable();

    /// @brief Renders the tile
	///
	/// @param target Rendering target, i.e. game window
    void render(sf::RenderTarget* target);

protected:
    /// Top left corner position of the tile
    sf::Vector2f pos_;

    /// Texture object
    sf::Texture texture_;

    /// Sprite of this tile
    sf::Sprite sprite_;

    /// Root filepath
    std::string root_filepath_;

    /// Name of the texture
    std::string texturename_;

    /// Value telling if the tile is buildable or not
    bool buildable_;

    /// Squaresize of the tile
    int squaresize_;
    
};

#endif