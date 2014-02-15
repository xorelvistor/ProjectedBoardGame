#include "m_desk.h"

m_desk::m_desk(int number) {
	//cout << "Obycejny konstruktor planu" << endl;
	desk_type = number;
	cout << "herni plan s typem: " << desk_type << endl;
	int hracu = 4;
	m_sector *sector;
	for (int i = 0; i < hracu ; i++) {
		sector = new m_sector(i);
		sectors.push_back(sector);
	}
}
/*
m_desk::m_desk(const m_desk &original) {
	cout << "Kopirovaci konstruktor planu" << endl;
	
	desk_type = original.desk_type;
	std::list<m_sector>::const_iterator iter_orig;
	iter_orig = original.sectors.begin();
	for (;iter_orig != original.sectors.end();iter_orig++) {
		sectors.push_back(*iter_orig);
	}
	
}
*/
/// vrati typ hry
int m_desk::getDeskType() {
	return desk_type;
}
/// vrati seznam sektoru
list<m_sector*> m_desk::getSectors() {
	return sectors;
}

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
	int toss = xK6;
	int sector_i = 0;
	int id = 0;
	figures = player->getMyFigures();
	it_figures = figures.begin();
	
	
	for (; it_figures != figures.end(); it_figures++) {
		if ((*it_figures)->getAtHome()) continue;
		sector = (*it_figures)->getField()->getSector(); //vychozi sektor figury
		it_sectors = sectors.begin();
		std::advance(it_sectors,sector->getID()); // posun na vychozi sektor
		sector_i = (*it_figures)->getField()->getSector()->getID(); // cislo vychoziho sektoru
		field = (*it_figures)->getField(); // pocatecni pole figury
		toss = xK6;
		id = (*it_figures)->getID(); // oznaceni figury

		cout << ">>> ID figury: " << id << " <<<<" << endl;
		cout << "pocatecni hod: " << toss << endl;
		cout << "pocatecni pole: " << endl << *field;
		cout << "------------------------------------\n";
		
		while (toss > field->leftUntilEnd()) { // presahne pohyb aktualni sektor?
			cout << "hod: " << toss << endl;
			cout << "do konce sektoru zbyva: " << field->leftUntilEnd() << endl;
			cout << "soucasne pole:" << endl << *field;
			cout << "!!! musim prejit na dalsi sektor !!!" << endl;
			
			it_sectors++;
			sector_i++;
			toss -= field->leftUntilEnd(); // simulujeme pruchod aktualnim sektorem
			
			if (sector_i >= 3) { // musime zajistit projiti vsech sektoru
				sector_i = 0;
				it_sectors = sectors.begin();
			}
			/* musime se posunout na prvni pole dalsiho sektoru */
			field = *(*it_sectors)->getFields().begin();
			cout << "+ + + + + + + + + + + + + + + " << endl;
			--toss;
		} 
	
		if ((*it_sectors) == player->getSector() && (*it_figures)->getOutOfSector()) {
			fields = (*it_sectors)->getFinish()->getFields(); // figura miri do finishe
			it_fields = fields.begin();
		} else {
			fields = (*it_sectors)->getFields();
			it_fields = fields.begin();
			std::advance(it_fields,field->getID());
		}
		

		cout << "hod pred pruchodem: " << toss << endl;
		for(; it_fields != fields.end() ; it_fields++) {
			if (toss == 0) {
				field = (*it_fields);
				break;
			}
			--toss;
		}
		cout << "nalezene pole je: " << endl;
		cout << *field;
		
		player->strokes[id] = field;
		
	}
	return field;
}