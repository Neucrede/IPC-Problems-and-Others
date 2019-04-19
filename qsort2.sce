function [y]=push(x, d)
    y = [x, d];
endfunction

function [d, y]=pop(x)
    d = x($);
    y = x(1:$-1);
endfunction

function [y] = swap(x, n1, n2)
    y = x;
    
    t = y(n1);
    y(n1) = y(n2);
    y(n2) = t;
endfunction

function [y, p] = partition(x, l, r)
    y = x;
    s = y(r);
    i = l;
    
    for j = l:r-1
        if y(j) <= s then
            y = swap(y, i, j);
            i = i + 1;
        end
    end

    y=swap(y, i, r);
    
    p = i;
endfunction

function [y]=qsort2(x, l, r)
    y = x;
    s = [];
    
    if l > r then
        return;
    end
    
//    scf();
//    a = gcf();
//    imgidx = 0;
    
    s = push(s, l);
    s = push(s, r);
    
//    plot(y, 'o');
//    xs2png(a, msprintf('F:\\qsort2_%d.png', imgidx));
    
    yy = [];
    while (~isempty(s))
        [rr, s] = pop(s);
        [ll, s] = pop(s);
        
        if ll < rr then
            //yy = y;
            [y, p] = partition(y, ll, rr);
            
            s = push(s, p + 1);
            s = push(s, rr);
            
            s = push(s, ll);
            s = push(s, p - 1);
            
//            if ~isequal(y, yy) then
//                clf(a);
//                plot(y, 'o');
//                plot(p, y(p), 'r.', ll, y(ll), 'g.', rr, y(rr), 'm.');
//                plot2d3([p, ll, rr], [y(p), y(ll), y(rr)]);
//                imgidx = imgidx + 1;
//                xs2png(a, msprintf('F:\\qsort2_%d.png', imgidx));
//            end
        end
    end
endfunction

