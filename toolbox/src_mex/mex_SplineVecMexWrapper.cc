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
#include "mex_utils.hh"

#define ASSERT(COND,MSG)                           \
  if ( !(COND) ) {                                 \
    std::ostringstream ost;                        \
    ost << "SplineVecMexWrapper: " << MSG << '\n'; \
    mexErrMsgTxt(ost.str().c_str());               \
  }

#define MEX_ERROR_MESSAGE \
"%======================================================================%\n" \
"% SplineVecMexWrapper:  Compute spline curve                           %\n" \
"%                                                                      %\n" \
"% USAGE:                                                               %\n" \
"%   obj = SplineVecMexWrapper( 'new' );                                %\n" \
"%   SplineVecMexWrapper( 'delete', obj );                              %\n" \
"%   SplineVecMexWrapper( 'setup', obj, Y );                            %\n" \
"%   SplineVecMexWrapper( 'knots', obj, X );                            %\n" \
"%   SplineVecMexWrapper( 'chordal', obj );                             %\n" \
"%   SplineVecMexWrapper( 'centripetal', obj );                         %\n" \
"%   SplineVecMexWrapper( 'CatmullRom', obj );                          %\n" \
"%   P    = SplineVecMexWrapper( 'eval', obj, X );                      %\n" \
"%   DP   = SplineVecMexWrapper( 'eval_D', obj, X );                    %\n" \
"%   DDP  = SplineVecMexWrapper( 'eval_DD', obj, X );                   %\n" \
"%   DDDP = SplineVecMexWrapper( 'eval_DDD', obj, X );                  %\n" \
"%                                                                      %\n" \
"% On input:                                                            %\n" \
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
  DATA_NEW( mxArray * & mx_id, SplineVec * ptr ) {
    mx_id = convertPtr2Mat<SplineVec>(ptr);
  }

  static
  inline
  SplineVec *
  DATA_GET( mxArray const * & mx_id ) {
    return convertMat2Ptr<SplineVec>(mx_id);
  }

  static
  void
  DATA_DELETE( mxArray const * & mx_id ) {
    destroyObject<SplineVec>(mx_id);
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_new( int nlhs, mxArray       *plhs[],
          int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'new' ): "
    MEX_ASSERT2( nrhs == 1, CMD "expected 1 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );

    DATA_NEW( arg_out_0, new SplineVec() );

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_delete( int nlhs, mxArray       *plhs[],
             int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'delete', OBJ ): "
    MEX_ASSERT2( nrhs == 2, CMD "expected 2 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 0, CMD "expected 0 output, nlhs = {}\n", nlhs );

    // Destroy the C++ object
    DATA_DELETE(arg_in_1);

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_setup( int nlhs, mxArray       *plhs[],
            int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'setup', obj, Y  ): "
    MEX_ASSERT2( nrhs == 3, CMD "expected 3 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 0, CMD "expected 0 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    mwSize dim, npts;
    real_type const * Y = getMatrixPointer(
      arg_in_2, dim, npts, CMD "error in reading 'Y'"
    );
    ptr->setup( dim, npts, Y, dim );

    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_knots( int nlhs, mxArray       *plhs[],
            int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'knots', obj, X  ): "
    MEX_ASSERT2( nrhs == 3, CMD "expected 3 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 0, CMD "expected 0 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    mwSize npts;
    real_type const * X = getVectorPointer(
      arg_in_2, npts, CMD "error in reading 'X'"
    );
    MEX_ASSERT(
      npts == ptr->numPoints(),
      CMD "size(X) = " << npts << " must be = " << ptr->dimension()
    );
    ptr->setKnots( X );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_chordal( int nlhs, mxArray       *plhs[],
              int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'chordal', obj ): "
    MEX_ASSERT2( nrhs == 2, CMD "expected 2 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 0, CMD "expected 0 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    ptr->setKnotsChordLength();
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_centripetal( int nlhs, mxArray       *plhs[],
                  int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'centripetal', obj ): "
    MEX_ASSERT2( nrhs == 2, CMD "expected 2 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 0, CMD "expected 0 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    ptr->setKnotsCentripetal();
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_CatmullRom( int nlhs, mxArray       *plhs[],
                 int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'CatmullRom', obj ): "
    MEX_ASSERT2( nrhs == 2, CMD "expected 2 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 0, CMD "expected 0 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    ptr->CatmullRom();
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_getNodes( int nlhs, mxArray       *plhs[],
               int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'getNodes', obj ): "
    MEX_ASSERT2( nrhs == 2, CMD "expected 2 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    //! return the number of support points of the splines
    integer N = ptr->numPoints();
    real_type * X = createMatrixValue( arg_out_0, 1, N );
    for ( integer i = 0; i < N; ++i ) X[i] = ptr->xNode( i );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval( int nlhs, mxArray       *plhs[],
           int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'eval', obj, x ): "
    MEX_ASSERT2( nrhs == 3, CMD "expected 3 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = getVectorPointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    mwSize dim = ptr->dimension();
    real_type * Y = createMatrixValue( arg_out_0, dim, nx );

    for ( mwSize i = 0; i < nx; ++i, Y += dim )
      ptr->eval( x[i], Y, 1 );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval_D( int nlhs, mxArray       *plhs[],
             int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'eval_D', obj, x ): "
    MEX_ASSERT2( nrhs == 3, CMD "expected 3 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = getVectorPointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    mwSize dim = ptr->dimension();
    real_type * Y = createMatrixValue( arg_out_0, dim, nx );

    for ( mwSize i = 0; i < nx; ++i, Y += dim )
      ptr->eval_D( x[i], Y, 1 );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval_DD( int nlhs, mxArray       *plhs[],
              int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'eval_DD', obj, x ): "
    MEX_ASSERT2( nrhs == 3, CMD "expected 3 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = getVectorPointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    mwSize dim = ptr->dimension();
    real_type * Y = createMatrixValue( arg_out_0, dim, nx );

    for ( mwSize i = 0; i < nx; ++i, Y += dim )
      ptr->eval_DD( x[i], Y, 1 );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval_DDD( int nlhs, mxArray       *plhs[],
               int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'eval_DDD', obj, x ): "
    MEX_ASSERT2( nrhs == 3, CMD "expected 3 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = getVectorPointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    mwSize dim = ptr->dimension();
    real_type * Y = createMatrixValue( arg_out_0, dim, nx );

    for ( mwSize i = 0; i < nx; ++i, Y += dim )
      ptr->eval_DDD( x[i], Y, 1 );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval_curvature( int nlhs, mxArray       *plhs[],
                     int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'eval_curvature', obj, x ): "
    MEX_ASSERT2( nrhs == 3, CMD "expected 3 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = getVectorPointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    real_type * curvature = createMatrixValue( arg_out_0, 1, nx );

    for ( mwSize i = 0; i < nx; ++i )
      curvature[i] = ptr->curvature( x[i] );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_eval_curvature_D( int nlhs, mxArray       *plhs[],
                       int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper( 'eval_curvature', obj, x ): "
    MEX_ASSERT2( nrhs == 3, CMD "expected 3 inputs, nrhs = {}\n", nrhs );
    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    mwSize nx;
    real_type const * x = getVectorPointer(
      arg_in_2, nx, CMD "error in reading `x`"
    );

    real_type * curvature_D = createMatrixValue( arg_out_0, 1, nx );

    for ( mwSize i = 0; i < nx; ++i )
      curvature_D[i] = ptr->curvature_D( x[i] );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_tmin( int nlhs, mxArray       *plhs[],
           int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper('tmin',OBJ): "

    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );
    MEX_ASSERT2( nrhs == 2, CMD "expected 2 input, nrhs = {}\n", nrhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    setScalarValue( arg_out_0, ptr->xMin() );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  static
  void
  do_tmax( int nlhs, mxArray       *plhs[],
           int nrhs, mxArray const *prhs[] ) {

    #define CMD "SplineVecMexWrapper('tmax',OBJ): "

    MEX_ASSERT2( nlhs == 1, CMD "expected 1 output, nlhs = {}\n", nlhs );
    MEX_ASSERT2( nrhs == 2, CMD "expected 2 input, nrhs = {}\n", nrhs );

    SplineVec * ptr = DATA_GET( arg_in_1 );

    setScalarValue( arg_out_0, ptr->xMax() );
    #undef CMD
  }

  // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

  typedef void (*DO_CMD)( int nlhs, mxArray *plhs[], int nrhs, mxArray const *prhs[] );

  static map<string,DO_CMD> cmd_to_fun = {
    {"new",do_new},
    {"delete",do_delete},
    {"setup",do_setup},
    {"knots",do_knots},
    {"chordal",do_chordal},
    {"centripetal",do_centripetal},
    {"CatmullRom",do_CatmullRom},
    {"getNodes",do_getNodes},
    {"eval",do_eval},
    {"eval_D",do_eval_D},
    {"eval_DD",do_eval_DD},
    {"eval_DDD",do_eval_DDD},
    {"eval_curvature",do_eval_curvature},
    {"eval_curvature_D",do_eval_curvature_D},
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
      MEX_ASSERT( mxIsChar(arg_in_0), "First argument must be a string" );
      string cmd = mxArrayToString(arg_in_0);
      DO_CMD pfun = cmd_to_fun.at(cmd);
      pfun( nlhs, plhs, nrhs, prhs );
    } catch ( exception const & e ) {
      mexErrMsgTxt( fmt::format( "SplineVecMexWrapper Error: {}", e.what() ).c_str() );
    } catch (...) {
      mexErrMsgTxt("SplineVecMexWrapper failed\n");
    }

  }

}
