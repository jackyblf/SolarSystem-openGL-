#pragma comment(lib,"winmm.lib")
#include "base.h"
#include <string>
#include "gltexture.h"
#include "glDrawer.h"
#include "glcamera.h"
#include "planet.h"
#include "glSkybox.h"
#include "filesystem.h"
#include "glexplosion.h"
#include "trail.h"

//全局变量声明
bool lighting = true;
fileSystem filesys;//record/replay系统使用
int frontViewOnOff;//开关变量，前视图和正视图转换
glTexture *boxtex;//天空盒纹理贴图，理论上天空盒需要六张无缝拍摄的纹理，现在就用一张贴在天空盒六个面上，简化一下
glTexture *parttex;
glExplosion * glexp = new glExplosion(1, vec3(0, 0, 0), 1.0f, 0);
bool isexp = true;
int numexp;
float px, py, pz;
glTrail* trail;

//行星系统
planet *sun; //太阳
planet *sx;  //水星
planet *jx;  //金星
planet *hx;  //火星
planet *mx;  //木星
planet *tx;  //土星
planet *twx; //天王星
planet *hwx; //海王星

planet * earth; //地球
planet * moon;  //月亮

glCamera camera(vec3(0.0f, 0.0f, 10.0f)); //摄像机初始位置,w/s键控制摄像机前后移动，a/d控制摄像机左右移动，鼠标左键按下拖动控制摄像机的pitch/yaw旋转

int angle = 0; //核心变量，所有行星的移动和转动的速度是以angle为基础的啦，改变angle旋转速度会影响所有行星的运动速度，包括角速度和距离，所有行星都是按照angle相对比例进行运动

void SetLight(bool b)
{
	float amb[4] = { 1.0, 0.8, 0.8, 1 };
	float dif[4] = { 1.0, 1.0, 1.0, 1 };
	float pos[4] = { 0, 10, 0, 1 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, dif);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);

	if (b)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
	}
	else
	{
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}

	glShadeModel(GL_SMOOTH);
}

void init()
{
	boxtex = new glTexture("星空图.png", true);

	sun = new planet("太阳.png", 0.0f, 1.0f, 1.0f, vec3(0.0f, 0.0f, 0.0f));
	sx = new planet("水星.png", 0.5f, 0.5f, 0.2f, vec3(1.4f, 0.4f, 0.0f));
	jx = new planet("金星.png", 0.5f, 0.5f, 0.2f, vec3(3.0f, -0.4f, 0.0f));
	earth = new planet("地球.png", 1.0f, 2.0f, 0.5f, vec3(5.0f, 2.0f, 8.0f));
	moon = new planet("月亮.png", 0.5f, 0.5f, 0.2f, vec3(1.5f, 0.0f, 0.0f));
	hx = new planet("火星.png", 0.2f, 0.3f, 0.3f, vec3(7.0f, 0.0f, 0.0f));
	mx = new planet("木星.png", 0.4f, 1.0f, 0.5f, vec3(10.0f, 0.0f, 0.0f));
	tx = new planet("土星.png", -0.4f, 0.2f, 1.0f, vec3(15.0f, 1.0f, 0.0f));
	twx = new planet("天王星.png", 0.8f, 0.5f, 0.3f, vec3(17.0f, 0.0f, 0.0f));
	hwx = new planet("海王星.png", 0.6f, 0.5f, 0.4f, vec3(19.0f, 0.8f, 0.0f));

	parttex = new glTexture("particle.png");

	trail = new glTrail("spawnflash.png");
	trail->setPath(vec3(0, 0, 0));

	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	//初始化二次曲面对象
	drawInit();
}

void deinit()
{
	delete boxtex;
	delete sun;
	delete sx;
	delete jx;
	delete earth;
	delete moon;
	delete hx;
	delete mx;
	delete tx;
	delete twx;
	delete hwx;
	delete parttex;
	delete trail;
	delete glexp;
	drawDeInit();
}

void record()
{
	if (filesys.beginWrite("test.txt"))
	{
		filesys.writeInt(angle);
		filesys.closeFile();
	}
}

void rePlay()
{
	if (filesys.beginRead("test.txt"))
	{
		filesys.readInt(&angle);
		filesys.closeFile();
	}
}

static void DrawEarthAndMoon(planet *earth, planet *moon)
{
	glPushMatrix();//地球公转+自转（围绕太阳）
	earth->texture->MakeCurrent();

	glRotatef(angle*earth->aroundRotatedSpeed, 0.0f, 1.0f, 0.0f);
	glTranslatef(earth->pos.x, earth->pos.y, earth->pos.z);
	glRotatef(angle*earth->selfRotatedSpeed, 0.0f, 1.0f, 0.0f);
	drawSphere(earth->radius, 20, 20, true);

	glPushMatrix();//月球公转+自转（围绕地球）
	moon->texture->MakeCurrent();
	glRotatef(angle*moon->aroundRotatedSpeed, 0.0f, 1.0f, 0.0f);
	glTranslatef(moon->pos.x, moon->pos.y, moon->pos.z);
	glRotatef(angle*moon->selfRotatedSpeed, 0.0f, 1.0f, 0.0f);
	drawSphere(moon->radius, 20, 20, true);
	glPopMatrix();

	glPopMatrix();
}
static void DrawOtherPlanet(planet * p)
{
	glPushMatrix();
	p->texture->MakeCurrent();
	glRotatef(angle*p->aroundRotatedSpeed, 0.0f, 1.0f, 0.0f);
	glTranslatef(p->pos.x, p->pos.y, p->pos.z);
	glRotatef(angle*p->selfRotatedSpeed, 0.0f, 1.0f, 0.0f);
	drawSphere(p->radius, 20, 20, true);
	glPopMatrix();
}

static void DrawTrail(planet * p)
{
	glPushMatrix();
	glRotatef(angle*p->aroundRotatedSpeed*2.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(p->pos.x + 0.4f, p->pos.y + 0.3, p->pos.z);
	glPushMatrix();
	glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
	trail->draw(camera.pos);
	glPopMatrix();
	glRotatef(angle*p->selfRotatedSpeed, 0.0f, 1.0f, 0.0f);
	drawSphere(p->radius, 20, 20, true);
	glPopMatrix();
}

static void DrawOtherPlanet2(planet * p)
{
	glPushMatrix();
	p->texture->MakeCurrent();

	glRotatef(angle*p->aroundRotatedSpeed, 0.0f, 1.0f, 0.0f);

	glTranslatef(p->pos.x, p->pos.y, p->pos.z);


	glRotatef(angle*p->selfRotatedSpeed, 0.0f, 1.0f, 0.0f);

	drawSphere(p->radius, 20, 20, true);

	glPushMatrix();
	glRotatef(85, 1.0f, 0.0f, 0.0f);

	drawDisk(1.5, 3, 20);

	glPopMatrix();
	glPopMatrix();

}
void DrawJX()
{
	glPushMatrix();
	jx->texture->MakeCurrent();

	glRotatef(angle*jx->aroundRotatedSpeed * 2, 1.0, 1.0, 1.0);

	glTranslatef(5.0f, 0.0, 0.0f);
	glRotatef(angle*jx->aroundRotatedSpeed * 2, 0.0f, 0.0f, 1.0f);

	drawSphere(jx->radius, 20, 20, true);
	glPopMatrix();
}
void DrawHX()
{
	glPushMatrix();
	hx->texture->MakeCurrent();

	glRotatef(angle*hx->aroundRotatedSpeed * 2, -1.0, -1.0, 0.0);

	glTranslatef(5.0f, 0.0, 0.0f);

	glRotatef(angle*hx->aroundRotatedSpeed * 2, 0.0f, 0.0f, 1.0f);

	drawSphere(hx->radius, 20, 20, true);

	glPopMatrix();

}
void testdraw1(planet *p)
{
	glPushMatrix();
	p->texture->MakeCurrent();

	glRotatef(45, 1.0, 0.0, 0.0);
	glRotatef(angle*p->aroundRotatedSpeed * 2, 0.0f, 1.0f, 0.0f);

	glTranslatef(5.0f, 0, 0.0f);
	glRotatef(angle*p->aroundRotatedSpeed * 2, 0.0f, 0.0f, 1.0f);

	drawSphere(p->radius, 20, 20, true);
	glPopMatrix();
}

void DrawSolarSystem(planet *sun)
{
	sun->texture->MakeCurrent();

	SetLight(lighting);
	glTranslatef(sun->pos.x, sun->pos.y, sun->pos.z);
	glRotatef(angle*sun->selfRotatedSpeed, 0.0f, 1.0f, 0.0f);
	drawSphere(sun->radius, 50, 50, true);
	DrawEarthAndMoon(earth, moon);//地月绘制
	DrawOtherPlanet(sx);
	DrawOtherPlanet(jx);
	DrawOtherPlanet(twx);
	DrawOtherPlanet(hwx);
	//带光圈绘制
	DrawOtherPlanet2(mx);
	DrawOtherPlanet2(tx);
	DrawTrail(jx);
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:		//ESC
		exit(0);	//退出系统
		break;
	case 'w'://摄像机向前运动

		camera.pos -= camera.forward*0.5;
		glutPostRedisplay();
		break;
	case 's'://摄像机向后运动
		camera.pos += camera.forward*0.5;

		glutPostRedisplay();
		break;
	case 'a'://摄像机向左运动

		camera.pos -= camera.right *0.5;
		glutPostRedisplay();
		break;
	case 'd'://摄像机向右运动

		camera.pos += camera.right*0.5;
		glutPostRedisplay();
		break;
	case 'f'://正视图和顶视图切换

		frontViewOnOff ^= 1;
		glutPostRedisplay();
		break;
	case 'r'://记录
		record();
		glutPostRedisplay();
		break;
	case 'p'://回放
		rePlay();
		glutPostRedisplay();
		break;
	case 'l'://灯源开关
		lighting = !lighting;
		glutPostRedisplay();
		break;
	}

}


void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 0.1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
}

void creatExplosion(float x, float y, float z, int num, float spread)
{
	if (glexp != NULL) { delete glexp; glexp = NULL; }
	glexp = new glExplosion(num, vec3(0, 0, 0), spread, parttex->m_tex);
	px = x, pz = z, py = y;
	isexp = false;
	numexp = 0;
}

void  drawExplosion()
{
	glPushMatrix();
	glTranslatef(px, py, pz);
	if (isexp == false)
	{
		glexp->Render();
		isexp = true;
	}
	if (isexp)
	{
		glexp->Update(0.03f);
		isexp = false;
	}
	glPopMatrix();
}

void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.update();

	//如果顶视图的话，沿着x轴旋转90度
	glRotatef(frontViewOnOff*90.0f, 1.0f, 0.0f, 0.0f);

	//绘制天空盒
	glDrawSkyBox(boxtex, 0.0f, 0.0f, 0.0f, 1000.0f, 1000.0f, 1000.0f);

	DrawJX();
	DrawHX();
	DrawSolarSystem(sun);

	if (numexp > 100)
	{
		creatExplosion(1.0f, 1.0f, 1.0f, 100, 1.5f);
	}

	drawExplosion();

	angle += 2;
	numexp++;

	glutSwapBuffers();
}

void myTimerFunc(int val)
{
	myDisplay();
	glutTimerFunc(25, myTimerFunc, 0);
}

int  main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);
	glutCreateWindow("太阳系 w/s/a/d控制摄像机运动 r/录制 p/回放 f/切换正顶视图 l/光源开关");
	init();
	glutDisplayFunc(&myDisplay);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&myKeyboardFunc);
	glutTimerFunc(25, myTimerFunc, 0);
	glutMainLoop();
	deinit();
	return 0;
}