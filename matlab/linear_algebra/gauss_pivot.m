clear

%pivoting is done
%avoid problems of division by zero
%reduce danger of loss of significance

%parameters to modify
A=[1, -1, 3; -1, 0, -2; 2, 2, 4];
b=[-3;1;0];
A_copy = A;
b_copy = b;

%algorithm variables
[n, m] = size(A);
x = sym('x', [1 n]);




%gaussian algorithm
for i=1:n
    %at the start of modifying every row call the pivoting function to reorganize the equations
    [A_after,b_after] = start_pivot(A,b,n,m);
    A=A_after;
    b=b_after;
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
x(n) = vpa(b(n)/A(m,n));
for i=n-1:-1:1
    sum=0;
    for j=i+1:n
        sum = sum + A(i,j)*x(j);
    end
    x(i) = vpa((b(i) - sum)/A(i,i));
end

%output
for i=1:n
    fprintf("x%d = %e\n", i, double(x(i)));
end

fprintf("\n\n");

%check
fprintf("The correct answer is: \n");
answer = A\b;
disp(answer);

%pivoting function
function [A_after, b_after] = start_pivot(Ain,bin,n,m)
    A=Ain;
    b=bin;
    for i=1:n
        max = abs(A(i,i));
        max_j = i;
        for j=i+1:n
            if(max<abs(A(j,i)))
                max=A(j,i);
                max_j = j;
            end
        end
        if(max_j>i)
            for k=1:m
                temp=A(i,k);
                A(i,k) = A(max_j,k);
                A(max_j,k) = temp;
            end
            tempB=b(i);
            b(i,1) = b(max_j,1);
            b(max_j,1) = tempB;
        end
    end
    A_after = A;
    b_after = b;
end


