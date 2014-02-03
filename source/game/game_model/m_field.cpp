#include "m_field.h"

/// nastavi sektor a id pole
m_field::m_field(m_sector *sector,int number){
		id = number;
		sector = sector;
		figure = NULL;
		cout << "\therni policko c." << id << endl;
}
m_sector m_field::getSector() {
	return NULL;
}

void m_field::putFigure(m_figure) {

}

void m_field::removeFigure() {

}