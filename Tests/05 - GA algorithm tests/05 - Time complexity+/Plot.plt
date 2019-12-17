set datafile separator "|"
set xrange [0:1000]
set xtics 100
set ytics 250
set grid
set xlabel "Generations"
set ylabel "Time, s."
show xlabel
show ylabel
set style line 1 lw 2
set style line 2 lw 1
plot 'Table.txt' using 1:5 smooth cspline ls 1 title sprintf("Accumulated time"), 'Table2.txt' using 1:2 smooth unique ls 2 title sprintf("Theoretical bound")
set yr [0:GPVAL_DATA_Y_MAX + 250]
replot
set term png medium
set output "Plot.png"
replot
set term windows