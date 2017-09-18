%
%---Basic system model
%
clear all;
h=0.1;
A=[1 h;0.5*h 1];
A2=[1 h;0.5*h*0.95 0.95];
B=[((h^2)/2); h];
C=eye(2);%[1 0];
n=size(A,1);
m=size(B,2);
%
%---Parameters
%
q=diag([10 1]);
Pf=q;%diag([1 1]);
r=0.1;
N=5;
x0=[0 0]';


%
%---Define matrices for the QP
AA=[A;zeros(n*N-2,2)];
Aeq =[kron(eye(N),eye(2))+kron(diag(ones(N-1,1),-1),-A),kron(eye(N),-B)];
H=blkdiag(kron(eye((N-1)),1*q.*C'*C),1*Pf*eye(2),kron(eye(N),1*r));

% 
% Gamma=kron(eye(N),B);
% temp2=[];
% Omega=[];
% for j=1:N
%     
%     temp=kron(diag(ones(N-j,1),-j),(A^j)*B);
%     Gamma=Gamma+temp;
%     temp2=[temp2 A^(N-j)*B]; 
%     Omega=[Omega; A^j];
% end
%Gamma=blkdiag(Gamma,B);
%Gamma(N:size(B,1)+N-1,1:N-1)=temp2
%Omega=vertcat(Omega,A^N);

% Q=q*eye(size(Gamma,1)-2);
% Q=blkdiag(Q,Pf);
% R=r*eye(N);
% 
% H=Gamma'*Q*Gamma+R;
% f=2*x0'*Omega'*Q*Gamma;

f=zeros(0,length(H));

%---For the case with actuator constraints
%

%%%Setpoint tracking
% System has 2 outputs, Angle and Angular velocity. 
C_z=[1 0];      %Setting angle as only controlled output
z_sp=[-0.3];    %Setpoint for angle. 
track_m=[(eye(2)-A) -B;C_z 0];  



%%

Ain=horzcat(zeros(2*N),vertcat(eye(N),-1*eye(N)));
bin=ones(2*N,1).*0.5%;*(1-abs(u_s));   %Calculate new constraints for the RHC depending on the steady state setpoint u_s. "Whats left of the input for me to play with"


%
%---Cost function

%%
%
%---MPC algorithm
%
T=50; % simulation time

xk=x0; % initialize state vector
yvec=zeros(0,2);
uvec=[];
delta_xkvec=[];
options = optimset('Algorithm','interior-point-convex','Display','off');
ss_target=track_m\([0;0;z_sp]);
x_s=ss_target(1:2,1);
%x_s(2,1)=0;
u_s=ss_target(2,1);

for k=1:T


%Compute an update steady state targets (Setpoint tracking) eq 56
% ss_target=track_m\([0;0;z_sp]);
% x_s=ss_target(1:2,1);
% u_s=ss_target(2,1);



delta_xk=xk-x_s;

beq=AA*delta_xk; % The matrix AA defines how the last measured state xk
% determines the right hand side of the inequality condition.

delta_z=quadprog(H,f,Ain,bin,Aeq,beq,[],[],[],options);
%u=quadprog(H,f,Ain,bin,[],[],[],[],[],options);
delta_uk=delta_z(n*N+1);

uk=delta_uk+u_s;
%xk=xk+delta_xk;
%xk=
% 
% if(k>150)
%     xk=A2*xk+B*uk;
% else
%     xk=A*xk+B*uk;
%     
% end
xk=A*xk+B*uk;
yk=C*xk;
yvec=[yvec; (yk)'];
delta_xkvec=[delta_xkvec; delta_xk];
uvec=[uvec; uk];
end
tvec=h*(1:1:T);
%subplot(3,1,1) % For the other two sets of parameters you should change
% the third index to 2 and 3, respectively.
figure(1)
plot(tvec,yvec(:,1),'-',tvec,uvec,'--'); grid
legend('Output angle','input torque')
figure(2)
plot(tvec,yvec(:,2),'-',tvec,uvec,'--'); grid
legend('angular velocity','input torque')

%% Simulation
% 
% us=-0.34;
% inputs=size(us,1);
% outputs=size(2,1);
% states=2;
% yvec=zeros(0,2);
% uvec=[];
% 
% x0=[0.69,0]';
% xk=x0;
% T=50
% for k=1:T,
% 
%     xk=A*xk+B*uk;
%     yk=C*xk;
%     yvec=[yvec; (yk)'];
%     uvec=[uvec; uk];
%     
% end
% tvec=h*(1:1:T);
% %subplot(3,1,1) % For the other two sets of parameters you should change
% % the third index to 2 and 3, respectively.
% figure(3)
% plot(tvec,yvec(:,1),'-',tvec,uvec,'--'); grid
% figure(4)
% plot(tvec,yvec(:,2),'-',tvec,uvec,'--'); grid

