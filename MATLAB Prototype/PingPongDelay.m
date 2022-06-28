function [out,buffer] = PingPongDelay(in,buffer,delay,fbGain)
%PINGPONGDELAY Feedback Delay to Opposite Stereo Channels
%   A copy of the feedback delay with feedback to other channels

out = in+fbGain*buffer(delay,1);
buffer = [out;buffer(1:end-1,1)];
end