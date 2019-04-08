# Experiment 1C−O s c i l l o s c o p e Data Lab
import visa
import numpy as np
import time
import matplotlib.pyplot as plt

# Inpu t v al u e s
awg_address = 'USB0::0x0957::0x0407::MY44043483::0::INSTR' # Waveform Generator Address # Waveform Generator Address
scope_address = 'USB0::0x0957::0x1799::MY58100818::0::INSTR' # O s c i l l o s c o p e Address
pulse_frequency = 1400 # Pul se f r e q u e n c y (Hz)
pulse_amplitude = 5 # Pul se ampli tude (V)
volts_per_division = 2 # Scope V ol t s pe r d i v i s i o n
time_range = 3E-3 # Scope time r an ge ( s )
duty_cycle = np.linspace(20, 80, 13) # Duty c y c l e v e c t o r (%)
numpoints = 5000
filename='experiment1c.csv' # F i l e name f o r data output

# I n i t i a l i z e imp o r t an t m a t ri c e s
num_dc = duty_cycle.size # Number o f duty c y c l e t r i a l s
freq = np.zeros(num_dc)
signals = np.zeros((numpoints, num_dc))
times = np.zeros(numpoints)

# I n i t i a t e communications with and open i n s t r um e n t s
rm = visa.ResourceManager()
awg = rm.open_resource(awg_address)
scope = rm.open_resource(scope_address)

# Pl ace waveform g e n e r a t o r i n High−Z
awg.write('OUTP:LOAD INF')

# Se t up waveform g e n e r a t o r
freq_str = str(pulse_frequency)
amp_str = str(pulse_amplitude)
offset_str = str(pulse_amplitude / 2 )
cmd_str = 'APPL:SQU' + freq_str + ' , ' + amp_str + ' , ' + offset_str
awg.write(cmd_str)

# Se t up o s c i l l o s c o p e
scope.values_format.is_binary = False
scope.values_format.datatype = ' f '
scope.values_format.is_big_endian = True
scope.values_format.container = np.array;
scope.write(':CHANNEL1:RANGE' + str( 8* volts_per_division))
scope.write(':TIMEBASE:MODE NORMAL; RANGE' + str(time_range))
scope.write(':AUTOSCALE;')
scope.write('WAV:SOUR CHAN1')
scope.write('WAV:FORM ASC;')
scope.write('WAV:POINTS:MODE RAW')
temp_str='WAV:POIN '+str(numpoints)
scope.write( temp_str)
scope.write(':WAVEFORM:BYTEORDER LSBFIRST')
scope.write(':TRIG:EDGE:SOUR CHAN1')
scope.write(':TRIG:EDGE:LEV 0.4')


 # Conduct Measurements
count = 0
for I in duty_cycle:
    print( 'Duty Cycle %i%c ' %(I , '%' ) )
    pulse_str='PULSE:DCYC ' + str(I)
    awg.write(pulse_str)
    scope.write('DIG CHAN1;')
    operationComplete = bool(scope.query('*OPC?'))
    while not operationComplete:
        operationComplete = bool(scope.query('*OPC?'))
    time.sleep(1)
    data=scope.query('WAV:DATA?')
    temp=data[10:-1]
    temp=temp.split(',')
    temp=np.array([float(x) for x in temp])
    freq [count]= scope.query(':MEAS:FREQUENCY? CHAN1' )
    signals[:, count] = temp
    count=count+1

# Break up the preamble bl o c k
preambleBlock = scope.query(':WAVEFORM:PREAMBLE?' )
preamble = preambleBlock.split(',')
dt = float(scope.query(':WAV:XINC?'))
t0 = float(scope.query(':WAV:XOR?'))
dV = float(scope.query('WAV:YINC?'))
V0 = float(scope.query(':WAV:YOR?'))
V_ref = float(scope.query(':WAV:YREF?'))

# Convert t o u s e f u l data
times = (np.linspace(0, numpoints -1, numpoints)).dot(dt)+ t0
#v o l t s = ( ( s i g n a l s − V r e f ) ) . dot (dV) + V0
volts = signals

# Write data t o f i l e
data=np.append(np.transpose([times]),volts, axis=1)
np.savetext(filename, data, delimiter=',')

# Pl o t S i g n a l s vs . V ol t a ge
plt.plot(times * 1000, volts, markersize =4)
plt.xlabel('Time (ms)')
plt.ylabel('Voltage (V)')
plt.title('PWM Signal Analysis')
plt.grid(True)
plt.show() # Make pl o t v i s i b l e