#pragma once

#include <cassert>

#include "types.hpp"
#include "lincorr_base.hpp"


// OBJECT 1 : Linear Correlator - Real Time
/**
 * @brief Defines a real time linear correlator with a fixed Channel_size and Series_Size. The correlator accepts counter blocks that are treated as sngle points in the timeseries.
 * */
template <index_t Channel_size, index_t Series_size>
 class LinCorr_RT_NoBaseline : public Linear_Correlator_Base
 {
 public:

//Members

 	//Data Arrays
 	counter_t Series_array[Series_size] = {0};
 	double Channel_array[Channel_size] = {0};

 	//Indexes
 	index_t Series_index = 0; //! Highest active series index
 	index_t Channel_index = 0; //! Highest active channel index


 	//1
 	/**
 	 * @Accepts new data by the counter → writes the Series_array block.*/
 	void inline push_back(counter_t counter_block) override
 	{
 		Series_array[Series_index] = counter_block;
 		Series_index++;
 		push_to_channel(Series_index); //Compute ACF contribution
 	}


 	//2
 	/**
 	 * @brief Calculates real time ACF contribution of the data point for the passed series index and updates the relavent ACF channels. It also sets the Channel index to the passed index.*/
 	void inline push_to_channel(index_t s_index) override
 	{
 		Channel_index = s_index;
 		double norm = 1.0/double(Series_size);

 		for(unsigned int i = s_index; i <= 0; i--)
 		{
 			Channel_array[s_index - i] += (norm * Series_array[s_index] * Series_array[i])
 		}
 	}
 	
 	//3
 	/**
 	 * @brief For a given time series (as in Series_array), the whole ACF Channel is updated.*/
 	void inline channel_update() override
 	{
 		for(unsigned int i = 0; i < Channel_size; i++)
 		{
 			Channel_array[i] = autocorr(i);
 		}
 	}
 	
 	//4
 	/**
 	 * @brief Calculates the Autocovariance at a given lag time.
 	 * @param Lag time.
 	 * */
 	double autocv(unsigned int lag) const override
 	{
 		double norm = 0.0; //Normalization
 		double autocv, new_val = 0.0; //The Autocovariance
 		for (unsigned int i = 0; i < (Series_size - lag); i++)
 		{
 			new_val = (Series_array[i]) * (Series_array[i + lag]);
 			norm = 1/(Series_size - lag);
 			autocv += norm * new_val;
 		}
 	}

 	//5
 	/**
 	 * @brief Calculates the autocorrelation for a given lag time.
 	 * @param Lag time.
 	 * */
 	double autocorr(unsigned int lag) const override
 	{
 		return autocv(lag)/variance(); 
 	}



 	//6
 	/**
 	 * @brief Calculates the mean of the time series array.
 	 * */
 	void series_mean() const override
 	{
 		double mean = 0.0
 		
 		for (unsigned int i = 0; i < Series_size; i++)
 		{
 		  mean += (Series_array[i])
 		}

 		return(mean/Series_size);
 	}

 	//7
 	/**
 	 * @brief Calculates the variance of the time series array.
 	 * */
 	void series_variance() const override
 	{
 		//Implement after checking theory
 	}


 	
 	//8
 	/**
 	 * @brief Appends a block of time series data to the time series array ```Series_Array```.
 	 * @warning Real time ACF computation is not available for this sort of data push operation.
 	 * */
 	void inline append_timeseries(counter_t *append_source, index_t append_size) override
 	{
 		#if LC_SAFETY_CHECKS == 1
 			assert(this->Series_index + time_series_size < Series_size);
 		#endif

 		//Copy to series_array
 		memcpy(Series_array[Series_index], append_source, append_size);


 		//Compute ACF Contribution
 		index_t NewSeriesIndex = this->Series_index + append_size;
 		for(index_t i = Series_index; i < NewSeriesIndex; i++)
 		{
 			this->push_to_channel(i);
 		}

 		this->Series_index = NewSeriesIndex;

 	}

 	//9
	/**
	 * @brief Returns a tuple of the pointer of the channel array and size of the series array. 
	 * */
	std::tuple<double*, index_t> inline get_channel_array() const override
	{
		return std::make_tuple(&Channel_array, Channel_size);
	}

 };