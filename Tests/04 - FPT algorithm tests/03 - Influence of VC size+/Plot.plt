set datafile separator "|"
set grid
set xrange[0:10]
set yrange[0:650]
set style line 1 lw 2
set style line 2 lw 2
set style line 3 lw 2
set style line 4 lw 2
set style line 5 lw 2
set style line 6 lw 2
set style line 7 lw 1
set xlabel "Size of the vertex cover"
set ylabel "Time, s"
plot 'Table 1.txt' using 1:2 ls 1 title sprintf("Graphs with 200 vertices"), 'Table 1.txt' using 1:2 smooth cspline ls 2 title sprintf("Arithmetic mean"), 'Table 2.txt' using 1:2 ls 3 title sprintf("Graphs with 100 vertices"), 'Table 2.txt' using 1:2 smooth cspline ls 4 title sprintf("Arithmetic mean"), 'Table 3.txt' using 1:2 ls 5 title sprintf("Graphs with 50 vertices"), 'Table 3.txt' using 1:2 smooth cspline ls 6 title sprintf("Arithmetic mean"), 'Table 4.txt' using 1:2 smooth cspline linetype 1 dashtype (5,50) title sprintf("Theoretical bound")
set term png medium
set output "Plot.png"
replot
set term windows