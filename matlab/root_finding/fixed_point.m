%Fixed point method
clear
syms g(x) x;

%Parameters to modify
epsilon = 0.00005; %target precision
f = exp(x)-4*x^2-x; %function f(x)=0
g(x) = exp(x)-4*x^2;  %function g(x)=x
c = -1; %initial guess
nmax = 25; %desired maximum number of iterations


%variables
error =  5; %initial error
n = 1; %track iterations

%finding a good g(x) to use based on a function f(x)
newton = simplify(expand(x-f/diff(f,x)));
fprintf("A good g(x) would be the following:\n");
disp(newton);

%convergence test
fprintf("Convergence test: \n");
convergence = abs(g(c));
if(convergence <1)
    fprintf("May converge, absolute g(x) is %f \n", convergence);
else
    fprintf("Convergence not possible, absolute g(x) is %f \n", convergence);
end

%print table
fprintf('%-15s%-15s%-15s%-15s\n', 'n', 'c', 'g(c)', 'error');
while n<=nmax && error>epsilon
    c_old = c;
    c = g(c);
    error = abs(c-c_old);
    fprintf('%-15d', n);
    fprintf('%-15f', c);
    fprintf('%-15E', g(c));
    fprintf('%-15E', error);
    n=n+1;
    fprintf('\n');
end
fprintf('The answer is %f to the plus/minus %e\n', c, epsilon);
