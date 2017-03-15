#include "Fleet.h"

Fleet::Fleet(Faction _fleetFaction, std::string texturePath, Grid &_grid) : grid(_grid)
{
	fleetFaction = _fleetFaction;
	shipTexture.loadFromFile(texturePath);
	shipCount = 0;

	for (int i = 0; i < MAX_SHIPS; i++) {
		if (i == 1) shipType--;
		else if (i == 3) shipType--;
		Ship newShip(sf::Vector2i(0+100*i, 640), (ShipType)shipType, fleetFaction, shipTexture);
		ships.push_back(newShip);
	}
}

void Fleet::PlaceFleet(sf::RenderWindow &window, sf::Event &evento,sf::Mouse &mouseEvent, bool &isPlaced)
{

	while (window.pollEvent(evento)) {
		sf::Vector2i relativeMousePosition = sf::Vector2i(int(mouseEvent.getPosition(window).x / CELL_SIZE) * CELL_SIZE,
														  int(mouseEvent.getPosition(window).y / CELL_SIZE) * CELL_SIZE);

		if (shipCount < MAX_SHIPS) {
			ships[shipCount].Update();

			for (int i = 0; i < ships[shipCount].GetType() + 2; i++) {
				if (ships[shipCount].GetRotation()) {
					if (grid.GetCell(sf::Vector2i(relativeMousePosition.x / CELL_SIZE + i, relativeMousePosition.y / CELL_SIZE)) != 0) {
						canBePlaced = false;
						break;
					}
					else canBePlaced = true;

					if (grid.GetCell(sf::Vector2i(relativeMousePosition.x / CELL_SIZE , relativeMousePosition.y / CELL_SIZE+i)) != 0) {
						canBeRotated = false;
						break;
					}		
					else canBeRotated = true;
				}
				else {
					if (grid.GetCell(sf::Vector2i(relativeMousePosition.x / CELL_SIZE, relativeMousePosition.y / CELL_SIZE + i)) != 0) {
						canBePlaced = false;
						break;
					}
					else canBePlaced = true;

					if (grid.GetCell(sf::Vector2i(relativeMousePosition.x / CELL_SIZE + i, relativeMousePosition.y / CELL_SIZE )) != 0) {
						canBeRotated = false;
						break;
					}
					else canBeRotated = true;
				}
			}
				
			if (!ships[shipCount].GetRotation() && mouseEvent.getPosition(window).x > 0 && relativeMousePosition.x < CELL_SIZE*MAX_CELLS &&
				mouseEvent.getPosition(window).y > 0 && relativeMousePosition.y < CELL_SIZE*(MAX_CELLS-ships[shipCount].GetType()-1) && canBePlaced|| 
				ships[shipCount].GetRotation() && mouseEvent.getPosition(window).x > 0 && relativeMousePosition.x < CELL_SIZE*(MAX_CELLS - ships[shipCount].GetType() - 1) &&
				mouseEvent.getPosition(window).y > 0 && relativeMousePosition.y < CELL_SIZE*MAX_CELLS && canBePlaced){
				isInsideGrid = true;
				ships[shipCount].SetPosition(relativeMousePosition.x, relativeMousePosition.y);
			}
			else isInsideGrid = false;

			if (canBeRotated && evento.type == sf::Event::MouseButtonReleased && evento.mouseButton.button == sf::Mouse::Right &&
				relativeMousePosition.x / CELL_SIZE + ships[shipCount].GetType() + 1 < 10 ) ships[shipCount].SetRotation();

			if (evento.type == sf::Event::MouseButtonReleased && evento.mouseButton.button == sf::Mouse::Left && canBePlaced && isInsideGrid) {
				ships[shipCount].SetPlaced(true);
				for (int i = 0; i <= ships[shipCount].GetType()+1; i++) {
					if(ships[shipCount].GetRotation())
						grid.SetCell(sf::Vector2i(ships[shipCount].GetPosition().x / CELL_SIZE + i, ships[shipCount].GetPosition().y / CELL_SIZE), shipCount+1);
					else
						grid.SetCell(sf::Vector2i(ships[shipCount].GetPosition().x / CELL_SIZE, ships[shipCount].GetPosition().y / CELL_SIZE + i), shipCount+1);
				}
				shipCount++;
			}
		}
		else isPlaced = true;
	}
}

void Fleet::Render(sf::RenderWindow & window)
{
	for (int i = 0; i < ships.size(); i++) ships[i].Render(window);
}


Fleet::~Fleet()
{
}
