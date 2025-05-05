clear


%x=(A^T*A)^-1 * A^T *y

%parameters to modify
A=[1,1,0;0,1,1;1,2,1;1,4,-2];
y=[2;3;-1;4];

%algorithm
x=inv(A'*A)*A'*y;
yestimate = A*x;
SE = norm(yestimate - y,2);
disp(x);
fprintf("error is %.7f \n",SE); 