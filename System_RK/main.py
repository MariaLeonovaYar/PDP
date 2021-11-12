import matplotlib.pyplot as plt

with open("point.txt") as textFile:
    lines = [line.split() for line in textFile]

z = lines[0]
y = lines[1]
x = lines[2]

for index in range (len(x)):
    x[index] = float(x[index])
for index in range (len(x)):
    y[index] = float(y[index])
for index in range (len(x)):
    z[index] = float(z[index])

fig = plt.figure()
ax = fig.add_subplot(projection='3d')

ax.plot(x, y)
ax.plot(x, z)

plt.show()