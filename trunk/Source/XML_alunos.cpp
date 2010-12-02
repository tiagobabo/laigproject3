// G1_Ilum.cpp : Defines the entry point for the console application.
//
#include "SceneLoader.h"

#include <GL\glui.h>
#include <math.h>
#include <iostream>

using namespace std;

RGBpixmap pixmap2;



// dimensoes e localizacao da janela
#define DIMX 500
#define DIMY 500
#define INITIALPOS_X 200
#define INITIALPOS_Y 200

#define BUFSIZE 512

#define VLENGTH 3
#define mesaList 1

float xy_aspect;		// aspect ratio da area de visualizacao
int window_w=DIMX;
int window_h=DIMY;

// picking
GLuint selectBuf[BUFSIZE];

Node* raiz;
Scene cena;

// matriz de transf. geometrica utilizada pelo botao esferico
float view_rotate[16] =	{ 1,0,0,0,
						  0,1,0,0,
						  0,0,1,0,
						  0,0,0,1 };



// vector de posicao utilizado pelo botao de afastamento
float obj_pos[] = { 0.0, 0.0, 0.0 };

// declara��es para os tres eixos (cilindros)
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

// declara��es para as stripes que formam o plano
double i,j;
double di, limi=dimx, divisoes_i = 60;	//60
double dj, limj=dimz, divisoes_j = 100;	//100

// declara��es para a fonte de luz LIGHT0;
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
//SceneLoader *scene;


GLUquadric* glQ;	// nec. p/ criar sup. quadraticas (cilindros, esferas...)

//Utiliza as estruturas de dados com a informa��o do xml para construir o plano

/* Function to normalise a vector to unit length */
void normalise(GLdouble *vec)
{
  GLdouble length = 0.0;
  int i;

  for (i=0;i<VLENGTH;i++){
     length += vec[i]*vec[i]; 
  }
  length= (GLdouble) sqrt((double)length); 

  for (i=0;i<VLENGTH;i++){
     vec[i] = vec[i]/length; 
  }
}




void newellSquare(GLdouble *vec1,GLdouble *vec2,GLdouble *vec3,GLdouble *vec4,GLdouble *normal)
{
  normal[0] = (vec1[1]-vec2[1])*(vec1[2]+vec2[2]) + 
	      (vec2[1]-vec3[1])*(vec2[2]+vec3[2]) + 
	      (vec3[1]-vec4[1])*(vec3[2]+vec4[2]) +
	      (vec4[1]-vec1[1])*(vec4[2]+vec1[2]);
  normal[1] = (vec1[2]-vec2[2])*(vec1[0]+vec2[0]) + 
	      (vec2[2]-vec3[2])*(vec2[0]+vec3[0]) + 
	      (vec3[2]-vec4[2])*(vec3[0]+vec4[0]) +
	      (vec4[2]-vec1[2])*(vec4[0]+vec1[0]);
  normal[2] = (vec1[0]-vec2[0])*(vec1[1]+vec2[1]) + 
	      (vec2[0]-vec3[0])*(vec2[1]+vec3[1]) + 
	      (vec3[0]-vec4[0])*(vec3[1]+vec4[1]) +
	      (vec4[0]-vec1[0])*(vec4[1]+vec1[1]);

  normalise(normal);

}

void paralelo(GLdouble dimx, GLdouble dimy, GLdouble dimz){
	GLdouble dx=dimx/2, dy=dimy/2, dz=dimz/2;
	
	GLdouble v1[3] = {dx,-dy,dz};
	GLdouble v2[3] = {dx,-dy,-dz};
	GLdouble v3[3] = {dx,dy,dz};
	GLdouble v4[3] = {dx,dy,-dz};
	GLdouble v5[3] = {-dx,-dy,dz};
	GLdouble v6[3] = {-dx,dy,dz};
	GLdouble v7[3] = {-dx,dy,-dz};
	GLdouble v8[3] = {-dx,-dy,-dz};
	GLdouble normal[VLENGTH];

	float mat_shininess[] = {20.0}; /* How shiny is the object (specular exponent)  */
	float mat_specular[] = {1.0, 1.0, 1.0, 1.0}; /* specular reflection. */
	float mat_diffuse[] = {1.0, 1.00, 1.00, 1.0}; /* diffuse reflection. */
	// define as caracteristicas do material (dos materiais seguintes, i.e. ate nova alteracao
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);


	//Face frente - 0
	newellSquare(v1,v3,v6,v5,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glVertex3dv(v1);
		glVertex3dv(v3);
		glVertex3dv(v6);
		glVertex3dv(v5);
	glEnd();

	// face anterior - 1
	newellSquare(v8,v7,v4,v2,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glVertex3dv(v8);
		glVertex3dv(v7);
		glVertex3dv(v4);
		glVertex3dv(v2);
	glEnd();

	// face lateral - 2
	newellSquare(v2,v4,v3,v1,normal);
	glBegin(GL_POLYGON);
 	    glNormal3dv(normal);
		glVertex3dv(v2);
		glVertex3dv(v4);
		glVertex3dv(v3);
		glVertex3dv(v1);
	glEnd();

	newellSquare(v5,v6,v7,v8,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glVertex3dv(v5);
		glVertex3dv(v6);
		glVertex3dv(v7);
		glVertex3dv(v8);
	glEnd();

	// base
	newellSquare(v1,v5,v8,v2,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glVertex3dv(v1);
		glVertex3dv(v5);
		glVertex3dv(v8);
		glVertex3dv(v2);
	glEnd();


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 2001);			// activa a textura 2001

	// topo 
	newellSquare(v3,v4,v7,v6,normal);
	glBegin(GL_POLYGON);
	    glNormal3dv(normal);
		glTexCoord2f(0.0,0.0); glVertex3dv(v3);
		glTexCoord2f(1.0,0.0); glVertex3dv(v4);
		glTexCoord2f(1.0,1.0); glVertex3dv(v7);
		glTexCoord2f(0.0,1.0); glVertex3dv(v6);
	glEnd();
	glDisable(GL_TEXTURE_2D);

}


 void drawScene(GLenum mode)
{
	glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 50.0 );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    
	glMultMatrixf(&cena.m[0][0]);
    glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
	glMultMatrixf( view_rotate );
	
	// tabuleiro
	if (mode == GL_SELECT)
		glPushName(0);

	glCallList(mesaList);

	// esfera 1
	if (mode == GL_SELECT) 
		glLoadName (1);

	// define caracteristicas do material da esfera 1
	glMaterialfv(GL_FRONT, GL_SHININESS, mat1_shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat1_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat1_diffuse);

	glPushMatrix();

	glTranslated(0.0,1.0,0.0);
	gluSphere(glQ, 0.5, 16, 8);
	glPopMatrix();

	// fim de esfera 1

	// esfera 2
	if (mode == GL_SELECT)
		glLoadName (2);
	
	glColorMaterial(GL_FRONT_AND_BACK,GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0,1.0,0.0);
	glPushMatrix();

	glTranslated(2.5,1.0,-1.0);
	gluSphere(glQ, 0.5, 16, 8);
	glPopMatrix();

	// fim da esfera


	// teapot
	
	if (mode == GL_SELECT)
		glLoadName (3);
	
	glColor3f(1.0,1.0,0.0);
	glPushMatrix();

	glTranslated(1.0,1.0,2.0);
	glutSolidTeapot(0.80);
	// fim teapot

	if (mode == GL_SELECT)
		glLoadName (4);
	glColor3f(1.0,0.0,0.0);
	glPushMatrix();
	glTranslatef(0.0,22.7,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( 0.0, 0.0,  0.0);
		glTexCoord2f(1.0,0.0); glVertex3d(5.0, 0.0,  0.0);
		glTexCoord2f(1.0,1.0); glVertex3d(5.0, 0.0,  -5.0);
		glTexCoord2f(0.0,1.0); glVertex3d(0.0, 0.0,  -5.0);
	glEnd();
	glPopMatrix();

	glPopName();

	glDisable(GL_COLOR_MATERIAL);

	glPopName();
}


void drawBackground() {
	glDisable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glOrtho(0, 1, 0, 1, 0, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	// No depth buffer writes for background.
	glDepthMask( false );
	glEnable(GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, background_text );
	glBegin(GL_QUADS ); {
	glTexCoord2f( 0.0f, 0.0f );
	glVertex2f( 0, 0 );
	glTexCoord2f( 0.0f, 1.0f );
	glVertex2f( 0, 1.0f );
	glTexCoord2f( 1.0f, 1.0f );
	glVertex2f( 1.0f, 1.0f );
	glTexCoord2f( 1.0f, 0.0f );
	  glVertex2f( 1.0f, 0 );
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDepthMask( true );

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
	
	// ****** declaracoes internas 'a funcao display() ******


	// ****** fim de todas as declaracoes da funcao display() ******

	glQ = gluNewQuadric();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//drawBackground();

	// inicializacoes da matriz de visualizacao
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect*0.04, xy_aspect*0.04, -0.04, 0.04, cena.near, cena.far);
	//inicializacoes da matriz de transformacoes geometricas
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// ...decorrente da utilizacao do botao de afastamento
	glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2]);    
	glMultMatrixf(&cena.m[0][0]);
	// aplica efeito do botao de rotacao
	glMultMatrixf( view_rotate );
	/*glPopMatrix();*/

	//propriedades das luzes
	for(int i = 0; i < cena.lights.size(); i++)
		glLightfv(GL_LIGHT0+i, GL_POSITION, cena.lights.at(i)->position);
	glEnable(GL_NORMALIZE);
	axis_lenght =cena.axisscale;
	glEnable(GL_COLOR_MATERIAL);
	glPushMatrix();
	// cilindro representativo do eixo X
	glColor3f(1.0,0.0,0.0);		// vermelho
	glPushMatrix();
	glRotated(90.0, 0.0,1.0,0.0 );
	gluCylinder(glQ, axis_radius_begin, axis_radius_end,
		             axis_lenght, axis_nslices, axis_nstacks);   // nao tem bases
	glPopMatrix();

	// cilindro representativo do eixo Y
	glColor3f(0.0,1.0,0.0);		// verde
	glPushMatrix();
	glRotated(-90.0, 1.0,0.0,0.0 );
	gluCylinder(glQ, axis_radius_begin, axis_radius_end,
		             axis_lenght, axis_nslices, axis_nstacks);   // nao tem bases
	glPopMatrix();
	
	// cilindro representativo do eixo Z
	glColor3f(0.0,0.0,1.0);		// azul
	glPushMatrix();
	// nao necessita rotacao... glRotated(...);
	gluCylinder(glQ, axis_radius_begin, axis_radius_end,
		             axis_lenght, axis_nslices, axis_nstacks);   // nao tem bases
	glPopMatrix();
	glPopMatrix();

	// ... e da esfera que a simboliza
	glColor3f(1.0,1.0,0.0);		// cor amarela
	gluQuadricOrientation( glQ, GLU_INSIDE);
	glPushMatrix();
	glTranslated(cena.lights.at(i)->position[0],cena.lights.at(i)->position[1],cena.lights.at(i)->position[2]);
	gluSphere(glQ, symb_light0_radius, symb_light0_slices, symb_light0_stacks);
    glPopMatrix();
	gluQuadricOrientation( glQ, GLU_OUTSIDE);

	glColor3f(1.0,1.0,1.0);
	glDisable(GL_COLOR_MATERIAL);
	

	//desenha a cena
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene(GL_RENDER);
	raiz->draw();
	glDisable(GL_NORMALIZE);
	// swapping the buffers causes the rendering above to be shown
	glutSwapBuffers();
	glFlush();
}

// AC��O DO PICKING
void pickingAction(GLuint answer) {
	printf("%d\n", answer);
}

// processa os hits no picking
void processHits (GLint hits, GLuint buffer[]) {
	GLuint *ptr = buffer;
	GLuint mindepth = 0xFFFFFFFF;
	GLuint *answer=NULL;
	GLuint nn;

	for (int i=0;i<hits;i++) {
		int num = *ptr; ptr++;
		GLuint z1 = *ptr; ptr++;
		ptr++;
		if (z1 < mindepth && num>0) {
			mindepth = z1;
			answer = ptr;
			nn=num;
		}
		for (int j=0; j < num; j++) 
			ptr++;
	}
	
	// existe uma resposta
	if (answer!=NULL) 
		pickingAction(*answer);
	
}

struct g_mouseState{
	bool leftButton;
	bool rightButton;
	bool middleButton;
	int x;
	int y;
} MouseState;

/* Mouse handling */
void processMouse(int button, int state, int x, int y) {
    GLint hits;
	GLint viewport[4];

	// update our button state
	if(button == GLUT_LEFT_BUTTON) {
		if(state == GLUT_DOWN)
			MouseState.leftButton = true;
		else
			MouseState.leftButton = false;
	}
	if(button == GLUT_RIGHT_BUTTON) {
		if(state == GLUT_DOWN)
			MouseState.rightButton = true;
		else
			MouseState.rightButton = false;
	}
	if(button == GLUT_MIDDLE_BUTTON) {
		if(state == GLUT_DOWN)
			MouseState.middleButton = true;
		else
			MouseState.middleButton = false;
	}

	// update our position so we know a delta when the mouse is moved
	MouseState.x = x;
	MouseState.y = y;
	
	if (MouseState.leftButton && !MouseState.rightButton && !MouseState.middleButton) {
		/* obrigatorio para o picking */
		// obter o viewport actual
		glGetIntegerv(GL_VIEWPORT, viewport);

		glSelectBuffer (BUFSIZE, selectBuf);
		glRenderMode (GL_SELECT);

		// inicia processo de picking
		glInitNames();
		glMatrixMode (GL_PROJECTION);
		glPushMatrix ();

		//  cria uma regi�o de 5x5 pixels em torno do click do rato para o processo de picking
		glLoadIdentity ();
		gluPickMatrix ((GLdouble) x, (GLdouble) (window_h - y), 1.0, 1.0, viewport);

		drawScene(GL_SELECT);
		raiz->draw();

		glMatrixMode (GL_PROJECTION);
		glPopMatrix ();
		glFlush ();

		hits = glRenderMode(GL_RENDER);
		processHits(hits, selectBuf);
	}
}

void processMouseMoved(int x, int y)
{
	
	// pedido de refrescamento da janela
	glutPostRedisplay();				

}

void processPassiveMouseMoved(int x, int y)
{

	// pedido de refrescamento da janela
	glutPostRedisplay();				
}

void reshape(int w, int h)
{
	int tx, ty, tw, th;

	window_w = w;	//variaveis globais; window_h e' usado em processMouse()
	window_h = h;

	GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
	glViewport( tx, ty, tw, th );
	// ver, nos manuais, a funcao GLUI_Master.auto_set_viewport()

	xy_aspect = (float)tw / (float)th;

	glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
     case 27:		// tecla de escape termina o programa
        exit(0);
        break;
	 case 's':
	  background_text++;
	  break;
	case 'a':
	  background_text--;
	  break;
   }
}

void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  


  glutPostRedisplay();

  /****************************************************************/
  /*            This demonstrates GLUI::sync_live()               */
  /*   We change the value of a variable that is 'live' to some   */
  /*   control.  We then call sync_live, and the control          */
  /*   associated with that variable is automatically updated     */
  /*   with the new value.  This frees the programmer from having */
  /*   to always remember which variables are used by controls -  */
  /*   simply change whatever variables are necessary, then sync  */
  /*   the live ones all at once with a single call to sync_live  */
  /****************************************************************/

//  glui->sync_live();

}

void inicializacao()
{
	glFrontFace(GL_CCW);		/* Front faces defined using a counterclockwise rotation. */
	glDepthFunc(GL_LEQUAL);		/* Por defeito e GL_LESS */
	glEnable(GL_DEPTH_TEST);	/* Use a depth (z) buffer to draw only visible objects. */
	glEnable(GL_CULL_FACE);		/* Use face culling to improve speed. */
	glCullFace(GL_BACK);		/* Cull only back faces. */


	// por defeito a cor e de fundo e o preto
	glClearColor(cena.background[0],cena.background[1],cena.background[2], cena.background[3]);    // cor de fundo a branco

	glLightModelf (GL_LIGHT_MODEL_TWO_SIDE, cena.doublesided);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, cena.local);

	//luz ambient
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, cena.ambient);  // define luz ambiente

	// a direccao e a posicao estao na rotina display()
	//glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 90.0);
	//glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	//glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glEnable(GL_LIGHTING);
	for(int i = 0; i < cena.lights.size(); i++)
	{
		glLightfv(GL_LIGHT0+i, GL_AMBIENT, cena.lights.at(i)->ambient);
		glLightfv(GL_LIGHT0+i, GL_DIFFUSE, cena.lights.at(i)->diffuse);
		glLightfv(GL_LIGHT0+i, GL_SPECULAR, cena.lights.at(i)->specular);
		glLightf(GL_LIGHT0+i, GL_CONSTANT_ATTENUATION, 1.0f);
		if(cena.lights.at(i)->enabled)
			glEnable(GL_LIGHT0+i);
	}

	glShadeModel(GL_SMOOTH);				// GL_FLAT / GL_SMOOTH

	glPolygonMode(GL_FRONT, GL_FILL);	// preence a face da frente dos poligonos
	//glPolygonMode(GL_FRONT, GL_LINE);	// desenha arestas dos poligonos

	pixmap2.readBMPFile("textures/background.bmp");
	pixmap2.setTexture(0);
	pixmap2.readBMPFile("textures/wall.bmp");
	pixmap2.setTexture(2);
	pixmap2.readBMPFile("textures/tiles.bmp");
	pixmap2.setTexture(3);

	// display list para a mesa
	glNewList(mesaList, GL_COMPILE);
		glPushMatrix();
		paralelo(10.0,1.0,10.0);
		glPopMatrix();
	glEndList();

}

void control_cb( int control )
{
	for(int i = 0; i < cena.lights.size(); i++){
		if ( control == 200+i) {
			  if ( !cena.lights.at(i)->enabled ) {
				glEnable( GL_LIGHT0+i );
				cena.lights.at(i)->enabled = true;
			  }
			else {
				glDisable( GL_LIGHT0 +i); 
				cena.lights.at(i)->enabled = false;
			  }
		}
	}
}

void change_visi(Node* node){

	vector<Node*>::iterator it;
	for(it=node->nodes.begin() ; it < node->nodes.end(); it++)
	{
		if((*it)->visi_enable==0)
			(*it)->visi_enable=1;
		else
			(*it)->visi_enable=0;

		if((*it)->getType() == "compound") change_visi(*it);
	}
}

void visib_control( int control )
{
	change_visi(raiz);
}

int main(int argc, char* argv[])
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize (DIMX, DIMY);
	glutInitWindowPosition (INITIALPOS_X, INITIALPOS_Y);
	main_window = glutCreateWindow (argv[0]);
	
	raiz = loadScene(&cena);
	

   glutDisplayFunc(display);
   GLUI_Master.set_glutReshapeFunc(reshape);
   GLUI_Master.set_glutKeyboardFunc (keyboard);
   GLUI_Master.set_glutMouseFunc(processMouse);
   glutMotionFunc(processMouseMoved);
   glutPassiveMotionFunc(processPassiveMouseMoved);   
   GLUI_Master.set_glutSpecialFunc( NULL );
   

	/*** Create the bottom subwindow ***/
	glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
	glui2->set_main_gfx_window( main_window );

	GLUI_Rotation *view_rot = glui2->add_rotation( "Rotacao", view_rotate );
	view_rot->set_spin( 1.0 );
	glui2->add_column( false );

	GLUI_Translation *trans_z = 
	glui2->add_translation( "Zoom", GLUI_TRANSLATION_Z, &obj_pos[2] );
	trans_z->set_speed( 2 );

	/******** Add some controls for lights ********/
	glui2->add_column( false );
	for(int i = 0; i < cena.lights.size(); i++)
	{
		int ena = cena.lights.at(i)->enabled;
		glui2->add_checkbox(const_cast<char*> (cena.lights.at(i)->id.c_str()), &ena,
				200+i, control_cb );
	}
	glui2->add_column( false );
	int enable = 1;
	glui2->add_checkbox("Visibilidade", &enable,0, visib_control);
	/* We register the idle callback with GLUI, not with GLUT */
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
   
	inicializacao();

	glutMainLoop();

	return 0;
}
