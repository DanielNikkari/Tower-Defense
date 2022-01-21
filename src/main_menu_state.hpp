#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "gaming_state.hpp"
#include "gaming_state.cpp"
#include "button.hpp"
#include "button.cpp"
#include "highscore_state.hpp"
#include "highscore_state.cpp"
#include "createmap_state.hpp"
#include "createmap_state.cpp"
#include "mapselector_state.hpp"
#include "mapselector_state.cpp"

class MainMenuState : public State
{
public:
	/// Constructor and destructor
	MainMenuState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Event *Event);
	virtual ~MainMenuState();

	/// Functions
	int getSFXVolume();
	virtual void updateInput(const float& dt);
	virtual void endState();
	virtual void update(const float& dt);
	virtual void render(sf::RenderTarget* target = nullptr);
	void updateButtons();
	void renderButtons(sf::RenderTarget* target = nullptr);

	/// Main menu animation
	void updateAnimation(const float& dt);
	


private:
	/// Variables
	sf::RenderWindow* window_;
	std::string root_filepath_;

	/// Background variables
	sf::Texture backgroundTexture_;
	sf::RectangleShape background_;
	sf::RectangleShape backgroundText_;
	sf::RectangleShape backgroundShader_;
	sf::Text backgroundTextUsingFont_;


	sf::Font font_;
	sf::Music backgroundMusic_;

	std::map<std::string, Button*> buttons_;
	float windowX_;
	float windowY_;

	sf::Event *Event_;

	/// Rocket animation
	sf::Texture rocketTexture_;
	sf::Texture rocketExhaustTX_;
	sf::Sprite rocket_;
	sf::Sprite rocketExhaust_;
	bool passState_;
	float rocketMoveSpeed_;
	sf::Clock timer_;
	sf::Int32 timerMax_;

	/// Press timer
	sf::Clock pressTimer_;
	sf::Int32 pressTimerMax_;

	/// Volume settings
	int volumeLevel_;
	int soundEffectVolumeLevel_;
	sf::Text volumeLevelText_;
	sf::Text soundEffectvolumeLevelText_;
	sf::SoundBuffer buffer;
    sf::Sound clickSound_;

	/// Functions
	void initVariables();
	void initBackground();
	void initKeyBinds();
	void initFonts();
	void initButtons();
	void initMusic();
	void initBGAnimation(); // For background animation

	/// Timer function
	const bool getTimer();

	/// Button press timer
	const bool getPressTimer();
};

#endif