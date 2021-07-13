#pragma once

#include <tuple>
#include "types.hpp"

class Linear_Correlator_Base
{
public:

	//1
	/**
	 * @Accepts new data by the counter → equivalen to the lowest data point.*/
	void virtual inline push_back(counter_t block) = 0;

	//2
	/**
	 * @brief Calculates real time ACF contribution of the data point and updates the relavent ACF channel.*/
	void virtual inline push_to_channel(index_t block_index) = 0;
	
	//3
	/**
	 * @brief For a given time series (as in Series_array), all the ACF channels are updated.*/
	void virtual inline channel_update() = 0;

	//4
	/**
	 * @brief Calculates the Autocovariance at a given lag time.
	 * @param Lag time.
	 * */
	double virtual inline autocv(unsigned int lag) const = 0;

	//5
	/**
	 * @brief Calculates the autocorrelation for a given lag time.
	 * @param Lag time.
	 * */
	double virtual inline autocorr(unsigned int lag) const = 0;

	//6
	/**
	 * @brief Calculates the mean of the time series array.
	 * */
	void virtual inline series_mean() const = 0;

	//7
	/**
	 * @brief Calculates the variance of the time series array.
	 * */
	void virtual inline series_variance() const = 0;

	//8
	/**
	 * @brief Appends a block of time series data to the time series array.
	 * @param A pointer to the time series array to be appended and the size to be appended.
	 * */
	void virtual inline append_timeseries(counter_t *timeseries, index_t append_size) = 0;

	//9
	/**
	 * @brief Returns a tuple of the pointer of the channel array and size of the series array. 
	 * */
	std::tuple<double*, index_t> virtual inline get_channel_array() const = 0;

}; //End of Linear_Correlator_Base



//Under development → 
/*

//10 Friend Declarations
std::ostream& operator<< (std::ostream &stream, const Linear_Correlator_Base &lincorr)
 
//10
//! Stream operator used to 
std::ostream& operator<< (std::ostream &stream, const Linear_Correlator_Base &lincorr)
{
    counter_t *array;
    index_t size = 0;
    std::bind(array, size) = lincorr.get_series_array();
    
    stream << array[0]; //Print first element
    for(index_t i = 1; i < size-2; i++)
    {
    	stream << LC_DSep << array[i];
    }
    stream << LC_DSep << array[size-1]; //Print last element
    
    return stream;
} //End of friend overload operator<<
*/