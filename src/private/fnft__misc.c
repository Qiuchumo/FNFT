/*
* This file is part of FNFT.
*
* FNFT is free software; you can redistribute it and/or
* modify it under the terms of the version 2 of the GNU General
* Public License as published by the Free Software Foundation.
*
* FNFT is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contributors:
* Sander Wahls (TU Delft) 2017.
* Peter J Prins (TU Delft) 2018
*/
#define FNFT_ENABLE_SHORT_NAMES

#include "fnft__errwarn.h"
#include <stdio.h>
#include "fnft__misc.h"

void misc_print_buf(const INT len, COMPLEX const * const buf,
                    char const * const varname)
{
    INT i;
    printf("%s = [", varname);
    for (i = 0; i < len; i++) {
        printf("%g+%gj", CREAL(buf[i]), CIMAG(buf[i]));
        if (i != len-1)
            printf(", ");
    }
    printf("];\n");
}

REAL misc_rel_err(const INT len, COMPLEX const * const vec_numer,
    COMPLEX const * const vec_exact)
{
    INT i;
    double n = 0.0, d = 0.0;
    for (i=0; i<len; i++) {
        n += CABS(vec_numer[i] - vec_exact[i]);
        d += CABS(vec_exact[i]);
    }
    return n/d;
}

REAL misc_hausdorff_dist(const UINT lenA,
    COMPLEX const * const vecA, const UINT lenB,
    COMPLEX const * const vecB)
{
    UINT i, j;
    double tmp, dist, max_dist = -1.0;

    for (i=0; i<lenA; i++) {
        dist = INFINITY;
        for (j=0; j<lenB; j++) {
            tmp = CABS(vecA[i] - vecB[j]);
            if (tmp < dist)
                dist = tmp;
        }
        if (dist > max_dist)
            max_dist = dist;
    }

    for (j=0; j<lenB; j++) {
        dist = INFINITY;
        for (i=0; i<lenA; i++) {
            tmp = CABS(vecA[i] - vecB[j]);
            if (tmp < dist)
                dist = tmp;
        }
        if (dist > max_dist)
            max_dist = dist;
    }

    return max_dist;
}

COMPLEX misc_sech(COMPLEX Z)
{
    return 2.0 / (CEXP(Z) + CEXP(-Z));
}

REAL misc_l2norm2(const UINT N, COMPLEX const * const Z,
    const REAL a, const REAL b)
{
    REAL val, h, tmp;
    UINT i;

    // Check inputs
    if (N < 2 || a >= b)
        return NAN;

    // Integrate |q(t)|^2 numerically
    h = (b - a)/N;
    tmp = CABS(Z[0]);
    val = 0.5 * h * tmp * tmp;
    for (i=1; i<N-1; i++) {
        tmp = CABS(Z[i]);
        val += h * tmp * tmp;
    }
    tmp = CABS(Z[N-1]);
    val += 0.5 * h * tmp * tmp;

    return val;
}

INT misc_filter(UINT * const N_ptr, COMPLEX * const vals,
    COMPLEX * const rearrange_as_well,
    REAL const * const bounding_box)
{
    UINT i, N_local, N_filtered;

    if (N_ptr == NULL)
        return E_INVALID_ARGUMENT(N_ptr);
    if (vals == NULL)
        return E_INVALID_ARGUMENT(vals);
    if (bounding_box == NULL)
        return E_INVALID_ARGUMENT(bounding_box);
    if ( !(bounding_box[0] <= bounding_box[1]) //!(...) ensures error with NANs
        || !(bounding_box[2] <= bounding_box[3]) )
        return E_INVALID_ARGUMENT(bounding_box);

    N_filtered = 0; // Will contain number of values that survived the
                    // filtering (the current no of candidates is in N)
    N_local = *N_ptr;

    for (i=0; i<N_local; i++) {

        // Check if this value is in the bounding box, proceed to next value
        // if not. Due to the formulation, NANs are not in the box.

        if (! (CREAL(vals[i]) >= bounding_box[0]) )
            continue;
        if (! (CREAL(vals[i]) <= bounding_box[1]) )
            continue;
        if (! (CIMAG(vals[i]) >= bounding_box[2]) )
            continue;
        if (! (CIMAG(vals[i]) <= bounding_box[3]) )
            continue;

        // Keep value since no reason to skip it has been found
        vals[N_filtered] = vals[i];
        if (rearrange_as_well != NULL)
            rearrange_as_well[N_filtered] = rearrange_as_well[i];
        N_filtered++;
    }
    *N_ptr = N_filtered;

    return SUCCESS;
}

INT misc_filter_inv(UINT * const N_ptr, COMPLEX * const vals,
    COMPLEX * const rearrange_as_well,
    REAL const * const bounding_box)
{
    UINT i, N_local, N_filtered;
    INT ok_flag;

    if (N_ptr == NULL)
        return E_INVALID_ARGUMENT(N_ptr);
    if (vals == NULL)
        return E_INVALID_ARGUMENT(vals);
    if (bounding_box == NULL)
        return E_INVALID_ARGUMENT(bounding_box);
    if ( !(bounding_box[0] <= bounding_box[1]) //!(...) ensures error with NANs
        || !(bounding_box[2] <= bounding_box[3]) )
        return E_INVALID_ARGUMENT(bounding_box);

    N_filtered = 0; // Will contain number of values that survived the
                    // filtering (the current no of candidates is in N)
    N_local = *N_ptr;

    for (i=0; i<N_local; i++) {

        // Check if this value is in the bounding box, proceed to next value
        // if so. Due to the formulation, NANs are skipped.
        ok_flag = 0; // not ok
        if (!(CREAL(vals[i]) > bounding_box[0]))
            ok_flag = 1;
        if (!(CREAL(vals[i]) < bounding_box[1]))
            ok_flag = 1;
        if (!(CIMAG(vals[i]) > bounding_box[2]))
            ok_flag = 1;
        if (!(CIMAG(vals[i]) < bounding_box[3]))
            ok_flag = 1;
        if (ok_flag == 1) {
            vals[N_filtered] = vals[i];
            if (rearrange_as_well != NULL)
                rearrange_as_well[N_filtered] = rearrange_as_well[i];
            N_filtered++;
        }
    }
    *N_ptr = N_filtered;

    return SUCCESS;
}

INT misc_filter_nonreal(UINT *N_ptr, COMPLEX * const vals, const REAL tol_im)
{
    UINT i, N_local, N_filtered;

    if (N_ptr == NULL)
        return E_INVALID_ARGUMENT(N_ptr);
    if (vals == NULL)
        return E_INVALID_ARGUMENT(vals);
    if (!(tol_im >= 0))
        return E_INVALID_ARGUMENT(tol_im);

    N_local = *N_ptr;
    N_filtered = 0;
    for (i=0; i<N_local; i++) {
        if (!(FABS(CIMAG(vals[i])) > tol_im))
            continue;
        vals[N_filtered++] = vals[i];
    }
    *N_ptr = N_filtered;

    return SUCCESS;
}

INT misc_merge(UINT *N_ptr, COMPLEX * const vals, REAL tol)
{
    REAL dist = -1.0;
    UINT i, j, N, N_filtered;

    if (N_ptr == NULL)
        return E_INVALID_ARGUMENT(N_ptr);
    if (*N_ptr == 0)
        return SUCCESS;
    if (vals == NULL)
        return E_INVALID_ARGUMENT(vals);
    if (tol < 0.0)
        return E_INVALID_ARGUMENT(tol);

    N = *N_ptr;
    N_filtered = 1;
    for (i=1; i<N; i++) {
        for (j = 0; j < i; j++) {
            dist = CABS(vals[j] - vals[i]);
            if (dist < tol)
                break;
        }
        if (dist < tol)
            continue;

        // Keep bound value since it is not close to previous values
        vals[N_filtered++] = vals[i];
    }
    *N_ptr = N_filtered;

    return SUCCESS;
}

INT misc_downsample(const UINT D, COMPLEX const * const q,
    UINT * const Dsub_ptr, COMPLEX ** qsub_ptr, UINT * const first_last_index)
{
    if (q == NULL)
        return E_INVALID_ARGUMENT(q);
    if (D <= 2)
        return E_INVALID_ARGUMENT(D);
    if (qsub_ptr == NULL)
        return E_INVALID_ARGUMENT(qsub_ptr);
    if (Dsub_ptr == NULL)
        return E_INVALID_ARGUMENT(Dsub_ptr);
    if (first_last_index == NULL)
        return E_INVALID_ARGUMENT(first_last_index);

    // Determine number of samples after downsampling, Dsub
    UINT Dsub = *Dsub_ptr; // desired Dsub
    if (Dsub < 2)
       Dsub = 2;
    if (Dsub > D)
        Dsub = D;
    const UINT nskip_per_step = ROUND((REAL)D / Dsub);
    Dsub = ROUND((REAL)D / nskip_per_step); // actual Dsub

    COMPLEX * const qsub = malloc(Dsub * sizeof(COMPLEX));
    if (qsub == NULL)
        return E_NOMEM;

    // Perform the downsampling
    UINT isub, i = 0;
    for (isub=0; isub<Dsub; isub++) {
        qsub[isub] = q[i];
        i += nskip_per_step;
    }

    // Original index of the first and last sample in qsub
    first_last_index[0] = 0;
    first_last_index[1] = i - 1;

    *qsub_ptr = qsub;
    *Dsub_ptr = Dsub;
    return SUCCESS;
}

// Computes sinc(x):= 1 if x=0 and sin(x)/x otherwise. If x is close to 0, the
// calculation is approximated with sinc(x) = cos(x/sqrt(3)) + O(x^4)
COMPLEX misc_CSINC(COMPLEX x)
{
    const REAL sinc_th=1.0E-8;

    if (CABS(x)>=sinc_th)
        return CSIN(x)/x;
    else
        return CCOS(x/CSQRT(3));
}

UINT misc_nextpowerof2(const UINT number)
{
    if (number == 0)
        return 0;
    UINT result = 1;
    while (result < number)
        result *= 2;
    return result;
}
