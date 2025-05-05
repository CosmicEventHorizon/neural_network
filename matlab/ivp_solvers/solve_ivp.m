clear
format long


%parameters to modify + function below
my_x0 = 0; %x0 initial value problem
my_y0 = [0.5]; %y(x0)=y0 initial value problem for each y in the system, an array
xn = 2; %y(xn) = ? to find
h=10^(-2); %step size
%euler_method, rk4_method, trapezoid_method, midpoint_method
result = rk4_method(@my_function,my_x0,my_y0,xn,h,false); 
%disp(result)

%optional parameters
error = true; %show error estimation
htest = 0.1; %used for richardson error
q = 4; %the error estimation O(h^q), depends on RKq
tolerance = 10^(-6); %compare error with tolerance


%calculate error
if(error==true)
    result_test = rk4_method(@my_function,my_x0,my_y0,xn,htest,false);
    w1 = result(1);
    w1t = result_test(1);
    richardson = abs((w1-w1t)/((htest/h)^q-1));
    fprintf("The richardson error (global error estimation) is %.16f\n",richardson);
    if richardson>tolerance
        fprintf("True\n");
    else
        fprintf("False\n");
    end
end



%results
%{
fprintf("\nThe result is\n"); 
m=length(my_y0);
for i=1:1:m
    fprintf("\t%.12f",result(i));
end
fprintf("\n");
fprintf("\nThe expected result is\n"); 
sol = ode45(@my_function,[my_x0 xn],[1]);
disp(deval(sol,xn));
%}

%parameters to modify
function result = my_function(x,y)
  result(1) = 3*exp(x^2)-y(1);
end