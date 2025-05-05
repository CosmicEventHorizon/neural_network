clear
syms x

%parameters to modify
f(x) = exp(-4.6*x^2); %function to integrate
a = 1.5; %integrate from a to b
b= 8.4;

%algorithm
answer = double((b-a)*(f(a)+f(b))/2);
fprintf("The answer is %.12f\n",answer);