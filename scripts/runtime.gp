reset
set ylabel 'time(sec)'
set style fill solid
set title 'perfomance comparison'
set term png enhanced font 'Verdana,10'
set output 'runtime.png'
 
plot [:][:0.0002]'output.txt' using 3:xtic(1) with histogram title '128 bucket'  , \
'' using 4:xtic(1) with histogram title '5120 bucket'  , \
'' using 5:xtic(1) with histogram title '10240 bucket'  , \
'' using 6:xtic(1) with histogram title '20480 bucket'  