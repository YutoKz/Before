if(exist("n")==0 || n<0) n = n0 
set view 45, 230, 1, 1
set hidden3d

splot "2d_temperature_for_gif.dat" index n using 1:2:3 with lines

n = n + dn            
if ( n < n1 ) reread 
undefine n 