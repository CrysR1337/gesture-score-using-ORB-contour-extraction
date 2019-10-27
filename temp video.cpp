#include<iostream>
#include<vector>

#include<opencv2\core\core.hpp>
#include<opencv2\features2d\features2d.hpp>
#include<opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;


int main()
{
	//【1】从摄像头读入视频  
	VideoCapture capture(0);//打开摄像头  
	if (!capture.isOpened())//没有打开摄像头的话，就返回。
		return 0;
	Mat edges; //定义一个Mat变量，用于存储每一帧的图像
//【2】循环显示每一帧  
	while (1)
	{
		Mat frame, image, temp; //定义一个Mat变量，用于存储每一帧的图像  
		capture >> frame;  //读取当前帧                          
		if (frame.empty())
		{
			break;
		}
		else
		{
			//读取图片
			Mat rgbd1 = frame;
			Mat rgbd2 = imread("hand4.png");
			//imshow("rgbd1", depth2);
			//waitKey(0);
			Ptr<ORB> orb = ORB::create();
			vector<KeyPoint> Keypoints1, Keypoints2;
			Mat descriptors1, descriptors2;
			orb->detectAndCompute(rgbd1, Mat(), Keypoints1, descriptors1);
			orb->detectAndCompute(rgbd2, Mat(), Keypoints2, descriptors2);

			//cout << "Key points of image" << Keypoints.size() << endl;

			//可视化，显示关键点
			Mat ShowKeypoints1, ShowKeypoints2;
			drawKeypoints(rgbd1, Keypoints1, ShowKeypoints1);
			drawKeypoints(rgbd2, Keypoints2, ShowKeypoints2);
			imshow("Keypoints1", ShowKeypoints1);
			imshow("Keypoints2", ShowKeypoints2);
			waitKey(0);

			//Matching
			vector<DMatch> matches;
			BFMatcher bfMatcher(NORM_HAMMING, true);
			bfMatcher.match(descriptors1, descriptors2, matches);
			//Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce");
			//matcher->bfmatch(descriptors1, descriptors2, matches);

			const float minRatio = 1.f / 1.5f;
			const int k = 2;

			vector<vector<DMatch>> knnMatches;
			bfMatcher.knnMatch(descriptors1, descriptors2, knnMatches, 2);
			//matcher->knnMatch(leftPattern->descriptors, rightPattern->descriptors, knnMatches, k);

			for (size_t i = 0; i < knnMatches.size(); i++) {
				const DMatch& bestMatch = knnMatches[i][0];
				const DMatch& betterMatch = knnMatches[i][1];
				float  distanceRatio = bestMatch.distance / betterMatch.distance;
				if (distanceRatio < minRatio)
					matches.push_back(bestMatch);
			}

			sort(matches.begin(), matches.end());
			//cout << "find out total " << matches.size() << " matches" << endl;
			float avg = 0.0, score;
			for (int i = 0; i <= 50; i++)
			{
				avg += matches[i].distance;
				//cout << "matches[" << i << "].distance= " << matches[i].distance<<endl;
			}
			avg = avg / matches.size();
			cout << "avg= " << avg << endl;
			score = 50 + 50 / avg;
			cout << "score= " << score << endl;


			//可视化
			Mat ShowMatches;
			drawMatches(rgbd1, Keypoints1, rgbd2, Keypoints2, matches, ShowMatches);
			//imshow("matches", ShowMatches);
			waitKey(0);
		}
		waitKey(30); //延时30ms  
	}
	capture.release();//释放资源
	destroyAllWindows();//关闭所有窗口



	return 0;
}