#include "functions.h"
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

double *arr1;
double *arr2;
double *arr3;
double user_input;

/**
 * @brief Fill Array 1 with the user input
 *
 * Function will be called to fill a double array
 * with the value chosen by the user.
 *
 * @param param represents the index to differentiate threads
 */
void *fillArray1(void *param)
{
    int *i;
    i = (int *)param;
    if (i == 0)
    {

        for (int j = 0; j < 250000; j++)
        {
            arr1[j] = user_input;
        }
    }
    else if (i == 1)
    {

        for (int j = 250000; j < 500000; j++)
        {
            arr1[j] = user_input;
        }
    }
    else if (i == 2)
    {

        for (int j = 500000; j < 750000; j++)
        {
            arr1[j] = user_input;
        }
    }
    else if (i == 3)
    {

        for (int j = 750000; j < 1000000; j++)
        {
            arr1[j] = user_input;
        }
    }
}

/**
 * @brief Fill Array 2 with the user input
 *
 * Function will be called to fill an array of type double
 * with the value chosen by the user.
 *
 * @param param represents the index to differentiate threads
 */
void *fillArray2(void *param)
{
    int *i;
    i = (int *)param;
    if (i == 0)
    {

        for (int j = 0; j < 250000; j++)
        {
            arr2[j] = user_input;
        }
    }
    else if (i == 1)
    {

        for (int j = 250000; j < 500000; j++)
        {
            arr2[j] = user_input;
        }
    }
    else if (i == 2)
    {

        for (int j = 500000; j < 750000; j++)
        {
            arr2[j] = user_input;
        }
    }
    else if (i == 3)
    {

        for (int j = 750000; j < 1000000; j++)
        {
            arr2[j] = user_input;
        }
    }
}

/**
 * @brief Sum elements from two arrays into a result array
 *
 * Function will sum elements at the i-th index of array #1
 * and array #2 into the result array array #3
 *
 * @param param represents the index to differentiate threads
 */
void *Sum(void *param)
{
    int *i;
    i = (int *)param;
    if (i == 0)
    {

        for (int j = 0; j < 250000; j++)
        {
            arr3[j] = arr1[j] + arr2[j];
        }
    }
    if (i == 1)
    {

        for (int j = 250000; j < 500000; j++)
        {
            arr3[j] = arr1[j] + arr2[j];
        }
    }
    if (i == 2)
    {

        for (int j = 500000; j < 750000; j++)
        {
            arr3[j] = arr1[j] + arr2[j];
        }
    }
    if (i == 3)
    {

        for (int j = 750000; j < 1000000; j++)
        {
            arr3[j] = arr1[j] + arr2[j];
        }
    }
}

/**
 * @brief Multiply elements from two arrays into a result array
 *
 * Function will multiply elements at the i-th index of array #1
 * and array #2 into the result array array #3
 *
 * @param param represents the index to differentiate threads
 */
void *Multiply(void *param)
{

    int *i;
    i = (int *)param;
    if (i == 0)
    {

        for (int j = 0; j < 250000; j++)
        {
            arr3[j] = arr1[j] * arr2[j];
        }
    }
    if (i == 1)
    {

        for (int j = 250000; j < 500000; j++)
        {
            arr3[j] = arr1[j] * arr2[j];
        }
    }
    if (i == 2)
    {

        for (int j = 500000; j < 750000; j++)
        {
            arr3[j] = arr1[j] * arr2[j];
        }
    }
    if (i == 3)
    {

        for (int j = 750000; j < 1000000; j++)
        {
            arr3[j] = arr1[j] * arr2[j];
        }
    }
}

/**
 * @brief Executes all program logic
 *
 * Provides a user interface in the terminal to interact with three arrays
 * that execute operations (sum, multiplication, plot) with multi-threading
 * and provides display of metrics
 *
 */
void mainProcess()
{

    struct timespec start, end;
    double elapsed_time;

    arr1 = (double *)malloc(sizeof(double) * 1000000);
    arr2 = (double *)malloc(sizeof(double) * 1000000);
    arr3 = (double *)malloc(sizeof(double) * 1000000);

    for (int i = 0; i < 1000000; i++)
    {
        arr1[i] = 2.0;
        arr2[i] = 2.0;
        arr3[i] = 2.0;
    }

    pthread_t thread_counter[4];

    printf("\n arr1 and arr2 were initialized with value 2.0 at every idx\n");
    printf("\n Program A Start \n");

    int loop_val = 1;
    while (loop_val)
    {
        sleep(1);
        printf("  --------- \n");
        printf(" | M e n u |\n");
        printf("  --------- \n");
        printf("__________________________________\n");
        printf("0. Exit program                         |\n");
        printf("1. Fill Array 1                         |\n");
        printf("2. Fill Array 2                         |\n");
        printf("3. Add Arrays                           |\n");
        printf("4. Multiply Arrays                      |\n");
        printf("5. Plot Results                         |\n");
        printf("6. Metrics for 4 Threads                |\n");
        printf("7. Metrics for 1 Thread                 |\n");
        printf("Enter your selection here: ");
        int input;
        scanf("%d", &input);

        printf("\n\n --------------------------- \n\n");
        switch (input)
        {
        case 0:
            loop_val = 0;
            break;

        // user input for arr1
        case 1:

            printf("Enter input: ");
            scanf("%lf", &user_input);
            printf("\n%lf entered\n", user_input);
            for (int i = 0; i < 4; i++)
            {
                pthread_create(&thread_counter[i], NULL, fillArray1, (void *)i);
                for (int i = 0; i < 4; i++)
                {
                    pthread_join(thread_counter[i], NULL);
                }
            }
            break;
        // user input for arr2
        case 2:
            printf("Enter input: ");
            scanf("%lf", &user_input);
            printf("\n%lf entered\n", user_input);
            for (int i = 0; i < 4; i++)
            {
                pthread_create(&thread_counter[i], NULL, fillArray2, (void *)i);
                for (int i = 0; i < 4; i++)
                {
                    pthread_join(thread_counter[i], NULL);
                }
            }
            break;
        case 3:

            for (int i = 0; i < 4; i++)
            {
                pthread_create(&thread_counter[i], NULL, Sum, (void *)i);
                for (int i = 0; i < 4; i++)
                {
                    pthread_join(thread_counter[i], NULL);
                }
            }

            break;
        case 4:

            for (int i = 0; i < 4; i++)
            {
                pthread_create(&thread_counter[i], NULL, Multiply, (void *)i);
                for (int i = 0; i < 4; i++)
                {
                    pthread_join(thread_counter[i], NULL);
                }
            }

            break;
        case 5:

            for (int i = 0; i < 10; i++)
            {
                printf("Element at index %d: %f\n", i, arr3[i]);
            }
            break;

        case 6:
            for (int ctr = 0; ctr < 10; ctr++)
            {
                printf("Iteration: %d \n", ctr + 1);
                for (int i = 0; i < 4; i++)
                {
                    clock_gettime(CLOCK_MONOTONIC, &start);

                    pthread_create(&thread_counter[i], NULL, Multiply, (void *)i);
                    for (int i = 0; i < 4; i++)
                    {
                        pthread_join(thread_counter[i], NULL);
                    }

                    clock_gettime(CLOCK_MONOTONIC, &end);
                    elapsed_time =
                        (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
                    printf("%f\n", elapsed_time);
                }
            }
            break;
        case 7:

            for (int ctr = 0; ctr < 10; ctr++)
            {
                printf("Iteration: %d \n", ctr + 1);
                clock_gettime(CLOCK_MONOTONIC, &start);
                for (int j = 0; j < 1000000; j++)
                {

                    arr3[j] = arr1[j] * arr2[j];
                }
                clock_gettime(CLOCK_MONOTONIC, &end);
                elapsed_time =
                    (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1E9;
                printf("%f\n", elapsed_time);
            }
            break;
        default:
            printf("Invalid option!\n");
        }
    }

    printf("end of program\n");

    return 0;
}
