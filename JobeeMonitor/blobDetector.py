#!/usr/bin/env python

# Standard imports
import cv2
import numpy as np;
import sys

from os.path import basename, dirname

# Read image
#im = cv2.imread("2016-10-31-100701.jpg", cv2.IMREAD_GRAYSCALE)
im = cv2.imread(sys.argv[1], cv2.IMREAD_GRAYSCALE)

imgname = basename(sys.argv[1])
imgdirname = dirname(sys.argv[1])

blobImgName = imgdirname + '/blob_' + imgname
print blobImgName

# Setup SimpleBlobDetector parameters.
params = cv2.SimpleBlobDetector_Params()

# Change thresholds
params.minThreshold = 20;
params.maxThreshold = 150;

# Filter by Area.
params.filterByArea = True
params.minArea = 110
#params.maxArea = 100

# Filter by Circularity
params.filterByCircularity = True
params.minCircularity = 0.14

# Filter by Convexity
params.filterByConvexity = True
params.minConvexity = 0.02

# Filter by Inertia
params.filterByInertia = True
params.minInertiaRatio = 0.09

# Create a detector with the parameters
ver = (cv2.__version__).split('.')
if int(ver[0]) < 3 :
    detector = cv2.SimpleBlobDetector(params)
else :
    detector = cv2.SimpleBlobDetector_create(params)

# Set up the detector with default parameters.
# detector = cv2.SimpleBlobDetector()

# Detect blobs.
keypoints = detector.detect(im)
print keypoints
# Draw detected blobs as red circles.
# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
im_with_keypoints = cv2.drawKeypoints(im, keypoints, np.array([]), (0,0,255), cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)


cv2.imwrite(blobImgName, im_with_keypoints)
# Show keypoints
cv2.imshow("Keypoints", im_with_keypoints)
cv2.waitKey(0)
