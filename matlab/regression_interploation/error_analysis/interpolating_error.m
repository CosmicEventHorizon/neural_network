clear
syms x

%parameters to modify
f = sin(x); %function to be interpolated
x_i = [0,pi/6,2*pi/6, pi/2]; %points to interpolate
maximum_c = 1; %change depedning on the derivative 
x_p = 1; %point to usee fo error calculation

%algorithm parameters
m = length(x_i);
dif_f_m=diff(f,x,m);
disp(dif_f_m);


%calculate error
error=1;
for i=1:m
    error = error*(x_p - x_i(i));
    disp(x_p-x_i(i));
end
error = maximum_c*error/factorial(m);
disp(error);
