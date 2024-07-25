#include "Camera.h"

Camera::Camera(sf::Vector2f size, sf::FloatRect viewport)
{
	setSize(size);
	setViewport(viewport);
}

void Camera::set_map_size(sf::Vector2f size)
{
	map_size = size;
}

void Camera::setDstPos(sf::Vector2f pos)
{
	dstPos = pos;
}

void Camera::moveByPixels(int x, int y)
{
	dstPos.x += x;
	dstPos.y += y;
}

void Camera::moveCameraByFields(int x, int y)
{
	moveByPixels(x * tile_size, y * tile_size);
}

void Camera::update(float delta)
{
	// zablokowanie pozycji dstPos na do granic całej mapy
	if (dstPos.x < getSize().x / 2)
		dstPos.x = getSize().x / 2;
	else if (dstPos.x > map_size.x - getSize().x / 2)
		dstPos.x = map_size.x - getSize().x / 2 ;
	if (dstPos.y < getSize().y / 2)
		dstPos.y = getSize().y / 2;
	else if (dstPos.y > map_size.y - getSize().y / 2)
		dstPos.y = map_size.y - getSize().y / 2;

	// dopóki kamera nie wskazuje na miejsce decolowe wykonuj przesunięcie
	if (getCenter() != dstPos) {

		float speedi = 650.0f;

		// wyliczenie przesunięcia między obecną pozycją a docelową
		sf::Vector2f offset;
		offset =  dstPos - getCenter();

		float mx = std::min(delta * speedi, std::abs(offset.x)) * (offset.x > 0 ? 1 : -1 );
		float my = std::min(delta * speedi, std::abs(offset.y)) * (offset.y > 0 ? 1 : -1 );

		move(mx, my);
	}
}