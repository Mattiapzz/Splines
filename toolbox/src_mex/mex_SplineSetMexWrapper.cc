/****************************************************************************\
Copyright (c) 2015, Enrico Bertolazzi
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
\****************************************************************************/

#include "Splines.hh"
#include "Utils_mex.hh"

#define ASSERT(COND,MSG)                           \
  if ( !(COND) ) {                                 \
    std::ostringstream ost;                        \
    ost << "SplineSetMexWrapper: " << MSG << '\n'; \
    mexErrMsgTxt(ost.str().c_str());               \
  }

#define MEX_ERROR_MESSAGE \
"%======================================================================%\n" \
"% SplineSetMexWrapper:  Compute spline curve                           %\n" \
"%                                                                      %\n" \
"% USAGE:                                                               %\n" \
"%   obj = SplineSetMexWrapper( 'new' );                                %\n" \
"%   SplineSetMexWrapper( 'delete', obj );                              %\n" \
"%   SplineSetMexWrapper( 'build', obj, kinds, X, Y );                  %\n" \
"%   P    = SplineSetMexWrapper( 'eval', obj, X );                      %\n" \
"%   DP   = SplineSetMexWrapper( 'eval_D', obj, X );                    %\n" \
"%   DDP  = SplineSetMexWrapper( 'eval_DD', obj, X );                   %\n" \
"%   DDDP = SplineSetMexWrapper( 'eval_DDD', obj, X );                  %\n" \
"%                                                                      %\n" \
"% On input:                                                            %\n" \
"%                                                                      %\n" \
"%  kinds = string cell array with the kind of spline, any of:          %\n" \
"%          'linear', 'cubic', 'akima', 'bessel', 'pchip', 'quintic'    %\n" \
"%  X = vector of X coordinates                                         %\n" \
"%  Y = vector of Y coordinates                                         %\n" \
"%                                                                      %\n" \
"% On output:                                                           %\n" \
"%                                                                      %\n" \
"%  P    = vector of Y values                                           %\n" \
"%  DP   = vector of dimension size(X) with derivative                  %\n" \
"%  DDP  = vector of dimension size(X) with second derivative           %\n" \
"%  DDDP = vector of dimension size(X) with third derivative            %\n" \
"%                                                                      %\n" \
"%======================================================================%\n" \
"%                                                                      %\n" \
"%  Autor: Enrico Bertolazzi                                            %\n" \
"%         Department of Industrial Engineering                         %\n" \
"%         University of Trento                                         %\n" \
"%         enrico.bertolazzi@unitn.it                                   %\n" \
"%                                                                      %\n" \
"%======================================================================%\n"

using namespace std;

namespace Splines {

  static
  void
  DATA_NEW( mxArray * & mx_id, SplineSet * ptr ) {
    mx_id = Utils::mex_convert_ptr_to_mx<SplineSet>(ptr);
  }

  static
  inline
  SplineSet *
  DATA_GET( mxArray const * & mx_id ) {
    return Utils::mex_convert_mx_to_ptr<SplineSet>(mx_id);
  }

  static
  void
  DATA_DELETE( mxArray const * & mx_id ) {
    Utils::mex_destroy_object<SplineSet>(mx_id);
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_new( int nlhs, mxArray       *plhs[],
          int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineSetMexWrapper( 'new' ): "
    UTILS_MEX_ASSERT( nrhs == 1, CMD "expected 1 inputs, nrhs = {}\n", nrhs );
    UTILS_MEX_ASSERT( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );

    DATA_NEW( arg_out_0, new SplineSet() );

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_delete( int nlhs, mxArray       *plhs[],
             int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineSetMexWrapper( 'delete', OBJ ): "
    UTILS_MEX_ASSERT( nrhs == 2, CMD "expected 2 inputs, nrhs = {}\n", nrhs );
    UTILS_MEX_ASSERT( nlhs == 0, CMD "expected 0 output, nlhs = {}\n", nlhs );

    // Destroy the C++ object
    DATA_DELETE(arg_in_1);

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_build( int nlhs, mxArray       *plhs[],
            int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineSetMexWrapper( 'build', OBJ, kinds, X, Y ): "

    UTILS_MEX_ASSERT( nlhs == 0, CMD "expected NO output, nlhs = {}\n", nlhs );
    UTILS_MEX_ASSERT( nrhs == 5, CMD "expected 5 input, nrhs = {}\n", nrhs );

    SplineSet * ptr = DATA_GET( arg_in_1 );

    mwSize n, nn, nspl;
    real_type const * x = Utils::mex_vector_pointer(
      arg_in_3, n, CMD "error in reading 'x'"
    );
    real_type const * y = Utils::mex_matrix_pointer(
      arg_in_4, nn, nspl, CMD "error in reading 'y'"
    );

    UTILS_MEX_ASSERT0(
      n == nn,
      CMD "lenght of 'x' must be the number of rows of 'y'"
    );

    std::vector<Splines::SplineType1D> types;
    types.reserve(nspl);

    if ( mxIsChar(arg_in_2) ) {
      string tname = mxArrayToString(arg_in_2);
      Splines::SplineType1D st;
      if      ( tname == "linear"  ) st = LINEAR_TYPE;
      else if ( tname == "cubic"   ) st = CUBIC_TYPE;
      else if ( tname == "akima"   ) st = AKIMA_TYPE;
      else if ( tname == "bessel"  ) st = BESSEL_TYPE;
      else if ( tname == "pchip"   ) st = PCHIP_TYPE;
      else if ( tname == "quintic" ) st = QUINTIC_TYPE;
      else {
        UTILS_MEX_ASSERT0(
          false,
          CMD "Cell array of strings must contains the strings:\n"
          "'linear', 'cubic', 'akima', 'bessel', 'pchip', 'quintic'"
        );
      }
      for ( mwSize i = 0; i < nspl; ++i ) types.push_back(st);

    } else if ( mxIsCell(arg_in_2) ) {

      mwSize const *dims = mxGetDimensions(arg_in_2);
      UTILS_MEX_ASSERT(
        dims[0] == nspl && dims[1] == 1,
        CMD "Third argument expected to be cell array {} x 1, found {} x {}\n",
        nspl, dims[0], dims[1]
      );

      for ( mwSize i = 0; i < nspl; ++i ) {
        mxArray const * cell = mxGetCell(arg_in_2,i);
        UTILS_MEX_ASSERT0(
          mxIsChar(cell),
          CMD "Third argument expected to be cell array of strings"
        );
        string tname = mxArrayToString(cell);
        Splines::SplineType1D st;
        if      ( tname == "linear"  ) st = LINEAR_TYPE;
        else if ( tname == "cubic"   ) st = CUBIC_TYPE;
        else if ( tname == "akima"   ) st = AKIMA_TYPE;
        else if ( tname == "bessel"  ) st = BESSEL_TYPE;
        else if ( tname == "pchip"   ) st = PCHIP_TYPE;
        else if ( tname == "quintic" ) st = QUINTIC_TYPE;
        else {
          UTILS_MEX_ASSERT0(
            false,
            CMD "Cell array of strings must contains the strings:\n"
            "'linear', 'cubic', 'akima', 'bessel', 'pchip', 'quintic'"
          );
        }
        types.push_back(st);
      }
    } else {
      UTILS_MEX_ASSERT(
        false,
        CMD "Third argument expected to be a string\n"
        "of a cell array of strings, found ``{}''\n",
        mxGetClassName(arg_in_2)
      );
    }

    std::vector<char const *>      headers;
    std::vector<real_type const *> pY;
    headers.reserve(nspl);
    pY.reserve(nspl);
    for ( mwSize i = 0; i < nspl; ++i ) {
      headers.push_back("noname");
      pY.push_back(y+i*nn);
    }
    ptr->build(
      nspl, n,
      &headers.front(), &types.front(),
      x, &pY.front(), nullptr
    );

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval( int nlhs, mxArray       *plhs[],
           int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineSetMexWrapper( 'eval', OBJ, X ): "

    UTILS_MEX_ASSERT( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );
    UTILS_MEX_ASSERT( nrhs == 3, CMD "expected 3 input, nrhs = {}\n", nrhs );

    SplineSet * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = Utils::mex_vector_pointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    mwSize dim = ptr->numSplines();
    real_type * Y = Utils::mex_create_matrix_value( arg_out_0, dim, nx );

    for ( mwSize nsp = 0; nsp < dim; ++nsp ) {
      Spline const * S = ptr->getSpline( nsp );
      real_type * y = Y+nsp;
      for ( mwSize i = 0; i < nx; ++i, y += dim ) *y = S->eval( x[i] );
    }

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval_D( int nlhs, mxArray       *plhs[],
             int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineSetMexWrapper( 'eval_D', OBJ, X ): "

    UTILS_MEX_ASSERT( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );
    UTILS_MEX_ASSERT( nrhs == 3, CMD "expected 3 input, nrhs = {}\n", nrhs );

    SplineSet * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = Utils::mex_vector_pointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    mwSize dim = ptr->numSplines();
    real_type * Y = Utils::mex_create_matrix_value( arg_out_0, dim, nx );

    for ( mwSize nsp = 0; nsp < dim; ++nsp ) {
      Spline const * S = ptr->getSpline( nsp );
      real_type * y = Y+nsp;
      for ( mwSize i = 0; i < nx; ++i, y += dim ) *y = S->eval_D( x[i] );
    }

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval_DD( int nlhs, mxArray       *plhs[],
              int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineSetMexWrapper( 'eval_D', OBJ, X ): "

    UTILS_MEX_ASSERT( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );
    UTILS_MEX_ASSERT( nrhs == 3, CMD "expected 3 input, nrhs = {}\n", nrhs );

    SplineSet * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = Utils::mex_vector_pointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    mwSize dim = ptr->numSplines();
    real_type * Y = Utils::mex_create_matrix_value( arg_out_0, dim, nx );

    for ( mwSize nsp = 0; nsp < dim; ++nsp ) {
      Spline const * S = ptr->getSpline( nsp );
      real_type * y = Y+nsp;
      for ( mwSize i = 0; i < nx; ++i, y += dim ) *y = S->eval_DD( x[i] );
    }

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval_DDD( int nlhs, mxArray       *plhs[],
               int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineSetMexWrapper( 'eval_D', OBJ, X ): "

    UTILS_MEX_ASSERT( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );
    UTILS_MEX_ASSERT( nrhs == 3, CMD "expected 3 input, nrhs = {}\n", nrhs );

    SplineSet * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = Utils::mex_vector_pointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    mwSize dim = ptr->numSplines();
    real_type * Y = Utils::mex_create_matrix_value( arg_out_0, dim, nx );

    for ( mwSize nsp = 0; nsp < dim; ++nsp ) {
      Spline const * S = ptr->getSpline( nsp );
      real_type * y = Y+nsp;
      for ( mwSize i = 0; i < nx; ++i, y += dim ) *y = S->eval_DDD( x[i] );
    }

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_tmin( int nlhs, mxArray       *plhs[],
           int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineSetMexWrapper( 'tmin', OBJ ): "

    UTILS_MEX_ASSERT( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );
    UTILS_MEX_ASSERT( nrhs == 2, CMD "expected 2 input, nrhs = {}\n", nrhs );

    SplineSet * ptr = DATA_GET( arg_in_1 );

    Utils::mex_set_scalar_value( arg_out_0, ptr->xMin() );

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_tmax( int nlhs, mxArray       *plhs[],
           int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineSetMexWrapper('tmax',OBJ): "

    UTILS_MEX_ASSERT( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );
    UTILS_MEX_ASSERT( nrhs == 2, CMD "expected 2 input, nrhs = {}\n", nrhs );

    SplineSet * ptr = DATA_GET( arg_in_1 );

    Utils::mex_set_scalar_value( arg_out_0, ptr->xMax() );

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  typedef void (*DO_CMD)( int nlhs, mxArray *plhs[], int nrhs, mxArray const *prhs[] );

  static map<string,DO_CMD> cmd_to_fun = {
    {"new",do_new},
    {"delete",do_delete},
    {"build",do_build},
    {"eval",do_eval},
    {"eval_D",do_eval_D},
    {"eval_DD",do_eval_DD},
    {"eval_DDD",do_eval_DDD},
    {"tmin",do_tmin},
    {"tmax",do_tmax}
  };

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  extern "C"
  void
  mexFunction( int nlhs, mxArray       *plhs[],
               int nrhs, mxArray const *prhs[] ) {
    // the first argument must be a string
    if ( nrhs == 0 ) {
      mexErrMsgTxt(MEX_ERROR_MESSAGE);
      return;
    }

    try {
      UTILS_MEX_ASSERT0(
        mxIsChar(arg_in_0), "First argument must be a string"
      );
      string cmd = mxArrayToString(arg_in_0);
      DO_CMD pfun = cmd_to_fun.at(cmd);
      pfun( nlhs, plhs, nrhs, prhs );
    } catch ( exception const & e ) {
      mexErrMsgTxt( fmt::format( "SplineSetMexWrapper Error: {}", e.what() ).c_str() );
    } catch (...) {
      mexErrMsgTxt("SplineSetMexWrapper failed\n");
    }

  }

}
