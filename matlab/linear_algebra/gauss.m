clear
sym x

%parameters to modify
A=[3, 5, 2; 9, 20, 10; 21, 45, 27];
b=[-5; -12; 4];
A_copy = A;
b_copy = b;

%algorithm variables
[n, m] = size(A);
x = sym('x', [1 n]);


%gaussian algorithm
for i=1:n
    for j=i+1:n
        c=A(j,i)/A(i,i);
        for k=1:n
            A(j,k) = A(j,k) - c*A(i,k);
        end
        b(j) = b(j) - c*b(i);
    end
end

fprintf("The upper diagnoal matrix is:\n");
disp(A);


%backward substitution
%xi = (bi-sigma(j=i+1,n)(A(i,j)*x(j)))/cii  i=n-1,n-2...1
x(n) = vpa(b(n))/A(m,n);
for i=n-1:-1:1
    sum=0;
    for j=i+1:n
        sum = sum + A(i,j)*x(j);
    end
    x(i) = vpa((b(i) - sum)/A(i,i));
end

for i=1:n
    fprintf("x%d = %e\n", i, double(x(i)));
end

fprintf("\n\n");

%check
fprintf("The correct answer is: \n");
answer = A\b;
disp(answer);
