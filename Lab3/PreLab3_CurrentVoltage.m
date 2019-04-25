close all;clear all;clc

% Create equipment connections and objects
DMM=visa('agilent', 'USB0::0x2A8D::0xB318::MY58150015::0::INSTR');

% Open instruments 
fopen(DMM);

% Conduct experiment by sweeping waveform generator voltage
Imeas = [0];
Vmeas = [5];
t = [0];
tic();
while Vmeas(end) > 4.8
    %pause(1)
    voltage = str2num(query(DMM, 'MEAS:VOLT:DC?'));
    current = str2num(query(DMM, 'MEAS:CURR:DC? 1, 3.0e-5'));
    Vmeas = [Vmeas ; -voltage];
    Imeas = [Imeas ; current];
    t = [t ; toc()];
end

% Close and delete instrument connections and objects
fclose(DMM);
delete(DMM);

