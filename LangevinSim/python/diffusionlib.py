#!/usr/bin/env python3
# coding: utf-8


import math
import numpy


#pars, cov = curve_fit(f=exponential, xdata=x_dummy, ydata=y_dummy, p0=[0, 0], bounds=(-np.inf, np.inf))

#Calculate the residuals
res = y_dummy - exponential(x_dummy, *pars)

# *pars — allows us to unroll the pars array, i.e. [a, b] gets inputted as a, b
#residuals must be close to zero and rndomly distributed along y = 0

#fitorder = 0 → autofit
def MSDfit(amsd, atime, diffcoeff, dim=3):
	#Set Scale
	scale = 2*dim*diffcoeff

	#Define Power Law Function
	def power_law(x, power):
		return scale*np.power(x, power)

	#Do curve fitting
	pars, cov = curve_fit(f=power_law, xdata=atime, ydata=amsd, 
						  p0=[0, 0], bounds=(-np.inf, np.inf))

	# Get the standard deviations of the parameters (square roots of the # diagonal of the covariance)
	stdevs = np.sqrt(np.diag(cov))

	# Calculate the residuals
	residuals = amsd - power_law(amsd, *pars) #*pars unrolls the array into individual parameters

	return pars, cov, stdevs, residuals


	def VeffVol(x,y,z):
		return 1.25992104989487316476721 * x * y * z

	def GetCharResTime(diffcoeff,x , y):
		return 0.25 * x * y / diffcoeff

	def NormalGaussDiffusion(diffcoeff, radius, sf):
		TauD = GetCharResTime(D, radius, radius)
		return 1 / (1 + (T/TauD))* np.sqrt(1 + (T/TauD)/(sf*sf))

	def GaussianFit(acf, time, diffcoeff, radius, sf):

		TauD = GetCharResTime(D, radius, radius)

		def FitFn():
			return 1 / (1 + (T/TauD))* np.sqrt(1 + (T/TauD)/(sf*sf))
		

		#Do curve fitting
		pars, cov = curve_fit(f=FitFn, xdata=time, ydata=acf, 
							  p0=[0, 0], bounds=(-np.inf, np.inf))

		# Get the standard deviations of the parameters (square roots of the # diagonal of the covariance)
		stdevs = np.sqrt(np.diag(cov))

		# Calculate the residuals
		residuals = amsd - power_law(amsd, *pars) #*pars unrolls the array into individual parameters

		return pars, cov, stdevs, residuals