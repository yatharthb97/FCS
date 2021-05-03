l=15.8114
dt=1e-3

se term gif medium enhanced animate delay 0.1 font "Droid Sans, 8"
se out 'anim_prob.gif'

set xrange [-0.5*l-0.5 : 0.5*l+0.5]
set yrange [-0.5*l-0.5 : 0.5*l+0.5]
set size sq
unset grid

stats 'config.txt' u 3
print STATS_blocks

se arrow 1 from -0.5*l,-0.5*l to 0.5*l,-0.5*l nohead lw 2 lc rgbcolor "blue"
se arrow 2 from 0.5*l,-0.5*l to 0.5*l,0.5*l nohead lw 2 lc rgbcolor "blue"
se arrow 3 from 0.5*l,0.5*l to -0.5*l,0.5*l nohead lw 2 lc rgbcolor "blue"
se arrow 4 from -0.5*l,0.5*l to -0.5*l,-0.5*l nohead lw 2 lc rgbcolor "blue"

set object 5 circle back at 0,0 size 1 fillcolor "cyan" lw 1 fillstyle solid 0.5
set key off
set cbrange [0:1]

set palette defined (0 "black", 1 "red")

do for [i=1:500] { 
  print i
  pl 'config.txt' i i u 1:2:3 w p pt 7 palette; 
  #plot for [j=1:i-1] 'config.txt' i j u 1:2 w p pt 7 ps 0.25 lc rgbcolor "yellow";
  set title sprintf("tstep = %d, t = %f", i, i*dt) ; 
  pause 0.1;
}

se out 