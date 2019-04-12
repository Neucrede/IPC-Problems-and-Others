#include "C:/Program Files/scilab-5.5.1/modules/scicos_blocks/includes/scicos_block4.h"
#include <math.h>

double sgn(double x);
double fal(double e, double a, double d);

/* 
 * Input ports (1)
 *      [u, y_in]
 * Output ports (3)
 *      [y1, y2, y3]
 * Activation input ports (1)
 *      [e]
 * Discrete states (3)
 *      [z1, z2, z3]
 * Real parameters (8)
 *      [h, alpha1, alpha2, beta01, beta02, beta03, b, d]
 *
 */

void adrc_eso2(scicos_block* block, int flag)
{
    double*     y = GetOutPortPtrs(block, 1);
    double*     u = GetInPortPtrs(block, 1);
    double*     z = GetDstate(block);
    double      prev_z1 = z[0];
    double      prev_z2 = z[1];
    double      prev_z3 = z[2];
    double*     rparptr = GetRparPtrs(block);

    double      h = rparptr[0];     /* Step size*/
    double      alpha1 = rparptr[1];
    double      alpha2 = rparptr[2];
    double      beta01 = rparptr[3];
    double      beta02 = rparptr[4];
    double      beta03 = rparptr[5];
    double      b = rparptr[6];
    double      d = rparptr[7];

    double e;
    switch (flag)
    {
        case 4:     /* Initialization */
            break;
        case 1:     /* Output update */
            y[0] = z[0];
            y[1] = z[1];
            y[2] = z[2];
            break;
        case 0:     /* Integrator call */
            break;
        case 2:     /* State update */
            e = prev_z1 - (u[1]);
            z[0] = prev_z1 + h * (prev_z2 - beta01 * e);
            z[1] = prev_z2 + h * (prev_z3 - beta02 * fal(e, alpha1, d) + b * u[0]);
            z[2] = prev_z3 - h * (beta03 * fal(e, alpha2, d));
            break;
        case 5:     /* Ending */
            break;
        default:
            break;
    }
}

double sgn(double x)
{
    if (x == 0.0)
    {
        return 0.0;
    }

    if (x > 0.0)
    {
        return 1.0;
    }
    else
    {
        return -1.0;
    }

    return 0.0;
}

double fal(double e, double a, double d)
{
    if (fabs(e) > d)
    {
        return pow(fabs(e), a) * sgn(e);
    }
    else
    {
        return e / pow(d, 1-a);
    }

    return 0.0;
}

