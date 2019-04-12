function [xe, xo, m] = evenodd(x, n)
    m = -n(:, $:-1:1);
    m1 = min([m, n]);
    m2 = max([m, n]);
    m = m1:m2;
    
    n1 = 1:length(n);
    nm = n(1) - m(1);
    xx = zeros(1, length(m));
    xx(n1+nm) = x;
    
    xe = 0.5*(xx + xx(:, $:-1:1));
    xo = 0.5*(xx - xx(:, $:-1:1));
endfunction
