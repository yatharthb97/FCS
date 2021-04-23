#animated graph script

#importing libraries
import matplotlib.pyplot as plt
import matplotlib.animation as animation





def animate(i):

    ax1.clear()
    ax1.plot(xarray, serialarray)

    plt.xlabel('G(Tau)')
    plt.ylabel('Time(MicroSeconds')
    plt.title(string('Autocorrelation - Update ', i))	
	
    
ani = animation.FuncAnimation(fig, animate, interval=1000) 
plt.show()