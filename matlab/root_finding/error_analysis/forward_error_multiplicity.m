clear
syms f(x) x

%parameters to modify
f(x) = x^4+13.8*x^3+35.41*x^2-84.18*x+37.21; %the function f(x)
%xr = 0; %the approximated root
r=-7.69294 ; %the real root
n = 5; %number of times to differentiate 
%note that you have to run the app once to look for the order,m,
%when the derivative goes from 0 to something other than 0
m=4;


%algorithm variables
range_x = linspace(-10,10);
plot(range_x, f(range_x)); 
grid on;
xlabel('x');
ylabel('y');
count=1;
dif = f(x);
delta = 1;
x_axis =f(r+delta)*f(r-delta);

if x_axis>0
    fprintf("Possibly even multiplicity, does not cross the x-axis, the change in f is %f\n", x_axis);
else
    fprintf("Possibly odd multiplicity, crosses the x-axis, the change in f is %f\n", x_axis);
end
    
fprintf("f(r) which is order(0) is %e\n", f(r));
while count<n
    dif = diff(dif,x);
    answ = double(subs(dif, x, r));
    fprintf("derivative of f at the order(%d) at r is %e\n", count, answ);
    count = count+1;
end

%calculate forward error using multiplicty
dif_m = diff(dif,x,m);
%forward_error = (abs((factorial(m)*f(xr))/subs(dif_m,x,xr)))^(1/m);
%fprintf("The estimated error is approximately %.6f\n",forward_error);