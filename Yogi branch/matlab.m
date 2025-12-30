function matlab_base_spec_full()
% MATLAB Base Specification Full
% Comprehensive script covering signal processing, control design,
% optimization, machine learning, simulation, and GUI.
%
% This file serves as a master template for MATLAB educational projects.

%% Section 1: Signal Processing Demo
fs = 1000;               % Sampling frequency [Hz]
t = 0:1/fs:1-1/fs;       % Time vector
x = sin(2*pi*50*t) + 0.3*randn(size(t));
[b,a] = butter(5,100/(fs/2));  % 5th-order lowpass Butterworth
y = filter(b,a,x);
figure('Name','Signal Processing');
subplot(2,1,1); plot(t, x); title('Noisy Signal');
subplot(2,1,2); plot(t, y); title('Filtered Signal');

%% Section 2: Control Design (State Feedback)
A = [0 1; -2 -3];
B = [0; 1];
C = [1 0]; D = 0;
Q = diag([10,1]); R = 0.1;
K = lqr(A,B,Q,R);
sys_cl = ss(A-B*K, B, C, D);
figure('Name','Control Design'); step(sys_cl); title('Closed-Loop Step Response');

%% Section 3: Optimization Demo
f = [-1; -2];
A_opt = [1 2; -1 1; 0 1];
b_opt = [4; 1; 1];
lb = [0; 0];
options = optimoptions('linprog','Display','none');
[x_opt,fval] = linprog(f, A_opt, b_opt, [], [], lb, [], options);
disp('Optimization Result:'); disp(x_opt');

%% Section 4: Machine Learning (K-means)
data = [randn(100,2)+1; randn(100,2)-1];
[idx, C] = kmeans(data, 2);
figure('Name','Machine Learning'); gscatter(data(:,1), data(:,2), idx); hold on;
plot(C(:,1), C(:,2), 'kx','LineWidth',2); title('K-means Clustering');

%% Section 5: Inverted Pendulum Simulation
dt = 0.01; T = 10; steps = T/dt;
m = 0.2; M = 0.5; l = 0.3; g = 9.81;
A = [0 1 0 0; 0 0 -m*g/M 0; 0 0 0 1; 0 0 (M+m)*g/(M*l) 0];
B = [0; 1/M; 0; -1/(M*l)];
Qp = diag([10,1,10,1]); Rp = 0.001;
Kp = lqr(A,B,Qp,Rp);
x = [0.1;0;0.2;0]; X = zeros(4,steps); U = zeros(1,steps);
for k=1:steps
    u = -Kp*x;
    x = x + (A*x + B*u)*dt;
    X(:,k) = x; U(k) = u;
end
figure('Name','Inverted Pendulum');
subplot(3,1,1); plot(0:dt:T-dt, X(1,:)); ylabel('Position'); title('Cart Position');
subplot(3,1,2); plot(0:dt:T-dt, X(3,:)); ylabel('Angle'); title('Pendulum Angle');
subplot(3,1,3); plot(0:dt:T-dt, U); ylabel('Control'); xlabel('Time [s]');

%% Section 6: GUI Interface
if ~ismcc && ~isdeployed
    fig = uifigure('Name','Pendulum Control GUI','Position',[100 100 600 400]);
    lbl = uilabel(fig,'Position',[20 350 100 22],'Text','Angle Weight');
    sld = uislider(fig,'Position',[150 360 300 3],'Limits',[1 100],'Value',10);
    ax = uiaxes(fig,'Position',[50 50 500 280]);
    sld.ValueChangedFcn = @(src,event) updatePlot(src,ax,dt,T,A,B);
end
end

function updatePlot(src, ax, dt, T, A, B)
% Update pendulum response based on slider value
Q = diag([10,1,round(src.Value),1]); R = 0.001;
Kp = lqr(A,B,Q,R);
steps = T/dt; x = [0.1;0;0.2;0]; X = zeros(4,steps);
for k=1:steps
    u = -Kp*x; x = x + (A*x + B*u)*dt; X(:,k) = x;
end
plot(ax, 0:dt:T-dt, X(3,:)); title(ax,'Pendulum Angle');
end
