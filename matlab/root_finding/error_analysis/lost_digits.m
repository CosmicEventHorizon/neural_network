clear

%parameters to modify
A=[2.2220, 16.710, 9.6120; 3.3330, 15920, -10.333; 1.5611, 5.1791, 1.6852];
maximum_M = cond(A);
digits_lost = 16-log10(maximum_M);

fprintf("Maximum magnification factor is: %f\nThe number of digits lost is %f\n", maximum_M, digits_lost);