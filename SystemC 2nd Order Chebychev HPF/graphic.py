import matplotlib.pyplot as plt

time = list()
input = list()
output = list()

f = open("Chebychev 2nd Order.dat")
for line in f.readlines():
    time_s, input_s, output_s = line.split()

    if str(time_s).startswith("%"):
        continue
    time.append(float(time_s))
    input.append(float(input_s))
    output.append(float(output_s))


plt.plot(time, input, label = "input")
plt.plot(time, output, label = "output")
plt.legend()
plt.show()

f.close()
