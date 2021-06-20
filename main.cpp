#include<GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <mmsystem.h>
#include<math.h>
#include<time.h>
#include "BmpLoader.h"
#include "curveObject.h"

const int width = 800;
const int height = 600;

const double pi = 3.14159265389;

float road_width = 15;
float footpath_width = 2.5;
float houseLength = 4;
float houseWidth = 4;
float houseHeight = 3;
float windowLength = 1.5;
float windowWidth = 0.01;
float windowHeight = 1.75;

float rltvX=0,rltvY=0,rltvZ=-7.5;
float rltvZ1=7.5,rltvZ2=22.5,rltvZ3=37.5,rltvZ4=52.5,rltvZ5=67.5,rltvZ6=82.5;
float zoom=4;
float rotX=0,rotY=0,rotZ=0;
float tyre_rotate_x = 0;
float speed = 0.6;
float angleBackFrac = 0.25;
int LIFE = 5;
int SCORE =0;
int safeMood = 15;
int TIME=0;
bool START = false;
float theta = 0.0;

float objPosX[] = {1,-2,4,-5,-2,0,2};
float objPosZ[] = {2,3,4.5,-2,3,4,1};
float carPosX[] = {3,-5.5,1};
float carPosZ[] = {2,9,3};

float brightness = 0.75;
float amb_perc = 0.3;
int tola[5000][5000];
unsigned int ID[20];

bool flagScale=true;
bool light1 = true;
bool light2 = true;
bool light3 = false;
bool ambient = true;
bool diffuse = true;
bool specular = true;

GLfloat eyeX = 0;
GLfloat eyeY = 10;
GLfloat eyeZ = -20;

GLfloat lookX = 0;
GLfloat lookY = 8;
GLfloat lookZ = 0;

enum Textures
{
    textureBc1,
    textureBc2,
    textureBc3,
    textureBc4,
    textureBc5,
    textureRoad,
    textureFoothpath,
    textureWindow,
    textureRock,
    textureFrontEngine,
    textureMorningSky
};

Textures bc_arra[] = {textureBc1,textureBc2,textureBc3,textureBc4,textureBc5};

float rot = 0;
const float ar = 1.0 * width / height;

static void resize(int width, int height)
{
    glViewport(0, 0, width, width/ar);
}

static void getNormal3p(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
    GLfloat Ux, Uy, Uz, Vx, Vy, Vz, Nx, Ny, Nz;

    Ux = x2-x1;
    Uy = y2-y1;
    Uz = z2-z1;

    Vx = x3-x1;
    Vy = y3-y1;
    Vz = z3-z1;

    Nx = Uy*Vz - Uz*Vy;
    Ny = Uz*Vx - Ux*Vz;
    Nz = Ux*Vy - Uy*Vx;

    glNormal3f(Nx,Ny,Nz);
}

static GLfloat cube_points[8][3] =
{
    {0,0,0},
    {0,0,1},
    {0,1,0},
    {0,1,1},

    {1,0,0},
    {1,0,1},
    {1,1,0},
    {1,1,1}
};

static GLubyte surface[6][4] =
{
    {0,2,6,4},
    {1,5,7,3},
    {0,4,5,1},
    {2,3,7,6},
    {0,1,3,2},
    {4,6,7,5}
};

void cube(float r=0.5, float g=0.5, float b=0.5, int emit = 0, float trX = 1, float trZ = 0, float alpha=1)
{
    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };

    GLfloat material_ambient[] = {r*amb_perc*brightness,g*amb_perc*brightness,b*amb_perc*brightness,1.0};
    GLfloat material_diffuse[] = {r*brightness,g*brightness,b*brightness,1.0};
    GLfloat material_specular[] = {1,1,1,1.0};
    GLfloat material_shininess[] = {90};
    GLfloat mat_emission[] = {0.545, 0,0, 0.0};


    //GLfloat m_emission[] = {1.0,1.0,1.0,1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    if(emit==1)
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);

    else
        glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);


    /*if(light == 1){
        if(emission && light1) glMaterialfv(GL_FRONT, GL_EMISSION, m_emission);
        else glMaterialfv(GL_FRONT, GL_EMISSION, m_no);
    }
    else if(light == 2){
        if(emission && light2) glMaterialfv(GL_FRONT, GL_EMISSION, m_emission);
        else glMaterialfv(GL_FRONT, GL_EMISSION, m_no);
    }
    else if(light == 3){
        if(emission && light3) glMaterialfv(GL_FRONT, GL_EMISSION, m_emission);
        else glMaterialfv(GL_FRONT, GL_EMISSION, m_no);
    }*/

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        //glColor4f(colR,colG,colB,alpha);

        //colR += 0.03;
        //colG += 0.03;
        //olB += 0.03;
        getNormal3p(cube_points[surface[i][0]][0], cube_points[surface[i][0]][1], cube_points[surface[i][0]][2],
                    cube_points[surface[i][1]][0], cube_points[surface[i][1]][1], cube_points[surface[i][1]][2],
                    cube_points[surface[i][2]][0], cube_points[surface[i][2]][1], cube_points[surface[i][2]][2]);

        glVertex3fv(&cube_points[surface[i][0]][0]);
        glTexCoord2f(0,trZ);
        glVertex3fv(&cube_points[surface[i][1]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&cube_points[surface[i][2]][0]);
        glTexCoord2f(trX,0);
        glVertex3fv(&cube_points[surface[i][3]][0]);
        glTexCoord2f(trX,trZ);
    }
    glEnd();
}

void Light1(float xPos, float yPos, float zPos)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { xPos, yPos, zPos, 1.0 };

    glEnable( GL_LIGHT0);
    if(light1)
    {
        if(ambient)
        {
            glLightfv( GL_LIGHT0, GL_AMBIENT, light_ambient);
        }
        else
        {
            glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
        }

        if(diffuse)
        {
            glLightfv( GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        }
        else
        {
            glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
        }

        if(specular)
        {
            glLightfv( GL_LIGHT0, GL_SPECULAR, light_specular);
        }
        else
        {
            glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
        }

    }
    else
    {
        glLightfv( GL_LIGHT0, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT0, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT0, GL_SPECULAR, no_light);
    }
    glLightfv( GL_LIGHT0, GL_POSITION, light_position);
}

void Light2(float xPos, float yPos, float zPos)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.3, 0.3, 0.3, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light_position[] = { xPos, yPos, zPos, 1.0 };

    glEnable( GL_LIGHT1);
    if(light2)
    {
        if(ambient)
        {
            glLightfv( GL_LIGHT1, GL_AMBIENT, light_ambient);
        }
        else
        {
            glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);
        }

        if(diffuse)
        {
            glLightfv( GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        }
        else
        {
            glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);
        }

        if(specular)
        {
            glLightfv( GL_LIGHT1, GL_SPECULAR, light_specular);
        }
        else
        {
            glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);
        }

    }
    else
    {
        glLightfv( GL_LIGHT1, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT1, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT1, GL_SPECULAR, no_light);
    }

    glLightfv( GL_LIGHT1, GL_POSITION, light_position);
}

void spotLight(float xPos, float yPos, float zPos)
{
    GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_ambient[]  = {0.3, 0.3, 0.0, 1.0};
    GLfloat light_diffuse[]  = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat light_specular[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat light_position[] = { xPos, yPos, zPos, 1.0 };
    GLfloat spot_direction[] = { 0.0, 0.0, 1.0 };
    GLfloat spot_cutOff[] = {60.0};

    glEnable( GL_LIGHT2);
    if(light3)
    {
        if(ambient)
        {
            glLightfv( GL_LIGHT2, GL_AMBIENT, light_ambient);
        }
        else
        {
            glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);
        }

        if(diffuse)
        {
            glLightfv( GL_LIGHT2, GL_DIFFUSE, light_diffuse);
        }
        else
        {
            glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);
        }

        if(specular)
        {
            glLightfv( GL_LIGHT2, GL_SPECULAR, light_specular);
        }
        else
        {
            glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);
        }

    }
    else
    {
        glLightfv( GL_LIGHT2, GL_AMBIENT, no_light);
        glLightfv( GL_LIGHT2, GL_DIFFUSE, no_light);
        glLightfv( GL_LIGHT2, GL_SPECULAR, no_light);
    }

    glLightfv( GL_LIGHT2, GL_POSITION, light_position);
    glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
    glLightfv( GL_LIGHT2, GL_SPOT_CUTOFF, spot_cutOff);
}

void LoadTexture(const char*filename, Textures tex)
{
    glGenTextures(1, &ID[tex]);
    glBindTexture(GL_TEXTURE_2D, ID[tex]);
    glPixelStorei(GL_UNPACK_ALIGNMENT, ID[tex]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    BmpLoader bl(filename);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, bl.iWidth, bl.iHeight, GL_RGB, GL_UNSIGNED_BYTE, bl.textureData );
}

void axes()
{
    float length = 10;
    float width = 0.3;

    // X-axis
    glPushMatrix();
    glTranslatef(length/2,0,0);
    glScalef(length,width,width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.8,0.1,0.1);
    glPopMatrix();

    // Y-axis
    glPushMatrix();
    glTranslatef(0,length/2,0);
    glScalef(width,length,width);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.1,0.8,0.1);
    glPopMatrix();

    // Z-axis
    glPushMatrix();
    glTranslatef(0,0,length/2);
    glScalef(width,width,length);
    glTranslatef(-0.5,-0.5,-0.5);
    cube(0.1,0.1,0.8);
    glPopMatrix();
}

void singleTolaHouse(Textures houseTex)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ID[houseTex]);
    glPushMatrix();
    glTranslatef(0,0,0);
    glScalef(houseWidth,houseHeight,houseLength);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ID[textureWindow]);
    glPushMatrix();
    glTranslatef(-0.02,0.85,0.33);
    glScalef(windowWidth,windowHeight,windowLength);
    cube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-0.02,0.85,0.66+windowLength);
    glScalef(windowWidth,windowHeight,windowLength);
    cube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.8,0.85,-0.02);
    glRotatef(90,0,0,1);
    glRotatef(90,0,1,0);
    glScalef(windowWidth,windowLength,windowHeight);
    cube(1,1,1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(1.98+windowHeight,0.85,-0.02);
    glRotatef(90,0,0,1);
    glRotatef(90,0,1,0);
    glScalef(windowWidth,windowLength,windowHeight);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

}

void house(int n, Textures houseTex)
{
    for(int i=0; i<n; i++)
    {
        glPushMatrix();
        glTranslated(0,i*3,0);
        singleTolaHouse(houseTex);
        glPopMatrix();
    }
}

void rock(float r = 1.0, float g = 1.0, float b = 1.0)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID[textureRock]);

    glPushMatrix();

    GLfloat no_mat[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5*r, 0.5*g, 0.5*b, 1.0 };
    GLfloat mat_diffuse[] = { r, g, b, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = {60};
    GLfloat mat_emission[] = {0.3*r, 0.2*g, 0.2*b, 1.0};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    GLUquadric *qobj = gluNewQuadric();
    gluQuadricTexture(qobj, GL_TRUE);
    glRotatef(90.0f, 1.0f, 1.0f, 0.0f);
    gluSphere(qobj, 1, 20, 16);
    gluDeleteQuadric(qobj);

    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

void lampPost()
{
    glPushMatrix();
    glRotatef(90,0,1,0);
    glRotatef(45,0,0,1);
    pipeBezier(0.35,0.35,0.35,0.2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.3,9,-0.15);
    glRotatef(-2,0,0,1);
    glScalef(1.5,0.3,0.3);
    cube(0.85,0.85,0.85);
    glPopMatrix();
}

bool collisionCheck(float x, float z,float scaledX, float scaledZ, float scaledObsX, float scaledObsZ)
{
    bool isCollision = false;
    int indexOfSceneObj[] = {2,1,3,1,5,4,2};
    float objLocalX;
    float objLocalZ;
    float objGlobalX;
    float objGlobalZ;
    for(int i=0; i<7; i++)
    {
        objLocalX = objPosX[indexOfSceneObj[i]];
        objLocalZ = objPosZ[indexOfSceneObj[i]];

        objGlobalX = objLocalX + rltvX;
        if(i==0) objGlobalZ = objLocalZ + rltvZ;
        if(i==1) objGlobalZ = objLocalZ + rltvZ1;
        if(i==2) objGlobalZ = objLocalZ + rltvZ2;
        if(i==3) objGlobalZ = objLocalZ + rltvZ3;
        if(i==4) objGlobalZ = objLocalZ + rltvZ4;
        if(i==5) objGlobalZ = objLocalZ + rltvZ5;
        if(i==6) objGlobalZ = objLocalZ + rltvZ6;
        if(((objGlobalX-scaledObsX/2>=x && objGlobalX-scaledObsX/2<=x+scaledX)||(objGlobalX+scaledObsX/2>=x && objGlobalX+scaledObsX/2<=x+scaledX))
                && (objGlobalZ>=z && objGlobalZ<=z+scaledZ))
        {
            isCollision = true;
            break;
        }
    }
    return isCollision;
}

bool collisionCheckCar(float x, float z,float scaledX, float scaledZ, float scaledObsX, float scaledObsZ)
{
    bool isCollision = false;
    float carLocalX;
    float carLocalZ;
    float carGlobalX;
    float carGlobalZ;
    for(int i=0; i<3; i++)
    {
        carLocalX = carPosX[i];
        carLocalZ = carPosZ[i];

        carGlobalX = carLocalX + rltvX;

        if(i==0) carGlobalZ = carLocalZ + rltvZ2;
        if(i==1) carGlobalZ = carLocalZ + rltvZ3;
        if(i==2) carGlobalZ = carLocalZ + rltvZ5;

        if(((carGlobalX>=x && carGlobalX<=x+scaledX)||(carGlobalX+scaledObsX>=x && carGlobalX+scaledObsX<=x+scaledX))
                && (carGlobalZ>=z && carGlobalZ<=z+scaledZ))
        {
            isCollision = true;
            break;
        }
    }
    return isCollision;
}

static GLfloat carLid_points[8][3] =
{
    {0,0,0},
    {0,0,1},
    //{0.2,0.5,0.2},
    {0,0.5,0.2},
    //{0.2,0.5,0.8},
    {0,0.5,0.8},

    {1,0,0},
    {1,0,1},
    //{0.8,0.5,0.2},
    {1,0.5,0.2},
    //{0.8,0.5,0.8}
    {1,0.5,0.8}
};

static GLubyte carLid_surface[6][4] =
{
    {0,2,6,4},
    {1,5,7,3},
    {0,4,5,1},
    {2,3,7,6},
    {0,1,3,2},
    {4,6,7,5}
};

void carLid(float r=0, float g=0, float b=0, float trX = 1, float trZ = 0, float alpha=1)
{
    //GLfloat m_no[] = {0.0, 0.0, 0.0, 1.0};
    GLfloat material_ambient[] = {r*amb_perc*brightness,g*amb_perc*brightness,b*amb_perc*brightness,1.0};
    GLfloat material_diffuse[] = {r*brightness,g*brightness,b*brightness,1.0};
    GLfloat material_specular[] = {1,1,1,1.0};
    GLfloat material_shininess[] = {90};

    //GLfloat m_emission[] = {1.0,1.0,1.0,1.0};

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    /*if(light == 1){
        if(emission && light1) glMaterialfv(GL_FRONT, GL_EMISSION, m_emission);
        else glMaterialfv(GL_FRONT, GL_EMISSION, m_no);
    }
    else if(light == 2){
        if(emission && light2) glMaterialfv(GL_FRONT, GL_EMISSION, m_emission);
        else glMaterialfv(GL_FRONT, GL_EMISSION, m_no);
    }
    else if(light == 3){
        if(emission && light3) glMaterialfv(GL_FRONT, GL_EMISSION, m_emission);
        else glMaterialfv(GL_FRONT, GL_EMISSION, m_no);
    }*/

    glBegin(GL_QUADS);
    for (GLint i = 0; i <6; i++)
    {
        //glColor4f(colR,colG,colB,alpha);

        //colR += 0.03;
        //colG += 0.03;
        //olB += 0.03;
        getNormal3p(carLid_points[carLid_surface[i][0]][0], carLid_points[carLid_surface[i][0]][1], carLid_points[carLid_surface[i][0]][2],
                    carLid_points[carLid_surface[i][1]][0], carLid_points[carLid_surface[i][1]][1], carLid_points[carLid_surface[i][1]][2],
                    carLid_points[carLid_surface[i][2]][0], carLid_points[carLid_surface[i][2]][1], carLid_points[carLid_surface[i][2]][2]);

        glVertex3fv(&carLid_points[carLid_surface[i][0]][0]);
        glTexCoord2f(0,trZ);
        glVertex3fv(&carLid_points[carLid_surface[i][1]][0]);
        glTexCoord2f(0,0);
        glVertex3fv(&carLid_points[carLid_surface[i][2]][0]);
        glTexCoord2f(trX,0);
        glVertex3fv(&carLid_points[carLid_surface[i][3]][0]);
        glTexCoord2f(trX,trZ);
    }
    glEnd();
}

void car_left_right_window1()
{
    glBegin(GL_QUADS);
    glColor3f(0.35,0.35,0.35);
    glVertex3f(0,0,0);
    glVertex3f(0,1,0);
    glVertex3f(0.7,1,0);
    glVertex3f(1,0,0);
    getNormal3p(0,0,0,0,1,0,0.7,1,0);
    glEnd();
}

void car_left_right_window2()
{
    glBegin(GL_QUADS);
    glColor3f(0.35,0.35,0.35);
    glVertex3f(0,0,0);
    glVertex3f(0.3,1,0);
    glVertex3f(1,1,0);
    glVertex3f(1,0,0);
    getNormal3p(0,0,0,0.3,1,0,1,1,0);
    glEnd();
}

void car(float r = 1, float g = 0, float b = 0, float x = 2.5, float y = 1, float z = 4)
{
    if(tyre_rotate_x >360) tyre_rotate_x = 0;

    ///Main Body
    glPushMatrix();
    glScalef(x,y,z);
    cube(r,g,b);
    glPopMatrix();

    /// Car Lid
    glPushMatrix();
    glTranslatef(0,y,0.12*z);
    glScalef(x,2*y,0.7*z);
    carLid(r,g,b);
    glPopMatrix();

    /// back window
    glPushMatrix();
    glTranslatef(.05*x,0.05*y+y,0.12*z+0.05);
    glRotatef(21.7,1,0,0);
    glScalef(0.9*x,2*y*0.5,0.1);
    cube(0.35,0.35,0.35);
    glPopMatrix();

    /// front window
    glPushMatrix();
    glTranslatef(.05*x,0.05*y+y,0.12*z-0.05+0.7*z);
    glRotatef(-25.0,1,0,0);
    glScalef(0.9*x,2*y*0.5,0.1);
    cube(0.35,0.35,0.35);
    glPopMatrix();

    /// left Door window 1
    glPushMatrix();
    glTranslatef(x+0.05,y+2*y*0.5*0.1,0.12*z+0.7*z/2+0.1*0.7*z/2);
    glRotatef(-90,0,1,0);
    glScalef(0.8*0.7*z/2,2*y*0.5*0.8,0.1);
    car_left_right_window1();
    glPopMatrix();

    /// Right Door window 2
    glPushMatrix();
    glTranslatef(-0.05,y+2*y*0.5*0.1,0.12*z+0.7*z/2);
    glRotatef(90,0,1,0);
    glScalef(0.8*0.7*z/2,2*y*0.5*0.8,0.1);
    car_left_right_window1();
    glPopMatrix();

    /// Right Door window 1
    glPushMatrix();
    glTranslatef(-0.05,y+2*y*0.5*0.1,0.12*z+0.7*z-0.1*0.7*z/2);
    glRotatef(90,0,1,0);
    glScalef(0.8*0.7*z/2,2*y*0.5*0.8,0.1);
    car_left_right_window2();
    glPopMatrix();

    /// Left Door Window 2
    glPushMatrix();
    glTranslatef(x+0.05,y+2*y*0.5*0.1,0.12*z+0.2*0.7*z/2);
    glRotatef(-90,0,1,0);
    glScalef(0.8*0.7*z/2,2*y*0.5*0.8,0.1);
    car_left_right_window2();
    glPopMatrix();

    ///Left Tyre 1
    glPushMatrix();
    glTranslatef(x/2-0.74,-0.225,3);
    glRotatef(tyre_rotate_x,1,0,0);
    glScalef(0.3,0.17,0.17);
    wheelBezier(0.25,0.25,0.25);
    glPopMatrix();

    ///Left Tyre 2
    glPushMatrix();
    glTranslatef(x/2-0.74,-0.225,1);
    glRotatef(tyre_rotate_x,1,0,0);
    glScalef(0.3,0.17,0.17);
    wheelBezier(0.25,0.25,0.25);
    glPopMatrix();

    ///Right Tyre 1
    glPushMatrix();
    glTranslatef(-x/2-0.35,-0.225,3);
    glRotatef(tyre_rotate_x,1,0,0);
    glScalef(0.3,0.17,0.17);
    wheelBezier(0.25,0.25,0.25);
    glPopMatrix();

    ///Right Tyre 2
    glPushMatrix();
    glTranslatef(-x/2-0.35,-0.225,1);
    glRotatef(tyre_rotate_x,1,0,0);
    glScalef(0.3,0.17,0.17);
    wheelBezier(0.25,0.25,0.25);
    glPopMatrix();

    /// Name Plate
    glPushMatrix();
    glTranslatef(x*0.25,y*0.1,-0.05);
    glScalef(x*0.5,y*0.8,0.1);
    cube(0.75,0.75,0.75);
    glPopMatrix();

    /// Front engine
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID[textureFrontEngine]);
    glPushMatrix();
    glTranslatef(x*0.25,y*0.1,z-0.05);
    glScalef(x*0.5,y*0.8,0.1);
    cube(0.75,0.75,0.75);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    /// Back Light 1
    glPushMatrix();
    glTranslatef(x*0.75+0.1,y*0.25,-0.05);
    glScalef(x*0.15,y*0.5,0.1);
    cube(1,0,0,1); // emission property red light
    glPopMatrix();

    /// Back Light 2
    glPushMatrix();
    glTranslatef(0.1,y*0.25,-0.05);
    glScalef(x*0.15,y*0.5,0.1);
    cube(1,0,0,1); // emission property red light
    glPopMatrix();

    /// Head light 1
    glPushMatrix();

    GLfloat material_ambient[] = {1*amb_perc*brightness,1*amb_perc*brightness,0*amb_perc*brightness,1.0};
    GLfloat material_diffuse[] = {1*brightness,1*brightness,0*brightness,1.0};
    GLfloat material_specular[] = {1,1,1,1.0};
    GLfloat material_shininess[] = {90};

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    glTranslatef(x-0.3-0.05,0.3+0.05,z-0.2);
    glutSolidSphere(0.3,16,24);
    glPopMatrix();

    /// Head light 2
    glPushMatrix();
    glTranslatef(0.3+0.05,0.3+0.05,z-0.2);
    glutSolidSphere(0.3,16,24);
    glPopMatrix();

    tyre_rotate_x += 5;
}

void sky()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID[textureMorningSky]);
    glPushMatrix();
    glTranslatef(-250,0,0);
    glScalef(500,500,0.1);
    cube(1,1,1,0,2.5,5);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void scene(int objIndex, int lampPostVisibility = 0, int carIndex = -1, float cR = 1, float cG =0, float cB = 0)
{
    /// Road
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,ID[textureRoad]);
    glPushMatrix();
    glRotatef(90,0,1,0);
    glTranslatef(-road_width/2,-0.5,-road_width/2);
    glScalef(road_width,0.5,road_width);
    cube(1,1,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    /// Obstacle
    glPushMatrix();
    glTranslatef(objPosX[objIndex],1,objPosZ[objIndex]);
    rock();
    glPopMatrix();

    /// Lamp Post
    if(lampPostVisibility)
    {
        glPushMatrix();
        glTranslatef(1.5,1.5,0);
        lampPost();
        glPopMatrix();
    }

    /// Car
    if(carIndex != -1)
    {
        glPushMatrix();
        glTranslatef(carPosX[carIndex],0.7,carPosZ[carIndex]);
        car(cR,cG,cB);
        glPopMatrix();
    }

    /// footpath
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ID[textureFoothpath]);
    glPushMatrix();
    glTranslatef(road_width/2,-0.5,-road_width/2);
    glScalef(footpath_width,2,road_width);  ///This chunk of road is assumed to be a square. So length = width
    cube(1,1,1,0,2,1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-road_width/2-footpath_width,-0.5,-road_width/2);
    glScalef(footpath_width,2,road_width);  ///This chunk of road is assumed to be a square. So length = width
    cube(1,1,1,0,2,1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    /// Buildings
    int building_col = 0;
    for(int i = (road_width/2+footpath_width+houseWidth+2); i>=(road_width/2+footpath_width+1); i-=(1+houseWidth))
    {
        for(int j = -(road_width/2); j<=(road_width/2-houseLength-1)+1; j+=(1+houseLength))
        {
            if(tola[int(abs(i))][int(abs(j))]!=0)
            {
                glPushMatrix();
                glTranslated(i,0,j);
                if(building_col>5) building_col = 0;
                house(tola[int(abs(i))][int(abs(j))],bc_arra[building_col++]);
                glPopMatrix();
            }
            else
            {
                tola[int(abs(i))][int(abs(j))] = (rand()%5)+1;
                glPushMatrix();
                glTranslated(i,0,j);
                if(building_col>5) building_col = 0;
                house(tola[int(abs(i))][int(abs(j))],bc_arra[building_col++]);
                glPopMatrix();
            }
        }
    }

    building_col = 0;
    for(int i = -(road_width/2+footpath_width+houseWidth+2); i<=-(road_width/2+footpath_width+1); i+=(1+houseWidth))
    {
        for(int j = -(road_width/2); j<=(road_width/2-houseLength-1)+1; j+=(1+houseLength))
        {
            if(tola[int(abs(i))][int(abs(j))]!=0)
            {
                glPushMatrix();
                glTranslated(i,0,j);
                if(building_col>5) building_col = 0;
                glRotatef(270,0,1,0);
                house(tola[int(abs(i))][int(abs(j))],bc_arra[building_col++]);
                glPopMatrix();
            }
            else
            {
                tola[int(abs(i))][int(abs(j))] = (rand()%5)+1;
                glPushMatrix();
                glTranslated(i,0,j);
                if(building_col>5) building_col = 0;
                glRotatef(270,0,1,0);
                house(tola[int(abs(i))][int(abs(j))],bc_arra[building_col++]);
                glPopMatrix();
            }
        }
    }

}

void game()
{
    safeMood -=1;
    if(safeMood<=0) safeMood = 0;

    ///Car
    if(rotY>9)rotY=9;
    if(rotY<-9)rotY=-9;

    glPushMatrix();
    glTranslated(0,0.7,-10);
    glRotated(rotY,0,1,0);
    /*glScaled(2,1.5,3.5);
    cube(1,1,1);*/
    car(0,1,1);
    glPopMatrix();

    ///Environment
    if(rltvX>=6.5)rltvX=6.5;
    if(rltvX<=-4.5)rltvX=-4.5;

    glPushMatrix();
    glTranslated(rltvX,rltvY,rltvZ);
    scene(2);
    glPopMatrix();

    glPushMatrix();
    glTranslated(rltvX,rltvY,rltvZ1);
    scene(1);
    glPopMatrix();

    glPushMatrix();
    glTranslated(rltvX,rltvY,rltvZ2);
    scene(3,1,0);
    glPopMatrix();

    glPushMatrix();
    glTranslated(rltvX,rltvY,rltvZ3);
    scene(1,0,1,0.4,0.8,0.7);
    glPopMatrix();

    glPushMatrix();
    glTranslated(rltvX,rltvY,rltvZ4);
    scene(5);
    glPopMatrix();

    glPushMatrix();
    glTranslated(rltvX,rltvY,rltvZ5);
    scene(4,0,2,0.9,0.2,0.6);
    glPopMatrix();

    glPushMatrix();
    glTranslated(rltvX,rltvY,rltvZ6);
    scene(2);
    glPopMatrix();

    /// Sky
    glPushMatrix();
    glTranslatef(0,0,75);
    sky();
    glPopMatrix();

    /// Collision Check
    if(safeMood == 0)
    {
        if(collisionCheck(0,-10,2.5,4,2,2) || collisionCheckCar(0,-10,2.5,4,2.5,4))
        {
            PlaySound("C://Users//nhsha//OneDrive//Desktop//1607037 Car Game//CollisionSound.wav", NULL, SND_ASYNC|SND_FILENAME);
            LIFE-=1;
            safeMood = 15;
            printf("X: %f, Z: %f, z1: %f,z2: %f,z3: %f,z4: %f,z5: %f,z6: %f\n",rltvX,rltvZ,rltvZ1,
                   rltvZ2,rltvZ3,rltvZ4,rltvZ5,rltvZ6);
        }
    }
    printf("Life: %d\n",LIFE);

    if(LIFE==0)
    {
        printf("\n\nYour score: %d\n\n",SCORE);
        exit(0);
    }

    rltvZ-=speed;
    rltvZ1-=speed;
    rltvZ2-=speed;
    rltvZ3-=speed;
    rltvZ4-=speed;
    rltvZ5-=speed;
    rltvZ6-=speed;

    if(rltvZ<=-22.5)rltvZ=rltvZ6+road_width;
    if(rltvZ1<=-22.5)rltvZ1=rltvZ+road_width;
    if(rltvZ2<=-22.5)rltvZ2=rltvZ1+road_width;
    if(rltvZ3<=-22.5)rltvZ3=rltvZ2+road_width;
    if(rltvZ4<=-22.5)rltvZ4=rltvZ3+road_width;
    if(rltvZ5<=-22.5)rltvZ5=rltvZ4+road_width;
    if(rltvZ6<=-22.5)rltvZ6=rltvZ5+road_width;

    if(rotX>0)rotX-=angleBackFrac;
    if(rotX<0)rotX+=angleBackFrac;
    if(rotY>0)rotY-=angleBackFrac;
    if(rotY<0)rotY+=angleBackFrac;
    if(rotZ>0)rotZ-=angleBackFrac;
    if(rotZ<0)rotZ+=angleBackFrac;

    speed += 0.0002;
    if(speed>=0.8)speed=0.8;
    SCORE+= int(ceil(speed*0.1));
    printf("Score: %d\n",SCORE);
}

void delay(float number_of_seconds)
{
    // Converting time into milli_seconds
    int milli_seconds = 1000 * number_of_seconds;

    // Storing start time
    clock_t start_time = clock();

    // looping till required time is not achieved
    while (clock() < start_time + milli_seconds)
        ;
}

void drawStrokeText(char* str,int x,int y,int z,float r = 0,float g = 0, float b = 0)
{
    char *c;
    glPushMatrix();

    GLfloat material_ambient[] = {r*amb_perc*brightness,g*amb_perc*brightness,b*amb_perc*brightness,1.0};
    GLfloat material_diffuse[] = {r*brightness,g*brightness,b*brightness,1.0};
    GLfloat material_specular[] = {1,1,1,1.0};
    GLfloat material_shininess[] = {90};

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    glTranslatef(x, y+27,z);
    glRotatef(180,0,1,0);
    glScalef(0.015f,0.02f,z);

    for (c=str; *c != '\0'; c++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
    }
    glPopMatrix();
}

void drawStrokeChar(char c,float x,float y,float z,float r = 0,float g = 0, float b = 0)
{
    glPushMatrix();

    GLfloat material_ambient[] = {r*amb_perc*brightness,g*amb_perc*brightness,b*amb_perc*brightness,1.0};
    GLfloat material_diffuse[] = {r*brightness,g*brightness,b*brightness,1.0};
    GLfloat material_specular[] = {1,1,1,1.0};
    GLfloat material_shininess[] = {90};

    glMaterialfv(GL_FRONT, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, material_shininess);

    glTranslatef(x, y+27,z);
    glRotatef(180,0,1,0);
    glScalef(0.015f,0.02f,z);
    glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
    glPopMatrix();
}

float cartToPolarTheta(float x, float z)
{
    float theta = 0.0;
    if(x==0)theta=90*z/abs(z);
    else theta = atan(z/x)*180/pi;

    if(x<0 && z<=0) theta-=180;
    else if(x<0 && z>0) theta+=180;
    return theta;
}

static void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-2, 2, -2, 2, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX,eyeY,eyeZ, lookX,lookY,lookZ, 0,1,0);
    glViewport(0, 0, width, height);
    //axes();

    Light1(-15+1,12,-7);
    Light2(15-1,12,-7);
    spotLight(1.25,0.3,-10+4);

    glPushMatrix();
    glRotatef(rot, 0,1,0);
    game();
    glPopMatrix();

    /// SCORE and LIFE
    drawStrokeText("SCORE: ",-5,0,5);

    int num,tempScore,factor=1,digit;
    tempScore = SCORE;
    num = SCORE;

    while(tempScore)
    {
        tempScore=tempScore/10;
        factor = factor*10;
    }
    float tmp=0;
    while(factor>1)
    {
        factor = factor/10;
        digit=num/factor;
        drawStrokeChar(digit+48,-12-tmp,0,5);
        num=num%factor;
        tmp+=1.2;
    }

    drawStrokeText("LIFE : ",-5,-3,5);
    int mod = 0;
    int number = 0;
    int tempLife =LIFE;
    while(tempLife)
    {
        mod=tempLife%10;
        number=number*10+mod;
        tempLife/=10;
    }
    tmp = 0;
    while(number)
    {
        mod=number%10;
        drawStrokeChar(mod+48,-12-tmp,-3,5);
        number/=10;
        tmp+=1.2;
    }

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    float movement = 0.4;
    float rotAngle = 1;

    switch (key)
    {
    //case 27 :

    case 't':
        lookX -= eyeX;
        lookZ-=eyeZ;
        theta = cartToPolarTheta(lookX,lookZ);
        theta+=4;
        eyeX = sqrt(lookX*lookX+lookZ*lookZ) * cos(theta*pi/180) + eyeX;
        eyeZ = sqrt(lookX*lookX+lookZ*lookZ) * sin(theta*pi/180) + eyeZ;
        break;

    case 'r':
        lookX -= eyeX;
        lookZ-=eyeZ;
        theta = cartToPolarTheta(lookX,lookZ);
        theta-=4;
        eyeX = sqrt(lookX*lookX+lookZ*lookZ) * cos(theta*pi/180) + eyeX;
        eyeZ = sqrt(lookX*lookX+lookZ*lookZ) * sin(theta*pi/180) + eyeZ;
        break;

    case 'l':
        rot++;
        break;

    case 'j':
        rot--;
        break;

    case 'i':
        eyeY++;
        break;

    case 'k':
        eyeY--;
        break;

    case 'z':
        eyeZ++;
        lookZ++;
        break;

    case 'x':
        eyeZ--;
        lookZ--;
        break;

    case 'o':
        lookX -= eyeX;
        lookZ-=eyeZ;
        theta = cartToPolarTheta(lookX,lookZ);
        theta-=4;
        lookX = sqrt(lookX*lookX+lookZ*lookZ) * cos(theta*pi/180) + eyeX;
        lookZ = sqrt(lookX*lookX+lookZ*lookZ) * sin(theta*pi/180) + eyeZ;
        break;

    case 'p':
        lookX -= eyeX;
        lookZ-=eyeZ;
        theta = cartToPolarTheta(lookX,lookZ);
        theta+=4;
        lookX = sqrt(lookX*lookX+lookZ*lookZ) * cos(theta*pi/180) + eyeX;
        lookZ = sqrt(lookX*lookX+lookZ*lookZ) * sin(theta*pi/180) + eyeZ;
        break;

    case '1':
        light1 = !light1;
        break;

    case '2':
        light2 = !light2;
        break;

    case '3':
        light3 = !light3;
        break;

    case 'a':
        ambient = !ambient;
        break;

    case 'd':
        diffuse = !diffuse;
        break;

    case 's':
        specular = !specular;
        break;

    case 'b':
        rltvX += movement;
        rotY -= rotAngle;
        break;
    case 'c':
        rltvX -= movement;
        rotY += rotAngle;
        break;

    case 'q':
        exit(0);
        break;

    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(width,height);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);

    glutCreateWindow("Car Game 1607037");

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutReshapeFunc(resize);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDepthFunc(GL_LESS);

    glEnable(GL_DEPTH_TEST);
    glShadeModel( GL_SMOOTH );
    glEnable(GL_NORMALIZE);
    glEnable(GL_BLEND);
    glEnable(GL_LIGHTING);

    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\road.bmp",textureRoad);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\foothpath.bmp",textureFoothpath);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\window.bmp",textureWindow);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\bc1.bmp",textureBc1);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\bc2.bmp",textureBc2);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\bc3.bmp",textureBc3);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\bc4.bmp",textureBc4);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\bc5.bmp",textureBc5);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\rock.bmp",textureRock);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\front_engine.bmp",textureFrontEngine);
    LoadTexture("C:\\Users\\nhsha\\OneDrive\\Desktop\\1607037 Car Game\\morning_sky.bmp",textureMorningSky);

    printf("\t********** FUNCTIONS AND CORRESPONDING KEYS **********\n \tFOR LEFT ROTATION, PRESS j \n \tFOR RIGHT ROTATION, PRESS l\n ");
    printf("\tFOR MOVING THE CAR LEFT, PRESS c \n");
    printf("\tFOR MOVING THE CAR RIGHT, PRESS b \n");
    printf("\tFOR UP DIRECTION, PRESS I \n");
    printf("\tFOR DOWN DIRECTION, PRESS K \n");
    printf("\tFOR ZOOM IN, PRESS z \n");
    printf("\tFOR ZOOM OUT, PRESS x \n");
    printf("\tFOR PANNING RIGHT, PRESS p \n");
    printf("\tFOR PANNING LEFT, PRESS o \n");
    printf("\tFOR RIGHT ROTATION FIXING LOOK AT POINT, PRESS r \n");
    printf("\tFOR LEFT ROTATION FIXING LOOK AT POINT, PRESS t \n");
    printf("\tFOR LIGHT1(Point Light) ON/OFF, TOGGLE 1 \n");
    printf("\tFOR LIGHT2 (Point Light) ON/OFF, TOGGLE 2 \n");
    printf("\tFOR LIGHT3 (Spot Light) ON/OFF, TOGGLE 3 \n");
    printf("\tFOR AMBIENT ON/OFF, TOGGLE a \n");
    printf("\tFOR SPECULAR ON/OFF, TOGGLE s \n");
    printf("\tFOR DIFFUSE ON/OFF, TOGGLE d \n");
    printf("\tTO QUIT, PRESS q \n");

    glutMainLoop();

    return EXIT_SUCCESS;
}
