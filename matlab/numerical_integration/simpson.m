clear
syms x

%parameters to modify
f(x) = cos(2*x)*exp(-3*x); %function to integrate
a = 1.9; %integrate from a to b
b= 3.2;

%algorithm
answer = double((b-a)/6*(f(a)+4*f((a+b)/2)+f(b)));
fprintf("The answer is %.12f\n",answer);