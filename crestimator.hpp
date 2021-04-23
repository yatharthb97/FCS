//CREstimator → Count Rate Estimator
#pragma once

 class CREstimator()
 {
 	
private:
 	float scale = 0;
 	int max = 0; //Do we really need to store max?
 	float count_rate = 0;


public:
 	//Constructor
 	CREstimator(int max): max(max)
 	{
 		scale = 1.0/max;
 	}

 	void inline push(int ni) __attribute__((always_inline));
 	{
 		count_rate += ni * scale;
 	}

 	float estimate() __attribute__((always_inline));
 	{
 		return count_rate;
 	}

 	float estimatesq() __attribute__((always_inline));
 	{
 		return count_rate * count_rate;
 	}

 }; //End of class CREstimator