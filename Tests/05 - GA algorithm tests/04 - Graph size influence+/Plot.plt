set datafile separator "|"
set xrange [0:250]
set xtics 10
set ytics 250
set grid
set xlabel "Graph size"
set ylabel "Time, s."
show xlabel
show ylabel
set style line 1 lw 2
set style line 2 lw 3
plot 'Table.txt' using 1:2 ls 1 title sprintf("Time needed for 1000 generations"), 'Table.txt' using 1:2 smooth unique ls 2 title sprintf("Arithmetic mean")
set yr [700:GPVAL_DATA_Y_MAX + 250]
replot
set term png medium
set output "GSI.png"
replot
set term windows