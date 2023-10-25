/**
 * @file ex3.c
 * @author Keoni Burns
 * @brief implements left Riemann, Trapezoidal, and Simpsons to find the integral of sin
 * adapted from mpi_trap.4 provided by profssor Sam Siewert
 * @version 0.1
 * @date 2023-10-24
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "ex4accel.h"
// #include "ex4vel.h"

// io and mesasage passing protos
void Build_mpi_type(double* a_p, double* b_p, int* n_p, MPI_Datatype* input_mpi_t_p);
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p);
double table_interp(double time);
double table_function(int timeidx);

// integral function protos
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len);
double LeftRiemann(double left_endpt, double right_endpt, int rect_count, double base_len);
double Simpson(double left_endpt, double right_endpt, int rect_count);

// sin
double f(double x);

int main(void) {
    int my_rank, comm_sz, n, local_n;
    double a, b, h, local_a, local_b;
    double local_int, total_int;

    /* Let the system do what it needs to start up MPI */
    MPI_Init(NULL, NULL);

    /* Get my process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out how many processes are being used */
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    Get_input(my_rank, comm_sz, &a, &b, &n);

    h = (b - a) / n;       /* h is the same for all processes */
    local_n = n / comm_sz; /* So is the number of trapezoids  */

    /* Length of each process' interval of
     * integration = local_n*h.  So my interval
     * starts at: */
    local_a = a + my_rank * local_n * h;
    local_b = local_a + local_n * h;

    printf("simpsons rule\n");
    local_int = Simpson(local_a, local_b, local_n);

    /* Add up the integrals calculated by each process */
    MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    /* Print the result */
    if (my_rank == 0) {
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %20.15f\n", a, b, total_int);
    }

    /* Shut down MPI */
    MPI_Finalize();

    return 0;
} /*  main  */

/**
 * @brief taken from mpi_trap4.c it builds a data type to pass variables between nodes
 *
 */
void Build_mpi_type(double* a_p, double* b_p, int* n_p, MPI_Datatype* input_mpi_t_p) {
    int array_of_blocklengths[3] = {1, 1, 1};
    MPI_Datatype array_of_types[3] = {MPI_DOUBLE, MPI_DOUBLE, MPI_INT};
    MPI_Aint a_addr, b_addr, n_addr;
    MPI_Aint array_of_displacements[3] = {0};

    MPI_Get_address(a_p, &a_addr);
    MPI_Get_address(b_p, &b_addr);
    MPI_Get_address(n_p, &n_addr);
    array_of_displacements[1] = b_addr - a_addr;
    array_of_displacements[2] = n_addr - a_addr;
    MPI_Type_create_struct(3, array_of_blocklengths, array_of_displacements, array_of_types, input_mpi_t_p);
    MPI_Type_commit(input_mpi_t_p);
} /* Build_mpi_type */

/**
 * @brief also taken from mpi_trap4.c takes the input from the command line and passes it amongst each node
 *
 */
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p) {
    MPI_Datatype input_mpi_t;
    int rc = 0;

    Build_mpi_type(a_p, b_p, n_p, &input_mpi_t);

    if (my_rank == 0) {
        printf("Enter a, b, and n\n");
        rc = scanf("%lf %lf %d", a_p, b_p, n_p);
        if (rc < 0) perror("Get_input");
    }
    MPI_Bcast(a_p, 1, input_mpi_t, 0, MPI_COMM_WORLD);

    MPI_Type_free(&input_mpi_t);
} /* Get_input */

// uses trapezoidal method to integrate
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len) {
    double estimate, x;
    int i;

    estimate = (f(left_endpt) + f(right_endpt)) / 2.0;
    for (i = 1; i <= trap_count - 1; i++) {
        x = left_endpt + i * base_len;
        estimate += f(x);
    }
    estimate = estimate * base_len;

    return estimate;
} /*  Trap  */

double LeftRiemann(double left_endpt, double right_endpt, int rect_count, double base_len) {
    double left_value, x, area = 0.0;
    int i;

    // estimate of function on left side to forward integrate
    x = left_endpt;
    left_value = f(x);

    for (i = 1; i <= rect_count - 1; i++) {
        // add area of each rectangle to overall area sum
        area += left_value * base_len;

        // advance x by base length for new values to add to area
        x += base_len;
        left_value = f(x);
    }

    return area;

} /*  LeftRiemann  */

double Simpson(double left_endpt, double right_endpt, int rect_count) {
    double h = (right_endpt - left_endpt) / rect_count;  // calculate the height
    double sum = f(left_endpt) + f(right_endpt);         // integrate

    for (int i = 1; i < rect_count; i++) {
        double x = left_endpt + i * h;
        // if even multiply by 2 otherwise multiply by 4
        if (i % 2 == 0) {
            sum += 2 * f(x);
        } else {
            sum += 4 * f(x);
        }
    }

    return (h / 3) * sum;
} /*  Simpsons  */

double table_interp(double time) {
    int timeidx = (int)time;
    int timeidx_next = ((int)time) + 1;
    double delta_t = time - (double)((int)time);
    // printf("delta t: %lf\n", delta_t);

    return (table_function(timeidx) + ((table_function(timeidx_next) - table_function(timeidx)) * delta_t));
}
// table look-up for function profile given and velocity profile determined
double table_function(int timeidx) {
    long unsigned int tsize = sizeof(DefaultProfile) / sizeof(double);

    // Check array bounds for look-up table
    if (timeidx > tsize) {
        printf("timeidx=%d exceeds table size = %lu and range %d to %lu\n", timeidx, tsize, 0, tsize - 1);
        exit(-1);
    }

    return DefaultProfile[timeidx];
}

// x is the time
double f(double x) { return (table_interp(x)); } /* f */