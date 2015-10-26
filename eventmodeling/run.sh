#!/bin/bash
make && mpirun -np 16 ./dist_event
