%False position method
clear
syms f(x) x;

%Parameters to modify
epsilon = 0.2  ; %target precision
f(x) = x^2-3;  %function f(x)
a = 1; %initial left bound
b = 2; %initial right bound
nmax = 11; %desired maximum number of iterations


%variables
%a = [];
c = (a*f(b)-b*f(a))/(f(b)-f(a)); %initial point
error =  abs(b-a);
c_old = 0;
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
    c_old =c;
    c = ((a*f(b))-(b*f(a)))/((f(b))-(f(a)));
    n=n+1;
    fprintf('\n');
end
fprintf('\nThe answer is %f to the plus/minus %e\n', c, epsilon);
disp(double(c));
