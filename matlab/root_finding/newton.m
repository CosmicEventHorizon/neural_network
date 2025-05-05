%Newton method
clear
syms f(x) x;
format long;

%Parameters to modify
epsilon = 10^(-19); %target precision
f(x) = tanh(x);  %function f(x)
c = 0.56; %initial guess
nmax = 100; %desired maximum number of iterations


%variables
Df = diff(f,x);
n = 1; %track iterations
gx = simplify(expand(x-f(x)/Df));
disp(gx);

%print table
fprintf('%-15s%-15s%-15s%-15s\n', 'n', 'c', 'f(c)', 'error');
while n<=nmax
    c_old = c;
    c = c - f(c)/double(subs(Df,x,c));
    error = abs(c-c_old);
    if error < epsilon
        break;
    end
    fprintf('%-15d', n);
    fprintf('%-15f', c);
    fprintf('%-15E', f(c));
    fprintf('%-15E', error);
    n=n+1;
    fprintf('\n');
end
fprintf('The answer is %.16f to the plus/minus %e\n', c, epsilon);

