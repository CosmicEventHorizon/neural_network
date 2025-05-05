clear

%parameters to modify
A=[3, -2, 4; 30, -21, 45; -24, 14, -15];
b=[-30; -309; 201];
A_copy = A;
b_copy = b;

%algorithm variables
[n, m] = size(A);
x = sym('x', [1 n]);
d = sym('d', [1 n]);
L=eye(n);

%gaussian algorithm without b modification
for i=1:n
    for j=i+1:n
        c=A(j,i)/A(i,i);
        L(j,i) = c;
        for k=1:n
            A(j,k) = A(j,k) - c*A(i,k);
        end
    end
end
U=A;

fprintf("The L U Matrixes are: \n");
disp(L);
disp(U);

%forward substitution
%di = (bi-sigma(j=1,i-1)(L(i,j)*d(j)))/cii i=2,3,4...n
d(1) = vpa(b(1)/L(1,1));
for i=2:n
    sum=0;
    for j=1:i-1
        sum = sum + L(i,j)*d(j);
    end
    d(i) = vpa((b(i) - sum)/L(i,i));
end

fprintf("The d matrix is: \n");
disp(d);

%backward substitution
%xi = (bi-sigma(j=i+1,n)(A(i,j)*x(j)))/cii  i=n-1,n-2...1
x(n) = vpa(d(n)/U(m,n));
for i=n-1:-1:1
    sum=0;
    for j=i+1:n
        sum = sum + U(i,j)*x(j);
    end
    x(i) = vpa((d(i) - sum)/U(i,i));
end


for i=1:n
    fprintf("x%d = %e\n", i, double(x(i)));
end

fprintf("\n\n");

%check
fprintf("The correct answer is: \n");
answer = A_copy\b_copy;
disp(answer);

