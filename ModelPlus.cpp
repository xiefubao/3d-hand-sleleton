#include "ModelPlus.h"
using namespace std;
using namespace cv;


string ModelPlus::Bone(int num) const {
  for(map<string, int>::const_iterator i = names.begin(); i != names.end(); i++)
    if(i->second == num) return i->first;
  return "";
}
bool ModelPlus::Check(std::vector<float> &DoF){
  bool res = true;
  for(int i = 0; i < min(DoF.size(),limit.size()); ++i){
    if(DoF[i] < limit[i].first){
      DoF[i] = limit[i].first;
      res =  false;
    }
    else if(DoF[i] > limit[i].second){
      DoF[i] = limit[i].second;
      res =  false;
    }
  }
  return res;
}
vector<Point3f> ModelPlus::GetObj(const vector<float> &DoF) const {;;
  vector<Point3f> Global_joints = joints;
  vector<Mat_<float> > GlobalR(R.size());
  for(int i = 0; i < Global_joints.size(); i++) {
    int st = DoF_idx[i], num = DoF_idx[i + 1] - st;
    GlobalR[i] = R[i].clone();
    Mat_<float> Rot(3, 3);
    switch(num) {
    case 6: Global_joints[i].x += DoF[st + 3];
            Global_joints[i].y += DoF[st + 4];
            Global_joints[i].z += DoF[st + 5];
            Rodrigues(Vec3f(DoF[st], DoF[st + 2],DoF[st + 1]), Rot); GlobalR[i] = Rot * GlobalR[i];
            break;
    case 2: Rodrigues(Vec3f(DoF[st], 0,DoF[st + 1]), Rot); GlobalR[i] = Rot * GlobalR[i];
            break;
    case 1: Rodrigues(Vec3f(DoF[st], 0,0  ), Rot); GlobalR[i] = Rot * GlobalR[i];
            break;
    default:break;
    }
  }
  for(int i = 0; i < tree_adj.size(); i++)
    for(int j = 0; j < tree_adj[i].size(); j++) {
      Mat_<float> tmp(3, 1);
      tmp(0, 0) = Global_joints[tree_adj[i][j]].x;
      tmp(1, 0) = Global_joints[tree_adj[i][j]].y;
      tmp(2, 0) = Global_joints[tree_adj[i][j]].z;
      tmp = GlobalR[i] * tmp;
      GlobalR[tree_adj[i][j]] = GlobalR[i] * GlobalR[tree_adj[i][j]];
      Global_joints[tree_adj[i][j]].x = tmp(0, 0) + Global_joints[i].x;
      Global_joints[tree_adj[i][j]].y = tmp(1, 0) + Global_joints[i].y;
      Global_joints[tree_adj[i][j]].z = tmp(2, 0) + Global_joints[i].z;;
    }

  return Global_joints;
}

bool ModelPlus::Init(const string &file) {
  // Load BoneInfo
  
    if(!ParseSkeleton(file)) return false;
    vector<int> DoF_num(DoF_idx.size());
     for(int i = 0; i < DoF_idx.size(); i++) DoF_num[i] = DoF_idx[i];
    DoF_idx[0] = 1; DoF_idx.resize(DoF_idx.size() + 1);
     for(int i = 1; i < DoF_idx.size(); i++) DoF_idx[i] = DoF_idx[i - 1] + DoF_num[i-1];
      for(int i = 0; i < DoF_idx.size(); i++)
        cout << DoF_idx[i] <<" ";
      cout << endl;

      for(int i = 0; i < DoF_idx.size(); i++)
        cout << DoF_num[i] <<" ";
      cout << endl;

      vector<Point3f> joints_ = joints;
  father.resize(joints.size());
  father[0] = 0;
  for(int i = 0; i < tree_adj.size(); i++)
    for(int j = 0; j < tree_adj[i].size(); j++) {
      R[tree_adj[i][j]] = R[i] * R[tree_adj[i][j]];
      father[tree_adj[i][j]] = i;
    }

    vector<Mat_<float> > R_inv(R.size());
    for(int i = 0; i < R.size(); i++) R_inv[i] = R[i].inv();

    joints_ = joints;
    for(int i = 0; i < tree_adj.size(); i++)
      for(int j = 0; j < tree_adj[i].size(); j++) {
        Mat_<float> tmp(3, 1);
        tmp(0, 0) = joints[tree_adj[i][j]].x;// - joints_[i].x;
        tmp(1, 0) = joints[tree_adj[i][j]].y;// - joints_[i].y;
        tmp(2, 0) = joints[tree_adj[i][j]].z;// - joints_[i].z;
        tmp = R_inv[i] * tmp;
        R[tree_adj[i][j]] = R_inv[i] * R_inv[tree_adj[i][j]].inv();
        joints[tree_adj[i][j]].x = tmp(0, 0);
        joints[tree_adj[i][j]].y = tmp(1, 0);
        joints[tree_adj[i][j]].z = tmp(2, 0);
     }
    joints[2]/=1.2;
    //joints[4]*=1.3;
    return true;
  }
