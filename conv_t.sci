function [y, ny]=conv_t(x, nx, h, nh)
    ny = nx(1)+nh(1) : nx(length(x))+nh(length(h));
    y = conv(x, h);
endfunction
