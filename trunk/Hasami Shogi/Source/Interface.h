#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <GL\glui.h>
#include <math.h>
#include <time.h>
#include "Socket.h"
#include "SceneLoader.h"
#include "game.h"
#include "text3d.h"

using namespace std;

RGBpixmap pixmap2;

// dimensoes e localizacao da janela
#define DIMX 600
#define DIMY 500
#define INITIALPOS_X 200
#define INITIALPOS_Y 200
#define mili_secs 20
#define PI 3.14159265
#define BUFSIZE 512

#define IPADDRESS "127.0.0.1"
#define PORT 60070

#define VLENGTH 3

float xy_aspect;		// aspect ratio da area de visualizacao
int window_w=DIMX;
int window_h=DIMY;

// picking
GLuint selectBuf[BUFSIZE];

Node* raiz;
Scene cena;
Jogo jogo;

// matriz de transf. geometrica utilizada pelo botao esferico
float view_rotate[16] =	{ 1,0,0,0,
						  0,1,0,0,
						  0,0,1,0,
						  0,0,0,1 };



// vector de posicao utilizado pelo botao de afastamento
float obj_pos[] = { 0.0, 0.0, 0.0 };

// declarações para os tres eixos (cilindros)
double axis_radius_begin =  0.2;
double axis_radius_end   =  0.0;
double axis_lenght       = 1.0;
int axis_nslices = 8;
int axis_nstacks = 1;

// declaracoes para a esfera de origem de coordenadas
double orig_radius = 0.5;
int orig_slices = 8;
int orig_stacks =16;
	
// declaracoes para o plano e caixa
float mat1_shininess[] = {128.0}; 

//float mat1_specular[] = {1.0,1.0,1.0};	/* specular reflection. */
float mat1_specular[] = {0.4, 0.4, 0.4, 1.0};	/* specular reflection. */
//float mat1_diffuse[] =  {0.0, 0.0, 0.0, 1.0};	/* diffuse reflection. */
float mat1_diffuse[] =  {0.6, 0.6, 0.6, 1.0};	/* diffuse reflection. */
float mat1_ambient[] =  {0.6, 0.6, 0.6, 1.0};	/* ambient reflection. */
double dimx= 6.0;
double dimy= 2.0;
double dimz=10.0;

float light0_kc=0.0;
float light0_kl=0.0;
float light0_kq=0.1;

// declarações para as stripes que formam o plano
double i,j;
double di, limi=dimx, divisoes_i = 60;	//60
double dj, limj=dimz, divisoes_j = 100;	//100

// declarações para a fonte de luz LIGHT0;
float light0_position[]  = {0.0, 3.0, 4.0, 1.0}; // nao necessaria...
float light0_ambient[] =   {0.0, 0.0, 0.0, 1.0}; // sem componente ambiente
//float light0_diffuse[] =   {1.0, 1.0, 0.0, 0.0};
float light0_diffuse[] =   {0.8, 0.8, 0.8, 1.0};
//float light0_specular[] =  {0.0, 0.0, 0.0, 0.0};
float light0_specular[] =  {0.8, 0.8, 0.8, 1.0};
double light0x = dimx/2.0;
double light0y = 1;
double light0z = dimz/4.0;
double symb_light0_radius = 0.2;	// esfera que
int symb_light0_slices = 8;			// simboliza a
int symb_light0_stacks =16;			// fonte de luz light0

// fonte (global) de luz ambiente 
//float light_ambient[] = {0.0, 0.0, 0.0, 1.0}; /* Set the background ambient lighting. */
float light_ambient[] = {0.2, 0.2, 0.2, 1.0}; /* Set the background ambient lighting. */

// variaveis para a janela
int main_window;
GLUI  *glui2;
int background_text=0;

GLUquadric* glQ;	// nec. p/ criar sup. quadraticas (cilindros, esferas...)

Socket* m_socket = new Socket();

float rotY = 0, rotX = 0;

int ghost = 0;
string ghosts;

Texture* board1;
Texture* board2;
Texture* floor1;
Texture* floor2;
Texture* marble1;
Texture* marble2;
Texture* wall1;
Texture* wall2;
Texture* bar1;
Texture* bar2;

// Declaracao de algumas funcoes necessarias em mais do que uma parte do codigo

void processPlay(float row, float column, float answerRow, float answerColumn, float answer);
void checkConquest(vector<Peca*> conq);
Peca* getPiece(float row, float column);
void changePlayer();
void pecaAniSelect(int status);
void pickingAction(GLuint answer);
void requestCPUPlay(int i);

#endif