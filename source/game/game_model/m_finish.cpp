#include "m_finish.h"
m_finish::m_finish(){cout << "Obycejny konstruktor cile" << endl;}
m_finish::m_finish(m_sector *sector) {
	//cout << "Obycejny konstruktor cile" << endl;
	this->sector = sector;
	figure_count = 0;
	list<m_field*>::iterator iterF;
	m_field *pole;
	for(int i = 0; i < sector->getStart()->getFigureCount(); i++) {
		pole = new m_field(sector,i,"finish");
		fields.push_back(pole);
		if (fields.empty()) {
			cout << "koncim" << endl;
			return;
		}
	}
	
}
/*
m_finish::m_finish(const m_finish &original) {
	cout << "Kopirovaci konstruktor cile" << endl;
	
	figure_count = original.figure_count;
	sector = new m_sector();
	*sector = *original.sector;
	std::list<m_field>::const_iterator iter_orig;
	iter_orig = original.fields.begin();
	for (;iter_orig != original.fields.end();iter_orig++) {
		fields.push_back(*iter_orig);
	}
	
}
*/
m_finish::~m_finish() {
	sector = NULL;
	delete sector;
}
std::list<m_field*>  m_finish::getFields() {
	return fields;
}

int m_finish::getFigureCount () {
	return figure_count;
}