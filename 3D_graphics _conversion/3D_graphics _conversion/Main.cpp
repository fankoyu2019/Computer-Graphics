#include "iostream"
#include "gl/glut.h"
#include "string"
#include "vector"
#include "set"
#include "stdio.h"
#include "math.h"
#include "algorithm"
#include "fstream"

#define WIDTH_RIGHT 300 //窗口右边界
#define WIDTH_LEFT -300	//窗口左边界
#define HEIGHT_TOP 300	//窗口上边界
#define HEIGHT_BOTTOM -300 //窗口下边界
#define MAGNIFICATION 20//放大倍数
#define PI 3.14159265 
using namespace std;

void Display1();
void Display2();
void Draw(int argc,char *argv[]);
void Matrix_mul(vector<vector<double>> &vertex1,vector<vector<double>> &vertex2,vector<vector<double>> &vertex);
void Revolve();
void Symmetric();
void Shear();
void BasicTransformation();
void Complex_Revolve();
void Complex_Symmetric_Line();
void Complex_Symmetric_Point();
void Complex_Symmetric();
void ComplexTransformation();

vector<vector<double>> T(4,vector<double>(4,0));//最后的变换矩阵
vector<vector<double>> Tnew(4,vector<double>(4,0));//进行这一步操作要用到的变换矩阵
vector<vector<double>> Ttmp(4,vector<double>(4,0));//临时变换矩阵，用来存矩阵相乘的结果
vector<vector<double>> GraphPoint;
vector<vector<int>> IndexPoint;//对要进行绘制的面进行编号，便于绘制
int arr[3]; //arr[0] 表示端点总数 arr[1]表示面片总数

/*
矩阵相乘函数
功能:实现矩阵相乘运算
传入参数：vertex1:左矩阵 vertex2 右矩阵 vertex 结果矩阵
*/
void Matrix_mul(vector<vector<double>> &vertex1,vector<vector<double>> &vertex2,vector<vector<double>> &vertex){
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++){
			vertex[i][j]=0;
		}
	}
	int n=vertex1.size();
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<4;k++)
			{
				vertex[i][j]+=vertex1[i][k]*vertex2[k][j];
			}
		}
	}

	/*for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++){
			cout<<vertex[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<endl;*/
}


/*
绘制变换前图形函数
功能：运用OpenGL绘制变换前的图形
*/
void Display1()
{   

	//gluOrtho2D(WIDTH_LEFT,WIDTH_RIGHT,HEIGHT_BOTTOM,HEIGHT_TOP);//控制绘图取值范围
	//glTranslatef(WIDTH_LEFT+100,HEIGHT_BOTTOM+100,0);
	/*
	vector<vector<double>> axis(4,vector<double>(4,0));
	for(int i=0;i<2;i++)
	{
		axis[2*i][i]=-1;axis[2*i][3]=1;
		axis[2*i+1][i]=1;axis[2*i+1][3]=1;
	}
	//vector<vector<double>> axisTrs(6,vector<double>(4,0));
	//Matrix_mul(axis,DisT,axisTrs);
	*/
	
	double disT[4][4]={
		1,0,0,0,
		0,1,0,0,
		0.5*cos(45*PI/180),0.5*sin(45*PI/180),0,0,
		0,0,0,1,
	};
	vector<vector<double>> DisT;//斜二投影转换矩阵
	for(int i=0;i<4;i++){
		vector<double> temp(disT[i],disT[i]+4);
		DisT.push_back(temp);
	}
	vector<vector<double>> Vertex(GraphPoint.size(),vector<double>(4,0));
	Matrix_mul(GraphPoint,DisT,Vertex);//斜二投影转换
	GraphPoint=Vertex;
	
	glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_TRIANGLES);
	for (int i = 0; i <arr[1]; i++)
    {
		int size=IndexPoint[i].size();
        for (int j = 0; j <size; j++) {
			glVertex3f(GraphPoint[IndexPoint[i][j]][0],GraphPoint[IndexPoint[i][j]][1],GraphPoint[IndexPoint[i][j]][2]);
        }
    }
	glEnd();
	glFlush();
}
/*
绘制变换后图形函数
功能：运用OpenGL绘制变换后的图形
*/
void Display2()
{   

	//gluOrtho2D(WIDTH_LEFT,WIDTH_RIGHT,HEIGHT_BOTTOM,HEIGHT_TOP);//控制绘图取值范围
	//glTranslatef(WIDTH_LEFT+100,HEIGHT_BOTTOM+100,0);
	
	
	
	vector<vector<double>> VertexTrs(GraphPoint.size(),vector<double>(4,0));
	Matrix_mul(GraphPoint,T,VertexTrs);
	

	double disT[4][4]={
		1,0,0,0,
		0,1,0,0,
		0.5*cos(45*PI/180),0.5*sin(45*PI/180),0,0,
		0,0,0,1,
	};
	vector<vector<double>> DisT;
	for(int i=0;i<4;i++){
		vector<double> temp(disT[i],disT[i]+4);
		DisT.push_back(temp);
	}
	vector<vector<double>> Vertex(GraphPoint.size(),vector<double>(4,0));
	Matrix_mul(VertexTrs,DisT,Vertex);//斜二投影转换
	VertexTrs=Vertex;
	
	glClear(GL_COLOR_BUFFER_BIT); 
    glBegin(GL_TRIANGLES);
	for (int i = 0; i <arr[1]; i++)
    {
		int size=IndexPoint[i].size();
        for (int j = 0; j <size; j++) {
			glVertex3f(VertexTrs[IndexPoint[i][j]][0],VertexTrs[IndexPoint[i][j]][1],VertexTrs[IndexPoint[i][j]][2]);
        }
    }
	glEnd();
	glFlush();
}

/*
展示窗口函数
功能：将绘制的图形展示
*/
void Draw(int argc,char *argv[])
{
	
	glutInit(&argc,argv);    //初始化glut,必须调用，复制黏贴这句话即可  
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE); //设置显示方式，RGB、单缓冲。当然还有GLUT_INDEX索引颜色 GLUT_DOUBLE双缓冲(Qt中看到过双缓冲)  
    glutInitWindowPosition(50,100);   //位置  
	glutInitWindowSize(WIDTH_RIGHT-WIDTH_LEFT,HEIGHT_TOP-HEIGHT_BOTTOM);//窗口大小  
    glutCreateWindow("3D_graphics_conversion1");  //创建窗口，设置标题  
	
	glutDisplayFunc(&Display1);  // 当绘制窗口时调用myDisplay，像Cocos2d-x刷帧Draw中的操作  
	//glutIdleFunc(&renderScene);
	glutInitWindowPosition(650,100);   //位置  
	glutCreateWindow("3D_graphics_conversion2");  //创建窗口，设置标题  
	
	glutDisplayFunc(&Display2);  // 当绘制窗口时调用myDisplay，像Cocos2d-x刷帧Draw中的操作  

    glutMainLoop();  //消息循环 
}
/*
基本变换-旋转界面函数
功能：对图形进行旋转
*/
void Revolve()
{
	cout<<"*------------------------------------*"<<endl;
	cout<<"          1.绕x轴旋转                      "<<endl;
	cout<<"          2.绕y轴旋转                   "<<endl;
	cout<<"          3.绕z轴旋转                      "<<endl;
	cout<<"请选择：";int choose; cin>>choose;
	cout<<"输入按逆时针方向旋转角度[0--360]（°）：";double p;
	cin>>p;
	double Cos=cos(p*PI/180),Sin=sin(p*PI/180);
	switch (choose)
	{
	case 1:
		T[0][0]=Cos;T[0][1]=Sin;T[1][0]=-Sin;T[1][1]=Cos;break;
	case 2:
		T[0][0]=Cos;T[0][2]=-Sin;T[2][0]=Sin;T[2][2]=Cos;break;
	case 3:
		T[1][1]=Cos;T[1][2]=Sin;T[2][1]=-Sin;T[2][2]=Cos;break;
	default:
		break;
	}
}

/*
基本变换-对称界面函数
功能：对图形进行对称变换
*/
void Symmetric()
{
	cout<<"*------------------------------------*"<<endl;
	cout<<"          1.XOY平面对称                      "<<endl;
	cout<<"          2.YOZ平面对称                   "<<endl;
	cout<<"          3.XOZ平面对称                      "<<endl;
	cout<<"          4.X轴对称                      "<<endl;
	cout<<"          5.Y轴对称               "<<endl;
	cout<<"          6.Z轴对称                      "<<endl;
	cout<<"请选择：";
	int k;
	cin>>k;
	switch (k)
	{
	case 1:T[2][2]=-1;break;
	case 2:T[0][0]=-1;break;
	case 3:T[1][1]=-1;break;
	case 4:T[1][1]=-1;T[2][2]=-1;break;
	case 5:T[0][0]=-1;T[2][2]=-1;break;
	case 6:T[0][0]=-1;T[1][1]=-1;break;
	default:
		break;
	}
}
/*
基本变换-错切界面函数
功能：对图形进行错切变换
*/
void Shear()
{
	cout<<"*------------------------------------*"<<endl;
	cout<<"          1.沿x轴方向错切                      "<<endl;
	cout<<"          2.沿y轴方向错切                   "<<endl;
	cout<<"          3.沿z轴方向错切                      "<<endl;
	cout<<"请选择：";
	int k;
	cin>>k;
	double a,b;
	cout<<"输入错切系数:";
	cin>>a>>b;
	switch (k)
	{
	case 1:
		Tnew[1][0]=a;Tnew[2][0]=b;break;
	case 2:
		Tnew[0][1]=a;Tnew[2][1]=b;break;
	case 3:
		Tnew[0][2]=a;Tnew[1][2]=b;break;
	default:
		break;
	}
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
}
/*
基本几何变换界面函数
功能：选择要进行的变换操作
*/
void BasicTransformation()
{
	cout<<"*--------基本几何变换----------------*"<<endl;
	cout<<"          1.平移                      "<<endl;
	cout<<"          2.旋转                   "<<endl;
	cout<<"          3.缩放                      "<<endl;
	cout<<"          4.对称                   "<<endl;
	cout<<"          5.错切                   "<<endl;
	cout<<"请选择：";
	int k;
	cin>>k;
	double x,y,z;
	switch (k)
	{
	case 1:
		cout<<"x轴方向平移距离 y轴方向平移距离 z轴方向平移距离:" ;cin>>x>>y>>z;
		T[3][0]=x;T[3][1]=y;T[3][2]=z;T[3][3]=1;
		break;
	case 2:
		Revolve();	break;
	case 3:
		cout<<"x轴方向缩放倍数 y轴方向缩放倍数 z轴方向缩放倍数:" ;cin>>x>>y>>z;
		T[0][0]=x;T[1][1]=y;T[2][2]=z;
		break;
	case 4:
		Symmetric();
		break;
	case 5:
		Shear();break;
	default:
		break;
	}
}
/*
复合变换-绕直线旋转界面函数
功能：设置旋转轴以及旋转角度，进行旋转
*/
void Complex_Revolve()
{
	cout<<"请输入绕直线旋转的直线两端点值(x1,y1,z1) , (x2,y2,z2)：";
	double x1,y1,z1,x2,y2,z2;
	cin>>x1>>y1>>z1>>x2>>y2>>z2;
	cout<<"请输入绕逆时针旋转角度[0--360]（°）：";
	double p;
	cin>>p;
	double Sinp=sin(p*PI/180),Cosp=cos(p*PI/180);

	bool flag1=false;
	if(x1==0&&y1==0&&z1==0){}
	else{
		//进行平移变换
		flag1=true;
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++){
				Tnew[i][j]=0;
			}
			Tnew[i][i]=1;
		}
		Tnew[3][0]=-x1,Tnew[3][1]=-y1,Tnew[3][2]=-z1;
		Matrix_mul(T,Tnew,Ttmp);
		T=Ttmp;
	}
	double v_x=x2-x1,v_y=y2-y1,v_z=z2-z1;//线段的向量表示
	double ABlen=sqrt(pow(v_x,2)+pow(v_y,2)+pow(v_z,2));//线段的长度
	double BB_=v_x;//B点与它在YOZ平面的垂点B'的x方向 向量
	double AB_len=sqrt(pow(v_z,2)+pow(v_y,2));//A点与垂点B'的长度
	double Sina=v_y/AB_len,Cosa=v_z/AB_len;
	if(AB_len==0){//特殊判断
		Sina=0;Cosa=1;
	}
	double Sinb=BB_/ABlen,Cosb=AB_len/ABlen;
	//cout<<ABlen<<" "<<BB_<<" "<<AB_len<<" "<<Sina<<" "<<Cosa<<" "<<Sinb<<" "<<Cosb<<endl<<endl;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++){
			Tnew[i][j]=0;
		}
		Tnew[i][i]=1;
	}
	//将旋转轴旋转到XOZ平面
	Tnew[1][1]=Cosa;Tnew[1][2]=Sina;Tnew[2][1]=-Sina;Tnew[2][2]=Cosa;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[1][1]=1;Tnew[1][2]=0;Tnew[2][1]=0;Tnew[2][2]=1;
	//将旋转轴旋转到与Z轴重合
	Tnew[0][0]=Cosb;Tnew[0][2]=Sinb;Tnew[2][0]=-Sinb;Tnew[2][2]=Cosb;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[0][0]=1;Tnew[0][2]=0;Tnew[2][0]=0;Tnew[2][2]=1;
	//进行设置的旋转变换
	Tnew[0][0]=Cosp;Tnew[0][1]=Sinp;Tnew[1][0]=-Sinp;Tnew[1][1]=Cosp;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[0][0]=1;Tnew[1][1]=1;Tnew[0][1]=0;Tnew[1][0]=0;
	//将旋转轴旋转到与Z轴重合的逆变换
	Tnew[0][0]=Cosb;Tnew[0][2]=-Sinb;Tnew[2][0]=Sinb;Tnew[2][2]=Cosb;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[0][0]=1;Tnew[0][2]=0;Tnew[2][0]=0;Tnew[2][2]=1;
	//将旋转轴旋转到XOZ平面的逆变换
	Tnew[1][1]=Cosa;Tnew[1][2]=-Sina;Tnew[2][1]=Sina;Tnew[2][2]=Cosa;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[1][1]=1;Tnew[1][2]=0;Tnew[2][1]=0;Tnew[2][2]=1;
	//如果进行了平移变换最后还要进行平移变换的逆变换
	if(flag1)
	{
		Tnew[3][0]=x1,Tnew[3][1]=y1,Tnew[3][2]=z1;
		Matrix_mul(T,Tnew,Ttmp);
		T=Ttmp;
	}
}
/*
复合变换-关于直线对称界面函数
功能：设置对称轴，找到它的对称变换矩阵
*/
void Complex_Symmetric_Line()
{
	cout<<"请输入关于某直线对称的直线两端点值(x1,y1,z1) , (x2,y2,z2)：";
	double x1,y1,z1,x2,y2,z2;
	cin>>x1>>y1>>z1>>x2>>y2>>z2;
	bool flag1=false;
	if(x1==0&&y1==0&&z1==0){}
	else{
		flag1=true;
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++){
				Tnew[i][j]=0;
			}
			Tnew[i][i]=1;
		}
		Tnew[3][0]=-x1,Tnew[3][1]=-y1,Tnew[3][2]=-z1;
		Matrix_mul(T,Tnew,Ttmp);
		T=Ttmp;
	}
	double v_x=x2-x1,v_y=y2-y1,v_z=z2-z1;
	double ABlen=sqrt(pow(v_x,2)+pow(v_y,2)+pow(v_z,2));
	double BB_=v_x;
	double AB_len=sqrt(pow(v_z,2)+pow(v_y,2));
	double Sina=v_y/AB_len,Cosa=v_z/AB_len;
	if(AB_len==0){
		Sina=0;Cosa=1;
	}
	double Sinb=BB_/ABlen,Cosb=AB_len/ABlen;
	cout<<ABlen<<" "<<BB_<<" "<<AB_len<<" "<<Sina<<" "<<Cosa<<" "<<Sinb<<" "<<Cosb<<endl<<endl;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++){
			Tnew[i][j]=0;
		}
		Tnew[i][i]=1;
	}
	Tnew[1][1]=Cosa;Tnew[1][2]=Sina;Tnew[2][1]=-Sina;Tnew[2][2]=Cosa;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[1][1]=1;Tnew[1][2]=0;Tnew[2][1]=0;Tnew[2][2]=1;

	Tnew[0][0]=Cosb;Tnew[0][2]=Sinb;Tnew[2][0]=-Sinb;Tnew[2][2]=Cosb;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[0][0]=1;Tnew[0][2]=0;Tnew[2][0]=0;Tnew[2][2]=1;

	Tnew[0][0]=-1;Tnew[1][1]=-1;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[0][0]=1;Tnew[1][1]=1;

	Tnew[0][0]=Cosb;Tnew[0][2]=-Sinb;Tnew[2][0]=Sinb;Tnew[2][2]=Cosb;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[0][0]=1;Tnew[0][2]=0;Tnew[2][0]=0;Tnew[2][2]=1;

	Tnew[1][1]=Cosa;Tnew[1][2]=-Sina;Tnew[2][1]=Sina;Tnew[2][2]=Cosa;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[1][1]=1;Tnew[1][2]=0;Tnew[2][1]=0;Tnew[2][2]=1;

	if(flag1)
	{
		Tnew[3][0]=x1,Tnew[3][1]=y1,Tnew[3][2]=z1;
		Matrix_mul(T,Tnew,Ttmp);
		T=Ttmp;
	}
}
/*
复合变换-关于一点对称界面函数
功能：设置对称点，求其对称变换矩阵
*/
void Complex_Symmetric_Point()
{
	cout<<"请输入某端点值(x1,y1,z1)：";
	double x1,y1,z1;
	cin>>x1>>y1>>z1;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++){
			Tnew[i][j]=0;
		}
		Tnew[i][i]=1;
	}
	Tnew[3][0]=-x1,Tnew[3][1]=-y1,Tnew[3][2]=-z1;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[3][0]=0,Tnew[3][1]=0,Tnew[3][2]=0;

	Tnew[0][0]=-1,Tnew[1][1]=-1,Tnew[2][2]=-1;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[0][0]=1,Tnew[1][1]=1,Tnew[2][2]=1;

	Tnew[3][0]=x1,Tnew[3][1]=y1,Tnew[3][2]=z1;
	Matrix_mul(T,Tnew,Ttmp);
	T=Ttmp;
	Tnew[3][0]=0,Tnew[3][1]=0,Tnew[3][2]=0;
}
/*
复合变换-对称界面函数
功能：选择关于直线对称还是关于点对称
*/
void Complex_Symmetric()
{
	cout<<"*------------------------------------*"<<endl;
	cout<<"          1.关于某直线对称                      "<<endl;
	cout<<"          2.关于某点对称                   "<<endl;
	cout<<"请选择：";
	int k;
	cin>>k;
	switch (k)
	{
	case 1:
		Complex_Symmetric_Line();break;
	case 2:
		Complex_Symmetric_Point();break;
	default:
		break;
	}
}
/*
复合变换界面函数
功能：选择要进行的变换操作
*/
void ComplexTransformation()
{
	while(true)
	{
		cout<<"*--------复合变换--------------------*"<<endl;
		cout<<"          1.平移                      "<<endl;
		cout<<"          2.旋转                   "<<endl;
		cout<<"          3.缩放                   "<<endl;
		cout<<"          4.对称                   "<<endl;
		cout<<"          5.错切                   "<<endl;
		cout<<"          6.       完成                   "<<endl;
		cout<<"请选择：";
		int k;
		cin>>k;
		double x,y,z;
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++){
				Tnew[i][j]=0;
				Ttmp[i][j]=0;
			}
			Tnew[i][i]=1;
		}
		switch (k)
		{
		case 1:
			cout<<"x轴方向平移距离 y轴方向平移距离 z轴方向平移距离:" ;cin>>x>>y>>z;
			Tnew[3][0]=x;Tnew[3][1]=y;Tnew[3][2]=z;
			Matrix_mul(T,Tnew,Ttmp);
			T=Ttmp;
			break;
		case 2:

			Complex_Revolve();	break;
		case 3:
			cout<<"x轴方向缩放倍数 y轴方向缩放倍数 z轴方向缩放倍数:" ;cin>>x>>y>>z;
			Tnew[0][0]=x;Tnew[1][1]=y;Tnew[2][2]=z;
			Matrix_mul(T,Tnew,Ttmp);
			T=Ttmp;
			break;
		case 4:
			Complex_Symmetric();
			break;
		case 5:
			Shear();break;
		default:
			return ;
		}
		
	}
}
/*
初始化函数
功能：文件数据进行读入
*/
void Init()
{
	ifstream infile;
	infile.open("模型/2.off");
	string s;
	double t;
	infile >> s;
	
	for(int i=0;i<3;i++)
	{
			infile >> arr[i];
	}
	GraphPoint=vector<vector<double>>(arr[0],vector<double>(4,1));
	IndexPoint=vector<vector<int>>(arr[1]);
	int cnt = 0;
	for (int i=0;i<arr[0];i++)
	{
		for (int j=0;j<3;j++)
			infile >> GraphPoint[i][j];
	}
	int size,temp;
	for(int i=0;i<arr[1];i++)
	{
		infile>>size;
		for(int j=0;j<size;j++)
		{
			infile>>temp;
			IndexPoint[i].emplace_back(temp);
		}
	}
	infile.close();//读取完成之后关闭文件
}
/*
主函数
*/
int main(int argc, char* argv[])
{
	Init();
	for(int i=0;i<4;i++){
		T[i][i]=1;
	}
	cout<<"*------------------------------------*"<<endl;
	cout<<"          1.基本几何变换              "<<endl;
	cout<<"          2.  复合变换                "<<endl;
	cout<<"请选择：";
	int x;
	cin>>x;
	switch (x)
	{
	case 1:BasicTransformation();break;
	case 2:ComplexTransformation();break;
	default:
		return 0;
	}
	Draw(argc,argv);
}























/*
float rotates = 0;
int times = 0;
void renderScene() 
{
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity();
    glPushMatrix();

    //glTranslatef(-0.2, 0, 0); // 平移

    //glScalef(2, 1, 1);    // 缩放

    times++;
    if(times > 100)
    {
        times = 0;
    }

    if(times % 100 == 0)
    {
        rotates += 0.3;
    }
    
    glRotatef(rotates, 0, 1, 0);
    glRotatef(rotates, 1, 0, 0);

    

	Display1();

    glPopMatrix();
    glutSwapBuffers();
}
*/
