import cv2
import numpy as np
from matplotlib import pyplot as plt

img = cv2.imread('../assets/Lenna.png',0)

blur = cv2.GaussianBlur(img,(5,5),1.4)

cv2.imshow('image',img)
cv2.imshow('image blue',blur)
cv2.waitKey(0)
cv2.destroyAllWindows()