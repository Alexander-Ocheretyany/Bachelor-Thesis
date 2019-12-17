set datafile separator "|"
set grid
set xrange[0:250]
set yrange[0:40]
set style line 1 lw 2
set style line 2 lw 2
set style line 7 lw 1
set xlabel "Graph size"
set ylabel "Time, s"
plot 'Table.txt' using 1:2 ls 1 title sprintf("FPT runs"), 'Table.txt' using 1:2 smooth cspline ls 2 title sprintf("Arithmetic mean"), 'Table2.txt' using 1:2 smooth cspline ls 7 title sprintf("Theoretical bound")
set term png medium
set output "Plot.png"
replot
set term windows