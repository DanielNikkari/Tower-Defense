#include "gaming_state.hpp"
#include "npc.cpp"
#include "tank.cpp"
#include "plane.cpp"
#include "soldier.cpp"
#include "super_tank.cpp"
#include "super_plane.cpp"
/* #include "insert_highscore_state.cpp" */
/* #include "insert_highscore_state.hpp" */

//Size of one square
int SQUARESIZE = 64;

//Constructor of GameState class
GameState::GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Event* Event, std::string mapfile, sf::Font *font, int volumeLevel, int soundEffectVolumeLevel) : State(window, supportedKeys, root_filepath, states)
{
	this->Event_ = Event;

	this->root_filepath_ = root_filepath;
    this->mapfile_ = mapfile;
    this->font_ = font;
    this->volumeLevel_ = volumeLevel;
    this->soundEffectVolumeLevel_ = soundEffectVolumeLevel;
	this->initKeyBinds();
    this->loadmap();
    this->initSpawnLocation();
    this->initExitLocation();
    this->initRoad();
    this->initVariables();
    this->initGhostTowers();
    this->initMusic();

    this->changeMoney(1000);
    this->changeHealth(1);
    this->changeScore(0);
    this->rect_menu_.setFillColor(sf::Color(34, 42, 52));
    this->rect_menu_.setSize(sf::Vector2f(512.f, 1080.f));
    this->rect_menu_.setPosition(1472, 0);

    this->money_.setPosition(1500, 35);
    this->money_.setCharacterSize(30);
    this->money_.setStyle(sf::Text::Bold);
    this->money_.setFillColor(sf::Color::Yellow);
    this->money_.setFont(*font_);

    this->health_.setPosition(1500, 5);
    this->health_.setCharacterSize(30);
    this->health_.setStyle(sf::Text::Bold);
    this->health_.setFillColor(sf::Color::Red);
    this->health_.setFont(*font_);

    this->score_.setPosition(1650, 5);
    this->score_.setCharacterSize(30);
    this->score_.setStyle(sf::Text::Bold);
    this->score_.setFillColor(sf::Color::White);
    this->score_.setFont(*font_);

    this->message_.setPosition(window_->getSize().x/3, window_->getSize().y/3);
    this->message_.setCharacterSize(50);
    this->message_.setStyle(sf::Text::Bold);
    this->message_.setFont(*font_);

    this->damage_text_.setPosition(1500, 75);
    this->damage_text_.setCharacterSize(20);
    this->damage_text_.setStyle(sf::Text::Bold);
    this->damage_text_.setFillColor(sf::Color::White);
    this->damage_text_.setFont(*font_);

    this->attack_speed_text_.setPosition(1500, 100);
    this->attack_speed_text_.setCharacterSize(20);
    this->attack_speed_text_.setStyle(sf::Text::Bold);
    this->attack_speed_text_.setFillColor(sf::Color::White);
    this->attack_speed_text_.setFont(*font_);

    this->tower_level_text.setPosition(1500, 125);
    this->tower_level_text.setCharacterSize(20);
    this->tower_level_text.setStyle(sf::Text::Bold);
    this->tower_level_text.setFillColor(sf::Color::White);
    this->tower_level_text.setFont(*font_);

    this->currentwave_.setPosition(1500, 850);
    this->currentwave_.setCharacterSize(30);
    this->currentwave_.setStyle(sf::Text::Bold);
    this->currentwave_.setFillColor(sf::Color::White);
    this->currentwave_.setFont(*font_);

    this->enemiesleft_.setPosition(1500, 800);
    this->enemiesleft_.setCharacterSize(30);
    this->enemiesleft_.setStyle(sf::Text::Bold);
    this->enemiesleft_.setFillColor(sf::Color::White);
    this->enemiesleft_.setFont(*font_);

    this->you_have_lost_.setPosition(900, 400);
    this->you_have_lost_.setCharacterSize(45);
    this->you_have_lost_.setStyle(sf::Text::Bold);
    this->you_have_lost_.setFillColor(sf::Color::Red);
    this->you_have_lost_.setFont(*font_);
    this->you_have_lost_.setString("You Lost!");

    this->messageForPlayer("Have fun!");

    //Testing purposes
    

    



    this->towerbuttons_["Basic Tower"] = new Button(1500, 400, 150, 100, this->font_, "Basic Tower\n      50", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 20, false);
    this->towerbuttons_["Bomb Tower"] = new Button(1720, 400, 150, 100, this->font_, "Bomb Tower\n      100", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 20, false);
    this->towerbuttons_["Sticky Tower"] = new Button(1500, 500, 150, 100, this->font_, "Sticky Tower\n       75", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 20, false);

    this->targeted_towerbuttons_["Upgrade"] = new Button(1500, 200, 150, 100, this->font_, "Upgrade", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 20, true);
    this->targeted_towerbuttons_["Sell"] = new Button(1720, 200, 150, 100, this->font_, "Sell", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 20, true);

	this->textbox_ = new Textbox(1500, 600, 370, 150, this->font_, "Name", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20,  200), 30, true, this->Event_);

    this->roundoverbuttons_["Skip"] = new Button(1500, 950, 150, 100, this->font_, "Skip", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 20, true);

    this->roundtimer_int_ = 30;

    this->roundtimer_text_.setPosition(1800, 950);
    this->roundtimer_text_.setCharacterSize(40);
    this->roundtimer_text_.setStyle(sf::Text::Bold);
    this->roundtimer_text_.setFillColor(sf::Color::White);
    this->roundtimer_text_.setFont(*font_);

    this->roundtimer_text_.setString(std::to_string(roundtimer_int_));
    
    
}

//Destructor of GameState class
GameState::~GameState()
{
    for(auto i : Tiles) {
        delete i.second;
    }
    for(auto i : SpawnAndExit_) {
        delete i.second;
    }
    roadTiles_.clear();
    for(auto i: enemies_) {
        delete i;
    }
    for(auto i : towers_) {
        delete i.second;
    }
    for(auto i : roundoverbuttons_) {
        delete i.second;
    }
    for(auto i : targeted_towerbuttons_) {
        delete i.second;
    }
    for(auto i : towerbuttons_) {
        delete i.second;
    }
  
  
}

void GameState::SaveToFile() {
	std::fstream File;
	File.open("highscores.txt", std::ios::app);
	if(File.is_open()) {
		std::string player_name = this->textbox_->GetText();
		if (player_name == "") {
			player_name = "LMAO";
		}
		File << this->mapfile_.substr(0, this->mapfile_.length() -4) << ";" << player_name << ";" << this->int_score_ << std::endl;
		File.close();
	}
}

//Initializes the keybinds
void GameState::initKeyBinds()
{
	// Open keys cpnfigure file
	std::ifstream ifs(root_filepath_ + "/src/Config/gamestate_keybinds.ini");
	if (ifs.is_open())
	{
		std::string key = "";
		std::string key_name = "";
		while (ifs >> key >> key_name )
		{
			// Add supported keys to the supportedKeys map
			this->keybinds_[key] = this->supportedKeys_->at(key_name);
		}
	}
	// Close keys configure file
	ifs.close();
}

void GameState::initNpcTextures() {
    if (!soldiertx_.loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/FootSoldiers/towerDefense_tile245.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_SOLDIER_TEXTURE";
    }
    if (!planetx_.loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/Airplanes/towerDefense_tile271.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_PLANE_TEXTURE";
    }
    if (!tanktx_.loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/Tanks/towerDefense_tileCustom.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_TANK_TEXTURE";
    }
    if (!superTanktx_.loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/Tanks/towerDefense_BossTank.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_SUPER_TANK_TEXTURE";
    }
    if (!superPlanetx_.loadFromFile( root_filepath_ + "/src/Textures/NPCTextures/EnemyNPC/Airplanes/towerDefense_superPlane.png")){
        throw "ERROR::GAMETILE::FAILED_TO_LOAD_SUPER_PLANE_TEXTURE";
    }
}

//Initializes background music
void GameState::initMusic()
{
    // Initialize music
    if (!this->gamingStateMusic_.openFromFile(root_filepath_ + "/src/Audio/BackgroundMusic/battleThemeB.wav"))
    {
        throw "ERROR::MAINMENUSTATE::UNABLE_TO_LOAD_BACKGROUND_MUSIC_FILE";
    }
    this->gamingStateMusic_.play();
    this->gamingStateMusic_.setLoop(true);
    this->gamingStateMusic_.setVolume(volumeLevel_);
}

//Gives the tile at that GridLocation
GameTile* GameState::TileAt(int x, int y) {
	std::pair<int, int> pair(x, y);
    if(this->Tiles.count(pair)) {
        return this->Tiles.find(pair)->second;
    }
	else {
        return nullptr;
    }
}

Tower* GameState::towerAt(int x, int y) {
	std::pair<int, int> pair(x, y);
    if(this->towers_.count(pair)) {
        return this->towers_.find(pair)->second;
    }
	else {
        return nullptr;
    }
}


//Locates the spawnlocation currently works with only one spawn
void GameState::initSpawnLocation() {
    for(auto i : SpawnAndExit_) {
        if(i.second->isSpawn()) {
            sf::Vector2f location = i.second->getPosition();

            spawnTile_ = i.second;

            if(location.x / SQUARESIZE == 0) {
                spawnlocation_.x = location.x - 96;
                spawnlocation_.y = location.y + 32;  
            }
            else if(location.y / SQUARESIZE == 0) {
                spawnlocation_.x = location.x + 32;
                spawnlocation_.y = location.y - 96;
            }
            else if(location.x / SQUARESIZE == 22) {
                spawnlocation_.x = location.x + 96;
                spawnlocation_.y = location.y + 32;
            }
            else if(location.y / SQUARESIZE == 16) {
                spawnlocation_.x = location.x + 32;
                spawnlocation_.y = location.y + 96;
            }
        } 
    }
}

//Locates the exitlocation currently works with only one exit
void GameState::initExitLocation() {
    for(auto i : SpawnAndExit_) {
        if(i.second->isExit()) {
            sf::Vector2f location = i.second->getPosition();

            exitTile_ = i.second;

            if(location.x / SQUARESIZE == 0) {
                exitlocation_.x = location.x - 96;
                exitlocation_.y = location.y + 32;  
            }
            else if(location.y / SQUARESIZE == 0) {
                exitlocation_.x = location.x + 32;
                exitlocation_.y = location.y - 96;
            }
            else if(location.x / SQUARESIZE == 22) {
                exitlocation_.x = location.x + 96;
                exitlocation_.y = location.y + 32;
            }
            else if(location.y / SQUARESIZE == 16) {
                exitlocation_.x = location.x + 32;
                exitlocation_.y = location.y + 96;
            }
        } 
    }
}

//Find all neighbours of the given Tile. Not diagonals.
std::vector<GameTile*> GameState::getNeighbours(GameTile* current) {
    std::vector<GameTile*> neighbours;
    std::pair<int, int> pair(current->getPosition().x / SQUARESIZE, current->getPosition().y / SQUARESIZE);
    if(pair.first - 1 >= 0 && pair.first - 1 < 23) {
        neighbours.push_back(TileAt(pair.first - 1, pair.second));
    }
    if(pair.first + 1 >= 0 && pair.first + 1 < 23) {
        neighbours.push_back(TileAt(pair.first + 1, pair.second));
    }
    if(pair.second - 1 >= 0 && pair.second - 1 < 17) {
        neighbours.push_back(TileAt(pair.first, pair.second - 1));
    }
    if(pair.second + 1 >= 0 && pair.second + 1 < 17) {
        neighbours.push_back(TileAt(pair.first, pair.second + 1));
    }
    return neighbours;
   

}

void GameState::messageForPlayer(std::string message) {
    this->message_.setString(message);
    this->message_.setFillColor(sf::Color(200, 46, 13));
    message_timer_.restart();
}


//Builds the road using recursion.
void GameState::buildRoad(std::pair<int, int> current, std::pair<int, int> previous) {

    GameTile* currentTile = TileAt(current.first, current.second);
    roadTiles_.push_back(currentTile);
    GameTile* previousTile = TileAt(previous.first, previous.second);

    std::vector<GameTile*> neighbours = getNeighbours(currentTile);
    std::vector<GameTile*>::iterator it = std::find(neighbours.begin(), neighbours.end(), previousTile);
    if (it != neighbours.end()) {
        neighbours.erase(it);
    }
    for (auto i : neighbours) {

        if(!i->isBuildable()) {
            buildRoad(std::pair<int, int>(i->getPosition().x / SQUARESIZE, i->getPosition().y / SQUARESIZE), current);
        }
    }
}

//Initializes the road
void GameState::initRoad() {
    sf::Vector2f location = spawnTile_->getPosition();
    std::pair<int,int> pair(location.x / SQUARESIZE, location.y / SQUARESIZE);

    buildRoad(pair, pair);

}

//Loads the map that is given
void GameState::loadmap() {
    std::fstream File;
    File.open(root_filepath_ + "/src/Maps/" + mapfile_, std::ios::in);
    if(File.is_open()) {
        std::string line;
        while (getline(File, line)) {
            if(!line.empty()) {
                std::string texturename;
                std::string buildable;
                std::string squaresize;
                std::string x;
                std::string y;
                bool buildable_ = false;

                std::stringstream ss(line);

                std::getline(ss, texturename, ';');
                std::getline(ss, buildable, ';');
                std::getline(ss, x, ';');
                std::getline(ss, y, ';');
                std::getline(ss, squaresize, ';');
                std::pair<int, int> pair(std::stoi(x) / SQUARESIZE, std::stoi(y) / SQUARESIZE);
                sf::Vector2f pos(std::stof(x), std::stof(y));
                if(buildable == "1") {
                    buildable_ = true;
                }
                if(texturename == "towerDefense_tile016.png" || texturename == "towerDefense_tile017.png") {
                    this->SpawnAndExit_[pair] = new GameTile(root_filepath_, texturename, pos, false, std::stoi(squaresize));
                }
                else {
                    this->Tiles[pair] = new GameTile(root_filepath_, texturename, pos, buildable_, std::stoi(squaresize));
                }
            }
        }
    }
}

///Osku addition
sf::Vector2f GameState::getSpawn(){
    return spawnlocation_;
}
sf::Vector2f GameState::getExit(){
    return exitlocation_;
}
std::vector<GameTile*> GameState::getRoad(){
    return roadTiles_;
}

void GameState::changeHealth(int health) {
    this->int_health_ = this->int_health_ + health;
    this->health_.setString("hp " + std::to_string(this->int_health_));
}

void GameState::changeMoney(int money) {
    this->int_money_ = this->int_money_ + money;
    this->money_.setString("$ " + std::to_string(this->int_money_));
}

void GameState::changeScore(int score) {
    this->int_score_ = this->int_score_ + score;
    this->score_.setString("score " + std::to_string(this->int_score_));
}

bool GameState::lostGame() {
    if(this->int_health_ <= 0) {
        return true;
    }
    else {
        return false;
    }
}

const bool GameState::getPressTimer()
{
	// Check for press timer
	if (this->pressTimer_.getElapsedTime().asMilliseconds() >= this->pressTimerMax_)
	{
		//std::cout << "PRESSED" << "\n";
		this->pressTimer_.restart();
		return true;
	}
	return false;
}

void GameState::initVariables()
{
	// Get window size
	windowX_ = static_cast<float>(this->window_->getSize().x);
	windowY_ = static_cast<float>(this->window_->getSize().y);
	std::cout << windowX_ << " and " << windowY_ << "\n";
	// Initialize timer and the max value for the timer
	this->timer_ .restart();
	this->timerMax_ = 200;

	// Initialize timer for pressing and the max value for the press timer
	this->pressTimer_.restart();
	pressTimerMax_ = 500;

}

void GameState::updateGameLogic() {
    this->currentwave_.setString("Wave: " + std::to_string(currentRound_));
    this->enemiesleft_.setString("Enemies Left: " + std::to_string(enemiestobespawnedremaining_ + enemies_.size()));

    //checks if rounds is over
    if(enemies_.empty() && enemiestobespawnedremaining_ == 0 && !roundover_flag_ && n_super_tank_<=0 && n_super_plane_<=0) {
        for (auto i : tobedeleted) {
            std::cout << "tobedeleted vector size:" << tobedeleted.size() << "\n";
            std::cout << "enemies list size:" << enemies_.size() << "\n";
            delete i;
        }
        tobedeleted.clear();
        std::cout << "tobedeleted vector size after clear:" << tobedeleted.size() << "\n";

        roundover_flag_ = true;
        currentRound_ += 1;
        this->roundtimer_int_ = 30;
        roundtimer_.restart();
    }

    //Checks if game is over
    if(lostGame()) {
        game_over_ = true;
    }


    sf::Time elapsed = roundtimer_.getElapsedTime();
    //Logic while rounds is over
    if(roundover_flag_) {
        int seconds = elapsed.asSeconds();
        this->roundtimer_text_.setString(std::to_string(roundtimer_int_ - seconds));
        if(seconds >= 30) {
            roundover_flag_ = false;
            //logistic-growth model first number changes the upper limit enemies per round
            enemiestobespawnedremaining_ = 500 / (1 + (200 * pow(M_E, -0.25 * currentRound_)));
            n_super_tank_ = currentRound_-3;
            n_super_plane_ = currentRound_-4;
            this->roundtimer_text_.setString(std::to_string(roundtimer_int_));
        }
    }
    if(!roundover_flag_) {
        this->spawnEnemies();
    }
    
}

void GameState::spawnEnemies() {
    sf::Time elapsed = spawntimer_.getElapsedTime();
    if(elapsed.asSeconds() >= pow(0.91, currentRound_)) {
    if(enemiestobespawnedremaining_ != 0) {
        int enemytype = rand() % 3;
        if(enemytype == 0) {
            this->enemies_.push_back(new Soldier(root_filepath_,spawnlocation_,exitlocation_,&roadTiles_));
            enemiestobespawnedremaining_ -= 1;
            spawntimer_.restart();
        }
        if(enemytype == 1) {
            if(currentRound_ >= 3) {
            this->enemies_.push_back(new Plane(root_filepath_,spawnlocation_,exitlocation_,&roadTiles_,currentRound_));
            enemiestobespawnedremaining_ -= 1;
            spawntimer_.restart();
            }
        }
        if(enemytype == 2) {
            if(currentRound_ >= 2) {
            this->enemies_.push_back(new Tank(root_filepath_,spawnlocation_,exitlocation_,&roadTiles_,currentRound_));
            enemiestobespawnedremaining_ -= 1;
            spawntimer_.restart();
            }
        }
	}
        if(currentRound_ >= 4) {
            sf::Time elapsed = superTankTimer_.getElapsedTime();
			// scale tank spawn interval so it spawns everything in time
            if(n_super_tank_ > 0 && elapsed.asSeconds() > 45.0 / currentRound_)
            {
                this->enemies_.push_back(new SuperTank(root_filepath_,spawnlocation_,exitlocation_,&roadTiles_,currentRound_));
                n_super_tank_ -= 1;
                superTankTimer_.restart();
            }
            
        }
        if(currentRound_ >= 5) {
            sf::Time elapsed = superPlaneTimer_.getElapsedTime();
			// scale tank spawn interval so it spawns everything in time
            if(n_super_plane_ > 0 && elapsed.asSeconds() > 60.0 / currentRound_)
            {
                this->enemies_.push_back(new SuperPlane(root_filepath_,spawnlocation_,exitlocation_,&roadTiles_,currentRound_));
                n_super_plane_ -= 1;
                superPlaneTimer_.restart();
            }
            
        }
    }
}



void GameState::update(const float& dt)
{
	if (this->game_over_) {
		this->quit_ = true;
		SaveToFile();
		return;
	}
	// Update called functions
    this->updateGameLogic();

	this->updateMousePosition();

	this->updateInput(dt);

    sf::Time message_elapsed = message_timer_.getElapsedTime();
    if(message_elapsed.asSeconds() > 3) {
        this->message_.setFillColor(sf::Color::Transparent);
    }

    for(auto i : towerbuttons_) {
        i.second->update(mousePosView);
    }
    for(auto i : roundoverbuttons_) {
        i.second->update(mousePosView);
    }

    mousePosGrid.x = mousePosView.x / SQUARESIZE;
	mousePosGrid.y = mousePosView.y / SQUARESIZE;

    // std::vector<Npc*> tobedeleted;
    for (auto i : enemies_) {
        i->Update(dt);
        //has died to a tower
        if(i->getHitpoints() <= 0) {
            //this is needed because the iterator would stop working if deleted midthrough a loop = crash.
            
            tobedeleted.push_back(i);

            //Spawns 3 basic soldiers when tank is deleted
            if(dynamic_cast<Tank*>(i) != nullptr) {
                for (int c = 0; c < 3; c++){
                    this->enemies_.push_back(new Soldier(root_filepath_,sf::Vector2f(i->getPosition().x + rand() % 61 + (-30),i->getPosition().y + rand() % 61 + (-30)),exitlocation_,&roadTiles_,i->getTileCount()));
                }
                
            }

            if(dynamic_cast<SuperTank*>(i) != nullptr) {
                for (int c = 0; c < 3; c++){
                    this->enemies_.push_back(new Tank(root_filepath_,sf::Vector2f(i->getPosition().x + rand() % 61 + (-30),i->getPosition().y + rand() % 61 + (-30)),exitlocation_,&roadTiles_,currentRound_,i->getTileCount()));
                }
                
            }

            if(dynamic_cast<SuperPlane*>(i) != nullptr) {
                for (int c = 0; c < 3; c++){
                    this->enemies_.push_back(new Plane(root_filepath_,sf::Vector2f(i->getPosition().x + rand() % 61 + (-30),i->getPosition().y + rand() % 61 + (-30)),exitlocation_,&roadTiles_,currentRound_,i->getTileCount()));
                }
                
            }

            changeMoney(i->getWorth()); //Maybe all npcs could have some kind of worth and change money according to that
            changeScore(1);
        }
        //has reached end of the line
        else if(i->hasReachedEnd()) {
            tobedeleted.push_back(i);
            changeHealth(-1);
        }
    }
    for (auto i : tobedeleted) {
        enemies_.remove(i);
    }
    // tobedeleted.clear();


    for (auto i : towers_) {
        i.second->update(dt, enemies_, this->mousePosView);
    }
    this->updateButtons();
    this->updateTargetedTowerInfo();  
}

void GameState::updateTargetedTowerInfo() {
    if(target_ != nullptr) {
        float attack_speed__ = target_->getAttackSpeed();
        float damage__ = target_->getDamage();
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << attack_speed__;
        std::string s = stream.str();
        std::stringstream streamm;
        streamm << std::fixed << std::setprecision(2) << damage__;
        std::string ss = streamm.str();
        damage_text_.setString(ss + " damage");
        attack_speed_text_.setString(s + " attack time");
        tower_level_text.setString(std::to_string(target_->getTowerLevel())+ " LVL");
        int price;
        if(dynamic_cast<BasicTower*>(target_) != nullptr) {
            price = 50;
        }
        if(dynamic_cast<SlowingTower*>(target_) != nullptr) {
            price = 75;
        }
        if(dynamic_cast<BombTower*>(target_) != nullptr) {
            price = 100;
        }
		upgradeprice_ = (price / 2.0) * pow(1.5, target_->getTowerLevel());
        sellprice_ = upgradeprice_ / 2;
        this->targeted_towerbuttons_["Upgrade"]->changeText("Upgrade " + std::to_string(upgradeprice_));
        this->targeted_towerbuttons_["Sell"]->changeText("Sell " + std::to_string(sellprice_));
    }
}

void GameState::buyTower() {
    auto i = TileAt(mousePosGrid.x, mousePosGrid.y);
    std::pair<int,int> pair(mousePosGrid.x, mousePosGrid.y);
    if(i != nullptr) {
        if(i->isBuildable()) {
            if(!towers_.count(pair)) {
                if(bomb_tower_flag_) {
                    this->towers_[pair] = new BombTower(root_filepath_, i->getCenterPosition(), 2, 15, 200, this->soundEffectVolumeLevel_);
                    changeMoney(-100);
                }
                if(basic_tower_flag_) {
                    this->towers_[pair] = new BasicTower(root_filepath_, i->getCenterPosition(), 0.5, 1, 200, this->soundEffectVolumeLevel_);
                    changeMoney(-50);
                }
                if(slowing_tower_flag_) {
                    this->towers_[pair] = new SlowingTower(root_filepath_, i->getCenterPosition(), 1, 0, 200, this->soundEffectVolumeLevel_);
                    changeMoney(-75);
                }
            }
            else {
                messageForPlayer("That spot is taken");
            }
        }
        else {
            messageForPlayer("You can't build there");
        }
        
    }
}

void GameState::updateButtons() {

    if (this->roundoverbuttons_["Skip"]->isPressed())
	    {
		    if (getPressTimer()) 
            {
                if(roundover_flag_) {
                    roundover_flag_ = false;
                    enemiestobespawnedremaining_ = 500 / (1 + (200 * pow(M_E, -0.25 * currentRound_)));
                    n_super_tank_ = currentRound_-3;
                    n_super_plane_ = currentRound_-4;
                }
               
            }
        }
    if (this->towerbuttons_["Basic Tower"]->isPressed())
	{
		if (getPressTimer())                                  
		{
            shoptimer_.restart();
            bomb_tower_flag_ = false;
            slowing_tower_flag_ = false;
            basic_tower_flag_ = false;
            if(int_money_ >= 50) 
            {
            bomb_tower_flag_ = false;
            slowing_tower_flag_ = false;
            basic_tower_flag_ = true;
            }
            else {
                messageForPlayer("Not enough money");
            }
        }
    }
    if (this->towerbuttons_["Bomb Tower"]->isPressed())
	{
		if (getPressTimer())
		{
            shoptimer_.restart();
            bomb_tower_flag_ = false;
            slowing_tower_flag_ = false;
            basic_tower_flag_ = false;
            if(int_money_ >= 100) 
            {
            bomb_tower_flag_ = true;
            slowing_tower_flag_ = false;
            basic_tower_flag_ = false;
            }
            else {
                messageForPlayer("Not enough money");
            }
        }
    }
    if (this->towerbuttons_["Sticky Tower"]->isPressed())
	{
		if (getPressTimer())
		{
            shoptimer_.restart();
            bomb_tower_flag_ = false;
            slowing_tower_flag_ = false;
            basic_tower_flag_ = false;
            if(int_money_ >= 75) 
            {
            bomb_tower_flag_ = false;
            slowing_tower_flag_ = true;
            basic_tower_flag_ = false;
            }
            else {
                messageForPlayer("Not enough money");
            }
        }
    }
    if(target_ != nullptr) {
        if (this->targeted_towerbuttons_["Upgrade"]->isPressed())
	    {
		    if (getPressTimer())
		    {
                if(int_money_ >= upgradeprice_) {
                    changeMoney(-upgradeprice_);
                    target_->upgrade();
                }
                else {
                    messageForPlayer("Not enough money");
                }
            }

        }
        if (this->targeted_towerbuttons_["Sell"]->isPressed())
	    {
		    if (getPressTimer())
		    {
                changeMoney(sellprice_);
                std::pair<int, int> pair(target_->getPosition().x / SQUARESIZE, target_->getPosition().y / SQUARESIZE);
                this->towers_.erase(pair);
                delete target_;
                target_ = nullptr;
            }

        }
    }
	this->textbox_->update(mousePosView);

    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        bomb_tower_flag_ = false;
        slowing_tower_flag_ = false;
        basic_tower_flag_ = false;
        target_ = nullptr;
    }

    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Time elapsed__ = shoptimer_.getElapsedTime();
        if(elapsed__.asSeconds() > 0.5) {
            if(bomb_tower_flag_ || slowing_tower_flag_ || basic_tower_flag_) {
                this->buyTower();
                bomb_tower_flag_ = false;
                slowing_tower_flag_ = false;
                basic_tower_flag_ = false;
                shoptimer_.restart();
            }
        }
        auto tower = towerAt(mousePosGrid.x, mousePosGrid.y);
        if(tower != nullptr) {
            target_ = tower;
        }
    }
    for(auto i : targeted_towerbuttons_) {
        i.second->update(mousePosView);
    }
}

void GameState::renderSoonToBeTower(sf::RenderTarget* target) {
    auto i = TileAt(mousePosGrid.x, mousePosGrid.y);
    if(basic_tower_flag_) {
        if(i != nullptr) {
            basic_ghost_.setPosition(i->getCenterPosition());
            target->draw(basic_ghost_);
        }
    }
    if(bomb_tower_flag_) {
        if(i != nullptr) {
            bomb_ghost_.setPosition(i->getCenterPosition());
            target->draw(bomb_ghost_);
        }
    }
    if(slowing_tower_flag_) {
        if(i != nullptr) {
            slowing_ghost_.setPosition(i->getCenterPosition());
            target->draw(slowing_ghost_);
        }
    }

}

void GameState::initGhostTowers() {

    if(!this->bomb_texture_ghost_.loadFromFile(root_filepath_ + "/src/Textures/NPCTextures/Towers/" + "towerDefense_tile206.png"))
	{
		throw "ERROR::GAMETILE::FAILED_TO_LOAD_gunghost_TEXTURE";
	}

    if(!this->slowing_texture_ghost_.loadFromFile(root_filepath_ + "/src/Textures/NPCTextures/Towers/" + "towerDefense_tile249.png"))
	{
		throw "ERROR::GAMETILE::FAILED_TO_LOAD_gunghost_TEXTURE";
	}

    if(!this->basic_texture_ghost_.loadFromFile(root_filepath_ + "/src/Textures/NPCTextures/Towers/" + "towerDefense_tile250.png"))
	{
		throw "ERROR::GAMETILE::FAILED_TO_LOAD_gunghost_TEXTURE";
	}
    this->bomb_ghost_.setTexture(this->bomb_texture_ghost_);
    this->basic_ghost_.setTexture(this->basic_texture_ghost_);
    this->slowing_ghost_.setTexture(this->slowing_texture_ghost_);

    this->bomb_ghost_.setOrigin(32, 32);
    this->basic_ghost_.setOrigin(32, 32);
    this->slowing_ghost_.setOrigin(32, 32);

    this->bomb_ghost_.setColor(sf::Color(30, 30, 30, 30));
    this->basic_ghost_.setColor(sf::Color(30, 30, 30, 30));
    this->slowing_ghost_.setColor(sf::Color(30, 30, 30, 30));
}

void GameState::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window_;
	}

    for (auto i : Tiles) {
		i.second->render(target);
	}

    this->renderSoonToBeTower(target);

    for (auto i : enemies_) {
        i->Render(target);
    }

    
    for(auto i : towers_) {
        i.second->render(target);
    }


    target->draw(this->rect_menu_);

    for(auto i : towerbuttons_) {
        i.second->render(target);
    }

	this->textbox_->render(target);

    target->draw(this->health_);
    target->draw(this->money_);
    target->draw(this->score_);

    target->draw(this->message_);
    if(target_ != nullptr) {
        target->draw(damage_text_);
        target->draw(attack_speed_text_);
        target->draw(tower_level_text);
        for(auto i : targeted_towerbuttons_) {
            i.second->render(target);
        }
    }

    if(roundover_flag_) {
        target->draw(roundtimer_text_);
        for(auto i : roundoverbuttons_) {
            i.second->render(target);
        }
    }
    target->draw(currentwave_);
    if(!roundover_flag_) {
        target->draw(enemiesleft_);
    }
    if(game_over_) {
        target->draw(you_have_lost_);
    }
}

void GameState::updateInput(const float& dt)
{
	this->checkForQuit();
	
}

void GameState::endState()
{
	std::cout << "Ending game state!" << "\n";
}
