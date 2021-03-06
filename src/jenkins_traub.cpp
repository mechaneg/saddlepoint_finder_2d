/// \file jenkins_traub.cpp

#include "jenkins_traub.h"
#include "common_defines.h"

#include <cctype>
#include <cmath>
#include <cfloat>

jenkins_traub::~jenkins_traub ()
{
  FREE_ARR (pr);
  FREE_ARR (pi);
  FREE_ARR (hr);
  FREE_ARR (hi);
  FREE_ARR (qpr);
  FREE_ARR (qpi);
  FREE_ARR (qhr);
  FREE_ARR (qhi);
  FREE_ARR (shr);
  FREE_ARR (shi);
}

int jenkins_traub::init (unsigned max_degree)
{
  max_deg = max_degree;

  if (check () < 0)
    return -1;

  pr  = new double[max_deg + 1];
  pi  = new double[max_deg + 1];
  hr  = new double[max_deg + 1];
  hi  = new double[max_deg + 1];
  qpr = new double[max_deg + 1];
  qpi = new double[max_deg + 1];
  qhr = new double[max_deg + 1];
  qhi = new double[max_deg + 1];
  shr = new double[max_deg + 1];
  shi = new double[max_deg + 1];

  return 0;
}

int jenkins_traub::check ()
{
  if (max_deg == 0)
    return -1;
  return 0;
}

int jenkins_traub::cpoly (const double *opr, const double *opi, unsigned actual_deg, double *zeror, double *zeroi)
{
  int cnt1, cnt2, idnn2, i, conv;
  double xx, yy, cosr, sinr, smalno, base, xxx, zr, zi, bnd;

  int degree_loc = static_cast<int> (actual_deg);

  mcon (&eta, &infin, &smalno, &base);
  are = eta;
  mre = 2.0 * sqrt ( 2.0 ) * eta;
  xx = 0.70710678;
  yy = -xx;
  cosr = -0.060756474;
  sinr = -0.99756405;
  nn = static_cast<int> (max_deg);

  // Algorithm fails if the leading coefficient is zero
  if (opr[0] == 0 && opi[0] == 0)
    return -1;

  /// Remove the zeros at the origin if any
  while (opr[nn] == 0 && opi[nn] == 0)
    {
      idnn2 = degree_loc - nn;
      zeror[idnn2] = 0;
      zeroi[idnn2] = 0;
      nn--;
    }

  /// Make a copy of the coefficients
  for (i = 0; i <= nn; i++)
    {
      pr[i]  = opr[i];
      pi[i]  = opi[i];
      shr[i] = cmod (pr[i], pi[i]);
    }

  // Scale the polynomial
  bnd = scale (nn, shr, eta, infin, smalno, base);
  if (bnd != 1)
    {
      for (i = 0; i <= nn; i++)
        {
          pr[i] *= bnd;
          pi[i] *= bnd;
        }
    }

  search:
  if (nn <= 1)
   {
     cdivid (-pr[1], -pi[1], pr[0], pi[0], &zeror[degree_loc - 1], &zeroi[degree_loc - 1]);
     goto finish;
   }

  // Calculate bnd, alower bound on the modulus of the zeros
  for (i = 0; i <= nn; i++)
    shr[i] = cmod (pr[i], pi[i]);

  cauchy (nn, shr, shi, &bnd);

  // Outer loop to control 2 Major passes with different sequences of shifts
  for (cnt1 = 1; cnt1 <= 2; cnt1++)
    {
      // First stage  calculation , no shift
      noshft( 5 );

      // Inner loop to select a shift
      for (cnt2 = 1; cnt2 <= 9; cnt2++)
        {
          // Shift is chosen with modulus bnd and amplitude rotated by 94 degree from the previous shif
          xxx = cosr * xx - sinr * yy;
          yy = sinr * xx + cosr * yy;
          xx = xxx;
          sr = bnd * xx;
          si = bnd * yy;

          // Second stage calculation, fixed shift
          fxshft (10 * cnt2, &zr, &zi, &conv);
          if (conv)
            {
              // The second stage jumps directly to the third stage ieration
              // If successful the zero is stored and the polynomial deflated
              idnn2 = degree_loc - nn;
              zeror[idnn2] = zr;
              zeroi[idnn2] = zi;
              nn--;
              for(i = 0; i <= nn; i++)
                {
                  pr[i] = qpr[i];
                  pi[i] = qpi[i];
                }
              goto search;
            }
           // If the iteration is unsuccessful another shift is chosen
        }
       // if 9 shifts fail, the outer loop is repeated with another sequence of shifts
    }

  // The zerofinder has failed on two major passes
  // return empty handed with the number of roots found (less than the original degree)
  degree_loc -= nn;

finish:
  return degree_loc;
}

// COMPUTES  THE DERIVATIVE  POLYNOMIAL AS THE INITIAL H
// POLYNOMIAL AND COMPUTES L1 NO-SHIFT H POLYNOMIALS.
//
void jenkins_traub::noshft (int l1)
{
  int i, j, jj, n, nm1;
  double xni, t1, t2;

  n = nn;
  nm1 = n - 1;
  for (i = 0; i < n; i++)
    {
      xni = nn - i;
      hr[i] = xni * pr[i] / n;
      hi[i] = xni * pi[i] / n;
    }
  for(jj = 1; jj <= l1; jj++)
    {
      if (cmod (hr[n - 1], hi[n - 1]) > eta * 10 * cmod (pr[n - 1], pi[n - 1]))
        {
          cdivid (-pr[nn], -pi[nn], hr[n - 1], hi[n - 1], &tr, &ti);
          for (i = 0; i < nm1; i++)
            {
              j  = nn - i - 1;
              t1 = hr[j - 1];
              t2 = hi[j - 1];
              hr[j] = tr * t1 - ti * t2 + pr[j];
              hi[j] = tr * t2 + ti * t1 + pi[j];
            }
          hr[0] = pr[0];
          hi[0] = pi[0];
        }
      else
        {
          // If the constant term is essentially zero, shift H coefficients
          for (i = 0; i < nm1; i++)
            {
             j = nn - i - 1;
             hr[j] = hr[j - 1];
             hi[j] = hi[j - 1];
            }
          hr[0] = 0;
          hi[0] = 0;
        }
    }
}

// COMPUTES L2 FIXED-SHIFT H POLYNOMIALS AND TESTS FOR CONVERGENCE.
// INITIATES A VARIABLE-SHIFT ITERATION AND RETURNS WITH THE
// APPROXIMATE ZERO IF SUCCESSFUL.
// L2 - LIMIT OF FIXED SHIFT STEPS
// ZR,ZI - APPROXIMATE ZERO IF CONV IS .TRUE.
// CONV  - LOGICAL INDICATING CONVERGENCE OF STAGE 3 ITERATION
//
void jenkins_traub::fxshft (int l2, double *zr, double *zi, int *conv)
{
  int i, j, n;
  int test, pasd, bol;
  double otr, oti, svsr, svsi;

  n = nn;
  polyev (nn, sr, si, pr, pi, qpr, qpi, &pvr, &pvi);
  test = 1;
  pasd = 0;

  // Calculate first T = -P(S)/H(S)
  calct (&bol);

  // Main loop for second stage
  for (j = 1; j <= l2; j++)
    {
      otr = tr;
      oti = ti;

      // Compute the next H Polynomial and new t
      nexth( bol );
      calct( &bol );
      *zr = sr + tr;
      *zi = si + ti;

      // Test for convergence unless stage 3 has failed once or this
      // is the last H Polynomial
      if (!(bol || !test || j == 12))
        {
          if (cmod (tr - otr, ti - oti) < 0.5 * cmod (*zr, *zi))
            {
              if( pasd )
                {
                  // The weak convergence test has been passwed twice, start the third stage
                  // Iteration, after saving the current H polynomial and shift
                  for (i = 0; i < n; i++)
                    {
                      shr[i] = hr[i];
                      shi[i] = hi[i];
                    }
                  svsr = sr;
                  svsi = si;
                  vrshft (10, zr, zi, conv);
                  if (*conv) return;

                  //The iteration failed to converge. Turn off testing and restore h,s,pv and T
                  test = 0;
                  for (i = 0; i < n; i++)
                    {
                      hr[i] = shr[i];
                      hi[i] = shi[i];
                    }
                  sr = svsr;
                  si = svsi;
                  polyev (nn, sr, si, pr, pi, qpr, qpi, &pvr, &pvi);
                  calct (&bol);
                  continue;
                }
              pasd = 1;
            }
          else
            pasd = 0;
        }
    }

  // Attempt an iteration with final H polynomial from second stage
  vrshft (10, zr, zi, conv);
}

// CARRIES OUT THE THIRD STAGE ITERATION.
// L3 - LIMIT OF STEPS IN STAGE 3.
// ZR,ZI   - ON ENTRY CONTAINS THE INITIAL ITERATE, IF THE
//           ITERATION CONVERGES IT CONTAINS THE FINAL ITERATE ON EXIT.
// CONV    -  .TRUE. IF ITERATION CONVERGES
//
void jenkins_traub::vrshft (int l3, double *zr, double *zi, int *conv)
{
   int b, bol;
   int i, j;
   double mp, ms, omp, relstp, r1, r2, tp;

   *conv = 0;
   b = 0;
   sr = *zr;
   si = *zi;

   // Main loop for stage three
   for( i = 1; i <= l3; i++ )
      {
      // Evaluate P at S and test for convergence
      polyev( nn, sr, si, pr, pi, qpr, qpi, &pvr, &pvi );
      mp = cmod( pvr, pvi );
      ms = cmod( sr, si );
      if( mp <= 20 * errev( nn, qpr, qpi, ms, mp, are, mre ) )
         {
         // Polynomial value is smaller in value than a bound onthe error
         // in evaluationg P, terminate the ietartion
         *conv = 1;
         *zr = sr;
         *zi = si;
         return;
         }
      if( i != 1 )
         {
         if( !( b || mp < omp || relstp >= 0.05 ) )
            {
            // Iteration has stalled. Probably a cluster of zeros. Do 5 fixed 
            // shift steps into the cluster to force one zero to dominate
            tp = relstp;
            b = 1;
            if( relstp < eta ) tp = eta;
            r1 = sqrt( tp );
            r2 = sr * ( 1 + r1 ) - si * r1;
            si = sr * r1 + si * ( 1 + r1 );
            sr = r2;
            polyev( nn, sr, si, pr, pi, qpr, qpi, &pvr, &pvi );
            for( j = 1; j <= 5; j++ )
               {
               calct( &bol );
               nexth( bol );
               }
            omp = infin;
            goto _20;
            }
         
         // Exit if polynomial value increase significantly
         if( mp *0.1 > omp ) return;
         }

      omp = mp;

      // Calculate next iterate
_20:  calct( &bol );
      nexth( bol );
      calct( &bol );
      if( !bol )
         {
         relstp = cmod( tr, ti ) / cmod( sr, si );
         sr += tr;
         si += ti;
         }
      }
}

// COMPUTES  T = -P(S)/H(S).
// BOOL   - LOGICAL, SET TRUE IF H(S) IS ESSENTIALLY ZERO.
void jenkins_traub::calct (int *bol)
{
   int n;
   double hvr, hvi;

   n = nn;

   // evaluate h(s)
   polyev( n - 1, sr, si, hr, hi, qhr, qhi, &hvr, &hvi );
   *bol = cmod( hvr, hvi ) <= are * 10 * cmod( hr[ n - 1 ], hi[ n - 1 ] ) ? 1 : 0;
   if( !*bol )
      {
      cdivid( -pvr, -pvi, hvr, hvi, &tr, &ti );
      return;
      }

   tr = 0;
   ti = 0;
}

// CALCULATES THE NEXT SHIFTED H POLYNOMIAL.
// BOOL   -  LOGICAL, IF .TRUE. H(S) IS ESSENTIALLY ZERO
//
void jenkins_traub::nexth( const int bol )
{
   int j, n;
   double t1, t2;

   n = nn;
   if( !bol )
      {
      for( j = 1; j < n; j++ )
         {
         t1 = qhr[ j - 1 ];
         t2 = qhi[ j - 1 ];
         hr[ j ] = tr * t1 - ti * t2 + qpr[ j ];
         hi[ j ] = tr * t2 + ti * t1 + qpi[ j ];
         }
      hr[ 0 ] = qpr[ 0 ];
      hi[ 0 ] = qpi[ 0 ];
      return;
      }

   // If h[s] is zero replace H with qh
   for( j = 1; j < n; j++ )
      {
      hr[ j ] = qhr[ j - 1 ];
      hi[ j ] = qhi[ j - 1 ];
      }
   hr[ 0 ] = 0;
   hi[ 0 ] = 0;
}

// EVALUATES A POLYNOMIAL  P  AT  S  BY THE HORNER RECURRENCE
// PLACING THE PARTIAL SUMS IN Q AND THE COMPUTED VALUE IN PV.
//  
void jenkins_traub::polyev (int nn, double sr, double si, const double *pr, const double *pi,
                            double *qr, double *qi, double *pvr, double *pvi)
{
   int i;
   double t;

   qr[ 0 ] = pr[ 0 ];
   qi[ 0 ] = pi[ 0 ];
   *pvr = qr[ 0 ];
   *pvi = qi[ 0 ];

   for( i = 1; i <= nn; i++ )
      {
      t = ( *pvr ) * sr - ( *pvi ) * si + pr[ i ];
      *pvi = ( *pvr ) * si + ( *pvi ) * sr + pi[ i ];
      *pvr = t;
      qr[ i ] = *pvr;
      qi[ i ] = *pvi;
      }
}

// BOUNDS THE ERROR IN EVALUATING THE POLYNOMIAL BY THE HORNER RECURRENCE.
// QR,QI - THE PARTIAL SUMS
// MS    -MODULUS OF THE POINT
// MP    -MODULUS OF POLYNOMIAL VALUE
// ARE, MRE -ERROR BOUNDS ON COMPLEX ADDITION AND MULTIPLICATION
//
double jenkins_traub::errev (int nn, const double *qr, const double *qi, double ms, double mp,
                             double are, double mre)
{
   int i;
   double e;

   e = cmod( qr[ 0 ], qi[ 0 ] ) * mre / ( are + mre );
   for( i = 0; i <= nn; i++ )
      e = e * ms + cmod( qr[ i ], qi[ i ] );

   return e * ( are + mre ) - mp * mre;
}

// CAUCHY COMPUTES A LOWER BOUND ON THE MODULI OF THE ZEROS OF A
// POLYNOMIAL - PT IS THE MODULUS OF THE COEFFICIENTS.
//
void jenkins_traub::cauchy (int nn, double *pt, double *q, double *fn_val)
{
   int i, n;
   double x, xm, f, dx, df;

   pt[ nn ] = -pt[ nn ];

   // Compute upper estimate bound
   n = nn;
   x = exp( log( -pt[ nn ] ) - log( pt[ 0 ] ) ) / n;
   if( pt[ n - 1 ] != 0 )
      {
      // Newton step at the origin is better, use it
      xm = -pt[ nn ] / pt[ n - 1 ];
      if( xm < x ) x = xm;
      }

   // Chop the interval (0,x) until f < 0
   while(1)
      {
      xm = x * 0.1;
      f = pt[ 0 ];
      for( i = 1; i <= nn; i++ )
         f = f * xm + pt[ i ];
      if( f <= 0 )
         break;
      x = xm;
      }
   dx = x;
   
   // Do Newton iteration until x converges to two decimal places
   while( fabs( dx / x ) > 0.005 )
      {
      q[ 0 ] = pt[ 0 ];
      for( i = 1; i <= nn; i++ )
         q[ i ] = q[ i - 1 ] * x + pt[ i ];
      f = q[ nn ];
      df = q[ 0 ];
      for( i = 1; i < n; i++ )
         df = df * x + q[ i ];
      dx = f / df;
      x -= dx;
      }

   *fn_val = x;
}

// RETURNS A SCALE FACTOR TO MULTIPLY THE COEFFICIENTS OF THE POLYNOMIAL.
// THE SCALING IS DONE TO AVOID OVERFLOW AND TO AVOID UNDETECTED UNDERFLOW
// INTERFERING WITH THE CONVERGENCE CRITERION.  THE FACTOR IS A POWER OF THE
// BASE.
// PT - MODULUS OF COEFFICIENTS OF P
// ETA, INFIN, SMALNO, BASE - CONSTANTS DESCRIBING THE FLOATING POINT ARITHMETIC.
//
double jenkins_traub::scale (int nn, const double *pt, double eta, double infin, double smalno, double base)
{
   int i, l;
   double hi, lo, max, min, x, sc;
   double fn_val;

   // Find largest and smallest moduli of coefficients
   hi = sqrt( infin );
   lo = smalno / eta;
   max = 0;
   min = infin;

   for( i = 0; i <= nn; i++ )
      {
      x = pt[ i ];
      if( x > max ) max = x;
      if( x != 0 && x < min ) min = x;
      }

   // Scale only if there are very large or very small components
   fn_val = 1;
   if( min >= lo && max <= hi ) return fn_val;
   x = lo / min;
   if( x <= 1 )
      sc = 1 / ( sqrt( max )* sqrt( min ) );
   else
      {
      sc = x;
      if( infin / sc > max ) sc = 1;
      }
   l = (int)( log( sc ) / log(base ) + 0.5 );
   fn_val = pow( base , l );
   return fn_val;
}

// COMPLEX DIVISION C = A/B, AVOIDING OVERFLOW.

void jenkins_traub::cdivid (double ar, double ai, double br, double bi, double *cr, double *ci)
{
   double r, d, t, infin;

   if( br == 0 && bi == 0 )
      {
      // Division by zero, c = infinity
      mcon( &t, &infin, &t, &t );
      *cr = infin;
      *ci = infin;
      return;
      }

   if( fabs( br ) < fabs( bi ) )
      {
      r = br/ bi;
      d = bi + r * br;
      *cr = ( ar * r + ai ) / d;
      *ci = ( ai * r - ar ) / d;
      return;
      }

   r = bi / br;
   d = br + r * bi;
   *cr = ( ar + ai * r ) / d;
   *ci = ( ai - ar * r ) / d;
}

// MODULUS OF A COMPLEX NUMBER AVOIDING OVERFLOW.
//
double jenkins_traub::cmod (double r, double i)
{
   double ar, ai;

   ar = fabs( r );
   ai = fabs( i );
   if( ar < ai )
      return ai * sqrt( 1.0 + pow( ( ar / ai ), 2.0 ) );

   if( ar > ai )
      return ar * sqrt( 1.0 + pow( ( ai / ar ), 2.0 ) );

   return ar * sqrt( 2.0 );
}

// MCON PROVIDES MACHINE CONSTANTS USED IN VARIOUS PARTS OF THE PROGRAM.
// THE USER MAY EITHER SET THEM DIRECTLY OR USE THE STATEMENTS BELOW TO
// COMPUTE THEM. THE MEANING OF THE FOUR CONSTANTS ARE -
// ETA       THE MAXIMUM RELATIVE REPRESENTATION ERROR WHICH CAN BE DESCRIBED
//           AS THE SMALLEST POSITIVE FLOATING-POINT NUMBER SUCH THAT
//           1.0_dp + ETA &gt; 1.0.
// INFINY    THE LARGEST FLOATING-POINT NUMBER
// SMALNO    THE SMALLEST POSITIVE FLOATING-POINT NUMBER
// BASE      THE BASE OF THE FLOATING-POINT NUMBER SYSTEM USED
//
void jenkins_traub::mcon (double *eta, double *infiny, double *smalno, double *base)
{
  *base = FLT_RADIX;
  *eta = DBL_EPSILON;
  *infiny = DBL_MAX;
  *smalno = DBL_MIN;
}


