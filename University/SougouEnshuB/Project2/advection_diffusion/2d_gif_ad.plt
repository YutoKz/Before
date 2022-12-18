if(exist("n")==0 || n<0) n = n0 
set view 45, 50, 1, 1
set hidden3d

splot "2d_temperature_for_gif_ad.dat" index n using 1:2:3 with points pt 7

n = n + dn            
if ( n < n1 ) reread 
undefine n 