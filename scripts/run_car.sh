mkdir -p results
rm -f car_frame_*.ppm

g++-15 Car.cpp -o car -fopenmp

OMP_NUM_THREADS=4 ./car 30 30 >> results/car_timing.txt
printf '\n' >> results/car_timing.txt
OMP_NUM_THREADS=4 ./car 30 15 >> results/car_timing.txt
printf '\n' >> results/car_timing.txt
OMP_NUM_THREADS=4 ./car 30 1 >> results/car_timing.txt

rm car
rm -f car_frame_*.ppm
