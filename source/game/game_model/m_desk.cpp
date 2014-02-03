#include "m_desk.h"

m_desk::m_desk(int number) {
	desk_type = number;
	cout << "herni plan s typem: " << desk_type << endl;
	for (int i = 0; i < 4 ; i++) {
		sectors[i] = new m_sector(i);
	}
}

int m_desk::getDeskType() {
	return desk_type;
}