#    Gesture scoring using ORB & contour extraction on C++

------

The whole project is based on opencv version 4.1.1. Version 4.1.1 has quite a lot differences with the earlier opencv 2 and 3. Some of the functions change and some variables or constants are moved to different libraries. 

Eg. The project refers to different blogs and research.

This is a individual C++ part of https://github.com/King-LeBron-James/Microsoft-Project. This project is contributed by King-LeBron-James & farthjun also.

## Basic theory

1. Firstly, to contour extraction, we use GaussianBlur (function) to smooth pixels and then Canny (function) to detect edge. At last, according to findContours and drawContours (function) to get and save the contours information. The result is shown below.

2. To score the posture and match the cosntours, we use ORB method to do feature extraction and matchin. We use detectAndCompute (function) provided by data type ORB to find out all the features and use drawKeypoints (function) to visualize feature. After calculating features of the video image and the model image, we use bfmatcher and knnmatch (function in bfmatcher) to calculate the distance of features in two images. Then, we add the most significant 50 distance and use a home-made algorithm to change it as a 100 score (different platform or opencv version needs a different algorithm). 

 

## Problems and mistakes during the process

1. To get contours of images more efficient and accurate, we have tried different methods. Canny, sobel algorithm and so on has been tested and we also test 3×3，5×5，9×9 operator. The results have not much difference.

2. Then we focus on how to distinguish the background. If we can ignore background noise and only do image processing to the main part, the result will be better. We find 3 typical ways: optical flow method, background subtraction method and frame difference method. The algorithm can detect the background and the method is sensitive to movement. If you are interested in these area, https://github.com/upcAutoLang/BackgroundSplit-OpenCV , it shows the C++ project and theory.

3. When doing research, we find that using one camera to get a body contour is limited by technology. Kinect camera can easily solve the problem, because it has two cameras with depth information. Moreover, Kinect provide us a new data source called BodyIndex. It can at most distinguish 6 bodies in one picture at a same time. And functions like get_FrameDescription are included in library Kinect.h. If you are interested in depth camera and more, please find by yourself.

4. When we concentrate on how to give score, we have spent quite a long time using findContours (function), trying to make the callback data suitable. However, the accurate detail about the operating principle of the function is scarce. We can’t find accurately when and how the function give us a number.
