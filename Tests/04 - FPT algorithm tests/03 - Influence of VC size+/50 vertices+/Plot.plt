set datafile separator "|"
set grid
set xrange[0:10]
set yrange[0:300]
set style line 1 lw 2
set style line 2 lw 2
set xlabel "Size of the vertex cover"
set ylabel "Time, s"
plot 'Table.txt' using 1:2 ls 1 title sprintf("FPT runs"), 'Table.txt' using 1:2 smooth cspline ls 2 title sprintf("Arithmetic mean")
set term png medium
set output "Plot.png"
replot
set term windows