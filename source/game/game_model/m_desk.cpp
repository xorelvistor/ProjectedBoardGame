#include "m_desk.h"
#include "m_sector.h"
#include "m_field.h"
#include "m_figure.h"
#include "m_player.h"
#include "m_finish.h"

/**
 * m_desk
 * konstruktor
 *
 * @param number cislo urcujici typ hry (pocet sektoru)
 */ 
m_desk::m_desk(int number) {
	desk_type = number;
	int hracu = 4;
	m_sector *sector;
	for (int i = 0; i < hracu ; i++) {
		sector = new m_sector(i);
		sectors.push_back(sector);
	}
}

/**
 * getDeskType
 * vrati typ hry
 *
 * @return typ hry
 */
int m_desk::getDeskType() {
	return desk_type;
}

/**
 * getSectors
 * vrati seznam sektoru
 *
 * @return seznam sektoru
 */
list<m_sector*> m_desk::getSectors() {
	return sectors;
}

/**
 * findStrokes
 * vyhleda a ulozi mozne tahy hrace
 * 
 * @param player hrac, jehoz tahy budou zjisteny
 * @param xK6 hodnota hodu 
 */
m_field* m_desk::findStrokes(m_player* player, int xK6) {
	cout << "--- findStrokes ---\n";
	m_sector* sector = NULL;
	m_field* field = NULL;
	m_figure* figure = NULL;

	std::list<m_sector*>::iterator it_sectors;
	std::list<m_field*> fields;
	std::list<m_field*> moznosti;
	std::list<m_field*>::iterator it_fields; 
	std::list<m_figure*> figures;
	std::list<m_figure*>::iterator it_figures;
	int toss = xK6; // hod
	int sector_i = 0; // cislo sektoru, pro kontrolu mezi
	int id = 0; // id figury
	figures = player->getMyFigures();
	it_figures = figures.begin();
	/* pro kazdou figuru */
	for (; it_figures != figures.end(); it_figures++) {
		if ((*it_figures)->getAtHome()) continue; // ktera je na hracim planu
		//if((*it_figures)->getField()->special == "finish") continue;
		sector = (*it_figures)->getField()->getSector(); //vychozi sektor figury
		it_sectors = sectors.begin();
		std::advance(it_sectors,sector->getID()); // posun na vychozi sektor
		sector_i = (*it_figures)->getField()->getSector()->getID(); // cislo vychoziho sektoru
		field = (*it_figures)->getField(); // pocatecni pole figury
		toss = xK6;
		id = (*it_figures)->getID(); // oznaceni figury

		cout << ">>> ID figury: " << id << " <<<<" << endl;
		//cout << "pocatecni hod: " << toss << endl;
		cout << "pocatecni pole: " << endl << *field;
		cout << "------------------------------------\n";
		
		while (toss > field->leftUntilEnd()) { // presahne pohyb aktualni sektor?

			//cout << "hod: " << toss << endl;
			//cout << "do konce sektoru zbyva: " << field->leftUntilEnd() << endl;
			//cout << "soucasne pole:" << endl << *field;

			//cout << "!!! musim prejit na dalsi sektor !!!" << endl;
			
			it_sectors++;
			sector_i++;
			toss -= field->leftUntilEnd(); // simulujeme pruchod aktualnim sektorem
			
			if (sector_i > 3) { // musime zajistit projiti vsech sektoru
				sector_i = 0;
				it_sectors = sectors.begin();
			}
			/* musime se posunout na prvni pole dalsiho sektoru */
			field = *(*it_sectors)->getFields().begin();

			//cout << "+ + + + + + + + + + + + + + + " << endl;

			--toss;
		} 
	
		if ((*it_sectors) == player->getSector() && (*it_figures)->getOutOfSector()) {
			fields = (*it_sectors)->getFinish()->getFields(); // figura miri do finishe
			toss--;
			it_fields = fields.begin();
		} else {
			fields = (*it_sectors)->getFields(); // pruchod planem
			it_fields = fields.begin();
			std::advance(it_fields,field->getID());
		}
		
		for(; it_fields != fields.end() ; it_fields++) {
			if (toss == 0) {
				field = (*it_fields); // nalezene pole
				break;
			}
			--toss;
		}
		cout << "koncove pole: " << endl << *field;
		cout << "------------------------------------\n";	
		player->strokes[id] = field; // ulozeni varianty pro kazdou figuru
	}
	return field;
}
