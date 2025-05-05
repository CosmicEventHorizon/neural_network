clear
syms x

%relative error is abs(f(xr)/(f'(xr)*xr))
xr = 0.91091849;
f = 3*x^2-exp(x);
tolerance = 10^(-5);

%algorithm
dif = diff(f,x);
error = abs(subs(f,x,xr)/(subs(dif,x,xr)*xr));
fprintf("Error is %.6f\n",error);
if error < tolerance
    fprintf("True\n");
else
    fprintf("False\n");
end