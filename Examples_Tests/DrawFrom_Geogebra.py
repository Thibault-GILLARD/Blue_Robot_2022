# PORT COM
import serial
import time
import csv
# Settings

SerialObj = serial.Serial('COM14')#
SerialObj.baudrate = 115200
SerialObj.bytesize = 8
SerialObj.parity = serial.PARITY_NONE
SerialObj.stopbits = 1
# INFOS
print("\nChanged Settings")
print('Port =', SerialObj.port)
print('Baudrate =', SerialObj.baudrate)
print('ByteSize =', SerialObj.bytesize)
print('Parity = ', SerialObj.parity)
print('StopBits =', SerialObj.stopbits)

# class in which we are defining the
# sliders and its effects

liste = []
liste_num = []
command = []


def print_out(message):
    msg = message
    mdg_str_encode = str(msg).encode('ascii')
    SerialObj.write(mdg_str_encode)
    RecivedStr = SerialObj.readline()
    print(RecivedStr)
    # return  RecivedStr


with open('Dessin_1.txt') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    line_count = 0
    for row in csv_reader:
        liste.append(row[0] + "," + row[1] + "," + row[2])
print(liste)

for element in liste:
    newstr = element.replace("draw((", "")
    newstr = newstr.replace("--","")
    subnewstr = newstr.split(')(')
    subnewstr[1] = subnewstr[1].replace(")","")
    subnewstr0 = subnewstr[0].split(',')
    subnewstr1 = subnewstr[1].split(',')
    liste_num.append(subnewstr0[0])
    liste_num.append(subnewstr0[1])
    liste_num.append(subnewstr1[0])
    liste_num.append(subnewstr1[1])

print(liste_num)

ints = []
for element in liste_num:
    ints.append(float(element))
print(ints)

Dim = 297
marge = 30
Dim_marge = (Dim - marge)/2
print(Dim_marge)


max_list = max(ints)

for i in range(len(ints)):
    if (i % 2) == 0:
        ints[i] = int((ints[i] * (Dim_marge/max_list)-125))
    else :
        ints[i] = int((ints[i] * (Dim_marge/max_list)-100))

print("-----------------------------------------------")
print(ints)
print("-----------------------------------------------")

command.append('HOME')
count = 0
for i in range(int(len(ints)/4)):
    command.append('AIR,U,800')
    command.append('MOVE,'+ str(ints[count])+',' + str(ints[count+1])+','+'0')
    command.append('AIR,D,2200')
    command.append('MOVE,'+ str(ints[count+2])+',' + str(ints[count+3])+','+'0')
    count += 4
print(command)
print(len(command))

for element in command:
    print_out(element)


