import sys
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import cv2
import numpy as np

src = cv2.imread('pi0.png') 
src_gray = cv2.cvtColor( src, cv2.COLOR_BGR2GRAY) #CV_BGR2GRAY );

source_window = cv2.namedWindow("Source") #,flags=CV_WINDOW_AUTOSIZE );

thresh = 30 

fig, (ax0,ax1,ax2) = plt.subplots(nrows=1,ncols=3,figsize=(10,5))

#blur = cv2.blur( src_gray, (17,17) )
#blur = cv2.resize(blur,(int(0.2*len(blur)),int(0.2*len(blur[0]))),0,0,0)

#blur = cv2.pyrDown(src_gray,(0.3*len(src_gray),0.3*len(src_gray[0]))) #,0,0,0)
blur = cv2.pyrDown(src_gray)
blur = cv2.medianBlur(src_gray,11)

canny_output=cv2.Canny( blur, thresh, 2*thresh, 3 );
_, contours, hier = cv2.findContours(canny_output, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

#ax0.imshow(canny_output,interpolation='nearest', cmap = 'gray')
ax1.imshow(canny_output,cmap='gray')
ax0.imshow(blur) #,cmap = 'gray')

for n, c in enumerate(contours):
#  print "the contour: ",c[:,0][:,0]
#  print "the contour: ",c[:,0][:,1]
  
  ar = c[:,0]
  temp_x = ar[:len(ar)/2,0]
  temp_x = np.append(temp_x,ar[0][0])

  temp_y = ar[:len(ar)/2,1]
  temp_y = np.append(temp_y,ar[0][1])
 
#  temp_x = ar[:,0]
#  temp_y = ar[:,1]
  area = cv2.contourArea(c)
  if area > 15:
    print "Area is : ", area
    #cv2.drawContours(src_gray,contours,n,(0,0,100),3)
    ax2.plot(temp_y, temp_x,linewidth= 2)
  #  ax2.axis([0,50,15,45])

#cv2.waitKey(0)

plt.title('Thresh %g, blur %g' % (thresh,17) )
fig.canvas.draw()

plt.draw()
plt.show(block=False)

cv2.waitKey(0)
#sys.stdin.readline()
