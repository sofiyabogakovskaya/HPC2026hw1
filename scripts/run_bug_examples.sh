mkdir -p results

gcc-15 BugReduction.c -o bugreduction -fopenmp
OMP_NUM_THREADS=4 ./bugreduction > results/bug_reduction.txt
rm bugreduction

gcc-15 BugParFor.c -o bugparfor -fopenmp
OMP_NUM_THREADS=4 ./bugparfor > results/bug_par_for.txt
rm bugparfor
