#!/bin/bash
#PBS -q large
#PBS -l select=1:ncpus=16:mpiprocs=4
#PBS -l walltime=00:01:00
#PBS -N job_h
#PBS -o stdoe
#PBS -j oe

source /etc/profile.d/modules.sh
module load compiler mpt
export OMP_NUM_THREADS=4
export KMP_AFFINITY=disabled
cd ${PBS_O_WORKDIR}

mpiexec_mpt omplace -nt ${OMP_NUM_THREADS} ./a.out
