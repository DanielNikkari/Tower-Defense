#ifndef MAPSELECTOR_H
#define MAPSELECTOR_H

#include "state.hpp"
#include "button.hpp"
#include <filesystem>

/// State that shows all current maps in a Maps folder that can be selected
class Mapselector : public State
{
public:

	/// @brief Constructor
	///
	/// @param window Window object where objects are drawn
	/// @param supportedKeys Keys that are supported
	/// @param root_filepath Root filepath
	/// @param states Stack containing all of the current states
	/// @param font Font used for texts
	/// @param Event Event
	/// @param volumeLevel Volume level
	/// @param soundEffectVolumeLevel SFX volume level
	Mapselector(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::string root_filepath, std::stack<State*>* states, sf::Font *font, sf::Event* Event, int volumeLevel, int soundEffectVolumeLevel);

	/// @brief Destructor
	virtual ~Mapselector();

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

	/// Opens the Maps folder and reads all the files in there
	void openFolder();

	/// Initializes some of the needed variables
	void initVariables();

	/// Initializes sound effects
	void initSoundEffect();

	/// Gets press timer
	const bool getPressTimer();

private:

	/// Root filepath
	std::string root_filepath_;

	/// Font
	sf::Font *font_;

	/// Window's x length
	float windowX_;

	/// Window's y length
	float windowY_;

	/// Background color
	sf::RectangleShape backgroundColor_;

	/// Limit to scrolling up
	int scrollUpperBoundary_;

	/// Limit to scrolling down
	int scrollLowerBoundary_;

	/// Volume level
	int volumeLevel_;

	/// SFX volume level
	int soundEffectVolumeLevel_;

	// Click sound effect
	sf::SoundBuffer buffer;
    sf::Sound clickSound_;

	/// Press timer
	sf::Clock pressTimer_;

	/// Press timer max
	sf::Int32 pressTimerMax_;

	/// Timer
	sf::Clock timer_;

	/// Timer max
	sf::Int32 timerMax_;

	/// All of the buttons
	std::map<std::string, Button*> buttons_;

	/// Event
	sf::Event *Event_;

	/// Clock timer
	sf::Clock clock_;

	/// Initializes keybinds
	void initKeyBinds();

};

#endif