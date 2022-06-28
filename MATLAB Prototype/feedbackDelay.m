function [out,buffer] = feedbackDelay(in,buffer,delay,fbGain)
%FEEDBACKDELAY Summary of this function goes here
%   Detailed explanation goes here

out = in+fbGain*buffer(delay,1);
buffer = [out;buffer(1:end-1,1)];
end

