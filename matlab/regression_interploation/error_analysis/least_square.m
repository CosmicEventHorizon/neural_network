clear
syms x real

load coal

%parameters to modify
X = coal(:,3); %points xi; 
Y = coal(:,2); %points yi
F = [x^0, x, x^2, x^3]; %functions array
solution=[3754574.076841494, 0180985.782675553,-0011783.336814019,0000350.993728570];

%algorithm parameters
N = length(F);
M = length(Y);



sum_Er=0;
for i=1:1:M
    y_app=0;
    for j=1:1:N
        y_app = y_app + solution(j)*subs(F(j),x, X(i));
    end
    sum_Er = sum_Er + (Y(i)-y_app)^2; %change this if format of y= is different
end
fprintf("The square error is %.4f\n", sum_Er);
fprintf("The mean square error is %.4f\n", sum_Er/M);
fprintf("The root mean square error is %.4f\n", sqrt(sum_Er/M));
