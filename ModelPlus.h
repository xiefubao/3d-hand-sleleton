#ifndef _MODEL_PLUS_H
#define _MODEL_PLUS_H
#include <list>
#include <vector>
#include <opencv2/opencv.hpp>
#define PRECISION 1e-5
class ModelPlus {
public:
  bool Init(const std::string &file);
  std::vector<int> DoF_idx;
  std::string Bone(int num) const;
  std::vector<cv::Point3f> GetObj(const std::vector<float> &DoF_)const;
  bool Check(std::vector<float> &DoF);
  void hand_init();
  bool ParseSkeleton(const std::string &file);


  std::vector<int> father;
  std::vector<cv::Point3f> joints;
  std::vector<cv::Point3f> current_joints;
  std::vector<cv::Mat_<float> > R;
  std::vector<std::vector<int> > tree_adj;
  std::vector<std::pair<float,float> > limit;
  std::map<std::string, int> names;

};
#endif
