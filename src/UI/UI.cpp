#include "UI.h"

UI::UI(sf::Vector2f _starts_at) 
:
	btnsStartPos{_starts_at + sf::Vector2f{20, 100} },
	ability_desc{_starts_at + sf::Vector2f{0, 0}, sf::Vector2f{300, 80}, TextBox::Alignment::start, L"Chosen ability"},
	ability_dmg{_starts_at + sf::Vector2f{400, 0}, sf::Vector2f{120, 80}, TextBox::Alignment::start, L"Damage"},
	textfieldSelectedCharacter{{880, 125}, {680, 80}, TextBox::Alignment::start, L"Current character",L"[Click on any character right mouse button]"}
{
}

void UI::addNewButton(sf::Vector2f pos, Button_data& data, Attack* ability)
{
	button.push_back(
		 std::make_unique<Button>(ability, btnsStartPos + pos, data.img_file_path, data.desc)
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
	Button* anyButtonActive = nullptr;

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

		std::stringstream ss;
		ss << anyButtonActive->getAbility()->get_min_dmg() << " - " << anyButtonActive->getAbility()->get_max_dmg();
		// wyświetlenie obrażeń zadawanych przez tą umiejętność
		ability_dmg.setString(ss.str());
	}
	else
	{
		ability_desc.setString("");
		ability_dmg.setString("");
	}

	//  wyświetla dookoła domyślnie uruchamianego przycisku
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

void UI::mouseClickedEvents(sf::Mouse::Button mousebutton, sf::Vector2f m_pos) 
{
	for (auto & b: button) {
		if (b->getGlobalBounds().contains(m_pos)) {
			// zaznaczenie klikniętego przycisku
			if (mousebutton == sf::Mouse::Button::Left)
				selectButton(b.get());
			// zaznaczenie przycisku jako domyślny wybór
			if (mousebutton == sf::Mouse::Button::Right)
				autoselectButton(b.get());
		}
	}
}

void UI::selectButton(Button* _selectedBtn)
{
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

	selectedBtn = nullptr;
	autoselectedBtn = nullptr;
}

void UI::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (autoselectedBtn) 
		target.draw(selectedBtnBorder);

	for (auto & b: button)
		target.draw(*b);

	// pole z nazwą zaznaczonej postaci
	target.draw(textfieldSelectedCharacter);

	target.draw(ability_desc);
	target.draw(ability_dmg);
}