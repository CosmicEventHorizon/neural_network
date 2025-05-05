%Bisection method
clear;
syms f(x) x;


%Parameters to modify
epsilon = 10^(-19); %target precision/number of decimal places of answer-1/if given -19 then 18 decimal places in answer
f(x) =  2.5*x^4+10.6*x^3+5.4*x^2+10.6*x+2.9;  %function f(x)
a = -2; %initial left bound
b = 0; %initial right bound
nmax = 100; %desired maximum number of iterations



%variables
%a = [];
c = (a+b)/2; %initial midpoint
c_old=0;
error =  abs(b-a);
n = 0; %track iterations
expected_n = ceil((log(b-a) - log(epsilon))/log(2) - 1); %calculate the expected number of iterations to reach epsilon

fprintf('The expected number of iterations is %d\n', expected_n);
%print table
fprintf('%-15s%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n', 'n', 'a', 'b', 'c', 'f(a)', 'f(b)', 'f(c)', 'error');
while n<=nmax && error>epsilon
    fprintf('%-15d', n);
    fprintf('%-15f', a);
    fprintf('%-15f', b);
    fprintf('%-15f', c);
    fprintf('%-15E', f(a));
    fprintf('%-15E', f(b));
    fprintf('%-15E', f(c));
    error =  abs(c-c_old);
    fprintf('%-15E', error);
    if(error<=epsilon ||  n==nmax)
        break;
    end
    if f(a)*f(c) < 0
        b = c;
    else
        a = c;
    end
    c_old = c;
    c=(a+b)/2;
    n=n+1;
    fprintf('\n');
end
fprintf('\nThe answer is %.11f to the plus/minus %e\n', c, epsilon);
