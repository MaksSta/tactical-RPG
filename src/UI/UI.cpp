#include "UI.h"

UI::UI(sf::Vector2f _starts_at) 
:
	btnsStartPos{_starts_at + sf::Vector2f{20, 100} },
	ability_desc{_starts_at + sf::Vector2f{0, 0}, sf::Vector2f{300, 80}, TextBox::Alignment::start, "Chosen ability"},
	ability_dmg{_starts_at + sf::Vector2f{400, 0}, sf::Vector2f{120, 80}, TextBox::Alignment::start, "Damage"},
	textfieldSelectedCharacter{{880, 125}, {680, 80}, TextBox::Alignment::start, "Current character","[Click on any character right mouse button]"},
	box_action_points{{880, 325}, {120, 80}, TextBox::Alignment::start, "Actions points", "", sf::Color::Yellow, 34}
{
}

void UI::addNewButton(sf::Vector2f pos, Button_data& data, Attack& ability)
{
	auto callType = ability.getCallType();
	Button::ActivationType buttonType;

	if (callType == Attack::CallType::targetable)
		buttonType = Button::ActivationType::selectable;
	else if (callType == Attack::CallType::instant)
		buttonType = Button::ActivationType::clickable;

	button.push_back(
		 std::make_unique<Button>(buttonType, &ability, btnsStartPos + pos, data.img_file_path, data.desc)
	);

	// tekst z ilością punktów akcji jaką pobiera umiejętność pod przyciskiem
	text_action_cost.push_back(
		std::make_unique<Textfield>(btnsStartPos + pos + sf::Vector2f{29, 83}, std::to_string(ability.getAP()), 28, sf::Color::Yellow)
	);
}

void UI::addNewButton(sf::Vector2f pos, Button_data& data, Button::Action _action)
{
	button.push_back(
		std::make_unique<Button>(Button::ActivationType::clickable, _action, btnsStartPos + pos, data.img_file_path, data.desc)
	);

}

void UI::updateButtons(sf::Vector2f m_pos, float deltaTime)
{
	hoveredBtn = nullptr;
	
	// odświeżenie statusów przycisków na podstawie tego czy myszka je najeżdża
	for (auto & b: button) 
	{
		// jeśli najechano myszką na jakieś pole, zapisuje je do wskaźnika
		if (b->getGlobalBounds().contains(m_pos))
		{
			hoveredBtn = b.get();
			b->state = Button::hovered;
		} 
		else	
		{	
			b->state = Button::inactive;
		}
	}

	if (simulatedHoveredBtn)
		simulatedHoveredBtn->state = Button::simulated_hovered;

	if (selectedBtn)
		selectedBtn->state = Button::selected;

	for (auto & b: button) 
		b->Update(deltaTime);

	// ustalenie do którego przycisku w tym momencie należy wyświetlić opis akcji jaką wykonuje
	anyButtonActive = nullptr;

	if (hoveredBtn)
		anyButtonActive = hoveredBtn;
	else if (simulatedHoveredBtn)
		anyButtonActive = simulatedHoveredBtn;
	else if (selectedBtn)
		anyButtonActive = selectedBtn;

	if (anyButtonActive)
	{
		// wyświetlenie opisu wskazywanej myszką bądź wybranej umiejętności
		ability_desc.setString(anyButtonActive->getDesc());

		if (anyButtonActive->getAction() == Button::Action::attack)
		{
			std::stringstream ss;
			ss << anyButtonActive->getAbility()->get_min_dmg() << " - " << anyButtonActive->getAbility()->get_max_dmg();
			// wyświetlenie obrażeń zadawanych przez tą umiejętność
			ability_dmg.setString(ss.str());
		}
	}
	else
	{
		ability_desc.setString("");
		ability_dmg.setString("");
	}

	//  ramka dookoła domyślnie uruchamianego przycisku
	if (autoselectedBtn)
	{
		// utworzenie ramki (technicznie jest to tło pod spodem)
		const short border_size = 4;
		selectedBtnBorder.setFillColor(sf::Color{240, 25, 25});
		selectedBtnBorder.setSize({	autoselectedBtn->getGlobalBounds().width + border_size * 2,
									autoselectedBtn->getGlobalBounds().height + border_size * 2});
		selectedBtnBorder.setPosition({	autoselectedBtn->getPosition().x - border_size,
										autoselectedBtn->getPosition().y - border_size});
	}
}

void UI::selectButton(Button* _selectedBtn)
{
	if (_selectedBtn->activationType != Button::ActivationType::selectable)
	{
		// TODO dodać wyjatęk
	}

	selectedBtn = _selectedBtn;
}

void UI::autoselectButton(Button* _autoselectedBtn)
{
	autoselectedBtn = _autoselectedBtn;
}

void UI::unselectButtons()
{
	selectedBtn = nullptr;
}

Button* UI::getHoveredBtn()
{
	return hoveredBtn;
}

Button* UI::getSelectedBtn()
{
	return selectedBtn;
}

Button* UI::getAutoselectedBtn()
{
	return autoselectedBtn;
}

void UI::simulateHover(Button* button)
{
	simulatedHoveredBtn = button;
}

void UI::cancelSimulatingHover()
{
	simulatedHoveredBtn = nullptr;
}

void UI::destroyButtons()
{
	button.clear();
	text_action_cost.clear();

	selectedBtn = nullptr;
	autoselectedBtn = nullptr;
}

void UI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (autoselectedBtn) 
		target.draw(selectedBtnBorder);

	for (auto & b: button)
		target.draw(*b);

	for (auto & t: text_action_cost)
		target.draw(*t);

	// pole z nazwą zaznaczonej postaci
	target.draw(textfieldSelectedCharacter);

	target.draw(box_action_points);

	target.draw(ability_desc);

	if (anyButtonActive)
	{
		// pole z widełkami obrażeń
		if (anyButtonActive->getAction() == Button::Action::attack)
			target.draw(ability_dmg);
	}
}