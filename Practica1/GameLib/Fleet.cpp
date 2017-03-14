#include "Fleet.h"

Fleet::Fleet(Faction _fleetFaction, sf::Texture &texture, Grid &_grid) : grid(_grid)
{
	fleetFaction = _fleetFaction;
	shipTexture = texture;
	ships.push_back(Ship(sf::Vector2i(0, 0), BIG, fleetFaction, shipTexture));

}

void Fleet::PlaceFleet(sf::RenderWindow &window, sf::Event &evento,sf::Mouse &mouseEvent)
{
	while (window.pollEvent(evento)) {
		sf::Vector2i relativeMousePosition = sf::Vector2i(int(mouseEvent.getPosition(window).x / CELL_SIZE) * CELL_SIZE,
														  int(mouseEvent.getPosition(window).y / CELL_SIZE) * CELL_SIZE);
		if (shipCount < MAX_SHIPS) {
			if (ships[shipCount].GetPlaced()) {
				if (shipCount == 0) shipType--;
				else if (shipCount == 2) shipType--;
				Ship newShip(ships[shipCount].GetPosition(), (ShipType)shipType, fleetFaction, shipTexture);
				ships.push_back(newShip);
				shipCount++;
			}
			else {
				//REMEMBER TO CHECK OVERLAPPING!!!!
				if (!ships[shipCount].GetRotation() && mouseEvent.getPosition(window).x > 0 && relativeMousePosition.x < CELL_SIZE*MAX_CELLS &&
					mouseEvent.getPosition(window).y > 0 && relativeMousePosition.y < CELL_SIZE*(MAX_CELLS-shipType-1) || 
					ships[shipCount].GetRotation() && mouseEvent.getPosition(window).x > 0 && relativeMousePosition.x < CELL_SIZE*(MAX_CELLS - shipType - 1) &&
					mouseEvent.getPosition(window).y > 0 && relativeMousePosition.y < CELL_SIZE*MAX_CELLS){

					ships[shipCount].SetPosition(relativeMousePosition.x, relativeMousePosition.y);
				}

				if (evento.type == sf::Event::MouseButtonReleased && evento.mouseButton.button == sf::Mouse::Right &&
					relativeMousePosition.x / CELL_SIZE + shipType + 1 < 10) ships[shipCount].SetRotation();
				if (evento.type == sf::Event::MouseButtonReleased && evento.mouseButton.button == sf::Mouse::Left) {
					ships[shipCount].SetPlaced(true);
					for (int i = 0; i <= ships[shipCount].GetType()+1; i++) {
						if(ships[shipCount].GetRotation())
							grid.SetCell(sf::Vector2i(relativeMousePosition.x / CELL_SIZE + i, relativeMousePosition.y / CELL_SIZE), shipCount);
						else
							grid.SetCell(sf::Vector2i(ships[shipCount].GetPosition().x / CELL_SIZE, ships[shipCount].GetPosition().y / CELL_SIZE + i), shipCount+1);
					}
				}
			}
			ships[shipCount].Update();
		}
	}
}

void Fleet::Render(sf::RenderWindow & window)
{
	for (int i = 0; i < ships.size(); i++) ships[i].Render(window);
}


Fleet::~Fleet()
{
}
