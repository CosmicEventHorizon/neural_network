clear
syms x

%parameters to modify
f(x) =(1-cos(x))/x^2; %function to integrate
a = 0; %integrate from a to b
b= pi/2;
m=8; %number of subintervals

%algorithm
h=(b-a)/m;
sigma = 0;
for i=0:1:m-1
    xi = a + h*i;
    xip1 = a+h*(i+1);
    sigma = sigma+f((xi+xip1)/2);
end 
answer = double(h*sigma);
fprintf("The answer is %.12f\n",answer);
