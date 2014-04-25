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
bool m_desk::findStrokes(m_player* player, int xK6) {
	cout << "--- findStrokes ---\n";
	m_sector* sector = NULL;
	m_field* field = NULL;
	m_field* target = NULL;

	std::list<m_sector*>::iterator it_sectors;
	std::list<m_field*> fields;
	std::list<m_field*> moznosti;
	std::list<m_field*>::iterator it_fields; 
	std::list<m_figure*> figures;
	std::list<m_figure*>::iterator it_figures;

	int toss = xK6; // hod
	int no_move = 0; // pocet figur, ktere se nameji kam pohnout
	int sector_i = 0; // cislo sektoru, pro kontrolu mezi
	int id = 0; // id figury
	figures = player->getMyFigures();
	it_figures = figures.begin();

	/* pro kazdou figuru */
	for (; it_figures != figures.end(); it_figures++) {
		if ((*it_figures)->getAtHome()) {
			no_move++;
			continue; // neni na hracim planu
		}

		field = (*it_figures)->getField(); // pocatecni pole figury
		id = (*it_figures)->getID(); // oznaceni figury

		if(field->special == "finish" && (toss > field->leftUntilEnd() || field->getID() == 3)) { // posledni pole ve finishi
			player->strokes[id] = NULL; // jiz se neni kam pohnout
			no_move++;
			continue;
		}

		sector = field->getSector(); // vychozi sektor figury
		it_sectors = sectors.begin(); // prvni sektor planu

		std::advance(it_sectors,sector->getID()); // posun na vychozi sektor
		sector_i = sector->getID(); // cislo vychoziho sektoru

		toss = xK6; // hodnota hodu
		
		cout << ">>> ID figury: " << id << " <<<<" << endl;
		cout << "pocatecni hod: " << toss << endl;
		cout << "pocatecni pole: " << endl << *field;
		cout << "------------------------------------" << endl;
		
		target = field;

		if(sector == player->getSector() && !(*it_figures)->getOutOfSector() ||
			sector != player->getSector()) { // figura nemiri do finishe
			while (toss > target->leftUntilEnd()) { // presahne pohyb aktualni sektor?
				cout << "hod: " << toss << endl;
				//cout << "do konce sektoru zbyva: " << target->leftUntilEnd() << endl;
				//cout << "soucasne pole:" << endl << *target;

				//cout << "!!! musim prejit na dalsi sektor !!!" << endl;
			
				toss -= target->leftUntilEnd(); // simulujeme pruchod aktualnim sektorem
				it_sectors++;
				sector_i++;
			
				if (sector_i > 3) { // musime zajistit projiti vsech sektoru
					sector_i = 0;
					it_sectors = sectors.begin();
				}


				/* musime se posunout na prvni pole dalsiho sektoru */
				toss--;
				target = *(*it_sectors)->getFields().begin();
				
				if ((*it_sectors) == player->getSector())
					break;

				cout << "+ + + + + + + + + + + + + + + " << endl;
			} 
			sector = (*it_sectors);
		}
		
		if (sector == player->getSector() && (*it_figures)->getOutOfSector() && toss > 0) {
			fields = sector->getFinish()->getFields(); // figura miri do finishe
			it_fields = fields.begin();
			if(field->special == "finish") {
				std::advance(it_fields,target->getID());
			} else {
				toss--;
			}
		} else {
			fields = sector->getFields();
			it_fields = fields.begin();
			std::advance(it_fields,target->getID());
		}
		
		

		if (toss == 0) {
			target = (*it_fields);
		} else {
			target = NULL;
			// pruchod oblasti
			//cout << "pruchod oblasti (" << toss << ") => zacinam na poli: " << endl << *(*it_fields);
			for(; it_fields != fields.end() ; it_fields++) {
				if (toss == 0) {
					target = (*it_fields); // nalezene pole
					break;
				}
				toss--;
			}
		}

		m_figure* figure = NULL;
		if (target != NULL && (figure = target->getFigure()) != NULL) {
			if(figure->getOwner() == player && target->special == "finish") {
				target = NULL;
				cout << "Nikam nejdu!" << endl;
			}
		}

		if (target != NULL)
			cout << "koncove pole: " << endl << *target;
		else {
			cout << "koncove pole: NULL" << endl;
			no_move++;
		}
		cout << "------------------------------------\n";	
		player->strokes[id] = target; // ulozeni varianty pro kazdou figuru
	}
	if (no_move == 4)
		return false;
	return true;
}
