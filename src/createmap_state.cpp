#include "createmap_state.hpp"

//How many squares x * y
int SQUARESX = 22;
int SQUARESY = 16;

//SquareSize. Textures are 64x64 so they look the best when the length is exactly 64
int SQUARELENGTH = 64;

//Constructor of Createmap class
Createmap::Createmap(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Font* font, sf::Event* Event) : State(window, supportedKeys, root_filepath, states)
{
	this->Event_ = Event;
	this->font_ = font;
	this->root_filepath_ = root_filepath;
	this->initKeyBinds();
	this->initTiles();
	this->initTextures();
	this->initButtons();
	this->initVariables();
	this->initSoundEffect();
}

//Destructor of Createmap class
Createmap::~Createmap()
{
	for(auto i : Tiles) {
		delete i.second;
	}
	for(auto i : SpawnAndExit_) {
		delete i.second;
	}
	for(auto i : mapcreateButtons_) {
		delete i.second;
	}
}

// Initialized click sound effect
void Createmap::initSoundEffect()
{
    if (!this->buffer.loadFromFile(root_filepath_ + "/src/Audio/SoundEffects/Click.wav"))
    {
        throw "ERROR::TOWER::UNABLE_TO_LOAD_SOUND_EFFECT_FILE";
    }

    this->clickSound_.setBuffer(this->buffer);
}

// Puts sand tiles everywhere for starters
void Createmap::initTiles() {
	for (int x = 0; x < SQUARESX + 1; x++) {
		for (int y = 0; y < SQUARESY + 1; y++) {
			sf::Vector2f pos(x * SQUARELENGTH, y * SQUARELENGTH);
			std::pair<int, int> pair;
			pair.first = x;
			pair.second = y;
			this->Tiles[pair] = new GameTile(root_filepath_, "snow.png", pos, true, SQUARELENGTH);
		}
	}
}

//Initializes time and window variables
void Createmap::initVariables()
{
	// Initialize timer and the max value for the timer
	this->pressTimer_ .restart();
	this->pressTimerMax_ = 500;

	// Get window size
	windowX_ = static_cast<float>(this->window_->getSize().x);
	windowY_ = static_cast<float>(this->window_->getSize().y);
	std::cout << windowX_ << " and " << windowY_ << "\n";
}

//Initializes buttons
void Createmap::initButtons()
{
	// Add buttons
	this->mapcreateButtons_["SAVE"] = new Button(1500, 900, 150, 150, this->font_, "Save", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 40, true);

	this->textbox_ = new Textbox(1500, 700, 400, 150, this->font_, "Mapname", sf::Color(100, 100, 100, 200), sf::Color(150, 150, 150, 255), sf::Color(20, 20, 20, 200), 30, true, this->Event_);
}

//Gets the tile at the given gridposition
GameTile* Createmap::TileAt(int x, int y) {
	std::pair<int, int> pair(x, y);
	return this->Tiles.find(pair)->second;
}

//Replaces the spawn or exit texture with the given texture
void Createmap::ReplaceSpawnOrExit(int x, int y, std::string texturename) {
	sf::Vector2f pos(x * SQUARELENGTH, y * SQUARELENGTH);
	std::pair<int, int> pair;
	pair.first = x;
	pair.second = y;
	if(texturename == "") {
		delete this->SpawnAndExit_[pair];
		SpawnAndExit_.erase(pair);
	}
	else {
		if(SpawnAndExit_.find(pair) != SpawnAndExit_.end()) {
			delete this->SpawnAndExit_[pair];
			SpawnAndExit_.erase(pair);
		}
		this->SpawnAndExit_[pair] = new GameTile(root_filepath_, texturename, pos, false, SQUARELENGTH);
	}
}

//Round-robins spawn and exit textures at the given gridposition
void Createmap::NextSpawnOrExit(int x, int y) {
	std::pair<int,int> pair(x, y);
	if (x <= SQUARESX && x >= 0 && y <= SQUARESY && y >= 0) {
		std::string newTexture;
		if(SpawnAndExit_.find(pair) != SpawnAndExit_.end()) {
			GameTile * current = SpawnAndExit_.find(pair)->second;
			std::string texture = current->CurrentTexture();
			std::pair<bool, std::string> pair(false, texture);

			std::vector<std::pair<bool, std::string>>::iterator itr = std::find(spawnandexittextures_.begin(), spawnandexittextures_.end(), pair);
			if (std::distance(spawnandexittextures_.begin(), itr) == spawnandexittextures_.size() - 1) {
				newTexture = spawnandexittextures_.at(0).second;
			}
			else {
				newTexture = spawnandexittextures_.at(std::distance(spawnandexittextures_.begin(), itr) + 1).second;
			}
		}
		else {
			newTexture = spawnandexittextures_[0].second;
		}
		ReplaceSpawnOrExit(x, y, newTexture);

	}
}

//Round-robins textures at the given gridposition
void Createmap::NextTextureAt(int x, int y) {
	if (x <= SQUARESX && x >= 0 && y <= SQUARESY && y >= 0) {
		std::string newTexture;
		bool newbuildable;
		GameTile* Tile = TileAt(x, y);
		std::string current = Tile->CurrentTexture();
		bool buildable = Tile->isBuildable();
		std::pair<bool, std::string> pair(buildable, current);
		std::vector<std::pair<bool, std::string>>::iterator itr = std::find(textures_.begin(), textures_.end(), pair);
		if (std::distance(textures_.begin(), itr) == textures_.size() - 1) {
			newTexture = textures_.at(0).second;
			newbuildable = textures_.at(0).first;
		}
		else {
			newTexture = textures_.at(std::distance(textures_.begin(), itr) + 1).second;
			newbuildable = textures_.at(std::distance(textures_.begin(), itr) + 1).first;
		}
		ReplacePosition(x, y, newTexture, newbuildable);
	}
}

//Inits all wanted nonRoad and Road textures
void Createmap::initTextures() {
	//You can add more if you want

	//nonRoads
	std::pair<bool, std::string> Grass(true, "towerDefense_tile231.png");
	textures_.push_back(Grass);
	std::pair<bool, std::string> Sand(true, "towerDefense_tile241.png");
	textures_.push_back(Sand);
	std::pair<bool, std::string> Snow(true, "snow.png");
	textures_.push_back(Snow);

	//Roads
	std::pair<bool, std::string> Grey(false, "towerDefense_tile172.png");
	textures_.push_back(Grey);
	std::pair<bool, std::string> Mud(false, "towerDefense_tile236.png");
	textures_.push_back(Mud);

	//Water
	std::pair<bool, std::string> Water(false, "water.png");
	textures_.push_back(Water);

	//Sand rocks and stones
	/*std::pair<bool, std::string> Rock(false, "towerDefense_snowyStones.png");
	textures_.push_back(Rock);
	std::pair<bool, std::string> Tree(false, "towerDefense_snowyTree.png");
	textures_.push_back(Tree);
	std::pair<bool, std::string> Bush(false, "towerDefense_snowyTree2.png");
	textures_.push_back(Bush);
	std::pair<bool, std::string> Presents(false, "towerDefense_snowyChristmas.png");
	textures_.push_back(Presents);*/
	std::pair<bool, std::string> SnowyTree(false, "snowy_tree.png");
	textures_.push_back(SnowyTree);
	std::pair<bool, std::string> SnowyForrest(false, "snowy_forrest.png");
	textures_.push_back(SnowyForrest);
	std::pair<bool, std::string> SnowyGreens(false, "snowy_greens.png");
	textures_.push_back(SnowyGreens);
	std::pair<bool, std::string> SnowyBush(false, "snowy_bush.png");
	textures_.push_back(SnowyBush);
	std::pair<bool, std::string> SnowyStones(false, "snowy_stones.png");
	textures_.push_back(SnowyStones);
	std::pair<bool, std::string> SnowyStone(false, "snowy_stone.png");
	textures_.push_back(SnowyStone);
	std::pair<bool, std::string> SnowyCottage(false, "snowy_cottage.png");
	textures_.push_back(SnowyCottage);
	std::pair<bool, std::string> SnowyCottageRotated(false, "snowy_cottage_Rotated.png");
	textures_.push_back(SnowyCottageRotated);


	//SpawnAndExit
	std::pair<bool, std::string> Spawn(false, "towerDefense_tile016.png");
	std::pair<bool, std::string> Exit(false, "towerDefense_tile017.png");
	std::pair<bool, std::string> Nothing(false, "");
	spawnandexittextures_.push_back(Spawn);
	spawnandexittextures_.push_back(Exit);
	spawnandexittextures_.push_back(Nothing);
}

//Initializes the keybinds
void Createmap::initKeyBinds() {
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

//Replaces the texture of tile with the given texture
void Createmap::ReplacePosition(int x, int y, std::string texturename, bool buildable) {
	sf::Vector2f pos(x * SQUARELENGTH, y * SQUARELENGTH);
	std::pair<int, int> pair;
	pair.first = x;
	pair.second = y;
	delete this->Tiles[pair];
	this->Tiles[pair] = new GameTile(root_filepath_, texturename, pos, buildable, SQUARELENGTH);
}

//Saves all Tiles to a text file
void Createmap::SaveToFile(std::string mapname) {
	std::fstream File;
	File.open(root_filepath_ + "/src/Maps/" + mapname + ".txt", std::ios::out);
	if(File.is_open()) {
		for(auto i : Tiles) {
			File << i.second->CurrentTexture() << ";" << i.second->isBuildable() << ";" << i.second->getPosition().x << ";" << i.second->getPosition().y << ";" << i.second->getSquareSize() << "\n";
		}
		for(auto i : SpawnAndExit_) {
			File << i.second->CurrentTexture() << ";" << i.second->isBuildable() << ";" << i.second->getPosition().x << ";" << i.second->getPosition().y << ";" << i.second->getSquareSize() << "\n";
		}
		File.close();
	}
}


void Createmap::update(const float& dt)
{
	// Update called functions
	
	this->updateMousePosition();
	this->updateButtons();

	//calculates the current gridposition the mouse is on
	mousePosGrid.x = mousePosView.x / SQUARELENGTH;
	mousePosGrid.y = mousePosView.y / SQUARELENGTH;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		sf::Time elapsed = clock.getElapsedTime();
		if(0.10 < elapsed.asSeconds()) {
			clock.restart();
			NextTextureAt(mousePosGrid.x, mousePosGrid.y);
		}
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		sf::Time elapsed = clock.getElapsedTime();
		if(0.10 < elapsed.asSeconds()) {
			clock.restart();
			NextSpawnOrExit(mousePosGrid.x, mousePosGrid.y);
		}
	}
	
	this->updateInput(dt);
	

}

const bool Createmap::getPressTimer()
{
	// Check for timer
	if (this->pressTimer_.getElapsedTime().asMilliseconds() >= this->pressTimerMax_)
	{
		std::cout << "PRESSED" << "\n";
		this->pressTimer_.restart();
		return true;
	}
	return false;
}

void Createmap::updateButtons()
{
	// Updates all the buttons in the state and handels their functionality
	for (auto &it : this->mapcreateButtons_)
	{
		it.second->update(this->mousePosView);
	}

	// Quit the game
	if (this->mapcreateButtons_["SAVE"]->isPressed()) {
		this->clickSound_.play();
		this->SaveToFile(this->textbox_->GetText());
	}
	this->textbox_->update(mousePosView);
	
}

void Createmap::render(sf::RenderTarget* target)
{
	if (!target)
	{
		target = this->window_;
	}
	for (auto i : Tiles) {
		i.second->render(target);
	}
	for (auto i : SpawnAndExit_) {
		i.second->render(target);
	}
	this->renderButtons(target);
	this->textbox_->render(target);
}

void Createmap::renderButtons(sf::RenderTarget* target)
{
	// Render buttons
	for (auto &it : this->mapcreateButtons_)
	{
		it.second->render(target);
	}
}

void Createmap::updateInput(const float& dt)
{
	this->checkForQuit();
	
}

void Createmap::endState()
{
	std::cout << "Ending game state!" << "\n";
}
