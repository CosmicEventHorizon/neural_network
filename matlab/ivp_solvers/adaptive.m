clear
format long


%parameters to modify + function below
my_x0 = 0; %x0 containing initial value problems
my_y0 = [1/3]; %y(x0)=y0 initial value problem for each y in the system, an array
xn = 3; %y(xn) = ? to find
h=0.1; %step size
%euler_method, rk4_method, trapezoid_method, midpoint_method
result = euler_method(@my_function,my_x0,my_y0,xn,h,true); 

%optional parameters
error = false; %show error estimation
htest = 0.1; %used for richardson error
q = 4; %the error estimation O(h^q), depends on RKq
tolerance = 10^(-8); %compare error with tolerance


%calculate error
if(error==true)
    result_test = rk4_method(@my_function,my_x0,my_y0,xn,htest,false);
    w1 = result(1);
    w1t = result_test(1);
    richardson = abs((w1-w1t)/((htest/h)^q-1));
    fprintf("The richardson error (global error estimation) is %.16f\n",richardson);
    if richardson<tolerance
        fprintf("True\n");
    else
        fprintf("False\n");
    end
end

%approximation of error across entire interval
h_adaptive = 0.05;
result_adaptive = rk4_method(@my_function,my_x0,my_y0,xn,h_adaptive,false); 
adaptive_error = abs(result_adaptive(1)-result(1))*h;



%results
fprintf("\nThe result is\n"); 
m=length(y0);
for i=1:1:m
    fprintf("\t%.12f",result(i));
end
fprintf("\nThe expected result is\n"); 
sol = ode45(@my_function,[my_x0 xn],my_y0);
disp(deval(sol,xn));

%parameters to modify
function result = my_function(x,y)
  result(1)= (cos(x)+2*x^3)*y(1)^3 -x*y(1);
end