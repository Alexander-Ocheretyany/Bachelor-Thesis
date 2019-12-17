set datafile separator "|"
set grid
set xrange[0:100]
set yrange[0:450]
set xtics 5
set style line 1 lw 2
set style line 2 lw 2
set style line 3 lw 2
set style line 4 lw 2
set style line 5 lw 2
set style line 6 lw 2
set xlabel "Partition size"
set ylabel "Time, s"
plot 'Table.txt' using 1:2 ls 1 title sprintf("LP"), 'Table.txt' using 1:2 smooth cspline ls 2 title sprintf("Arithmetic mean"), 'Table2.txt' using 1:2 ls 3 title sprintf("Boost"), 'Table2.txt' using 1:2 smooth cspline ls 4 title sprintf("Arithmetic mean")
set term png medium
set output "Plot.png"
replot
set term windows