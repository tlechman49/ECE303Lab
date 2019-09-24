%%
close all;clear all;clc;

batt_bins = [5.3852 5.5619 5.7674 5.9264 6.1138 6.2839 6.4414 6.7518 7.2364 8.3341]; %%10 bins 5.0444 

app = appGUI;

s = serial('COM66', 'BaudRate', 9600);
set(s,'Terminator','CR/LF');
fopen(s);


lastReadData = [0;0;0;0];
totalDistance = 0.0; %miles
lastPowerSwitchVal = app.PowerSwitch.Value;
lastRpmKnobVal = app.SetRPMKnob.Value;
countsPerSecond = 0;
liquidAlarm = 0;
tempAlarm = 0;
battPct = 0;
mph = 0;
rpm = 0;

if(1)
    % Create equipment connections and objects
    DMM=visa('agilent', 'USB0::0x2A8D::0xB318::MY58230014::0::INSTR');

    % Open instruments 
    fopen(DMM);

    % Conduct experiment by sweeping waveform generator voltage
    Vmeas = 0;
else
    Vmeas = 8.5;
end

voltCounter = 11;
%%

tStart = 0;
while(1)
    % update the flags if there is a change
    %disp(fscanf(s));
    [readData,ret] = fscanf(s, '%d.%d.%d.%d'); 
    if (ret < 4)
        %do nothing
        disp(ret);
        disp(readData);
    elseif (isequal(lastReadData, readData))
        % do nothing!
        disp(readData);
    else
        lastReadData = readData;
        disp(readData);
        
        countsPerSecond = readData(2);
        liquidAlarm = readData(3);
        tempAlarm = readData(4);
        
        %update alarms
        if(liquidAlarm)
           app.LowCoolantLamp.Color = 'red';
        else
           app.LowCoolantLamp.Color = 'green';
        end
        if(tempAlarm)
           app.HighTemperatureLamp.Color = 'red';
        else
           app.HighTemperatureLamp.Color = 'green';
        end
        if(countsPerSecond > 80)
            app.CoolingFanLamp.Color = 'blue';
        else
           app.CoolingFanLamp.Color = [0.65 0.65 0.65];
        end
        
        %update speed gauges
        rpm = countsPerSecond * 60 / 32;
        mph = rpm * 0.000137311 * 60;
        app.RPMGauge.Value = rpm;
        app.MPHGauge.Value = mph;
    end
    
    %send the message to the arduino if there is a change
    if (app.SetRPMKnob.Value == lastRpmKnobVal && app.PowerSwitch.Value == lastPowerSwitchVal)
        %do nothing!
    else
        lastRpmKnobVal = app.SetRPMKnob.Value;
        lastPowerSwitchVal = app.PowerSwitch.Value;
        
        fprintf(s,['%d.%c\n'],[int16(app.SetRPMKnob.Value) app.PowerSwitch.Value]);
        fprintf('%d.%c\n',int16(app.SetRPMKnob.Value), app.PowerSwitch.Value);
    end
    
    % total distance
    if(tStart == 0)
        tStart = tic;
    else
        tElapsed = toc(tStart);
        tStart = tic;
        totalDistance = totalDistance + tElapsed * mph / 3600.0;
        app.TotalDistancesmiTextArea.Value = sprintf('%f', totalDistance);
    end
    
    %measure battery
    if(voltCounter > 2)
        voltCounter = 0;
        Vmeas = str2num(query(DMM, 'MEAS:VOLT:DC?'));
    else
        voltCounter = voltCounter + 1;
        pause(0.5);
    end
    battPct = 0;
    for i = 1:length(batt_bins)
       if( Vmeas < batt_bins(i))
           battPct = (i - 1) * 10;
           break;
       elseif (i == 10 && Vmeas > batt_bins(i))
           battPct = i * 10;
       end
    end
    app.BatteryLifeGauge.Value = battPct;
end
%%
close all force;
delete(s)

% Close and delete instrument connections and objects
fclose(DMM);
delete(DMM);
