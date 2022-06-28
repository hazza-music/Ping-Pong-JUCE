clear all; clc;
% Input Signal
in = [1;-1;2;-1;zeros(6,1)];

buffer = zeros(20,1); % Long Delay

delay = 5; % Number of Samples to Delay by

% Feedback Gain
fbGain = 0.5;

% Output Vector
N = length(in);
out = zeros(N,1);


for n = 1:N
    [out(n,1), buffer] = feedbackDelay(in(n,1),buffer,delay,fbGain);
end

t = [1:1:N];
plot(t,in,t,out,'--', 'LineWidth',7);set(gca,'FontSize',28);
xlabel('Time (sec)', 'FontSize', 30); ylabel('Amplitude', 'FontSize', 30);
title('Feedback Delay', 'FontSize', 30); legend('Signal', 'Delayed Signal', 'FontSize', 20);