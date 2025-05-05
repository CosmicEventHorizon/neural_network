clear
format long

syms x real

%parameters to modify
X=[1,2];
Y=[4,3.4];
F = [x^0, x, x^2, x^3]; %functions array
x_interpolate = 1.5; %approxiamte a value of x


%test data
%{
y=10-4*sin(x)+2*cos(x);
X=[];
Y = [];
for i=1:1:10
    X(i) = i;
    Y(i) = subs(y,x,i);
end
disp(X);
disp(Y);
%}

%algorithm parameters
N = length(F);
M = length(X);

%construct the A coefficient matrix
A =[];
for i=1:1:N
    for j=1:1:N
        sum=0;
      for k=1:1:M
          sum = sum+subs(F(j),x,X(k))*subs(F(i),x,X(k));
      end
      A(i,j) = sum;
    end
end

%construct the b matrix
B=[];
for i=1:1:N
    sum=0;
    for j=1:1:M
        sum = sum + subs(F(i), x, X(j))*Y(j);
    end
    B(i,1)=sum;
end

solution = A\B;

format long
disp(solution);

%{
Xapp = linspace(0,10,100);    
for i=1:1:100
    Yapp(i)=0;
    for j=1:1:N
        Yapp(i) = Yapp(i) + solution(j,1)*subs(F(j),x, Xapp(i));
    end
end
plot(X,Y,'o',Xapp,Yapp);
%}

%RMSE stuff
sum_Er=0;
sum_y=0;
for i=1:1:M
    y_app=0;
    for j=1:1:N
        y_app = y_app + solution(j,1)*subs(F(j),x, X(i));
    end
    sum_Er = sum_Er + (Y(i)-(y_app))^2; %dont forget to change format of y_app
    sum_y = sum_y + Y(i);
end
mean_y = sum_y/M;

sum_Et=0;
for i=1:1:M
    sum_Et = sum_Et + (Y(i)-mean_y)^2;
end
fprintf("The regression coefficient is %.5f\n", 1-sum_Er/sum_Et);
fprintf("The square error is %.5f\n", sum_Er);
fprintf("The mean square error is %.5f\n", sum_Er/M);
fprintf("The root mean square error is %.5f\n", sqrt(sum_Er/M));



%interpolate test
%{
y_interpolate=0;
for j=1:1:N
    y_interpolate = y_interpolate + solution(j,1)*subs(F(j),x, x_interpolate);
end
fprintf("The value of y at x=%.2f is %.15f\n", x_interpolate, y_interpolate);
%}


%{
a0=solution(1);
a1 = solution(2);
a2 = solution(3);
a3 = solution(4);
x=X;
model=a0 + a1*x + a2*cos(2*pi*x)+a3*sin(2*pi*x);
plot(x,model);
residual = model'-Y;
%}