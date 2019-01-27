# set terminal svg size 1920,1080 fname 'Verdana'
# set output 'time_comparison.eps'
set terminal png size 1920,1080
set output 'performance.png'
array Class[3] = ["Pthread", "OpenBLAS", "MKL"]
set logscale y 2
set boxwidth 1 absolute
set style data histograms
set style histogram clustered gap 4 
set title "Mean and Standard deviation of latencies of Pthread, OpenBLAS and MKL implementations"
set style fill solid 1.0 border lt -1
set errorbars fullwidth
set style fill solid 1 border lt -1
set style histogram errorbars gap 5 lw 2
plot for [COL=2:6:2] 'comparison.dat' using COL:COL+1:xticlabels(1) title Class[COL/2]