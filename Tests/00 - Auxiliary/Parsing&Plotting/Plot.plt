set datafile separator "|"
set xrange [0:1000]
set xtics 100
set ytics 5
set grid
set style line 1 lw 3  
set style line 2 lw 3  
set style line 3 lw 3  
set style line 4 lw 3  
set style line 5 lw 3  
set style line 6 lw 3
set xlabel "Generations"
set ylabel "Solution size"
stats '01 - Kill parents and transfer 10%.txt' using 4 nooutput
min1 = STATS_min
stats '01 - Kill parents and transfer 10%.txt' using 5 nooutput
time1 = STATS_max
stats '02 - Kill parents and transfer 5%.txt' using 4 nooutput
min2 = STATS_min
stats '02 - Kill parents and transfer 5%.txt' using 5 nooutput
time2 = STATS_max
stats '03 - Kill parents, no transfer.txt' using 4 nooutput
min3 = STATS_min
stats '03 - Kill parents, no transfer.txt' using 5 nooutput
time3 = STATS_max
stats '04 - Best and transfer 10%.txt' using 4 nooutput
min4 = STATS_min
stats '04 - Best and transfer 10%.txt' using 5 nooutput
time4 = STATS_max
stats '05 - Best and transfer 5%.txt' using 4 nooutput
min5 = STATS_min
stats '05 - Best and transfer 5%.txt' using 5 nooutput
time5 = STATS_max
stats '06 - Best, no transfer.txt' using 4 nooutput
min6 = STATS_min
stats '06 - Best, no transfer.txt' using 5 nooutput
time6 = STATS_max
plot '01 - Kill parents and transfer 10%.txt' using 1:4 smooth cspline ls 1 title sprintf("Kill parents and transfer 10%%, min: %d, time: %d s.", min1, time1), '02 - Kill parents and transfer 5%.txt' using 1:4 smooth cspline ls 2 title sprintf("Kill parents and transfer 5%%, min: %d, time: %d s.", min2, time2), '03 - Kill parents, no transfer.txt' using 1:4 smooth cspline ls 3 title sprintf("Kill parents, no transfer, min: %d, time: %d s.", min3, time3), '04 - Best and transfer 10%.txt' using 1:4 smooth cspline ls 4 title sprintf("Best and transfer 10%%, min: %d, time: %d s.", min4, time4), '05 - Best and transfer 5%.txt' using 1:4 smooth cspline ls 5 title sprintf("Best and transfer 5%%, min: %d, time: %d s.", min5, time5), '06 - Best, no transfer.txt' using 1:4 smooth cspline ls 6 title sprintf("Best, no transfer, min: %d, time: %d s.", min6, time6)
set yr [GPVAL_DATA_Y_MIN:GPVAL_DATA_Y_MAX + 10]
replot
set term png medium
set output "Plot.png"
replot
set term windows