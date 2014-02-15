#include "m_sector.h"
m_sector::m_sector(){
	cout << "Obycejny konstruktor sektoru" << endl;
	//start = new m_home(this);
	//finish = new m_finish(this);
	id = 42;
}
m_sector::m_sector(int number) {
//	cout << "Obycejny konstruktor sektoru" << endl;
	id = number;
	start = new m_home(this);
	finish = new m_finish(this);
	list<m_field>::iterator iter;
	m_field *pole;
	for(int i = 0; i < 10 ; i++) {
		pole = new m_field(this,i,"hraci");
		fields.push_back(pole);
		if (fields.empty()) {
			cout << "koncim" << endl;
			return;
		}
	}
	
}

m_sector::m_sector(const m_sector &original) {
	cout << "Kopirovaci konstruktor sektoru" << endl;
	
	id = original.id;
	start = new m_home();
	finish = new m_finish();
	//*start = *original.start;
	//*finish = *original.finish;
	std::list<m_field*>::const_iterator iter_orig;
	iter_orig = original.fields.begin();
	for (;iter_orig != original.fields.end();iter_orig++) {
		fields.push_back(*iter_orig);
	}
	
}
m_sector::~m_sector() {
	start = NULL;
	finish = NULL;
	delete start;
	delete finish;
}
m_sector* m_sector::getSector() {
	return this;
}
int m_sector::getID() {
	return id;	
}
m_home* m_sector::getStart() {
	return start;	
}
m_finish* m_sector::getFinish() {
	return finish;
}
std::list<m_field*>  m_sector::getFields() {
	return fields;
}
m_sector m_sector::nextSector() {
return *this;
}