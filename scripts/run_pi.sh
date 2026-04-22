mkdir -p results

gcc-15 Pi.c -o pi -fopenmp

for threads in 1 2 4 8
do
    OMP_NUM_THREADS=$threads ./pi 50000000 >> results/pi_threads.txt
    printf '\n' >> results/pi_threads.txt
done

rm pi
