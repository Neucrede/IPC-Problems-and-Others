function [y, n] = sigfold(x1, n1)
    y = x(:, $:-1:1);
    n = -n1(:, $:-1:1)
endfunction
