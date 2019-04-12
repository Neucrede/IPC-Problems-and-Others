#include "C:/Program Files/scilab-5.5.1/modules/scicos_blocks/includes/scicos_block4.h"
#include <math.h>

double sgn(double x);
double sat(double x, double d);
double fst(double e, double x, double r, double h);

/* 
 * Input ports (1)
 *      [u]
 * Output ports (2)
 *      [y1, y2]
 * Activation input ports (1)
 *      [e]
 * Discrete states (2)
 *      [z1, z2]
 * Real parameters (3)
 *      [h, r, h1]
 *
 */

void adrc_td2(scicos_block* block, int flag)
{
    double*     y = GetOutPortPtrs(block, 1);
    double*     u = GetInPortPtrs(block, 1);
    double*     z = GetDstate(block);
    double      prev_z1 = z[0];
    double      prev_z2 = z[1];
    double*     rparptr = GetRparPtrs(block);

    double      h = rparptr[0];     /* Step size */
    double      r = rparptr[1];     /* Velocity factor */
    double      h1 = rparptr[2];    /* Filtering factor */

    switch (flag)
    {
        case 4:     /* Initialization */
            break;
        case 1:     /* Output update */
            y[0] = z[0];
            y[1] = z[1];
            break;
        case 0:     /* Integrator call */
            break;
        case 2:     /* State update */
            z[0] = prev_z1 + h * prev_z2;
            z[1] = prev_z2 + h * fst(prev_z1 - *u, prev_z2, r, h1);
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

double sat(double x, double d)
{
    if (fabs(x) < d)
    {
        return (x / d);
    }
    else
    {
        return sgn(x);
    }

    return 0.0;
}

double fst(double e, double x, double r, double h)
{
    double  d       = h * r;
    double  d1      = h * d;
    double  y       = e + h * x;
    double  yabs    = fabs(y);
    double  g       = 0.0;

    if (yabs <= d1)
    {
        g = x + (y / h);
    }
    else
    {
        g = x - sgn(y) * (0.5 * (d - sqrt(8 * r * yabs + pow(d, 2.0))));
    }

    return (-r * sat(g, d));
}
