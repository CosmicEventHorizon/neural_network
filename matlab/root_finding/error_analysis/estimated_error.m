clear
syms x f(x)

%parameters to modify
f(x) = x^5-2*x^4+2*x^2-x;
r = -1; %initial guess
initial_error = 0.02; %initial error(i)
choice = 3; %1 for bisection, 2 for fixed_point, 3 for newton

%algorithm variables
error = 0; %error(i+1)
bisection_alpha=1;
fixed_point_alpha=1;
newton_alpha=2;


if choice==1
    constant = 1/2;
    error = constant*(initial_error^bisection_alpha);
    fprintf("The bisection error is %f \n", error);
elseif choice==2
    dif = diff(f(x),x);
    constant = abs(subs(dif,x,r));
    error = constant*(initial_error^fixed_point_alpha);
    fprintf("The fixed point error is %f \n", error);
elseif choice==3
    dif = diff(f(x),x);
    dif2 = diff(f(x),x,2);
    constant = abs(subs(dif2,x,r)/(2*subs(dif,x,r)));
    error = constant*(initial_error^newton_alpha);
    fprintf("The newton error is %f \n", error);
end
