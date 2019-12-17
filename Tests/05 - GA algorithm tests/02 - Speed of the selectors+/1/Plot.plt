set datafile separator ":"
set grid
set xrange[50:210]
set yrange[500:3200]
set style line 1 lw 2
set style line 2 lw 2
set xlabel "Size of a population"
set ylabel "Time, s"
plot 'RW, X_0.9, M_0.4.txt' using 1:2 ls 1 title sprintf("Roulette Wheel Selector"), 'T, X_0.9, M_0.4 .txt' using 1:2 ls 2 title sprintf("Tournament Selector")
set term png medium
set output "Plot.png"
replot
set term windows