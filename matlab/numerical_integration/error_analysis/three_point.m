clear
syms x
%three point centered-difference

%parameters
epsilon_mach = 10^(-16);
x0=0;%estimate derivative of f at x0
derivative_real = 1; %real answer for derivative of f at x0

%algorithm
i=1;
for k=1:20
    h=10^(-k);
    h_plot(i) = h;
    derivative_estimate = (f(x0+h)-f(x0-h))/(2*h);
    error(i) = abs(derivative_estimate - derivative_real);
    i=i+1;
end
table = [h_plot;error]';
dif = diff(exp(x),x,3);
optimal_h = (3*epsilon_mach/abs(subs(dif,x,x0)))^(1/3);
fprintf("The optimal h is at %.6f\n",optimal_h);
loglog(h_plot,error,'o');
grid

function result = f(x)
    result = exp(x);
end