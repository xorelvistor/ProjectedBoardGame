#pragma once
#include "_game.h"
/// reprezentuje policko hraciho planu
class m_figure;
class m_sector;
class m_field {
private:
	m_figure *figure; /// figura na poli
	int id; /// cislo pole v ramci sektoru
	m_sector *sector; /// sektor do ktereho pole patri
public:
	m_field(m_sector *sector,int number);
	int getID();
	m_sector getSector();
	void putFigure(m_figure);
	void removeFigure();

};
