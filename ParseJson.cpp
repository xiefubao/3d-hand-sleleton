#include "ModelPlus.h"
#include <iostream>
#include <fstream>
using namespace std;
string GetString(ifstream &fid, char &c) {
  string tmp = "";
  while(fid.get(c) && c != '\"');
  while(fid.get(c) && c != '\"') tmp += c;
  return tmp;
}
int GetUnsignedInt(ifstream &fid, char &c) {
  int tmp = 0;
  while(fid.get(c) && (c < '0' || c > '9'));
  while(c >= '0' && c <= '9') {
    tmp = tmp * 10 + (c - '0');
    if(!fid.get(c)) break;
  }
  return tmp;
}
cv::Mat_<float> getMatrix(cv::Point3f &p, float &theta){
  cv::Mat_<float> R_(3,3);
  float u = p.x;
  float v = p.y;
  float w = p.z;
  R_(0, 0) = u*u+(v*v+w*w)*cos(theta);
  R_(0, 1) = u*v*(1.0-cos(theta))-w*sin(theta); 
  R_(0, 2) = u*w*(1.0-cos(theta))+v*sin(theta);

  R_(1, 0) = u*v*(1-cos(theta))+w*sin(theta); 
  R_(1, 1) = v*v+(u*u+w*w)*cos(theta);
  R_(1, 2) = v*w*(1-cos(theta))-u*sin(theta);

  R_(2, 0) = u*w*(1.0-cos(theta))-v*sin(theta); 
  R_(2, 1) = v*w*(1.0-cos(theta))+u*sin(theta); 
  R_(2, 2) = w*w+(u*u+v*v)*cos(theta);
  return R_;
}
float GetFloat(ifstream &fid, char &c) {
  float tmp = 0; bool sign = false;
  while(fid.get(c) && (c < '0' || c > '9') && c != '.')
    if(c == '-') sign = true;
    else if(c != ' ') sign = false;
  if(c != '.')
    if(c < '0' || c > '9') return tmp;
    else {
      tmp = c - '0';
      while(fid.get(c) && c >= '0' && c <= '9') tmp = tmp * 10 + (c - '0');
    }
  float f = 1;
  if(c == '.')
    while(fid.get(c) && c >= '0' && c <= '9') tmp += (c - '0') * (f /= 10.);
  if(sign) tmp = -tmp;
  return tmp;
}
bool ModelPlus::ParseSkeleton(const string &file) {
  ifstream fid(file.c_str());
  if(!fid.is_open()) return false;
  char c; string tmp; int i = 0;
  limit.push_back(make_pair<float,float>(0.5,1.5));
  while((tmp = GetString(fid, c)) != "")
    if(tmp == "bone") {
      tmp = GetString(fid, c);
      names[tmp] = i++;
      tree_adj.resize(i);
     
    } else if(tmp == "child") {
      while(c != ']'){
        tree_adj[i-1].push_back(GetUnsignedInt(fid, c));
      }
    } else if(tmp == "DoF") {
      DoF_idx.push_back(GetUnsignedInt(fid, c));
    } else if(tmp == "position") {
      cv::Point3f p;
      p.x = GetFloat(fid, c);
      p.y = GetFloat(fid, c);
      p.z = GetFloat(fid, c);
      // p.x = p.x/5;
      // p.y = p.y/5-1.8;
      // p.z = p.z/5;
      joints.push_back(p);
    } 
    // else if(tmp == "Rotation") {
    //   cv::Mat_<float> R_(3, 3);
    //   R_(0, 0) = GetFloat(fid, c); R_(0, 1) = GetFloat(fid, c); R_(0, 2) = GetFloat(fid, c);
    //   R_(1, 0) = GetFloat(fid, c); R_(1, 1) = GetFloat(fid, c); R_(1, 2) = GetFloat(fid, c);
    //   R_(2, 0) = GetFloat(fid, c); R_(2, 1) = GetFloat(fid, c); R_(2, 2) = GetFloat(fid, c);
    //   R.push_back(R_);
    // }
    else if(tmp == "limit"){
      for(int j=0;j<DoF_idx[i-1];j++){
        pair <float,float> li;
        li.first = GetFloat(fid,c);
        li.second = GetFloat(fid,c);
        limit.push_back(li);
      }
    }
    else if(tmp == "Rotation"){
      cv::Point3f p;
      p.x = GetFloat(fid, c);
      p.y = GetFloat(fid, c);
      p.z = GetFloat(fid, c);
      float theta = GetFloat(fid, c);
      R.push_back(getMatrix(p,theta));
    }
  fid.close();
  if(i == 0) return false;
  if(joints.size() < i)
    joints.push_back(cv::Point3f(0, 0, 0));
  if(R.size() < i)
    R.push_back(cv::Mat_<float>::eye(3, 3));
  if(DoF_idx.size() < i)
    DoF_idx.push_back(0);
  return true;
}
