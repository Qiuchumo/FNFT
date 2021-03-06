# Changelog

## [0.2.1] -- 2018-09-28

### Fixed

- Computation of norming constants from residues in fnft_nsev_inverse was incorrect when continuous spectrum was present.

## [0.2.0] -- 2018-09-21

### Added

- New public function fnft_nsev_inverse implementing a fast inverse NFT for the vanishing nonlinear Schroedinger equation (the inversion of the continuous spectrum is fast, bound states are added using a standard Darboux transform)
- New public function fnft_version returning current version of FNFT
- The version of FNFT can now be supplemented with a suffix
- The FFTW library can be used to compute internal FFTs instead of the standard Kiss FFT
- Building of the tests can now be turned off
- The number of samples D in fnft_nsev and fnft_kdvv no longer has to be a power of two
- The subsampling factor in the SUBSAMPLE_AND_REFINE method for locating the discrete spectrum in fnft_nsev can now be controlled by the user
- System-wide installation using "make install" is now possible
- New private function fnft__nse_finvscatter implementing fast inverse scattering
- New private function fnft__poly_specfact implementing Kolmogorov's method for spectral factorization
- New private function fnft__poly_eval implementing Horner's method
- New private functions fnft__akns_fscatter and fnft__akns_scatter_matrix implementing forward scattering for general AKNS systems
- New private functions fnft__akns_lambda_to_z and fnft__akns_z_to_lambda that centralize the continuous-time/discrete-time coordinate transforms

### Changed

- Forward scattering functions fnft__nse_fscatter, fnft__nse_scatter_matrix, fnft__kdv_fscatter and fnft__kdv_scatter_matrix now make use of their more general AKNS counter parts

### Fixed

- M could not be much larger than D in fnft_nsev
- dists[1] was not set correctly under some circumstances in nsev_compare_nfs
- Some public headers were including private ones
- Compiler warning about __Thread_local

## [0.1.1] -- 2018-05-14

### Added

- Mex files now compile also with Matlab R2018a

### Changed

- Fixed: Several potential memory violations in fnft_nsev, poly_chirpz and nsev_testcases
- Fixed: Some return codes in fnft_nsep had not been checked
- Fixed: misc_merge did not work correctly for empty input vectors
- Fixed: The continuous spectrum in mex_fnft_nsev_example.m was plotted over t, not xi
- Fixed: A superfluous parameter kappa was given in the documentation of mex_fnft_kdvv
