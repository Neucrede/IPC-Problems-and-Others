#include "C:/Program Files/scilab-5.5.2/modules/scicos_blocks/includes/scicos_block4.h"

/* 
 * Input ports (1)
 *      [u]
 * Output ports (1)
 *      [y]
 * Activation input ports (1)
 *      [e]
 * Discrete states (0)
 *      []
 * Continuous states (2)
 *      [x1, x2]
 * Real parameters (3)
 *      [T, alpha, k]
 *
 */

void norrbin(scicos_block* block, int flag)
{
    double*     y = GetOutPortPtrs(block, 1);
    double*     u = GetInPortPtrs(block, 1);
    double*     rparptr = GetRparPtrs(block);
    double*     x = GetState(block);
    double*     xd = GetDerState(block);

    double      T = rparptr[0];
    double      alpha = rparptr[1];
    double      k = rparptr[2];

    switch (flag)
    {
        case 4:     /* Initialization */
            break;
        case 1:     /* Output update */
            *y = x[0];
            break;
        case 0:     /* Integrator call */
            xd[0] = x[1];
            xd[1] = -(1.0 / T) * (x[1] + alpha * pow(x[1], 3.0)) + 
                    (k / T) * (*u);
            break;
        case 2:     /* State update */
            break;
        case 5:     /* Ending */
            break;
        default:
            break;
    }
}

