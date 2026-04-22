mkdir -p results

gcc-15 LeastSquares.c -o least_squares -fopenmp -lm

for threads in 1 2 4 8
do
    OMP_NUM_THREADS=$threads ./least_squares 5000000 >> results/least_squares_threads.txt
    printf '\n' >> results/least_squares_threads.txt
done

rm least_squares
