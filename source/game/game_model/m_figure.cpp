#include "m_figure.h"

m_figure::m_figure() {
	at_home = false;
}

m_field m_figure::getField() {
	return *field;
}

bool m_figure::getAtHome() {
	return at_home;
}

void m_figure::move(m_field) {
	
}
void m_figure::attack(m_figure) {

}