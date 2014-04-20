#include "../m_lib.h"
#include "../g_lib.h"
#include "../game_model/m_field.h"
#include "../game_model//m_player.h"
#include "graphic_object.h"
#pragma once


class m_field;

class g_field : public graphic_object {
	public:
		g_field(cv::Point position, int r);
		~g_field();
		inline void setColor (GLfloat r, GLfloat g, GLfloat b) {
			color.r = r;
			color.g = g;
			color.b = b;

			//cout << "nastavuji barvu - ";
			//printf("%lf, %lf, %lf\n",color.r, color.g, color.b);
		}
		inline void setModel(m_field* f) {
			model_field = f;
		}

		inline void setR_draw(int r) {
			r_draw = r;
			r_search = r-1;
		}
		inline m_field* getModel() {
			return model_field;
		};
		inline double getR_Search() {
			return r_search; 
		};

		void draw();
		void select();

	private:
		float r_search;
		float r_draw;
		m_field* model_field;
	protected:
	

	};