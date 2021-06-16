#include <GL/gl.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <iostream>


using namespace std;

const double PI2 = 3.14159265389;


/* GLUT callback Handlers */



const int L2=17;
const int dgre2=3;
int ncpt2=L2+1;
int clikd2=0;
const int nt2 = 50;				//number of slices along x-direction
const int ntheta2 = 120;


GLfloat ctrlpoints2[L2+1][3] =
{
    {0.1000, 6.3500, 13.0000},
    {0.6000, 6.3500, 13.0000},
    {1.0250, 6.3500, 13.0000},
    {1.4500, 6.3500, 13.0000},
    {2.0500, 6.3250, 13.0000},
    {2.7250, 6.3000, 13.0000},
    {3.5250, 6.3000, 13.0000},
    {4.1000, 6.3250, 13.0000},
    {4.5000, 6.3250, 13.0000},
    {5.1000, 6.2250, 13.0000},
    {5.5500, 5.9750, 13.0000},
    {6.0250, 5.5250, 13.0000},
    {6.3500, 4.9500, 13.0000},
    {6.4500, 4.5250, 13.0000},
    {6.5500, 3.9750, 13.0000},
    {6.5750, 3.4000, 13.0000},
    {6.6250, 2.7750, 13.0000},
    {6.6250, 2.4000, 13.0000}

};

float wcsClkDn2[3],wcsClkUp2[3];
///////////////////////////////
class point12
{
public:
    point12()
    {
        x=0;
        y=0;
    }
    int x;
    int y;
} clkpt2[2];

int flag2=0;
GLint viewport2[4]; //var to hold the viewport info
GLdouble modelview2[16]; //var to hold the modelview info
GLdouble projection2[16]; //var to hold the projection matrix info

//////////////////////////
void scsToWcs2(float sx,float sy, float wcsv[3] );
void processMouse2(int button, int state, int x, int y);
///////////////////////////

void scsToWcs2(float sx,float sy, float wcsv[3] )
{

    GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
    GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates

    //glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
    glGetDoublev( GL_PROJECTION_MATRIX, projection2 ); //get the projection matrix info
    glGetIntegerv( GL_VIEWPORT, viewport2 ); //get the viewport info

    winX = sx;
    winY = (float)viewport2[3] - (float)sy;
    winZ = 0;

    //get the world coordinates from the screen coordinates
    gluUnProject( winX, winY, winZ, modelview2, projection2, viewport2, &worldX, &worldY, &worldZ);
    wcsv[0]=worldX;
    wcsv[1]=worldY;
    wcsv[2]=worldZ;


}
void processMouse2(int button, int state, int x, int y)
{
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN)
    {
        if(flag2!=1)
        {
            flag2=1;
            clkpt2[0].x=x;
            clkpt2[0].y=y;
        }


        scsToWcs2(clkpt2[0].x,clkpt2[0].y,wcsClkDn2);
        //cout<<"\nD: "<<x<<" "<<y<<" wcs: "<<wcsClkDn[0]<<" "<<wcsClkDn[1];
        cout<<"{"<<wcsClkUp2[0]<<", "<<wcsClkUp2[1]<<", "<<0.0<<"},"<<endl;
    }
    else if(button==GLUT_LEFT_BUTTON && state==GLUT_UP)
    {
        if (flag2==1)
        {
            clkpt2[1].x=x;
            clkpt2[1].y=y;
            flag2=0;
        }
        float wcs[3];
        scsToWcs2(clkpt2[1].x,clkpt2[1].y,wcsClkUp2);
        //cout<<"\nU: "<<x<<" "<<y<<" wcs: "<<wcsClkUp[0]<<" "<<wcsClkUp[1];
        cout<<"{"<<wcsClkUp2[0]<<", "<<wcsClkUp2[1]<<", "<<0.0<<"},"<<endl;

        clikd2=!clikd2;
    }
}

//control points
long long nCr2(int n, int r)
{
    if(r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for(i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve2 ( double t,  float xy[2])
{
    double y=0;
    double x=0;
    t=t>1.0?1.0:t;
    for(int i=0; i<=L2; i++)
    {
        int ncr=nCr2(L2,i);
        double oneMinusTpow=pow(1-t,double(L2-i));
        double tPow=pow(t,double(i));
        double coef=oneMinusTpow*tPow*ncr;
        x+=coef*ctrlpoints2[i][0];
        y+=coef*ctrlpoints2[i][1];

    }
    xy[0] = float(x);
    xy[1] = float(y);

    //return y;
}

///////////////////////
void setNormal2(GLfloat x1, GLfloat y1,GLfloat z1, GLfloat x2, GLfloat y2,GLfloat z2, GLfloat x3, GLfloat y3,GLfloat z3)
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

    glNormal3f(-Nx,-Ny,-Nz);
}

void pipeColor(float r = 0.5, float g = 0.5, float b = 0.5)
{
    float brightness = 0.75;
    float amb_perc = 0.3;

    GLfloat material_ambient[] = {r*amb_perc*brightness,g*amb_perc*brightness,b*amb_perc*brightness,1.0};
    GLfloat material_diffuse[] = {r*brightness,g*brightness,b*brightness,1.0};
    GLfloat material_specular[] = {1,1,1,1.0};
    GLfloat material_shininess[] = {90};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, material_shininess);

}

void pipeBezier(float red = 0.5, float green = 0.5, float blue = 0.5, float rad = 1)
{
    pipeColor(red,green,blue);
    int i, j;
    float x, y, z, r;				//current coordinates
    float x1, y1, z1, r1;			//next coordinates
    float theta;

    const float startx = 0, endx = ctrlpoints2[L2][0];
    //number of angular slices
    const float dx = (endx - startx) / nt2;	//x step size
    const float dtheta = 2*PI2 / ntheta2;		//angular step size

    float t=0;
    float dt=1.0/nt2;
    float xy[2];
    BezierCurve2( t,  xy);
    x = xy[0];
    r = xy[1];
    //rotate about z-axis
    float p1x,p1y,p1z,p2x,p2y,p2z;
    for ( i = 0; i < nt2; ++i )  			//step through x
    {
        theta = 0;
        t+=dt;
        BezierCurve2( t,  xy);
        x1 = xy[0];
        r1 = xy[1];

        //draw the surface composed of quadrilaterals by sweeping theta
        glBegin( GL_QUAD_STRIP );
        for ( j = 0; j <= ntheta2; ++j )
        {
            theta += dtheta;
            double cosa = cos( theta );
            double sina = sin ( theta );
            y = x + rad * cosa;
            y1 = x1 + rad * cosa;	//current and next y
            z = r + rad * sina;
            z1 = r1 + rad * sina;	//current and next z

            //edge from point at x to point at next x


            if(j>0)
            {
                setNormal2(p1x,p1y,p1z,x, y, z,p2x,p2y,p2z);
            }
            //else
            //{
                p1x=x;
                p1y=y;
                p1z=z;
                p2x=x1;
                p2y=y1;
                p2z=z1;

            //}
            glVertex3f (x, y, z);
            glVertex3f (x1, y1, z1);

            //forms quad with next pair of points with incremented theta value
        }
        glEnd();
        x = x1;
        r = r1;
    } //for i

}

void showControlPoints2()
{
    glPointSize(5.0);
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_POINTS);
    for (int i = 0; i <=L2; i++)
        glVertex3fv(&ctrlpoints2[i][0]);
    glEnd();
}

