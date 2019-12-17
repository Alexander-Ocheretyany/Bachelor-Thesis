set datafile separator "|"
set xrange [0:1000]
set yrange [130:220]
set xtics 100
set ytics 5
set grid
set style line 1 lw 3  
set style line 2 lw 3  
set style line 3 lw 3  
set xlabel "Generations"
set ylabel "Solution size"
plot 'Table 1.txt' using 1:4 smooth cspline ls 1 title sprintf("Big Bang Mutation"), 'Table 2.txt' using 1:4 smooth cspline ls 2 title sprintf("Shift Mutation"), 152 ls 3 title sprintf("Optimum")
replot
set term png medium
set output "Plot.png"
replot
set term windows