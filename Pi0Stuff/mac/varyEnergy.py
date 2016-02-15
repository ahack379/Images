from pylab import *
import math as m
import numpy as np
import matplotlib.mlab as mlab
import sys

labels = []
levels = []

for e_pi0 in xrange(200,2300,400):

  theta=[]
  energy=[] 
  #e_pi0 = 139
  
  for e_1 in xrange(1,e_pi0):
  
    if abs(139 / 2 / m.sqrt(e_1*(e_pi0 - e_1)) ) > 1:
      continue
  
    angle = 2 * m.asin( 139 / 2 / m.sqrt(e_1*(e_pi0 - e_1)) )
    theta.append(180/3.142*angle)
    energy.append(e_1) 
  
  #for i, (x, y) in enumerate(energy):
  #plt.plot(x, y)

  
  plot(energy,theta,label=('$\pi^0$ Energy %g ' % e_pi0 ))
  text(energy[-1], theta[-1], '$\pi^0$ {i} MeV'.format(i=e_pi0))
  title('Energy of One Gamma vs Theta at Various $\pi^0$ Energies')
  xlabel('Energy of One Gamma')
  ylabel('3D Angle between showers')
  labels.append('$\pi^0$ Energy %s' % e_pi0)
  levels.append(e_pi0)


  #delta = 1 
  #X,Y = np.meshgrid(energy, theta) 
  #Z = 2 * np.arcsin( 139 / 2 / np.sqrt(X*(e_pi0 - X)) )

  #fmt ={}
  #for i,level in enumerate(levels):
  #  fmt[level] = labels[i]
  #  print 'first label: ', labels[i]
  #
  #CS = plt.contour(X,Y,Z,levels=levels)
  #plt.clabel(CS, inline=1, fontsize=14,fmt=fmt)

#print 'number of levels and labels: ', len(levels), ", " , len(labels)
#sys.stdin.readline()
grid()
show()
