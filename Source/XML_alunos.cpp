// G1_Ilum.cpp : Defines the entry point for the console application.
//
#include "SceneLoader.h"
#include "game.h"

#include <GL\glui.h>
#include <math.h>
#include <winsock2.h>

using namespace std;

RGBpixmap pixmap2;

// dimensoes e localizacao da janela
#define DIMX 500
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

SOCKET m_socket;

bool socketConnect() {// Initialize Winsock.
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != NO_ERROR)
		printf("Client: Error at WSAStartup().\n");
    else
        printf("Client: WSAStartup() is OK.\n");

	// Create a socket.
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET) {
        printf("Client: socket() - Error at socket(): %ld\n", WSAGetLastError());
        WSACleanup();
        return false;
    }
	else
       printf("Client: socket() is OK.\n");

    // Connect to a server.
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    // Just test using the localhost, you can try other IP address
    clientService.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientService.sin_port = htons(60070);

    if (connect(m_socket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        printf("Client: connect() - Failed to connect.\n");
        WSACleanup();
        return false;
    }
    else {
       printf("Client: connect() is OK.\n");
       printf("Client: Can start sending and receiving data...\n");
    }

    // Send and receive data.
	printf("Connected\n");
	return true;
}

void envia(char *s, int len) {
	int bytesSent = send(m_socket, s, len, 0);
	if(bytesSent == SOCKET_ERROR)
		printf("Client: send() error %ld.\n", WSAGetLastError());
}

void recebe(char *ans) {
	int bytesRecv = SOCKET_ERROR;
	int pos = 0;
	while (true) {
		recv(m_socket, &ans[pos], 1, 0);
		if (ans[pos] == '\n')
			break;
		pos++;
	}
	ans[pos] = 0;
	cout << "prolog answered: " << ans << endl;
}

void quit() {
	cout << "Asking prolog to quit" << endl;
	char buff[] = "quit.\n";
	envia(buff, 6);
	char ans[1024];
	recebe(ans);
}

//Utiliza as estruturas de dados com a informação do xml para construir o plano

void showCamera(char* camera)
{
	glDisable( GL_LIGHTING );  /* Disable lighting while we render text */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0.0, 100.0, 0.0, 100.0  );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glColor3ub( 0.5, 0.3, 0.8 );
	glRasterPos2i( 1, 96);
	int len = strlen(camera);
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, camera[i]);
	}
	glEnable( GL_LIGHTING );
}

void processView(int dummy){

	switch(flagJog){
		case 0:
			if(angView < 180){
				angView+=viewSpeed;
				matrixViewPlayer[0][0] = cos(angView*PI/180);
				matrixViewPlayer[0][2] = -sin(angView*PI/180);
				matrixViewPlayer[2][0] = sin(angView*PI/180);
				matrixViewPlayer[2][2] = cos(angView*PI/180);
				glutTimerFunc(mili_secs, processView, 0);
			}
			else{
				angView = 180.0;
				matrixViewPlayer[0][0] = cos(angView*PI/180);
				matrixViewPlayer[0][2] = -sin(angView*PI/180);
				matrixViewPlayer[2][0] = sin(angView*PI/180);
				matrixViewPlayer[2][2] = cos(angView*PI/180);
			}
			break;
		case 1:
			if(angView >0){
				angView-=viewSpeed;
				matrixViewPlayer[0][0] = cos(angView*PI/180);
				matrixViewPlayer[0][2] = -sin(angView*PI/180);
				matrixViewPlayer[2][0] = sin(angView*PI/180);
				matrixViewPlayer[2][2] = cos(angView*PI/180);
				glutTimerFunc(mili_secs, processView, 0);
			}
			else{
				angView = 0.0;
				matrixViewPlayer[0][0] = cos(angView*PI/180);
				matrixViewPlayer[0][2] = -sin(angView*PI/180);
				matrixViewPlayer[2][0] = sin(angView*PI/180);
				matrixViewPlayer[2][2] = cos(angView*PI/180);
			}
			break;
	}
}

void aniStartGame(int status){
	if(menuFade > 0)
	{
		menuFade-=viewSpeed;
		glutTimerFunc(mili_secs, aniStartGame, 0);
	}
}


void drawPieceLateral(){

	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( -0.1, 0.0,  0.5);
		glTexCoord2f(1.0,0.0); glVertex3d(0.1, 0.0,  0.5);
		glTexCoord2f(1.0,1.0); glVertex3d(0.1, 0.0,  -0.5);
		glTexCoord2f(0.0,1.0); glVertex3d(-0.1, 0.0,  -0.5);
	glEnd();
}

void drawPieceSquare(){

	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( -0.5, 0.0,  0.5);
		glTexCoord2f(1.0,0.0); glVertex3d(0.5, 0.0,  0.5);
		glTexCoord2f(1.0,1.0); glVertex3d(0.5, 0.0,  -0.5);
		glTexCoord2f(0.0,1.0); glVertex3d(-0.5, 0.0,  -0.5);
	glEnd();

}

void drawPiece(int player, int ghost){
	if(ghost)
	{
		glEnable(GL_COLOR_MATERIAL);
		glColor4f(1.0,1.0,1.0,0.4);
		glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);
	}
	//Topo
	if(!ghost)
		glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, player);
	glPushMatrix();
	glTranslatef(0.0,0.2,0.0);
	drawPieceSquare();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//Fundo
	glPushMatrix();
	glRotatef(180.0, 0.0,0.0,1.0);
	drawPieceSquare();
	glPopMatrix();

	//Lateral Direita
	glPushMatrix();
	glTranslatef(0.5,0.1,0.0);
	glRotatef(-90.0, 0.0,0.0,1.0);
	drawPieceLateral();
	glPopMatrix();
	
	//Lateral esquerda
	glPushMatrix();
	glTranslatef(-0.5,0.1,0.0);
	glRotatef(90.0, 0.0,0.0,1.0);
	drawPieceLateral();
	glPopMatrix();
	
	//Tras
	glPushMatrix();
	glTranslatef(0.0,0.1,0.5);
	glRotatef(90.0, 1.0,0.0,0.0);
	glRotatef(90.0, 0.0,1.0,0.0);
	drawPieceLateral();
	glPopMatrix();
	
	//Frente
	glPushMatrix();
	glTranslatef(0.0,0.1,-0.5);
	glRotatef(-90.0, 1.0,0.0,0.0);
	glRotatef(90.0, 0.0,1.0,0.0);
	drawPieceLateral();
	glPopMatrix();
	glEnable(GL_LIGHTING);
	glDisable (GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
}
float rotY = 0, rotX = 0;
 void drawScene(GLenum mode)
{
	glFrustum( -xy_aspect*0.04, xy_aspect*0.04, -0.04, 0.04, cena.near2, cena.far2);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
	switch(viewSelected){
		case 0:
			glTranslatef( 0, 0, -menuFade ); 
			glMultMatrixf(&cena.m[0][0]);
			glTranslatef(0.0,20.0,0.0);
			glMultMatrixf( view_rotate );
			glRotatef(rotX,1.0,0.0,0.0);
			glRotatef(rotY,0.0,1.0,0.0);
			glTranslatef(0.0,-20.0,0.0);
			glMultMatrixf( &matrixViewPlayer[0][0]);
			break;
		case 1:
			gluLookAt( 0.0, 45.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0 );
			break;
		case 2:
			gluLookAt( -30.0, 45.0, 0.0, 0.0, 22.0, 0.0, 1.0, 1.0, 0.0 );
			break;
		default:
			glMultMatrixf( view_rotate );
	}
	
	if (mode == GL_SELECT)
		glPushName(0);
	
	//Desenha Zonas do tabuleiro
	//Linhas
	for(int i = 0; i<gameRatio; i++){
	//Colunas	
		for(int j=0; j<gameRatio; j++){
			if (mode == GL_SELECT)
			glLoadName ((i*10)+j);
			glPushMatrix();
			glTranslatef(-9.0+(2*j),21.0,-7.0+(2*i));
			glBegin(GL_POLYGON);
				glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
				glTexCoord2f(0.0,0.0); glVertex3d( 0.0, 0.0,  0.0);
				glTexCoord2f(1.0,0.0); glVertex3d(2.0, 0.0,  0.0);
				glTexCoord2f(1.0,1.0); glVertex3d(2.0, 0.0,  -2.0);
				glTexCoord2f(0.0,1.0); glVertex3d(0.0, 0.0,  -2.0);
			glEnd();
			glPopMatrix();
		}
	}

	//Player1
	for(int i = 0; i<player1.size(); i++){
			if (mode == GL_SELECT)
			glLoadName (100+i);
			glPushMatrix();
			glTranslatef(player1.at(i)->x,player1.at(i)->y,player1.at(i)->z);
			drawPiece(1, 0);
			glPopMatrix();
		}

	//Player2
	for(int i = 0; i<player2.size(); i++){
			if (mode == GL_SELECT)
			glLoadName (200+i);
			glPushMatrix();
			glTranslatef(player2.at(i)->x,player2.at(i)->y,player2.at(i)->z);
			drawPiece(2, 0);
			glPopMatrix();
		}

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

int ghost = 0;
string ghosts;

void drawGhosts()
{
	int j = 0;
	while(j < ghosts.size()-1)
	{
		char temp[1], temp2[1];
		temp[0] = ghosts.c_str()[j];
		temp2[0] = ghosts.c_str()[++j];
		cout << atoi(temp)<< atoi(temp2) << endl;
		glPushMatrix();
		glTranslatef(-8.0+(2*(atoi(temp)-1)),21.2,2*(atoi(temp2)-1)-8);
		drawPiece(1, 1);
		glPopMatrix();
		j++;
	}
}

void drawStartGameButton()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 101);
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,45.0,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.375); glVertex3d( -40.0, 0.0,  10.0);
		glTexCoord2f(1.0,0.375); glVertex3d(40.0, 0.0,  10.0);
		glTexCoord2f(1.0,0.625); glVertex3d(40.0, 0.0,  -10.0);
		glTexCoord2f(0.0,0.625); glVertex3d(-40.0, 0.0,  -10.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawOptionsGameButton()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 102);
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,20.0,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.375); glVertex3d( -40.0, 0.0,  10.0);
		glTexCoord2f(1.0,0.375); glVertex3d(40.0, 0.0,  10.0);
		glTexCoord2f(1.0,0.625); glVertex3d(40.0, 0.0,  -10.0);
		glTexCoord2f(0.0,0.625); glVertex3d(-40.0, 0.0,  -10.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

}

void drawQuitGameButton()
{
	//gluQuadricTexture(glQ, GL_TRUE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 103);
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,-5.0,30);
	glRotatef(90,1.0,0.0,0.0);
	//glScalef(2.0,1.0,1.0);
	//gluDisk(glQ,0.0,20,50,50);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.375); glVertex3d( -40.0, 0.0,  10.0);
		glTexCoord2f(1.0,0.375); glVertex3d(40.0, 0.0,  10.0);
		glTexCoord2f(1.0,0.625); glVertex3d(40.0, 0.0,  -10.0);
		glTexCoord2f(0.0,0.625); glVertex3d(-40.0, 0.0,  -10.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	//gluQuadricTexture(glQ, GL_FALSE);
}

void drawMenu(GLenum mode)
{
	glFrustum( -xy_aspect*0.04, xy_aspect*0.04, -0.04, 0.04, cena.near2, cena.far2);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
    
    glTranslatef( 0, 0, -100 ); 
	glMultMatrixf(&cena.m[0][0]);
	
	if (mode == GL_SELECT)
		glPushName(0);
	
	
	if (mode == GL_SELECT)
		glLoadName(1);
	drawStartGameButton();	

	if (mode == GL_SELECT)
		glLoadName(2);
	drawOptionsGameButton();

	if (mode == GL_SELECT)
		glLoadName(3);
	drawQuitGameButton();

	glPopName();

	glDisable(GL_COLOR_MATERIAL);

	glPopName();
}

void display(void)
{
	glQ = gluNewQuadric();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// inicializacoes da matriz de visualizacao
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( -xy_aspect*0.04, xy_aspect*0.04, -0.04, 0.04, cena.near2, cena.far2);
	//inicializacoes da matriz de transformacoes geometricas
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

		if(ingame){
			switch(viewSelected){
				case 0:
					glTranslatef( 0, 0, -menuFade ); 
					glMultMatrixf(&cena.m[0][0]);
					glTranslatef(0.0,20.0,0.0);
					glMultMatrixf( view_rotate );
					glRotatef(rotX,1.0,0.0,0.0);
					glRotatef(rotY,0.0,1.0,0.0);
					glTranslatef(0.0,-20.0,0.0);
					glMultMatrixf( &matrixViewPlayer[0][0]);
					break;
				case 1:
					gluLookAt( 0.0, 45.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0 );
					break;
				case 2:
					gluLookAt( -30.0, 45.0, 0.0, 0.0, 22.0, 0.0, 1.0, 1.0, 0.0 );
					break;
				default:
					glMultMatrixf( view_rotate );
			}
		}
		else{
			glTranslatef(0,0,-100);
			glMultMatrixf(&cena.m[0][0]);
			glPopMatrix();
		}

		//propriedades das luzes
		for(int i = 0; i < cena.lights.size(); i++)
			glLightfv(GL_LIGHT0+i, GL_POSITION, cena.lights.at(i)->position);
		glEnable(GL_NORMALIZE);

		//desenha a cena
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glCallList(1);
		if(ingame)
			drawScene(GL_RENDER);
		else
			drawMenu(GL_RENDER);
		if(ghost)
			drawGhosts();
		glDisable(GL_NORMALIZE);
	
		if(ingame)
			switch(viewSelected){
				case 1:
					showCamera("TOP VIEW");
					break;
				case 2:
					showCamera("OBSERVER");
					break;
				default:
					break;
			}

		// swapping the buffers causes the rendering above to be shown
		glutSwapBuffers();
		glFlush();
}

string getMatrixGame(){
	for(int i=0; i< gameRatio; i++){
		for(int j=0; j< gameRatio; j++)
			gameMatrix[i][j]=0;
	}

	for(int i=0; i<player1.size(); i++){
		int pos = player1.at(i)->PosTab;
		int row = pos/10;
		int column = pos%10;
		gameMatrix[row][column] = 1;
	}

	for(int i=0; i<player2.size(); i++){
		int pos = player2.at(i)->PosTab;
		int row = pos/10;
		int column = pos%10;
		gameMatrix[row][column] = 2;
	}
	string matrix;
	matrix += '[';
	int w = 0;
	for(w = 0; w < 9; w++)
	{
		matrix +='[';
		for(int z = 0; z < 9; z++)
		{
			if(gameMatrix[w][z] == 1)
				matrix += '1';
			else if(gameMatrix[w][z] == 2)
				matrix += '2';
			else
				matrix +='0';
			if(z != 8)
				matrix += ',';
		}
		matrix += ']';
		if(w != 8)
			matrix += ',';
	}
	matrix += ']';
	
	return matrix;
}

void printMatrixGame(){
	getMatrixGame();
	for(int i=0; i< gameRatio; i++){
		cout << "[";
		for(int j=0; j< gameRatio; j++){
			cout<< gameMatrix[i][j] << ",";
		}
		cout<< "]" << endl;
	}

}

void changePlayer(){
	flagJog = !flagJog;
	processView(0);
}

int getJogador(float row, float column)
{
	float pos = 10*row + column;
	for(int i=0; i<player1.size(); i++){
		if(player1.at(i)->PosTab == pos)
			return 1;
	}
	for(int i=0; i<player2.size(); i++){
		if(player2.at(i)->PosTab == pos)
			return 2;
	}
	return 0;

}

Peca* getPiece(float row, float column){
	float pos = 10*row + column;
	for(int i=0; i<player1.size(); i++){
		if(player1.at(i)->PosTab == pos)
			return player1.at(i);
	}
	for(int i=0; i<player2.size(); i++){
		if(player2.at(i)->PosTab == pos)
			return player2.at(i);
	}
	return NULL;
}

void pecaAniSelect(int status){
	
	switch(status){
		case 0:
			mouseBlock = true;
			if(pecaSelected->y+gameSpeed < 23.0){
				pecaSelected->y+=gameSpeed;
				glutTimerFunc(mili_secs, pecaAniSelect, 0);
			}
			else{
				pecaSelected->y=23.0;
				mouseBlock = false;
			}
			break;
		case 1:
			mouseBlock = true;
			if(pecaSelected->y-gameSpeed > 21.2){
				pecaSelected->y-=gameSpeed;
				glutTimerFunc(mili_secs, pecaAniSelect, 1);
			}
			else{
				//end of play
				pecaSelected->y=21.2;
				mouseBlock=false;
				changePlayer();
			}
			break;
		case 2:
			mouseBlock = true;
			if(pecaSelected->y-gameSpeed > 21.2){
				pecaSelected->y-=gameSpeed;
				glutTimerFunc(mili_secs, pecaAniSelect, 2);
			}
			else{
				//end of play
				pecaSelected->y=21.2;
				mouseBlock=false;
			}
			break;
	}
}

void pecaAniMovH(int status){
	mouseBlock = true;
	if(pecaSelected->x-gameSpeed < movH  && movH < pecaSelected->x + gameSpeed){
		cout << "DONE"<< endl;
		pecaSelected->x = movH;
		pecaAniSelect(1);
	}
	else if(movH > pecaSelected->x){
		pecaSelected->x+=gameSpeed;
		glutTimerFunc(mili_secs, pecaAniMovH, 0);
	}
	else if(movH < pecaSelected->x){
		pecaSelected->x-=gameSpeed;
		glutTimerFunc(mili_secs, pecaAniMovH, 0);
	}

}

void pecaAniMovV(int status){
	mouseBlock = true;
	if(pecaSelected->z-gameSpeed < movV  && movV < pecaSelected->z + gameSpeed){
		cout << "DONE"<< endl;
		pecaSelected->z = movV;
		pecaAniSelect(1);
	}
	else if(movV > pecaSelected->z){
		pecaSelected->z+=gameSpeed;
		glutTimerFunc(mili_secs, pecaAniMovV, 0);
	}
	else if(movV < pecaSelected->z){
		pecaSelected->z-=gameSpeed;
		glutTimerFunc(mili_secs, pecaAniMovV, 0);
	}

}

void removePiece(int pos){
	vector<Peca*>::iterator it;
	for(it=player1.begin(); it!=player1.end();it++){
		if((*it)->PosTab == pos){
			if(it+1 == player1.end()){
				player1.pop_back();
				break;
			}
			else
				it=player1.erase(it);
		}
	}
	
	vector<Peca*>::iterator it2;
	for(it2=player2.begin(); it2!=player2.end();it2++){
		cout << (*it2)->PosTab << "==" << pos<< endl;
		if((*it2)->PosTab == pos){
			if(it2+1 == player2.end()){
				player2.pop_back();
				break;
			}
			else
				it2=player2.erase(it2);
		}
	}
}

void changeGameMatrix(float matrix[gameRatio][gameRatio])
{
	for(int i = 0; i < gameRatio; i++)
		for(int j = 0; j < gameRatio; j++)
			if(gameMatrix[i][j] != matrix[i][j]){
				cout << "encontrei um diferente em: [" << i << "|" << j <<"]" << endl; 
				int pos = i*10 + j;
				removePiece(pos);
			}
}

void removePecasConquistadas()
{
	string matrix = getMatrixGame();
	char s2[1024];
	sprintf(s2,"conquistas(%s).\n",matrix.c_str());
	envia(s2, strlen(s2));
	char ans[1024];
	recebe(ans);
	string resp;
	resp = ans;
	replace( resp.begin(), resp.end(), '[', ' ');
	replace( resp.begin(), resp.end(), ']', ' ');
	replace( resp.begin(), resp.end(), ',', ' ');
	replace( resp.begin(), resp.end(), '.', ' ');
	float matrix_recebe[gameRatio][gameRatio];
	//[[1,1,1,1,1,1,1,1,1],[0,1,2,1,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[2,2,2,2,2,2,2,2,2]]).
	cout << ans<< endl;
	int k= 0;
	int j= 0;
	int i= 0;
	while(1){
		if(ans[k] == '1' || ans[k] == '2' || ans[k] == '0'){
			if(j == gameRatio){
					j=0;
					i++;
			}
			matrix_recebe[i][j] = atoi(&ans[k]);
			j++;
		}
		k++;
		if(ans[k]=='.')
			break;
	}
	for(int i = 0; i < gameRatio; i++){
		for(int j = 0; j < gameRatio; j++)
			cout << matrix_recebe[i][j] << "," ;
		cout << endl;
	}
	changeGameMatrix(matrix_recebe);
}

void processPlay(float row, float column, float answerRow, float answerColumn, float answer) {
	if(answer == -1)
		pecaSelected = getPiece(row,column);
	Jogada* Jog = new Jogada(getJogador(row,column),row,column,answerRow,answerColumn);
		if(answerColumn == column && answer < 100){
			float mov = row - answerRow ;
			movV = -mov*2 + pecaSelected->z;
			pecaSelected->PosTab-=mov*10; 
			cout << "--------------------/JOGADA-----------------------" << endl;
			if(answer != -1)
				jogo.insertJog(Jog);
			pecaAniMovV(0);
			removePecasConquistadas();
		}else if(row == answerRow && answer < 100){
			float mov = column - answerColumn ;
			movH = -mov*2 + pecaSelected->x;
			pecaSelected->PosTab-=mov*1; 
			cout << "MOV: " << mov<< endl;
			cout << "--------------------/JOGADA-----------------------" << endl;
			if(answer != -1)
				jogo.insertJog(Jog);
			pecaAniMovH(0);
			removePecasConquistadas();
		}else{
			cout << "JOGADA INVALIDA!" << endl;
			cout << "--------------------/JOGADA-----------------------" << endl;
			pecaAniSelect(2);
		}
		pecaSelectedB = false;
}

void verificaJogadasPossiveis(int jog, int pos)
{
	ghosts.clear();
	string matrix = getMatrixGame();
	char s2[1024];
	sprintf(s2,"jogadasPossiveis(%d,%d,%d,%s).\n",jog, pos%10+1, pos/10+1, matrix.c_str());
	envia(s2, strlen(s2));
	char ans[1024];
	recebe(ans);
	cout << endl << ans << endl;
	string res;
	for(int i = 0; ans[i] != '\0'; i++)
		if(ans[i] != '-' && ans[i] != '.' && ans[i] != '[' && ans[i] != ']' && ans[i] != ',')
			res += ans[i];
	
	ghost = 1;
	ghosts = res;
}

// ACÇÃO DO PICKING
void pickingAction(GLuint answer) {
	
	if(ingame){
		int player = answer/100;
		int i = answer%10;

		if(pecaSelectedB){
			ghost = 0;

			int pos = pecaSelected->PosTab;
			int column = pos%10;
			int answerColumn = answer%10;
			int row = pos/10;
			int answerRow = answer/10;
		
			cout << "---------------------JOGADA-----------------------" << endl;
			cout << "FROM: "<<  pecaSelected->PosTab<< endl;
			cout << "TO: " << answer << endl;
			cout << "From Column: " << column<< endl << "To Column: " << answerColumn<<endl;
			cout << "From Row: " << row << endl << "To Row: " << answerRow<<endl;
			char s2[1024];
			string matrix = getMatrixGame();
			if(flagJog)
				sprintf(s2,"verificaCaminho(%d,%d,%d,%d,%d,%s).\n",1, column+1, row+1, answerColumn+1, answerRow+1, matrix.c_str());
			else
				sprintf(s2,"verificaCaminho(%d,%d,%d,%d,%d,%s).\n",2, column+1, row+1, answerColumn+1, answerRow+1, matrix.c_str());
			cout << s2;
			envia(s2, strlen(s2));
			char ans[1024];
			recebe(ans);
			cout << endl << ans << endl;
			if(ans[0] == '1')
				processPlay(row, column, answerRow, answerColumn, answer);
			else
				processPlay(row, column, answerRow, answerColumn, 200);
		}else if(player == 1 && flagJog) {
			cout << "---------------------JOGADOR 1-----------------------" << endl;
			cout << "Player1"<< endl<< "PECA: "<< answer << endl << "POS: "<< player1.at(i)->PosTab<< endl;
			pecaSelected = player1.at(i);
			pecaSelectedB = true;
			pecaAniSelect(0);
			verificaJogadasPossiveis(1, player1.at(i)->PosTab);
		}
		else if(player == 2 && !flagJog) {
			cout << "---------------------JOGADOR 2-----------------------" << endl;
			cout << "Player2"<< endl<< "PECA: "<< answer << endl << "POS: "<< player2.at(i)->PosTab<< endl;
			pecaSelected = player2.at(i);
			pecaSelectedB = true;
			pecaAniSelect(0);
			verificaJogadasPossiveis(2, player2.at(i)->PosTab);
		}
	}
	else{
		if(answer == 1){
			aniStartGame(0);
			ingame = true;
		}
		else if(answer == 2)
			cout << "OPTIONS!" << endl;
		else if(answer == 3){
			quit();
			exit(0);
		}

	}

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
int press = 0;
/* Mouse handling */
void processMouse(int button, int state, int x, int y) {
	
	if(!mouseBlock)
	{
		GLint hits;
		GLint viewport[4];

		// update our button state
		if(button == GLUT_LEFT_BUTTON) {
			press = 1;
			if(state == GLUT_DOWN)
			{
				MouseState.leftButton = true;
			}
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

			//  cria uma região de 5x5 pixels em torno do click do rato para o processo de picking
			glLoadIdentity ();
			gluPickMatrix ((GLdouble) x, (GLdouble) (window_h - y), 1.0, 1.0, viewport);

			if(ingame)
				drawScene(GL_SELECT);
			else
				drawMenu(GL_SELECT);
			raiz->draw();

			glMatrixMode (GL_PROJECTION);
			glPopMatrix ();
			glFlush ();

			hits = glRenderMode(GL_RENDER);
			processHits(hits, selectBuf);
		}
	}
}

float xant=0, yant = 0, xant2 = 0, yant2 = 0;
float dir = 1, dir2 = 1;
void processMouseMoved(int x, int y)
{	
	if(press == 1)
	{
		xant = x;
		yant = y;
		press = 0;
	}
	else
	{
		if(dir == -1 && x-xant2 < 0)
			dir = -1;
		else if(dir == -1 && x-xant2 > 0)
		{
			xant = x;
			dir = 1;
		}
		else if (dir == 1 && x-xant2 > 0)
			dir = 1;
		else
		{
			xant = x;
			dir = -1;
		}
		
		if(dir2 == -1 && y-yant2 < 0)
			dir2 = -1;
		else if(dir2 == -1 && y-yant2 > 0)
		{
			yant = y;
			dir2 = 1;
		}
		else if (dir2 == 1 && y-yant2 > 0)
			dir2 = 1;
		else
		{
			yant = y;
			dir2 = -1;
		}
		yant2 = y;
		xant2 = x;
		rotY += (x-xant)/100;
		rotX += (y-yant)/100;
	}
	if(rotX > 45)
		rotX = 45;
	else if(rotX < -45)
		rotX = -45;

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

void inicializacaoPecas()
{
	for(int i=0; i< gameRatio; i++){
		Peca* p1 = new Peca(-8.0+(2*i),21.2,8.0, 80+i);
		Peca* p2 = new Peca(-8.0+(2*i),21.2,-8.0,i);
		player1.push_back(p1);
		player2.push_back(p2);
	}

}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
     case 27:		// tecla de escape termina o programa
		 mouseBlock=false;
		 if(ingame){
			ingame=false;
			menuFade = 100;
		 }
		 else{
			 quit();
			 exit(0);
		 }
        break;
	 case 's':
	  background_text++;
	  break;
	case 'a':
	  background_text--;
	  break;
	case '-':
		player1.at(0)->z-=1;
	  break;
	case '+':
		inicializacaoPecas();
	  break;
	case 'p':
		jogo.printJogo();
		printMatrixGame();
	  break;
	case '1':
		viewSelected = 0;
	break;
	case '2':
		viewSelected = 1;
	break;
	case '3':
		viewSelected = 2;
	break;
	case 'z':
		if(jogo.getJogo().size() != 0 && !mouseBlock){
			processPlay(jogo.getJogo().back()->toRow, jogo.getJogo().back()->toColumn, jogo.getJogo().back()->fromRow, jogo.getJogo().back()->fromColumn, -1);
			jogo.retrieveLast();
		}
	  break;
	case 'i':
		ingame = !ingame;
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
	//glDepthFunc(GL_LEQUAL);		/* Por defeito e GL_LESS */
	glEnable(GL_DEPTH_TEST);	/* Use a depth (z) buffer to draw only visible objects. */
	glEnable(GL_CULL_FACE);		/* Use face culling to improve speed. */
	glCullFace(GL_BACK);		/* Cull only back faces. */

	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT,GL_NICEST);

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
	pixmap2.readBMPFile("textures/start.bmp");
	pixmap2.setTexture(101);
	pixmap2.readBMPFile("textures/options.bmp");
	pixmap2.setTexture(102);
	pixmap2.readBMPFile("textures/quit.bmp");
	pixmap2.setTexture(103);
	inicializacaoPecas();

	matrixViewPlayer[0][0] = cos(angView);
	matrixViewPlayer[0][2] = -sin(angView);
	matrixViewPlayer[2][0] = sin(angView);
	matrixViewPlayer[2][2] = cos(angView);

	glNewList(1,GL_COMPILE);
		raiz->draw();
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
	
	if(socketConnect() == false) 
	{
		system("pause");
		return -1;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	if(fullscreen){
		glutGameModeString( "1440x900:32@60" ); //the settings 
		glutEnterGameMode(); //set glut to fullscreen using the 
	}
	else{
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowSize (DIMX, DIMY);
		glutInitWindowPosition (INITIALPOS_X, INITIALPOS_Y);
		main_window = glutCreateWindow (argv[0]);
	}


	
	
	raiz = loadScene(&cena);
	

   glutDisplayFunc(display);
   GLUI_Master.set_glutReshapeFunc(reshape);
   GLUI_Master.set_glutKeyboardFunc (keyboard);
   GLUI_Master.set_glutMouseFunc(processMouse);
   glutMotionFunc(processMouseMoved);
   glutPassiveMotionFunc(processPassiveMouseMoved);   
   GLUI_Master.set_glutSpecialFunc( NULL );
   



   if(!fullscreen){
		/*** Create the bottom subwindow ***/
		glui2 = GLUI_Master.create_glui_subwindow( main_window, GLUI_SUBWINDOW_BOTTOM );
		glui2->set_main_gfx_window( main_window );

	
	/* We register the idle callback with GLUI, not with GLUT */
	GLUI_Master.set_glutIdleFunc( myGlutIdle );
   

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
   }

	inicializacao();
	char *s = "comando(1, 2).\n";
	char *s2 = "verificaCaminho(1,1,1,1,2,[[1,1],[0,0],[2,2]]).\n";
	char *s3 = "jogadasPossiveis(1,1,1,[[1,1],[0,0],[2,2]]).\n";
	char *s4 = "conquistas([[1,1,1,1,1,1,1,1,1],[0,1,2,1,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[2,2,2,2,2,2,2,2,2]]).\n";
	char *s5 = "terminouJogo([[1,1,1,1,1,1,1,1,1],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[2,2,2,2,2,2,2,2,2]]).\n";
	char *s6 = "jogada(1,1,1,1,2,[[1,1],[0,0],[2,2]]).\n";
	char *s7 = "modoIntermedio([[1,1,1,1,1,1,1,1,1],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[2,2,2,2,2,2,2,2,2]], 1).\n";
	char *s8 = "modoFacil([[1,1,1,1,1,1,1,1,1],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[2,2,2,2,2,2,2,2,2]], 1).\n";
	char *s9 = "modoDificil([[1,1,1,1,1,1,1,1,1],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0,0],[2,2,2,2,2,2,2,2,2]], 1).\n";
	//envia(s8, strlen(s8));
	char ans[1024];
	//recebe(ans);
	glutMainLoop();

	quit();
	return 0;
}
