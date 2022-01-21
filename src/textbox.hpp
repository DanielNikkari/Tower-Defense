/// Header for the button class

#ifndef TEXTBOX_H
#define TEXTBOX_H

/// Add independent includers
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <sstream>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>




/// Textbox that allows text input
class Textbox
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
	/// @param Event Event
	///
	/// @note (X,Y)-coordinates begin from the top-left corner (0,0)
	Textbox(float x, float y, float width, float height, sf::Font* font, std::string text,
	 sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor, int text_size, bool box, sf::Event *Event);

	/// @brief Destructor
	~Textbox();


	// Accessor
	
	/// @brief Accessor for determining whether a button is pressed
	const bool isPressed() const;

    std::string GetText();
	// Functions

	/// @brief Update button state based on mouse action
	///
	/// @param mousePos Mouse position on screen
	void update(const sf::Vector2f mousePos);

	/// @brief Render button on target (window)
	///
	/// @param target Rendering target, i.e. game window
	void render(sf::RenderTarget* target);



private:
	// Variables
	
	/// Box font
	sf::Font* font_;
	/// Box (rectangle) shape
	sf::RectangleShape shape_;
	/// Box text
	sf::Text buttonText_;
	/// Box font size
	int text_size_;
	/// Whether to draw box or not
	bool boxBoolean_;

	/// Box color when idle
	sf::Color idleColor_;
	/// Box color when hovered over
	sf::Color hoverColor_;
	/// Box color when pressed
	sf::Color activeColor_;

	/// Box text
    std::string text_;

	/// Timer for writing
    sf::Clock clock_;


	/// Event
    sf::Event *Event_;
	
	/// Box state between idle (0), hover (1) and active (2)
	short unsigned buttonState_;

};

#endif
