function [y] = hardlim(x)
    r = size(x, 'r');
    y = ones(r, 1);
    
    for i = 1:r
        if x(i) < 0 then
            y(i) = 0;
        end
    end
endfunction

function [t] = pctrn(w, b, p)
    t = hardlim(w*p + b);
endfunction

function [wnew, bnew] = pctrn_learn(w, b, p, t)
    s = size(p, 'c');
    wnew = w;
    bnew = b;
    
    for i = 1:s
        a = pctrn(wnew, bnew, p(:,i));
        e = t(:,i) - a;
        wnew = wnew + e*p(:,i)';
        bnew = bnew + e;
    end
endfunction

