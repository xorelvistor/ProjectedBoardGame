#include "../m_lib.h"
#include "../g_lib.h"
#include "../game_model/m_player.h"
#pragma once

class graphic_object {
	public:
		graphic_object (cv::Point p):position(p) {};
		~graphic_object () {};
		void setPosition(cv::Point p) {
			position = p;
		}
		cv::Point getPosition() {
			return position;
		};

		TColor getColor() {
			return color;
		}
	protected:
		virtual void draw() {};
		virtual void select() {};
		virtual void setModel() {};
		cv::Point position;
		TColor color;


};