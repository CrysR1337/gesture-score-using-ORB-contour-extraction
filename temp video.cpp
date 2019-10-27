#include<iostream>
#include<vector>

#include<opencv2\core\core.hpp>
#include<opencv2\features2d\features2d.hpp>
#include<opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;


int main()
{
	//��1��������ͷ������Ƶ  
	VideoCapture capture(0);//������ͷ  
	if (!capture.isOpened())//û�д�����ͷ�Ļ����ͷ��ء�
		return 0;
	Mat edges; //����һ��Mat���������ڴ洢ÿһ֡��ͼ��
//��2��ѭ����ʾÿһ֡  
	while (1)
	{
		Mat frame, image, temp; //����һ��Mat���������ڴ洢ÿһ֡��ͼ��  
		capture >> frame;  //��ȡ��ǰ֡                          
		if (frame.empty())
		{
			break;
		}
		else
		{
			//��ȡͼƬ
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

			//���ӻ�����ʾ�ؼ���
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


			//���ӻ�
			Mat ShowMatches;
			drawMatches(rgbd1, Keypoints1, rgbd2, Keypoints2, matches, ShowMatches);
			//imshow("matches", ShowMatches);
			waitKey(0);
		}
		waitKey(30); //��ʱ30ms  
	}
	capture.release();//�ͷ���Դ
	destroyAllWindows();//�ر����д���



	return 0;
}