#include "../m_lib.h"
#include "../g_lib.h"
#include "g_field.h"
#include "graphic_object.h"
#pragma once

class m_field;

class g_finish : public g_field {
	public:
		g_finish(cv::Point position, double r);
		~g_finish();

	private:
		double r_search;
		double r_draw;
		cv::Scalar color;
		m_field* model_field;
	protected:
	

	};