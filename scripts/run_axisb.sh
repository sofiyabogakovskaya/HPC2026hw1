mkdir -p results

gcc-15 Axisb.c -o axisb -fopenmp -lm

for threads in 1 2 4 8
do
    OMP_NUM_THREADS=$threads ./axisb 1200 1000 >> results/axisb_threads.txt
    printf '\n' >> results/axisb_threads.txt
done

rm axisb
