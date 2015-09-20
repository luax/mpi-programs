#!/bin/bash
make && mpirun -np 128 $1
