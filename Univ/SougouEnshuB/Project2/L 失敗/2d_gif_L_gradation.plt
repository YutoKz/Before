if(exist("n")==0 || n<0) n = n0 
set view 45, 50, 1, 1
set hidden3d

#set palette define (0 "red", 1 "blue")

#set pm3d map

splot "2d_temperature_for_gif_L.dat" index n using 1:2:3 with pm3d    # with linespoints pointtype 7 pointsize 1 linecolor palette

n = n + dn            
if ( n < n1 ) reread 
undefine n 