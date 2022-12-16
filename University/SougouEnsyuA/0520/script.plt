# use command `gnuplot -c script.plt` to launch the script from the command line

set terminal pngcairo
set output "myfigure.png"

set key bottom

set title "title for the figure"
set ylabel "y"
set xlabel "x"

plot "kadai6_data_500.dat" with points title "first data series", \
    "kadai6_data_200.dat" with points title "second data series"
