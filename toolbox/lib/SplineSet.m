%>
%>  MATLAB class wrapper for the underlying C++ class
%>
classdef SplineSet < handle
  properties (SetAccess = private, Hidden = true)
    %> Handle to the underlying C++ class instance
    objectHandle;
  end

  methods
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    %>
    %> Build a spline Set
    %>
    %> \rst
    %>
    %> .. code-block:: matlab
    %>
    %>
    %>    X = [ .... ];                  % vector or nodes
    %>    Y = [ .... ];                  % matrix of values or
    %>    Y = { [....],  ...., [....] }; % cell array of vectors
    %>    kind = { .... }                % cell array of strings
    %>                                   % with the kind of each spline
    %>
    %>    obj = SplineSet( kind, X, Y );
    %>
    %> \endrst
    %>
    %>
    %> Kind is a string and can be any of 
    %>
    %> |    kind      |   meaning                                                |
    %> | :----------- | :------------------------------------------------------- |
    %> | 'linear'     | linear spline (only continuous)                          |
    %> | 'cubic'      | cubic spline (\f$ C^2 \f$ function)                      |
    %> | 'akima'      | Akima non oscillatory spline (\f$ C^1 \f$ function)      |
    %> | 'bessel'     | bessel non oscillatory spline (\f$ C^1 \f$ function)     |
    %> | 'pchip'      | Monotone \f$ C^1 \f$ function                            |
    %> | 'hermite'    | Hermite spline (set \f$ p_k \f$ and \f$ p'_k \f$)        |
    %> | 'quintic'    | Quintic spline (\f$ C^3 \f$ function)                    |
    %>
    function self = SplineSet( varargin )
      self.objectHandle = SplineSetMexWrapper( 'new' );
      if nargin > 0
        SplineSetMexWrapper( 'build', self.objectHandle, ...
                             varargin{1}, varargin{2}, varargin{3} );
      end
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    function delete(self)
      SplineSetMexWrapper( 'delete', self.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    %>
    %> Build a spline Set
    %>
    %> \rst
    %>
    %>   .. code-block:: matlab
    %>
    %>     obj = SplineSet( kind, X, Y );
    %>
    %> \endrst
    %>
    function build( self, kinds, x, y )
      SplineSetMexWrapper( 'build', self.objectHandle, kinds, x, y );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    %>
    %> Evaluate spline at `x`
    %>
    %> \rst
    %>
    %>   .. code-block:: matlab
    %>
    %>     p = obj.eval( x );
    %>
    %> \endrst
    %>
    function p = eval( self, x )
      p = SplineSetMexWrapper( 'eval', self.objectHandle, x );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    %>
    %> Evaluate spline derivative at `x`
    %>
    %> \rst
    %>
    %>   .. code-block:: matlab
    %>
    %>     p_D = obj.eval_D( x );
    %>
    %> \endrst
    %>
    function dp = eval_D( self, x )
      dp = SplineSetMexWrapper( 'eval_D', self.objectHandle, x );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    %>
    %> Evaluate spline second derivative at `x`
    %>
    %> \rst
    %>
    %>   .. code-block:: matlab
    %>
    %>     p_DD = obj.eval_DD( x );
    %>
    %> \endrst
    %>
    function ddp = eval_DD( self, x )
      ddp = SplineSetMexWrapper( 'eval_DD', self.objectHandle, x );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    %>
    %> Evaluate spline third derivative at `x`
    %>
    %> \rst
    %>
    %>   .. code-block:: matlab
    %>
    %>     p_DDD = obj.eval_DDD( x );
    %>
    %> \endrst
    %>
    function dddp = eval_DDD( self, x )
      dddp = SplineSetMexWrapper( 'eval_DDD', self.objectHandle, x );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    %>
    %> Return initial t-coordinate of the spline
    %>
    %> \rst
    %>
    %>   .. code-block:: matlab
    %>
    %>     t = obj.tmin();
    %>
    %> \endrst
    %>
    function t = tmin( self, x )
      t = SplineSetMexWrapper( 'tmin', self.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    %>
    %> Return initial t-coordinate of the spline
    %>
    %> \rst
    %>
    %>   .. code-block:: matlab
    %>
    %>     t = obj.tmax();
    %>
    %> \endrst
    %>
    function t = tmax( self, x )
      t = SplineSetMexWrapper( 'tmax', self.objectHandle );
    end
    % - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  end
end
