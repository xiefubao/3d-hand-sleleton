#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#include "ModelPlus.h"
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>
#define SOLID 1
#define WIRE 2
using namespace std;
using namespace cv;
int moveX,moveY;
int spinX = 0;
int spinY = 0;
int des = 0;
int rot = 0;
const GLfloat PI = 3.1415927;
std::vector<float> DoF;
ModelPlus hand;
#include <sys/time.h>  
long getCurrentTime()  
{  
   struct timeval tv;  
   gettimeofday(&tv,NULL);  
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;  
} 

void init() {
   //定义光源的颜色和位置
   GLfloat ambient[] = { 1, 1, 1, 0.1 };
   GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat position[] = { 0.0, 0.0, -8.0, 1.0 };
   //选择光照模型
   GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };  
   GLfloat local_view[] = { 0.0 };  
   glClearColor(0.0, 0.0, 0.0, 0.0);   
   glShadeModel(GL_SMOOTH);  
   //设置环境光
   glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);  
   
   //设置漫射光
   glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);  
   //设置光源位置
   glLightfv(GL_LIGHT0, GL_POSITION, position); 
   //glLightfv(GL_LIGHT0, GL_POSITION, position1); 
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);  
   glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);  
   //启动光照
   glEnable(GL_LIGHTING);  
   //启用光源
   glEnable(GL_LIGHT0); 


//    GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };

// GLfloat light_ambient [] = { 0.0, 0.0, 0.0, 1.0 };

// GLfloat light_diffuse [] = { 1.0, 1.0, 1.0, 1.0 };

// GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

// glLightfv(GL_LIGHT0, GL_POSITION, light_position);

// glLightfv(GL_LIGHT0, GL_AMBIENT , light_ambient );

// glLightfv(GL_LIGHT0, GL_DIFFUSE , light_diffuse );

// glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

//画球
void drawBall(double R, cv::Point3f p,int model) {
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
   GLfloat mat_ambient[] = { 0.7, 0.7, 0.7, 1.0 };
   GLfloat mat_ambient_color[] = { 0.8, 0.8, 0.2, 1.0 };
   GLfloat mat_diffuse[] = { 0.1, 0.5, 0.8, 1.0 };
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat no_shininess[] = { 0.0 };
   GLfloat low_shininess[] = { 5.0 };
   GLfloat high_shininess[] = { 100.0 };
   GLfloat mat_emission[] = {0.3, 0.2, 0.2, 0.0};
   glPushMatrix();
   glTranslated(p.x,p.y,p.z);
   switch(model){
   	case 0:
	   glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	   glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	   glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	   break;
	case 1:
	   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	   glMaterialfv(GL_FRONT, GL_SHININESS, low_shininess);
	   glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	   break;
	case 2:
	   glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	   glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	   glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	   break;
	case 3:
	   glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient_color);
	   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	   glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);
	   glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);
	   break;
	case 4:
	   glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	   glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	   glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	   glMaterialfv(GL_FRONT, GL_SHININESS, no_shininess);
	   glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
	   break;

   }
   

   

	glutSolidSphere(R,20,20);
 	glPopMatrix();
}

void Keyboard(unsigned char key, int x, int y) {
  if(key>='A'&&key<='Z')
   	key = 'a'+key-'A';
  switch(key) {
  case  27: glutDestroyWindow(glutGetWindow()); exit(0);
  case 'r': memset(DoF.data() + 1, 0, sizeof(float) * (DoF.size() - 1));
            hand.GetObj(DoF); cout << "Reset" << endl; break;
  case '0': cout << "Adjust Translation" << endl; rot = 22; break;

  case 'a': cout << "Adjust Bone " << hand.Bone(rot = 0) << endl; break;
  case 'b': cout << "Adjust Bone " << hand.Bone(rot = 1) << endl; break;
  case 'c': cout << "Adjust Bone " << hand.Bone(rot = 2) << endl; break;
  case 'd': cout << "Adjust Bone " << hand.Bone(rot = 3) << endl; break;
  case 'e': cout << "Adjust Bone " << hand.Bone(rot = 5) << endl; break;
  case 'f': cout << "Adjust Bone " << hand.Bone(rot = 6) << endl; break;
  case 'g': cout << "Adjust Bone " << hand.Bone(rot = 7) << endl; break;
  case 'h': cout << "Adjust Bone " << hand.Bone(rot = 9) << endl; break;
  case 'i': cout << "Adjust Bone " << hand.Bone(rot = 10) << endl; break;
  case 'j': cout << "Adjust Bone " << hand.Bone(rot = 11) << endl; break;
  case 'l': cout << "Adjust Bone " << hand.Bone(rot = 13) << endl; break;
  case 'm': cout << "Adjust Bone " << hand.Bone(rot = 14) << endl; break;
  case 'n': cout << "Adjust Bone " << hand.Bone(rot = 15) << endl; break;
  case 'o': cout << "Adjust Bone " << hand.Bone(rot = 17) << endl; break;
  case 'p': cout << "Adjust Bone " << hand.Bone(rot = 18) << endl; break;
  case 'q': cout << "Adjust Bone " << hand.Bone(rot = 19) << endl; break;
  default: return;
  }
  glutPostRedisplay();
}
void Special(int key, int x, int y) {
  if(rot < hand.DoF_idx.size() - 1) {
    GLfloat d = 5.f /150.f * PI;
    switch(key) {
    case GLUT_KEY_UP:		DoF[hand.DoF_idx[rot]] += d;break;
    case GLUT_KEY_DOWN:     DoF[hand.DoF_idx[rot]] -= d;break;
    case GLUT_KEY_RIGHT:    if(hand.DoF_idx[rot + 1] - hand.DoF_idx[rot] > 1)
                              DoF[hand.DoF_idx[rot] + 1] += d;
                            break;
    case GLUT_KEY_LEFT:     if(hand.DoF_idx[rot + 1] - hand.DoF_idx[rot] > 1)
                              DoF[hand.DoF_idx[rot] + 1] -= d;
                            break;
    case GLUT_KEY_PAGE_UP:  if(hand.DoF_idx[rot + 1] - hand.DoF_idx[rot] > 2)
                              DoF[hand.DoF_idx[rot] + 2] += d;
                            break;
    case GLUT_KEY_PAGE_DOWN:if(hand.DoF_idx[rot + 1] - hand.DoF_idx[rot] > 2)
                              DoF[hand.DoF_idx[rot] + 2] -= d;
                            break;
    default: return;
    }
  } else {
    GLfloat d = .5f;
    switch(key) {
    case GLUT_KEY_DOWN:     DoF[5] -= d; break;
    case GLUT_KEY_UP:       DoF[5] += d; break;
    case GLUT_KEY_LEFT:     DoF[4] -= d; break;
    case GLUT_KEY_RIGHT:    DoF[4] += d; break;
    case GLUT_KEY_PAGE_UP:  DoF[6] -= d; break;
    case GLUT_KEY_PAGE_DOWN:DoF[6] += d; break;
    default: return;
    }
  }
  hand.Check(DoF);
  hand.GetObj(DoF);
  glutPostRedisplay();
}
void renderCylinder(cv::Point3f p1, cv::Point3f p2, float radius)
{
float x1 = p1.x;
float y1 = p1.y;
float z1 = p1.z;
float x2 = p2.x;
float y2 = p2.y;
float z2 = p2.z;

float vx = x2-x1;
float vy = y2-y1;
float vz = z2-z1;
//handle the degenerate case with an approximation
if(vz == 0)
vz = .00000001;
float v = sqrt( vx*vx + vy*vy + vz*vz );
float ax = 57.2957795*acos( vz/v ); 
if ( vz < 0.0 )
ax = -ax;
float rx = -vy*vz;
float ry = vx*vz;

GLUquadricObj *quadric=gluNewQuadric();
//gluQuadricNormals(quadric, GLU_SMOOTH);

glPushMatrix();

glTranslatef( x1,y1,z1 );
glRotatef(ax, rx, ry, 0.0);

//draw the cylinder
gluCylinder(quadric, radius, radius, v, 32, 1);
//gluCylinder(cyl_obj, 0.1, 0.1, cylHgt, 12, 1);

glTranslatef(-x1,-y1,-z1);
gluDisk(quadric, radius, radius, 32, 1);
glPopMatrix();

gluDeleteQuadric(quadric);
}
void RenderBone(cv::Point3f p1, cv::Point3f p2,float width){
	glLineWidth(width); 
	
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y, p1.z);
	glVertex3f(p2.x, p2.y, p2.z);
	glEnd();
}
void display(void) {
	//清除缓冲区颜色
	//glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.18f, 0.04f, 0.14f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//定义白色
	glColor3f(1.0, 1.0, 1.0); 
	//圆点放坐标中心
	glLoadIdentity();  
	//从哪个地方看

	gluLookAt(0.0, 0.0, -5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 
	glPushMatrix();
	glRotated(spinX, 0, 1, 0);
	glRotated(spinY, 1, 0, 0);
	GLfloat ambient[] = { 0,1,-5, 1 };

	glLightfv(GL_LIGHT0, GL_POSITION, ambient);
	
	//启用光
	glEnable(GL_LIGHT0);
	// glTranslated(0, 0, des);
	hand.current_joints=hand.GetObj(DoF);

	for(int  i = 0;i<DoF.size();++i){
		//cout << i << hand.current_joints[i] - hand.current_joints[hand.father[i]] << endl;
		cout << i<<": "<< DoF[i] <<endl;
	}
	for(int i = 0; i < hand.tree_adj.size(); i++)
      for(int j = 0; j < hand.tree_adj[i].size(); j++) 
      	{
      		//renderCylinder(hand.current_joints[i],hand.current_joints[hand.tree_adj[i][j]],0.08);
      		RenderBone(hand.current_joints[i],hand.current_joints[hand.tree_adj[i][j]],4);
        }
	for(int i =0;i<hand.current_joints.size();++i){
		int model;
		if(i==0)
			model = 0;
		else 
			model = i%4+1;
		drawBall(0.12,hand.current_joints[i],model);
	}
	glPopMatrix();
    glutSwapBuffers();
}
//鼠标点击事件
void mouseClick(int btn, int state, int x, int y) {
    moveX = x;
	moveY = y;

	//GLfloat ambient[] = { (float)rand() / RAND_MAX*3, (float)rand() / RAND_MAX*3, (float)rand() / RAND_MAX*3, 0.1 };
	GLfloat ambient[] = { 0,0,-3, 1 };
	//GLfloat ambient[] = { 1, 1, 1, 0.1 };
	 //设置环境光ïrot
	glLightfv(GL_LIGHT0, GL_POSITION, ambient);

	//启用光源

	glEnable(GL_LIGHT0);
}

// 鼠标移动事件 
void mouseMove(int x, int y) {
	int dx = x - moveX;
	int dy = y - moveY;
	//printf("dx;%dx,dy:%dy\n",dx,dy);
	spinX += dx;
	spinY += dy;
	glutPostRedisplay();
	moveX = x;
	moveY = y;
}

void reshape(int w, int h) {
	//定义视口大小
	glViewport(0, 0, (GLsizei) w, (GLsizei) h); 
	//投影显示
	glMatrixMode(GL_PROJECTION);
	//坐标原点在屏幕中心
	glLoadIdentity();
	//操作模型视景
	gluPerspective(60.0, (GLfloat) w/(GLfloat) h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
}
;
int main(int argc, char** argv) {

	if(!hand.Init("Skeleton3.json")){
		cout << "wrong with json" << endl;
	}

	DoF.resize(hand.DoF_idx[hand.DoF_idx.size()-1]);
	DoF[0] = 1;
	hand.current_joints = hand.GetObj(DoF);
	
	// long start = getCurrentTime();
	// for(int i  = 0;i<10000;++i){
	// 	hand.current_joints = hand.GetObj(DoF);
	// }
	// long end =getCurrentTime();
	// cout <<"cost time: " << end - start <<endl;
	cout <<"limit: " << hand.limit.size() << " " << endl;
	for(int i = 0 ;i<hand.limit.size();i++){
		cout << hand.limit[i].first << " " << hand.limit[i].second << endl;
	}
	
	//初始化
	glutInit(&argc, argv);
	//设置显示模式
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	//初始化窗口大小
	glutInitWindowSize(500, 500); 
	//定义左上角窗口位置
	glutInitWindowPosition(100, 100);
	//创建窗口
	glutCreateWindow(argv[0]);
	//初始化
	init();
	//显示函数
	glutKeyboardFunc(Keyboard);
  	glutSpecialFunc(Special);

	glutDisplayFunc(display);
	//窗口大小改变时的响应
	glutReshapeFunc(reshape);
	//鼠标点击事件，鼠标点击或者松开时调用
    glutMouseFunc(mouseClick);
	//鼠标移动事件，鼠标按下并移动时调用
    glutMotionFunc(mouseMove);
	//键盘事件
	//glutKeyboardFunc(keyPressed);
	//循环
	glutMainLoop();
	return 0;
}
