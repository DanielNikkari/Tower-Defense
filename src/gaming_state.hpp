#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "state.hpp"
#include "state.cpp"
#include "gametile.hpp"
#include <algorithm>
#include "npc.hpp"
#include "basic_tower.hpp"
#include "basic_tower.cpp"
#include "bomb_tower.hpp"
#include "bomb_tower.cpp"
#include "slowing_tower.hpp"
#include "slowing_tower.cpp"
#include <list>
#include "button.hpp"
#include "textbox.hpp"
#include <iomanip> 
#include <sstream>
#include <algorithm>

class GameState : public State
{
public:

    /// @brief Constructor
	///
	/// @param window Window object where objects are drawn
	/// @param supportedKeys Keys that are supported
	/// @param root_filepath Root filepath
	/// @param states Stack containing all of the current states
    /// @param mapfile Name of the map to be loaded
	/// @param font Font used for text
	GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Event* Event, std::string mapfile, sf::Font *font, int volumeLevel, int soundEffectVolumeLevel);

    /// @brief Destructor
	virtual ~GameState();

    /// @brief Updates inputs
	///
	/// @param dt Delta time
	virtual void updateInput(const float& dt);

    /// @brief Called when ending this state
	virtual void endState();

    /// @brief Updates objects
	///
	/// @param dt Delta time
	virtual void update(const float& dt);

    /// @brief Renders objects
	///
	/// @param target Rendering target, i.e. game window
	virtual void render(sf::RenderTarget* target = nullptr);

    /// @brief Reads the file given from the Maps folder and initializes the tiles
    void loadmap();

    /// @brief Initializes the spawnlocation
    void initSpawnLocation();

    /// @brief Initializes the exitlocation
    void initExitLocation();

    /// @brief Initializes the road for enemies
    void initRoad();

    /// @brief Initializes game state background music
    void initMusic();

    /// @brief Initialize NPC textures
    void initNpcTextures();

    /// @brief Returns tower, if there is one, at that gridlocation
    Tower* towerAt(int x, int y);

    /// @brief Builds the road recursively using this function
    void buildRoad(std::pair<int, int> current, std::pair<int, int> previous);

    /// @brief Returns tile, if there is one, at that gridlocation
    GameTile* TileAt(int x, int y);

    /// @brief Returns every neighbour (not diagonals) of this tile
    std::vector<GameTile*> getNeighbours(GameTile* current);

    /// @brief Changes the money player has
    void changeMoney(int money);

    /// @brief Changes the health player has
    void changeHealth(int health);

    /// @brief Changes the score player has
    void changeScore(int score);

    /// @brief Checks if the player has 0 or less health
    bool lostGame();

    /// @brief Returns the spawn position
    sf::Vector2f getSpawn();

    /// @brief Returns the exit position
    sf::Vector2f getExit();

    /// @brief Returns all roadtiles inside a vector
    std::vector<GameTile*> getRoad();

    /// @brief Puts message on the screen for the player for 5 seconds
    void messageForPlayer(std::string message);

    /// @brief Updates buttons
    void updateButtons();

    /// @brief Initializes variables
    void initVariables();

    /// @brief Gets press timer
    const bool getPressTimer();

    /// @brief Renders ghost towers if they are selected
    void renderSoonToBeTower(sf::RenderTarget* target);

    /// @brief Initializes ghost towers
    void initGhostTowers();

    /// @brief Buys the selected tower and puts at the wanted location
    void buyTower();

    /// @brief Updates info about selected tower to the UI
    void updateTargetedTowerInfo();

    /// @brief Handles the Round system logic
    void updateGameLogic();

    /// @brief Handles the enemy spawning
    void spawnEnemies();

	/// @brief Save score to highscores file
	void SaveToFile();

private:

    /// Npc textures
    sf::Texture soldiertx_;
    sf::Texture planetx_;
    sf::Texture tanktx_;
    sf::Texture superTanktx_;
    sf::Texture superPlanetx_;

    /// Root filepath
	std::string root_filepath_;

    /// Name of the map
    std::string mapfile_;

    /// All tiles in the map and their gridlocations
    std::map<std::pair<int, int>, GameTile *> Tiles;
 
    /// All spawn and exit tiles in the map and their gridlocations
    std::map<std::pair<int, int>, GameTile *> SpawnAndExit_;

    /// Spawn position
    sf::Vector2f spawnlocation_;

    /// Exit position
    sf::Vector2f exitlocation_;

    /// Current money as a text that can be drawn
    sf::Text money_;

    /// Current health as a text that can be drawn
    sf::Text health_;

    /// Current score as a text that can be drawn
    sf::Text score_;

    /// Current money
    int int_money_ = 0;

    /// Current health
    int int_health_ = 0;

    /// Current score
    int int_score_ = 0;

    /// Spawn tile
    GameTile* spawnTile_;

    /// Exit tile
    GameTile* exitTile_;

    /// Font
    sf::Font *font_;

    /// All road tiles
    std::vector<GameTile*> roadTiles_;

    /// RectangleShape used as sidebar
    sf::RectangleShape rect_menu_;

    /// Tracks the current mouseposition on grid
    sf::Vector2u mousePosGrid;

    /// Timer
    sf::Clock clock;

    /// Super tank timer
    sf::Clock superTankTimer_;

    /// Super plane timer
    sf::Clock superPlaneTimer_;

    /// All towers and their gridlocations
    std::map<std::pair<int, int>, Tower *> towers_;

    /// All enemies currently on the road
    std::list<Npc*> enemies_;

    /// Enemies to be deleted after the wave
    std::vector<Npc*> tobedeleted;

    /// Number of super tanks
    int n_super_tank_ = 0;

    /// Number of super planes
    int n_super_plane_ = 0;

	/// Initializes keybinds
	void initKeyBinds();

    /// Message to be displayed for the player
    sf::Text message_;

    /// Message timer
    sf::Clock message_timer_;
    
    std::map<std::string, Button*> towerbuttons_;

    std::map<std::string, Button*> targeted_towerbuttons_;

    bool basic_tower_flag_ = false;
    bool bomb_tower_flag_ = false;
    bool slowing_tower_flag_ = false;

    sf::Clock pressTimer_;
	sf::Int32 pressTimerMax_;
    sf::Clock timer_;
	sf::Int32 timerMax_;
    float windowX_;
	float windowY_;

    sf::Sprite basic_ghost_;
    sf::Sprite bomb_ghost_;
    sf::Sprite slowing_ghost_;

    sf::Texture basic_texture_ghost_;
    sf::Texture bomb_texture_ghost_;
    sf::Texture slowing_texture_ghost_;

    sf::Clock shoptimer_;

    Tower* target_ = nullptr;

    int upgradeprice_;
    int sellprice_;

    sf::Text damage_text_;
    sf::Text attack_speed_text_;
    sf::Text tower_level_text;

    int currentRound_ = 0;
    int enemiestobespawnedremaining_ = 0;

    sf::Clock roundtimer_;
    sf::Text roundtimer_text_;
    int roundtimer_int_;
    bool roundover_flag_;
    std::map<std::string, Button*> roundoverbuttons_;
    sf::Clock spawntimer_;
    sf::Text currentwave_;

    sf::Text enemiesleft_;
    sf::Text you_have_lost_;
    bool game_over_ = false;

    /// Music object and volume
    sf::Music gamingStateMusic_;
    int volumeLevel_;
    int soundEffectVolumeLevel_;

	sf::Event* Event_;

	Textbox* textbox_;
};


#endif
