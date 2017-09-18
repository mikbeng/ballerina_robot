function sys = lin_sys(m,M,l,g,b,k)

A=[0 1 0 0; (M+m)*g/(M*l) -(M+m)*k/(M*m*l^2) 0 b/(M*l); 0 0 0 1; -m*g/M k/(l*M) 0 -b/M];
B=[0; -1/(M*l); 0; 1/M];
C=eye(4);
D=zeros(size(C,1),1);


sys=ss(A,B,C,D);

end