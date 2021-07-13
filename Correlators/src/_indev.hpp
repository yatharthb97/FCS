#pragma once



/**
 * Before computation of the lag time Tau, the signal is averaged over a lag time delT such that Tau/delT = alpha → a constant.
 * 
 * */
template <unsigned int Channel_Size, Series_Size>
class Linear_Correlator
{

public:

//Memebers

	double Channel_array[Channel_Size] = {0.0}; //! Array to store the ACF values.
	counter_t Series_array[Series_Size] = {0}; //! Array to store the individual blocks of counts.
	counter_t Buffer_block = 0;

	//Number of blocks to locally average before pushing it to the correlator
	unsigned int Local_bincount = std::pow(m, s);

	Linear_Correlator(lcorr_id_t id_s, unsigned int block_ratio)
	{
		Local_bincount = std::pow(block_ratio, id_s);
	}

	Linear_Correlator(): Local_bincount(1)
	{}

	/**
	 * @brief Accepts a block from the counter module.*/
	void push_block(counter_t block)
	{
		local_counter++;
		Buffer_block += block;
		if(local_counter == local_binning_count)
		{
			compute_block(buffer_block); //Push block for computation
			buffer_block = 0;
			local_counter = 0;
		}
	}

	void compute_block(counter_t buffer_block)
	{
		block_array[block_index] = buffer_block;
		buffer_block++;
		push_block_to_channel(block_index);
	}

	void push_block_to_channel(index_t block_index)
	{

	}

	/**
	 * @brief The autocv function returns the covariance of the Block_array.
	 * @@param The lag time k.*/
	double autocv(unsigned int lag)
	{
		double norm = 0.0; //Normalization
		bouble autocv = 0.0; //The Autocovariance
		for (unsigned int i = 0; i < (Block_Size - lag); i++)
		{
			autocv += (Block_array[i]) * (Block_array[i+lag])
			norm = 1/(Block_Size - lag);
			autocv = norm * autocorr;
		}
	}

	/**
	 * @brief The autocorr function returns the autocovariance divided by the statistical variance of the Block_array.
	 * @param The lag time k.
	 * */
	double autocorr(unsigned int lag)
	{
		return autocv(lag)/variance(); 
	}

	/**
	 * @brief Updates the autocorrelation channel by calculating the ACF at the respective lag values.
	 * */
	void channel_update()
	{
		for(unsigned int i = 0; i < Channel_Size; i++)
		{
			Channel_array[i] = autocorr(i);
		}
	}



};



//Typical Multi-Tau correlators are made up of 8 linear correlators. We say a multi-tau correlator is composed of a block of size S of linear correlator modules. The integration time Ts for each block can be calculates as:

//Counter gate time == Ts(s=0). The fastest correlator. 

unsigned int integ_time(blockid_t s, binning_ratio_t m, gatetime_t counter_gate_time)
{
	double Ts = std::pow(m, s) * counter_gate_time;
	return unsigned int(Ts);
}

//Number of blocks to locally average before pushing it to the correlator
local_binning_count = std::pow(m, s);






 