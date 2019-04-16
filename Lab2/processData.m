vSegment = V(1:51);
fSegment = f(1:51)*60;
countsPerSecondSegment = countsPerSecond(1:51)*60;

for i = 1:length(fSegment)
    if fSegment(i) > 500*60
        fSegment(i) = 0;
    end
end

figure(1)
plot(vSegment, fSegment)
xlabel('Voltage')
ylabel('RPM')
title('RPM vs. Voltage from Oscilloscope')
grid on

figure(2)
plot(vSegment, countsPerSecondSegment)
xlabel('Voltage')
ylabel('RPM')
title('RPM vs. Voltage from Arduino')
grid on