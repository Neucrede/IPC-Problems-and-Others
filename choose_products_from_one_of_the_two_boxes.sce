clear all;
clc;

max_trials = 2^16;

tb1 = 50;               // tb1 products in box 1
pb1 = 10;               // premium products
nb1 = tb1 - pb1;        // products that are not premium
tb2 = 30;
pb2 = 18;
nb2 = tb2 - pb2;

f = 0;
g = 0;

for j = 1:100

    box = 0;
    tpp1 = 0;
    tpp2 = 0;
    cb1 = 0;
    cb2 = 0;
    tp1 = 0;
    tp2 = 0;
    
    // Initialize random number genrator.
    grand("setgen", "mt");
    grand("setsd", getdate("s"));
    
    tic();
    
    for i = 1:max_trials
        // Pick a box.
        n = grand(1, "prm", [1, 2]);
        box = n(1);
        
        // Choose products.
        if box == 1 then
            cb1 = cb1 + 1;
            m1 = grand(1, "prm", 1:tb1);
            if m1(1)<=pb1 then
                tp1 = tp1 + 1;
                m2 = grand(1, "prm", [1:m1(1)-1, m1(1)+1:tb1]);
                if m2(1)<=pb1 then
                    tpp1 = tpp1 + 1;
                end
            end
        elseif box == 2 then
            cb2 = cb2 + 1;
            m1 = grand(1, "prm", 1:tb2);
            if m1(1)<=pb2 then
                tp2 = tp2 + 1;
                m2 = grand(1, "prm", [1:m1(1)-1, m1(1)+1:tb2]);
                if m2(1)<=pb2 then
                    tpp2 = tpp2 + 1;
                end
            end
        else
            break;
        end
    end
    
    f(j) = (tpp1+tpp2) / (tp1+tp2);
    printf("%d:\t tpp1=%d, cb1=%d, tp1=%d, tpp2=%d, cb2=%d, tp2=%d, f=%f\n", ...
            j, tpp1, cb1, tp1, tpp2, cb2, tp2, f(j));
    printf("Time elapsed: %f (sec).\n\n", toc());
    
end
