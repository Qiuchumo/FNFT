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
 * Sander Wahls (TU Delft) 2018.
 */

#include "fnft_nsev_inverse_test_against_forward.inc"

int main()
{
    const INT kappa = -1;
    const nse_discretization_t discretization =
        nse_discretization_2SPLIT2_MODAL;
    UINT M;
    REAL error_bound;
    INT ret_code;

    M = 10;
    error_bound = 0.017;
    ret_code = fnft_nsev_inverse_test(M, error_bound, kappa, discretization);
    if (ret_code != SUCCESS) {
        return EXIT_FAILURE;
    }

    M = 32;
    error_bound = 7.5e-9;
    ret_code = fnft_nsev_inverse_test(M, error_bound, kappa, discretization);
    if (ret_code != SUCCESS) {
        return EXIT_FAILURE;
    }

	return EXIT_SUCCESS;
}
