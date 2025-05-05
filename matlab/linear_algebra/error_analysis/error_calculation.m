clear

%Parameters to modify
A=[4,4,-2;1,0,1;-2,-3,2]; %for calculating maximum forward error
b=[30;7;-18];
xr=[2.1;9.0;4.5]; %algorithm answer for calculating relative backward error/maximum relative forward error
r=A\b;  %real answer for calculating relative forward error


%calculate the relative backward error
infinity_norm = norm(A, Inf);
backward_error = norm(A*xr-b,Inf)/norm(b,Inf);

%calculate the relative forward error
%Method 1: using the real answer
forward_error = norm(r-xr,Inf)/norm(r,Inf);
%Method 2: using the estimated error maginification factor
maximum_m = cond(A,Inf);  %real answer
maximum_forward_error = maximum_m*backward_error;

%magnification factor
m = forward_error/backward_error;
precision=12;
digits_used=16;
number_of_digits_needed = precision+log10(maximum_m);
%digits_used is number of digits we using in calculation, default 16
%log10(M) gives me digits lost
%precision is the number of digits our answer is correct to
%digits_used-log10(M)=precision


%the error
fprintf("The infinity norm of A is: %.5f\n",infinity_norm);
fprintf("The maximum error magnification factor is %.16f\n", maximum_m);
fprintf("The number of digits needed for a precision of %d is atleast %.16f\n", precision, number_of_digits_needed);
fprintf("The maximum relative forward error is %.16f\n", maximum_forward_error);
fprintf("The relative backward error is %.16f\n", backward_error);
fprintf("The real relative forward error is %.16f\n", forward_error);
fprintf("The error maginification factor is %.16f\n", m);