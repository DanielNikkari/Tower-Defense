/// Header for the button class

#ifndef BUTTON_H
#define BUTTON_H

/// Add independent includers
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

enum button_states{BUTTON_IDLE = 0, BUTTON_HOVER, BUTTON_ACTIVE};


/// Button with visual parameters to initiate  some action
class Button
{
public:
	// Constructor and destructor

	/// @brief Constructor
	///
	/// @param x X-coordinate of the button
	/// @param y Y-coordinate of the button
	/// @param width Width of the button
	/// @param height Height of the button
	/// @param font Font of the button
	/// @param text Text of the button
	/// @param idleColor Color of the button while idle
	/// @param hoverColor Color of the button while hovering over
	/// @param activeColor Color of the button when pressed
	/// @param text_size Text size of the button
	/// @param box Whether to include a box around the button
	///
	/// @note (X,Y)-coordinates begin from the top-left corner (0,0)
	Button(float x, float y, float width, float height, sf::Font* font, std::string text,
	 sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor, int text_size, bool box);

	/// @brief Destructor
	~Button();


	// Accessor
	
	/// @brief Accessor for determining whether a button is pressed
	const bool isPressed() const;


	// Functions

	/// @brief Update button state based on mouse action
	///
	/// @param mousePos Mouse position on screen
	void update(const sf::Vector2f mousePos);

	/// @brief Render button on target (window)
	///
	/// @param target Rendering target, i.e. game window
	void render(sf::RenderTarget* target);

	void changeText(std::string text);



private:
	// Variables
	
	/// Button font
	sf::Font* font_;
	/// Button (rectangle) shape
	sf::RectangleShape shape_;
	/// Button text
	sf::Text buttonText_;
	/// Button font size
	int text_size_;
	/// Whether to draw box or not
	bool boxBoolean_;


	// Color variables
	
	/// Button color when idle
	sf::Color idleColor_;
	/// Button color when hovered over
	sf::Color hoverColor_;
	/// Button color when pressed
	sf::Color activeColor_;


	// Button state
	
	/// Button state between idle (0), hover (1) and active (2)
	short unsigned buttonState_;

};

#endif
