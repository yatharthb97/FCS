#pragma once

 //LinearChanneler
 //Current observation at the current index is already added to the p_count array
 void LinearChanneler(int index, double* channel, double* p_count) __attribute__((always_inline));
 {
 	//Channel loops from Max to Min → iterator i
 	for(int i = index; i >= 0; i--)
 	{
 		channel[i] += p_count[index]*p_count[i];
 	}

 } //End of LinearChanneler()