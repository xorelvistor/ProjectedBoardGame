#include "../m_lib.h"
#include "../g_lib.h"
#pragma once

class m_desk;
class g_sector;
class g_field;
class g_desk {
	public:
		std::list <g_sector*> graphic_sectors;

		g_desk(int rozmer, m_desk* deska);
		~g_desk();

		inline m_desk* getModel() {
			return model_desk;
		};
		void draw();
		void calc(int x, int y, int r, int posun, int skok);
		void select();
		void pair();
		void setModel(m_desk* deska) {
			model_desk = deska;
		}
	private:
		m_desk* model_desk;
		
};