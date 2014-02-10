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