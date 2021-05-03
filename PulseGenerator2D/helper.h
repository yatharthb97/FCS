#pragma once

//TODO: Check why const declaration is a problem. Post on stack exchange
class MinMax
{
public:
	int min_, max_;

	MinMax() = delete; //Default Constructor is deleted for Initilization safety
	
	MinMax(int minx, int maxx): min_(minx), max_(maxx)
	{}

	//Only left bounds are included
	bool within_bounds(int pos)
	{
		return ((pos >= min_) && (pos < max_));
	}

};


namespace Time
{
  
  
}
