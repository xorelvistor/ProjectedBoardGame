#include "../m_lib.h"
#include "../g_lib.h"
#include "graphic_object.h"

#pragma once

class m_sector;
class g_sector: public graphic_object {
	public:

		g_sector(cv::Point position,int sirka);
		~g_sector();

		inline m_sector* getModel() {
			return model_sector;
		};
		void draw(int sirka);
		void select();
		void setModel(m_sector* sector) {
			model_sector = sector;
		}
	private:
		m_sector* model_sector;

};