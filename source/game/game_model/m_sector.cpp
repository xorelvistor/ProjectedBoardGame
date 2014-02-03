#include "m_sector.h"

m_sector::m_sector(int number) {
	id = number;
	cout << "sektor c." << id << " (" << this << ")" << endl;
	start = new m_home(this);
	finish = new m_finish(this);
	for(int i = 0; i < 10 ; i++) {
		fields[i] = new m_field(this,i);
	}
}

int m_sector::getID() {
	return id;	
}
m_home m_sector::getStart() {
	return *start;	
}
m_finish m_sector::getFinish() {
	return *finish;
}
m_field* m_sector::getFields() {
	return *fields;
}
m_sector m_sector::nextSector() {
return *this;
}