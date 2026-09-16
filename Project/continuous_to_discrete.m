clear 
clc
s = tf('s');

sys = (0.3246*s + 0.08954)/(s+0.344);

Ts =1/100;

sysz = c2d(sys, Ts)