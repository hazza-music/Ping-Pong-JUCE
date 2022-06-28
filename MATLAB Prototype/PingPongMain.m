%% The DSP design for a JUCE Ping Pong Delay
%Changable variables:
%   fbGainL -> gain amount for left channel
%   fbGainR -> gain amount for right channel
%   delay -> number of samples to delay by
%   wetGainL -> Left Wet Mix
%   wetGainR -> Right Wet Mix

clear;clc; 
%% Creating a multi channel sine wave
% Allows for prototyping with clear results
Fs = 48000; 
Ts = 1/Fs; 
fl = 2; % Left channel 
fr = 5; % Right channel
t = [0:Ts:1].';
in = [sin(2*pi*fl*t),sin(2*pi*fr*t)]; % Creation of a sine wave channel array

%% Plotting the Original Signal
% For comparsion
figure(1)
hold on
plot(t,in,'LineWidth',5)
set(gca,'FontSize',28);
xlabel('Time (sec)', 'FontSize', 30); ylabel('Amplitude', 'FontSize', 30);
title('Mutli Channel Sine Waves', 'FontSize', 30); legend('Left Signal', 'Right Signal', 'FontSize', 20);
hold off

%% Ping Pong Delay
% Build around a feedback delay
N = length(in);
data = zeros(N,1);
%buffer = [zeros(N,1)';zeros(N,1)'];
buffer = [data, data];
delay = 9; % Number of Samples to Delay by

% Feedback Gains
fbGainL = 0.5;
fbGainR = 0.7;

% Wet Gains
wetGainL = 0.9;
wetGainR = 0.4;

% Output Vector
out = [data, data];

for n = 1:N
    [out(n,1), buffer] = PingPongDelay(in(n,1),buffer,delay,fbGainL);
    [out(n,2), buffer] = PingPongDelay(in(n,2),buffer,delay,fbGainR);

end

for n = 1:N

    out(n,1) = wetGainL.*out(n,1)+in(n,1);
    out(n,2) = wetGainR.*out(n,2)+in(n,2);

end

%% Plotting the Delayed Signal
% For comparsion
figure(2)
hold on
plot(t,in,t,out,'--', 'LineWidth',7)
set(gca,'FontSize',28);
xlabel('Time (sec)', 'FontSize', 30); ylabel('Amplitude', 'FontSize', 30);
title('Ping Pong Delay', 'FontSize', 30); legend('Left Signal', 'Right Signal','Left Delayed Signal','Right Delayed Signal' ,'FontSize', 20);
hold off