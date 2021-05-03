#pragma once

 //LinearChanneler
 //Current observation at the current index is already added to the p_count array
 void LinearChanneler(int index, float* channel, float* p_count)
 {
 	//Channel loops from Max to Min → iterator i
 	for(int i = index; i >= 0; i--)
 	{
 		channel[i] += p_count[index]*p_count[i];
 	}

 } //End of LinearChanneler()

 void LinearChanneler(float* channel, float* p_count)
 {
 	for(int k = 0; k < Channel_Size; k++)
 	{
 		unsigned int sizex = (Channel_Size-k);
 		for(int i = 0; i < sizex; i++)
 		{
 			channel[k] += p_count[i]*p_count[i+k];
 		}

	}
 }