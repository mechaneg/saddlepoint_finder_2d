#ifndef ROOTF_H
#define ROOTF_H

/*
 *                     INPUT:
 *                     opr - double precision vector of real coefficients in order of
 *                          decreasing powers.
 *                     opi - double precision vector of imaginary coefficients in order of
 *                          decreasing powers.
 *                     degree - integer degree of polynomial
 *
 *                     OUTPUT:
 *                     zeror,zeroi - output double precision vectors of the
 *                          real and imaginary parts of the zeros.
 *                            to be consistent with rpoly.cpp the zeros is inthe index
 *                            [0..max_degree-1]
 *
 *                     RETURN:
 *                     returnval:   -1 if leading coefficient is zero, otherwise
 *                          number of roots found. 
*/


#define MAXDEGREE 100
#define MDP1 MAXDEGREE+1


namespace JT
{

	int cpoly( const double *opr, const double *opi, int degree, double *zeror, double *zeroi );
	
	static double sr, si, tr, ti, pvr, pvi, are, mre, eta, infin;
	static int nn;
	static double *pr, *pi, *hr, *hi, *qpr, *qpi, *qhr, *qhi, *shr, *shi; 


	static void noshft( const int l1 );
	static void fxshft( const int l2, double *zr, double *zi, int *conv );
	static void vrshft( const int l3, double *zr, double *zi, int *conv );
	static void calct( int *bol );
	static void nexth( const int bol );
	static void polyev( const int nn, const double sr, const double si, const double pr[], \
		const double pi[], double qr[], double qi[], double *pvr, double *pvi );

	static double errev( const int nn, const double qr[], const double qi[], const double ms, \
		const double mp, const double are, const double mre );

	static void cauchy( const int nn, double pt[], double q[], double *fn_val );
	static double scale( const int nn, const double pt[], const double eta, const double infin, const double smalno, const double base );
	static void cdivid( const double ar, const double ai, const double br, const double bi, double *cr, double *ci );
	static double cmod( const double r, const double i );
	static void mcon( double *eta, double *infiny, double *smalno, double *base );

};

#endif /* ROOTF_H */