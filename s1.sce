clc;

// Plant
G = [0 1 0; 0 0 1; -0.12 -0.01 1];
H = [0 0 1]';
C = [0.5 1 0];
sl = syslin('d', G, H, C);
printf("System\n");
ssprint(sl);

// Servo controller design
Ghat = [G H; zeros(1,3) 0];
Hhat = [zeros(3,1); 1];
J = zeros(1,4);
Khat = ppol(Ghat, Hhat, J);
T = [G-eye(3,3) H; C*G C*H]
K2_K1 = (Khat + [zeros(1,3) 1])*inv(T);
K2 = K2_K1(1, 1:3)                  // State feedback gain matrix
K1 = K2_K1(1, 4)                    // Integral controller gain matrix
printf("\n\nIntegral controller gain matrix K1=\n");
disp(K1);
printf("\n\nState feedback gain matrix K2=\n");
disp(K2);

// Least order state observer
T = [0 0 1; 1 0 -0.5; 0 1 0];
Gnew = inv(T)*G*T;
Hnew = inv(T)*H;
Cnew = C*T;
sl2 = syslin('d', Gnew, Hnew, Cnew);
printf("\n\nTransformed system:\n");
ssprint(sl2);
Gaa = Gnew(1,1);
Gab = Gnew(1, 2:3);
Gba = Gnew(2:3, 1);
Gbb = Gnew(2:3, 2:3);
Ha = Hnew(1,1);
Hb = Hnew(2:3, 1);
Je = zeros(1,2);
Ke = (ppol(Gbb', Gab', Je))';       // Error gain matrix
printf("\n\nError gain matrix Ke=\n");
disp(Ke);

///////////////////////////////////////////////////////////////////////////////
r = 1;
u = 0;
v = 0;
x0 = [0.3 0.1 0.1]';

x = x0;
y = C*x;
last_y = y;
eta_tilde = zeros(2,1);

scf();
subplot(231);   xtitle('y');
subplot(232);   xtitle('x1');
subplot(234);   xtitle('x2');
subplot(235);   xtitle('x3');
subplot(233);   xtitle('u');
subplot(236);   xtitle('e');

printf("\n\nSimulating...\n");
printf("n\ty\tx1\tx2\tx3\tu\te\t\n");
for n = 0:10
    // State observer
    eta_tilde = (Gbb-Ke*Gab)*eta_tilde +((Gbb-Ke*Gab)*Ke+Gba-Ke*Gaa)*last_y ...
                + (Hb-Ke*Ha)*u;
    xia_tilde = y;
    xib_tilde = eta_tilde + Ke*xia_tilde;
    x_tilde = T*[xia_tilde; xib_tilde];
    
    // Integral controller
    v = v + (r - y);
    u = K1*v - K2*x_tilde;
    
    // Plant
    last_y = y;                     // Save last output
    x = G*x + H*u;
    y = C*x;    
    
    //disp(y, 'y', x, 'x');
    printf("%d\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t%.3f\t\n",    ...
            n, y, x(1), x(2), x(3), u, r-y);
    subplot(231);    plot(n, y, 'o');
    subplot(232);    plot(n, x(1), 'o');
    subplot(234);    plot(n, x(2), 'o');
    subplot(235);    plot(n, x(3), 'o');
    subplot(233);    plot(n, u, 'o');
    subplot(236);    plot(n, r-y, 'o');
end

printf("End of simulation.\n\n");
