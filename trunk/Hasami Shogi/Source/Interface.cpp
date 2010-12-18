// G1_Ilum.cpp : Defines the entry point for the console application.
//
#include "Interface.h"

void processScene(Node* node){

	for(int i=0; i< node->nodes.size(); i++)
	{
		if(gameScene == 301){
			node->nodes.at(i)->visi=1;
			if(node->nodes.at(i)->texture != NULL)
				if(node->nodes.at(i)->texture->id=="board2"){
						node->nodes.at(i)->texture=board1;
				}
				else if(node->nodes.at(i)->texture->id=="chao2"){
						node->nodes.at(i)->texture=floor1;
				}
				else if(node->nodes.at(i)->texture->id=="marble2"){
						node->nodes.at(i)->texture=marble1;
				}
				else if(node->nodes.at(i)->texture->id=="wall2"){
						node->nodes.at(i)->texture=wall1;
				}
				else if(node->nodes.at(i)->texture->id=="balcao2"){
						node->nodes.at(i)->texture=bar1;
				}
				processScene(node->nodes.at(i));
		}
		else if(gameScene == 302){
			node->nodes.at(i)->visi=1;
			if(node->nodes.at(i)->texture != NULL)
				if(node->nodes.at(i)->texture->id=="board1"){
						node->nodes.at(i)->texture=board2;
				}
				else if(node->nodes.at(i)->texture->id=="chao"){
						node->nodes.at(i)->texture=floor2;
				}
				else if(node->nodes.at(i)->texture->id=="marble"){
						node->nodes.at(i)->texture=marble2;
				}
				else if(node->nodes.at(i)->texture->id=="wall"){
						node->nodes.at(i)->texture=wall2;
				}
				else if(node->nodes.at(i)->texture->id=="balcao"){
						node->nodes.at(i)->texture=bar2;
				}
				processScene(node->nodes.at(i));
		}
		else if(gameScene == 303){
			if(node->nodes.at(i)->getID()!="tabuleiro"){
				node->nodes.at(i)->visi=0;
				processScene(node->nodes.at(i));
			}
		}
	}

}

void calcPont(){
int numPlayer1=0;
int numPlayer2=0;

	for(int i=0;i<player1.size(); i++)
	{
		if(player1.at(i)->active)
			numPlayer1++;
		if(player2.at(i)->active)
			numPlayer2++;
	}

	pontJog1 = numPlayer1;
	pontJog2 = numPlayer2;
	cout << "PLAYER1: " << numPlayer1<< endl;
	cout << "PLAYER2: " << numPlayer2<< endl;

}

string getMatrixGame(){
	for(int i=0; i< gameRatio; i++){
		for(int j=0; j< gameRatio; j++)
			gameMatrix[i][j]=0;
	}

	for(int i=0; i<player1.size(); i++){
		if(player1.at(i)->active){
			int pos = player1.at(i)->PosTab;
			int row = pos/10;
			int column = pos%10;
			gameMatrix[row][column] = 1;
		}
	}

	for(int i=0; i<player2.size(); i++){
		if(player2.at(i)->active){
			int pos = player2.at(i)->PosTab;
			int row = pos/10;
			int column = pos%10;
			gameMatrix[row][column] = 2;
		}
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

void verificaTermino()
{
	string matrix = getMatrixGame();
	char s2[1024];
	sprintf(s2,"terminouJogo(%s).\n",matrix.c_str());
	m_socket->envia(s2, strlen(s2));
	char ans[1024];
	m_socket->recebe(ans);
	//responde qual perdeu...
	if(ans[0] == '1' || ans[0] == '2')
	{
		if(ans[0] == '1')
		{
			if(modoCPUvsJogador || modoCPU)
				gameWon=404;
			else
				gameWon=402;
		}
		else{
			if(modoCPU)
				gameWon=403;
			else
				gameWon=401;

		}
		ingame = 0;
		terminajogo = 1;
	}
}

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

void showPont(int p,char* pont)
{
	glDisable( GL_LIGHTING );  /* Disable lighting while we render text */
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0.0, 100.0, 0.0, 100.0  );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glColor3ub( 0.5, 0.3, 0.8 );
	if(p == 1)
		glRasterPos2i( 1.0, 1.0);
	if(p == 2)
		glRasterPos2i(96.0,1.0);
	if(p == 3)
		glRasterPos2i(1.0,96.0);

	int len = strlen(pont);
	for (int i = 0; i < len; i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, pont[i]);
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
				time(&start);
				stopTime = 1;
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
				time(&start);
				stopTime = 1;
			}
			break;
	}
}

void aniStartGame(int status){
	if(menuFade > 0 && ingame)
	{
		menuFade-=viewSpeed;
		glutTimerFunc(mili_secs, aniStartGame, 0);
	}
	else
	{
		startGame=true; 
		stopTime = 1;
		time(&start);
	}
}

void drawPiece(int player, int ghost){
	double ang = atan(10.0)*180/PI;
	double ang2 = atan(0.1)*180/PI;

	if(ghost == 1)
	{
		glEnable(GL_COLOR_MATERIAL);
		glColor4f(1.0,1.0,1.0,0.4);
		glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);
	}
	else if(ghost == 2)
	{
		glEnable(GL_COLOR_MATERIAL);
		glColor4f(0.0,1.0,0.0,0.4);
		glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);
	}
	else if (ghost == 3)
	{
		glEnable(GL_COLOR_MATERIAL);
		glColor4f(1.0,0.0,0.0,0.4);
		glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);
	}
	//Topo
	else if(ghost == 0){
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(glQ,true);
	}
	glBindTexture(GL_TEXTURE_2D, (player+106));
	glTranslatef(0.0,0.5,0.0);
	gluSphere(glQ, 0.5,20,20);

	glEnable(GL_LIGHTING);
	glDisable (GL_BLEND);
	glColor4f(1.0,1.0,1.0,1.0);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);
	gluQuadricTexture(glQ,false);
}

void drawWidgetButton(float x, float y, int texture)
{
	//enableTransparent();
	gluQuadricTexture(glQ,true);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glTranslatef(x,y,2.0);
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,10.0,30);
	gluDisk(glQ,0.0,1.0,20,20);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluQuadricTexture(glQ,false);
	//disableTransparent();
}

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
		if(player1.at(i)->active)
		{		
			if (mode == GL_SELECT)
				glLoadName (100+i);
			glPushMatrix();
			glTranslatef(player1.at(i)->x,player1.at(i)->y,player1.at(i)->z);
			drawPiece(1, 0);
			glPopMatrix();
		}
	}

	//Player2
	for(int i = 0; i<player2.size(); i++){
		if(player2.at(i)->active)
		{
			if (mode == GL_SELECT)
				glLoadName (200+i);
			glPushMatrix();
			glTranslatef(player2.at(i)->x,player2.at(i)->y,player2.at(i)->z);
			glRotatef(180.0, 0.0,1.0,0.0);
			drawPiece(2, 0);
			glPopMatrix();
		}
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
	
	glEnable(GL_TEXTURE_2D);
	// No depth buffer writes for background.
	glDepthMask( false );

	glBindTexture(GL_TEXTURE_2D,104);
	glBegin( GL_QUADS );
	  glTexCoord2f( 0.0, 0.0 ); glVertex2f( 0, 0 );
	  glTexCoord2f( 0.0, 1.0 ); glVertex2f( 0, 1.0 );
	  glTexCoord2f( 1.0, 1.0 ); glVertex2f( 1.0, 1.0 );
	  glTexCoord2f( 1.0, 0.0 ); glVertex2f( 1.0, 0 );
	glEnd();

	glDepthMask( true );
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

}

void drawGhosts()
{
	int j = 0;
	while(j < ghosts.size()-1)
	{
		char temp[1], temp2[1];
		temp[0] = ghosts.c_str()[j];
		temp2[0] = ghosts.c_str()[++j];
		glPushMatrix();
		glTranslatef(-8.0+(2*(atoi(temp)-1)),21.2,2*(atoi(temp2)-1)-8);
		if(flagJog)
			drawPiece(1, 1);
		else{
			glRotatef(180.0, 0.0,1.0,0.0);
			drawPiece(2,1);
		}
		glPopMatrix();
		j++;
	}
}

void enableTransparent(){
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(1.0,1.0,1.0,0.0);
	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);
}

void disableTransparent(){
	glEnable(GL_LIGHTING);
	glDisable (GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
}

void enableSelected(){
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(1.0,0.0,0.0,0.2);
	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_LIGHTING);
}

void disableSelected(){
	glColor4f(0.0,0.0,0.0,0.0);
	glEnable(GL_LIGHTING);
	glDisable (GL_BLEND);
	glDisable(GL_COLOR_MATERIAL);
}

void drawMenuBackground(){
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, menuSelected);
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,0.0,-60.0);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( -100.0, 0.0,  100.0);
		glTexCoord2f(1.0,0.0); glVertex3d(100.0, 0.0,  100.0);
		glTexCoord2f(1.0,1.0); glVertex3d(100.0, 0.0,  -100.0);
		glTexCoord2f(0.0,1.0); glVertex3d(-100.0, 0.0,  -100.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
}

void drawStartGameButton()
{
	enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,25.0,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.375); glVertex3d( -25.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.375); glVertex3d(25.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.625); glVertex3d(25.0, 0.0,  -5.0);
		glTexCoord2f(0.0,0.625); glVertex3d(-25.0, 0.0,  -5.0);
	glEnd();
	glPopMatrix();
	disableTransparent();
}

void drawOptionsGameButton()
{
	enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,10.0,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.375); glVertex3d( -25.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.375); glVertex3d(25.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.625); glVertex3d(25.0, 0.0,  -5.0);
		glTexCoord2f(0.0,0.625); glVertex3d(-25.0, 0.0,  -5.0);
	glEnd();
	glPopMatrix();
	disableTransparent();
}

void drawHelpGameButton()
{
	enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,0.0,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.375); glVertex3d( -15.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.375); glVertex3d(15.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.625); glVertex3d(15.0, 0.0,  -5.0);
		glTexCoord2f(0.0,0.625); glVertex3d(-15.0, 0.0,  -5.0);
	glEnd();
	glPopMatrix();
	disableTransparent();
}

void drawCreditsGameButton()
{
	enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,-15.0,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.375); glVertex3d( -20.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.375); glVertex3d(20.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.625); glVertex3d(20.0, 0.0,  -5.0);
		glTexCoord2f(0.0,0.625); glVertex3d(-20.0, 0.0,  -5.0);
	glEnd();
	glPopMatrix();
	disableTransparent();
}

void drawQuitGameButton()
{
	enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(0.0,-25.0,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.375); glVertex3d( -25.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.375); glVertex3d(25.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.625); glVertex3d(25.0, 0.0,  -5.0);
		glTexCoord2f(0.0,0.625); glVertex3d(-25.0, 0.0,  -5.0);
	glEnd();
	glPopMatrix();
	disableTransparent();
}

void drawOptionScene(float x, float y, int texture)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(x,y,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( -20.0, 0.0,  3.0);
		glTexCoord2f(1.0,0.0); glVertex3d(20.0, 0.0,  3.0);
		glTexCoord2f(1.0,35.0/256.0); glVertex3d(20.0, 0.0,  -3.0);
		glTexCoord2f(0.0,35.0/256.0); glVertex3d(-20.0, 0.0,  -3.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawOptionBackForward(float x, float y)
{
	enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(x,y,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.375); glVertex3d( -3.0, 0.0,  3.0);
		glTexCoord2f(1.0,0.375); glVertex3d(3.0, 0.0,  3.0);
		glTexCoord2f(1.0,0.625); glVertex3d(3.0, 0.0,  -3.0);
		glTexCoord2f(0.0,0.625); glVertex3d(-3.0, 0.0,  -3.0);
	glEnd();
	glPopMatrix();
	disableTransparent();
}

void drawOptionSwitch(float x, float y, int switchNum)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, switchNum);
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(x,y,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,14.0/128.0); glVertex3d( -6.0, 0.0,  3.0);
		glTexCoord2f(104.0/128.0,14.0/128.0); glVertex3d(6.0, 0.0,  3.0);
		glTexCoord2f(104.0/128.0,51.0/128.0); glVertex3d(6.0, 0.0,  -3.0);
		glTexCoord2f(0.0,51.0/128.0); glVertex3d(-6.0, 0.0,  -3.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawBackMenu()
{
	enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(-45,-30,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( -6.0, 0.0,  3.0);
		glTexCoord2f(1.0,0.0); glVertex3d(6.0, 0.0,  3.0);
		glTexCoord2f(1.0,1.0); glVertex3d(6.0, 0.0,  -3.0);
		glTexCoord2f(0.0,1.0); glVertex3d(-6.0, 0.0,  -3.0);
	glEnd();
	glPopMatrix();
	disableTransparent();
}

void drawGameModeButton(float x, float y){
	enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(x,y,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( -15.0, 0.0,  3.0);
		glTexCoord2f(1.0,0.0); glVertex3d(15.0, 0.0,  3.0);
		glTexCoord2f(1.0,1.0); glVertex3d(15.0, 0.0,  -3.0);
		glTexCoord2f(0.0,1.0); glVertex3d(-15.0, 0.0,  -3.0);
	glEnd();
	glPopMatrix();
	disableTransparent();
}


void drawStartButton(float x, float y){
	enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(x,y,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( -20.0, 0.0,  4.0);
		glTexCoord2f(1.0,0.0); glVertex3d(20.0, 0.0,  4.0);
		glTexCoord2f(1.0,1.0); glVertex3d(20.0, 0.0,  -4.0);
		glTexCoord2f(0.0,1.0); glVertex3d(-20.0, 0.0,  -4.0);
	glEnd();
	glPopMatrix();
	disableTransparent();
}

void drawPlayerName(float x, float y){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, gameWon);
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(x,y,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( -18.0, 0.0,  5.0);
		glTexCoord2f(1.0,0.0); glVertex3d(18.0, 0.0,  5.0);
		glTexCoord2f(1.0,91.0/256.0); glVertex3d(18.0, 0.0,  -5.0);
		glTexCoord2f(0.0,91.0/256.0); glVertex3d(-18.0, 0.0,  -5.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void drawDifficultyButton(float x, float y, bool selected)
{
	if(selected)
		enableSelected();
	else
		enableTransparent();
	glPushMatrix();
	glRotatef(-45,1.0,0.0,0.0);
	glTranslatef(x,y,30);
	glRotatef(90,1.0,0.0,0.0);
	glBegin(GL_POLYGON);
		glNormal3d(0.0,1.0,0.0);  // esta normal fica comum aos 4 vertices
		glTexCoord2f(0.0,0.0); glVertex3d( -7.0, 0.0,  3.0);
		glTexCoord2f(1.0,0.0); glVertex3d(7.0, 0.0,  3.0);
		glTexCoord2f(1.0,1.0); glVertex3d(7.0, 0.0,  -3.0);
		glTexCoord2f(0.0,1.0); glVertex3d(-7.0, 0.0,  -3.0);
	glEnd();
	glPopMatrix();
	if(selected)
		disableSelected();
	else
		disableTransparent();
	//disableTransparent();
}

void drawMenu(GLenum mode)
{
	glFrustum( -xy_aspect*0.04, xy_aspect*0.04, -0.04, 0.04, cena.near2, cena.far2);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

    glTranslatef( 0, 0, -menuFade ); 
	glMultMatrixf(&cena.m[0][0]);
	
	if (mode == GL_SELECT)
		glPushName(0);
	drawMenuBackground();
	switch(menuSelected){
		
		case 100:
			if (mode == GL_SELECT)
				glLoadName(1);
			drawStartGameButton();	

			if (mode == GL_SELECT)
				glLoadName(2);
			drawOptionsGameButton();

			if (mode == GL_SELECT)
				glLoadName(3);
			drawHelpGameButton();

			if (mode == GL_SELECT)
				glLoadName(4);
			drawCreditsGameButton();

			if (mode == GL_SELECT)
				glLoadName(5);
			drawQuitGameButton();
			break;
		case 101:
			if (mode == GL_SELECT)
				glLoadName(1);
			drawOptionScene(20.0,8.0,gameScene);

			if (mode == GL_SELECT)
				glLoadName(2);
			drawOptionBackForward(-5.0,8.0);

			if (mode == GL_SELECT)
				glLoadName(3);
			drawOptionBackForward(45.0,8.0);
			
			if (mode == GL_SELECT)
				glLoadName(4);
			drawOptionScene(20.0,-3.0,gameTime);

			if (mode == GL_SELECT)
				glLoadName(5);
			drawOptionBackForward(-5.0,-3.0);

			if (mode == GL_SELECT)
				glLoadName(6);
			drawOptionBackForward(45.0,-3.0);

			if (mode == GL_SELECT)
				glLoadName(7);
			drawOptionSwitch(20.0,-11.0, switchFullScreen);

			if (mode == GL_SELECT)
				glLoadName(8);
			drawBackMenu();

			break;

		case 102:
			if (mode == GL_SELECT)
				glLoadName(1);
			drawBackMenu();
			break;
		case 103:
			if (mode == GL_SELECT)
				glLoadName(1);
			drawBackMenu();
			break;
		case 104:
			if (mode == GL_SELECT)
				glLoadName(1);
			drawBackMenu();

			if (mode == GL_SELECT)
				glLoadName(2);
			drawGameModeButton(0.0,2.5);

			if (mode == GL_SELECT)
				glLoadName(3);
			drawGameModeButton(0.0,-7.5);

			if (mode == GL_SELECT)
				glLoadName(4);
			drawGameModeButton(0.0,-17.5);
			break;
		case 105:
			if (mode == GL_SELECT)
				glLoadName(1);
			drawBackMenu();

			if (mode == GL_SELECT)
				glLoadName(2);
			drawDifficultyButton(1.0,2.5,easySelected);

			if (mode == GL_SELECT)
				glLoadName(3);
			drawDifficultyButton(1.0,-7.5,mediumSelected);

			if (mode == GL_SELECT)
				glLoadName(4);
			drawDifficultyButton(2.0,-18.5,hardSelected);

			if (mode == GL_SELECT)
				glLoadName(5);
			drawStartButton(0.0,-28.0);
			break;
		case 106:
			if (mode == GL_SELECT)
				glLoadName(1);
			drawBackMenu();

			if (mode == GL_SELECT)
				glLoadName(2);
			drawDifficultyButton(-14.0,2.5,easySelected);

			if (mode == GL_SELECT)
				glLoadName(3);
			drawDifficultyButton(-14.0,-7.5,mediumSelected);

			if (mode == GL_SELECT)
				glLoadName(4);
			drawDifficultyButton(-14.0,-17.5,hardSelected);

			if (mode == GL_SELECT)
				glLoadName(5);
			drawDifficultyButton(18.0,2.5,easySelected2);

			if (mode == GL_SELECT)
				glLoadName(6);
			drawDifficultyButton(18.0,-7.5,mediumSelected2);

			if (mode == GL_SELECT)
				glLoadName(7);
			drawDifficultyButton(18.0,-17.5,hardSelected2);
			
			if (mode == GL_SELECT)
				glLoadName(8);
			drawStartButton(0.0,-28.0);
			break;

		case 110:
			if (mode == GL_SELECT)
				glLoadName(1);
			drawOptionBackForward(-18.0,-15.0);

			if (mode == GL_SELECT)
				glLoadName(2);
			drawOptionBackForward(19.0,-15.0);

			drawPlayerName(2.0,18.0);
			break;
		default:
			break;
	}

	glPopName();

	glDisable(GL_COLOR_MATERIAL);

	glPopName();
}


void drawWidget(GLenum mode)
{
	glFrustum( -xy_aspect*0.04, xy_aspect*0.04, -0.04, 0.04, cena.near2, cena.far2);

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

    glTranslatef( 0, 0, -menuFade ); 
	glMultMatrixf(&cena.m[0][0]);

	if (mode == GL_SELECT)
		glPushName(1);

	if (mode == GL_SELECT)
		glLoadName(500);
	drawWidgetButton(10,-4,menuTexture);

	if (mode == GL_SELECT)
		glLoadName(501);
	drawWidgetButton(6,-4,undoTexture);

	if (mode == GL_SELECT)
		glLoadName(502);
	drawWidgetButton(2,-4,stopCameraTexture);

	glDisable(GL_COLOR_MATERIAL);

	glPopName();
}

int drawNeg = 0, negcount = 0;
float negCol, negRow;


//Draws the Negation of a movement
void drawNegation()
{
	glPushMatrix();
	glTranslatef(-8.0+(2*negCol),21.2,2*negRow-8);
	if(flagJog)
		drawPiece(1, 3);
	else{
		glRotatef(180.0, 0.0,1.0,0.0);
		drawPiece(2,3);
	}
	glPopMatrix();
}


int drawConf = 0;
float confCol, confRow;

//Draws the Confirmations of a movement
void drawConfirmation()
{
	glPushMatrix();
	glTranslatef(-8.0+(2*confCol),21.2,2*confRow-8);
	if(flagJog)
		drawPiece(1, 2);
	else{
		glRotatef(180.0, 0.0,1.0,0.0);
		drawPiece(2,2);
	}
	glPopMatrix();
}

void inicializacaoTexturas()
{
	for(int i=0; i<cena.textures.size(); i++)
	{
		if(cena.textures.at(i)->id=="board1")
			board1=cena.textures.at(i);
		else if(cena.textures.at(i)->id=="board2")
			board2=cena.textures.at(i);
		else if(cena.textures.at(i)->id=="chao")
			floor1=cena.textures.at(i);
		else if(cena.textures.at(i)->id=="chao2")
			floor2=cena.textures.at(i);
		else if(cena.textures.at(i)->id=="marble")
			marble1=cena.textures.at(i);
		else if(cena.textures.at(i)->id=="marble2")
			marble2=cena.textures.at(i);
		else if(cena.textures.at(i)->id=="wall")
			wall1=cena.textures.at(i);
		else if(cena.textures.at(i)->id=="wall2")
			wall2=cena.textures.at(i);
		else if(cena.textures.at(i)->id=="balcao")
			bar1=cena.textures.at(i);
		else if(cena.textures.at(i)->id=="balcao2")
			bar2=cena.textures.at(i);
	}
}

//Initializes the pieces of the game
void inicializacaoPecas()
{
	for(int i=0; i< gameRatio; i++){
		Peca* p1 = new Peca(-8.0+(2*i),21.1,8.0, 80+i);
		Peca* p2 = new Peca(-8.0+(2*i),21.1,-8.0,i);
		player1.push_back(p1);
		player2.push_back(p2);
	}
}

//Ends the game in progress
void terminaJogo()
{
	player1.clear();
	player2.clear();
	inicializacaoPecas();
	ingame = false;
	viewSelected = 0;
	terminajogo = 0;
	menuFade = 100;
	menuSelected = 110;
}

int row,column, row2, column2;
void processaJogadaCPU()
{
	stopTime = 0;
	pedeJogadaCPU = 0;
	string matrix = getMatrixGame();
	char s2[1024];
	if(flagJog)
	{
		switch(CPUMode1)
		{
		case 0:
			sprintf(s2,"modoFacil(%s,%d).\n",matrix.c_str(), 1);
			break;
		case 1:
			sprintf(s2,"modoIntermedio(%s,%d).\n",matrix.c_str(), 1);
			break;
		case 2:
			sprintf(s2,"modoDificil(%s,%d).\n",matrix.c_str(), 1);
			break;
		}
	}
	else
	{
		switch(CPUMode2)
		{
		case 0:
			sprintf(s2,"modoFacil(%s,%d).\n",matrix.c_str(), 2);
			break;
		case 1:
			sprintf(s2,"modoIntermedio(%s,%d).\n",matrix.c_str(), 2);
			break;
		case 2:
			sprintf(s2,"modoDificil(%s,%d).\n",matrix.c_str(), 2);
			break;
		}
	}
	m_socket->envia(s2, strlen(s2));
	char ans[1024];
	m_socket->recebe(ans);
	column = atoi(&ans[0])-1;
	row = atoi(&ans[2])-1;
	column2 = atoi(&ans[4])-1;
	row2 = atoi(&ans[6])-1;
	pecaSelected = getPiece(row, column);
	pecaAniSelect(0);
	cout << row << endl << column << endl << row2 << endl << column2 << endl;
}
void display(void)
{
	 glQ = gluNewQuadric();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // inicializacoes da matriz de visualizacao
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        glFrustum( -xy_aspect*0.04, xy_aspect*0.04, -0.04, 0.04, cena.near2, cena.far2);

        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
		glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] );
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

		//inicializacoes da matriz de transformacoes geometrica

        //propriedades das luzes
        for(int i = 0; i < cena.lights.size(); i++)
		{
            glLightfv(GL_LIGHT0+i, GL_POSITION, cena.lights.at(i)->position);
			if(ingame)
			{
				// ... e da esfera que a simboliza
				glColor3f(1.0,1.0,0.0);		// cor amarela
				gluQuadricOrientation( glQ, GLU_INSIDE);
				glPushMatrix();
				glTranslated(cena.lights.at(i)->position[0],cena.lights.at(i)->position[1],cena.lights.at(i)->position[2]);
				//gluSphere(glQ, 5, 20, 20);
				glPopMatrix();
				gluQuadricOrientation( glQ, GLU_OUTSIDE);
			}
		}

		if(!ingame)
		{
			float ambient[] = {0.9,0.9,0.9};
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		}
		else
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, cena.ambient);

        glEnable(GL_NORMALIZE);
        
        if(ingame && !terminajogo){
			drawWidget(GL_RENDER);
			drawScene(GL_RENDER);
			raiz->draw();
			if(modoJogVsJog)
			{
				glPushMatrix();
				if(viewSelected==2)
					glRotatef(-90.0,0.0,1.0,0.0);
				else
					glMultMatrixf( &matrixViewPlayer[0][0]);
				glTranslatef(0.0, 30.0, -10.0);
				glScalef(5.0, 5.0, 5.0);
				stringstream timedisp;
				time (&endtime);
				int dif = timeplay;
				if(stopTime)
				{
					int a = difftime (endtime,start);
					dif = timeplay-a;
					if(dif == -1)
					{
						CPUMode1 = 0;
						modoCPU = 1;
						processaJogadaCPU();
						if(!stopCamera) processView(0);
						else
						{
							time(&start);
							stopTime = 1;
						}
						dif = 0;
					}
				}
				timedisp << dif;
				string res2;
				timedisp >> res2;
				t3dDraw3D(res2.c_str(), 0, 0, 0.2f);
				glPopMatrix();
			}
			string res;
			stringstream pontuacao;
			pontuacao << 9-pontJog2 << "-" << 9-pontJog1;
			pontuacao >> res;
			glPushMatrix();
			if(viewSelected==2)
				glRotatef(-90.0,0.0,1.0,0.0);
			else
				glMultMatrixf( &matrixViewPlayer[0][0]);
			glTranslatef(0.0, 24.5, -10.0);
			glScalef(5.0, 5.0, 5.0);
			t3dDraw3D(res.c_str(), 0, 0, 0.2f);
			glPopMatrix();

		}
        else if(!ingame && !terminajogo)
            drawMenu(GL_RENDER);
        else
            terminaJogo();
        
		if(ingame && modoCPU && startGame)
		{
			mouseBlock=true;
			if(pedeJogadaCPU)
				processaJogadaCPU();
		}
		else if(modoCPUvsJogador && flagJog && pedeJogadaCPU)
		{
			mouseBlock=false;
			pedeJogadaCPU = 0;
			if(ghost)
				drawGhosts();
			if(drawConf)
				drawConfirmation();

			if(drawNeg && negcount < 15){
				negcount++;
				if(negcount%2 == 0)
					drawNegation();
			}
		}
		else if( modoCPUvsJogador && !flagJog)
		{
			mouseBlock=true;
			if(pedeJogadaCPU)
				processaJogadaCPU();
		}
		else
		{
			if(ghost)
				drawGhosts();
			if(drawConf)
				drawConfirmation();

			if(drawNeg && negcount < 15){
				negcount++;
				if(negcount%2 == 0)
					drawNegation();
			}
		}
        glDisable(GL_NORMALIZE);
        
        if(ingame){
			switch(viewSelected){
				case 1:
					showCamera("");
					break;
				case 2:
					showCamera("");
					break;
				default:
					break;
			}

		}
        // swapping the buffers causes the rendering above to be shown
        glutSwapBuffers();
        glFlush();
}

void changePlayer(){
	calcPont();
	verificaTermino();
	flagJog = !flagJog;
	if(!modoCPU && !modoCPUvsJogador)
		if(!stopCamera) processView(0);
		else
		{
			time(&start);
			stopTime = 1;
		}
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

int getJogador2(float pos)
{
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
void requestCPUPlay(int i);
void pecaAniConquest(int status){
	animBlock=true;
		switch(status){
			case 0:
				for(int i = 0; i<pecaConquest.size();i++){
					if(pecaConquest.at(i)->y+gameSpeed/2 < 30.0){
						pecaConquest.at(i)->y+=gameSpeed/2;
					}
					else{
						pecaConquest.at(i)->y=30.0;
						pecaConquest.at(i)->active=false;
						if(i==pecaConquest.size()-1){
							if(Record && numPlayRecord < jogo.getJogo().size()){
								processPlay(jogo.getJogo().at(numPlayRecord)->fromRow, jogo.getJogo().at(numPlayRecord)->fromColumn, jogo.getJogo().at(numPlayRecord)->toRow, jogo.getJogo().at(numPlayRecord)->toColumn, -1);
								if(numPlayRecord+1==jogo.getJogo().size())
									Record=false;
							}
							pecaConquest.clear();
							changePlayer();
							if(modoJogVsJog)
								modoCPU = 0;
							mouseBlock=false;
							glutTimerFunc(mili_secs, requestCPUPlay, 1);
						}
						//end of play
					}
				}
				if(pecaConquest.size() != 0 && pecaConquest.at(i)->y != 30.0) glutTimerFunc(mili_secs, pecaAniConquest, 0);
				break;
			case 1:
				for(int i=0; i<pecaConquest.size();i++){
					cout << pecaConquest.at(i)->y << endl;
					mouseBlock = true;
					pecaConquest.at(i)->active=true;
					if(pecaConquest.at(i)->y-gameSpeed > 21.1){
						pecaConquest.at(i)->y-=gameSpeed;
						glutTimerFunc(mili_secs, pecaAniConquest, 1);
					}
					else{
						pecaConquest.at(i)->y=21.1;
						if(i==pecaConquest.size()-1)
							pecaConquest.clear();
						mouseBlock=false;
						//end of play
					}
				}
				break;
		}
		mouseBlock=false;
	animBlock=false;
}

void requestCPUPlay(int i)
{
	pedeJogadaCPU = i;
}
void pickingAction(GLuint answer);
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
				if(modoCPU || (modoCPUvsJogador && !flagJog))
					processPlay(row,column, row2, column2,1);
				continuaCPU = 1;
			}
			break;
		case 1:
			mouseBlock = true;
			if(pecaSelected->y-gameSpeed > 21.1){
				pecaSelected->y-=gameSpeed;
				glutTimerFunc(mili_secs, pecaAniSelect, 1);
			}
			else{
				//end of play
				pecaSelected->y=21.1;
				numPlayRecord++;
				pecaAniConquest(0);
				if(pecaConquest.size()==0){
					if(Record && numPlayRecord < jogo.getJogo().size()){
						processPlay(jogo.getJogo().at(numPlayRecord)->fromRow, jogo.getJogo().at(numPlayRecord)->fromColumn, jogo.getJogo().at(numPlayRecord)->toRow, jogo.getJogo().at(numPlayRecord)->toColumn, -1);
						if(numPlayRecord+1==jogo.getJogo().size())
							Record=false;
					}
					if(modoJogVsJog)
						modoCPU = 0;
					changePlayer();
					mouseBlock=false;
					if(oneMoreUndo && modoCPUvsJogador) pickingAction(501);
					else
						glutTimerFunc(mili_secs, requestCPUPlay, 1); 
				}
			}
			break;
		case 2:
			mouseBlock = true;
			if(pecaSelected->y-gameSpeed > 21.1){
				pecaSelected->y-=gameSpeed;
				glutTimerFunc(mili_secs, pecaAniSelect, 2);
			}
			else{
				//end of play
				pecaSelected->y=21.1;
				mouseBlock=false;
			}
			break;
	}
}

double animFuncNorm(double x)
{
	return pow(x, 4);
}
clock_t startani = 0,endani;
clock_t currentani;
double posiani;

void pecaAniMovH(int status){
	mouseBlock = true;
	if(pecaSelected->x-gameSpeed < movH  && movH < pecaSelected->x + gameSpeed){
		cout << "DONE"<< endl;
		pecaSelected->x = movH;
		pecaAniSelect(1);
		drawConf = 0;
		startani = 0;
	}
	else if(movH > pecaSelected->x){
		if(startani == 0)
		{
			stopTime = 0;
			startani = clock();
			posiani = pecaSelected->x;
			endani = startani+2000;
		}
		currentani = clock();
		double val = animFuncNorm(difftime(startani,currentani)/difftime(startani,endani))*(movH-posiani);
		pecaSelected->x = posiani+val;
		glutTimerFunc(mili_secs, pecaAniMovH, 0);
	}
	else if(movH < pecaSelected->x){
		if(startani == 0)
		{
			startani = clock();
			posiani = pecaSelected->x;
			endani = startani+2000;
		}
		currentani = clock();
		double val = animFuncNorm(difftime(startani,currentani)/difftime(startani,endani))*(movH-posiani);
		pecaSelected->x = posiani+val;
		glutTimerFunc(mili_secs, pecaAniMovH, 0);
	}

}

void pecaAniMovV(int status){
	mouseBlock = true;
	if(pecaSelected->z-gameSpeed < movV  && movV < pecaSelected->z + gameSpeed){
		cout << "DONE"<< endl;
		pecaSelected->z = movV;
		pecaAniSelect(1);
		drawConf = 0;
		startani = 0;
	}
	else if(movV > pecaSelected->z){
		if(startani == 0)
		{
			stopTime = 0;
			startani = clock();
			posiani = pecaSelected->z;
			endani = startani+2000;
		}
		currentani = clock();
		double val = animFuncNorm(difftime(startani,currentani)/difftime(startani,endani))*(movV-posiani);
		pecaSelected->z = posiani+val;
		glutTimerFunc(mili_secs, pecaAniMovV, 0);
	}
	else if(movV < pecaSelected->z){
		if(startani == 0)
		{
			startani = clock();
			posiani = pecaSelected->z;
			endani = startani+2000;
		}
		currentani = clock();
		double val = animFuncNorm(difftime(startani,currentani)/difftime(startani,endani))*(movV-posiani);
		pecaSelected->z = posiani+val;
		glutTimerFunc(mili_secs, pecaAniMovV, 0);
	}

}

Peca* removePiece(int pos)
{
	int i=0;
	vector<Peca*>::iterator it;
	for(it=player1.begin(); it!=player1.end();it++){
		if((*it)->active){
			if((*it)->PosTab == pos){
				pecaConquest.push_back(*it);
				return (*it);
			}
		}
		i++;
	}
	
	i=0;
	vector<Peca*>::iterator it2;
	for(it2=player2.begin(); it2!=player2.end();it2++){
		if((*it2)->active){
			if((*it2)->PosTab == pos){
				pecaConquest.push_back(*it2);
				return (*it2);
			}
		}
		i++;
	}
}

vector<Peca*> changeGameMatrix(float matrix[gameRatio][gameRatio])
{
	vector<Peca*> conq;
	for(int i = 0; i < gameRatio; i++)
		for(int j = 0; j < gameRatio; j++)
			if(gameMatrix[i][j] != matrix[i][j]){
				cout << "encontrei um diferente em: [" << i << "|" << j <<"]" << endl; 
				int pos = i*10 + j;
				conq.push_back(removePiece(pos));
			}
			cout << "CONQ SIZE: " << conq.size()<< endl;
	return conq;
}

vector<Peca*> removePecasConquistadas()
{
	string matrix = getMatrixGame();
	char s2[1024];
	sprintf(s2,"conquistas(%s).\n",matrix.c_str());
	m_socket->envia(s2, strlen(s2));
	char ans[1024];
	m_socket->recebe(ans);
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
	return changeGameMatrix(matrix_recebe);
}

void checkConquest(vector<Peca*> conq){
	cout << "Pecas Conquistadas: " << conq.size() << endl;
	for(int i=0; i<conq.size(); i++){
		int jog=getJogador2(conq.at(i)->PosTab);
		if(jog==1)
			pecaConquest.push_back(conq.at(i));
		else if(jog==2)
			pecaConquest.push_back(conq.at(i));
	}
	pecaAniConquest(1);
}

void processPlay(float row, float column, float answerRow, float answerColumn, float answer) {
	if(answer == -1){
		pecaSelected = getPiece(row,column);
		startani = 0;
		//pecaSelected->active=true;
	}
	Jogada* Jog = new Jogada(getJogador(row,column),row,column,answerRow,answerColumn);
		if(answerColumn == column && answer < 100){
			float mov = row - answerRow ;
			movV = -mov*2 + pecaSelected->z;
			pecaSelected->PosTab-=mov*10; 
			cout << "--------------------/JOGADA-----------------------" << endl;
			confCol = answerColumn;
			confRow = answerRow;
			if((modoJogVsJog || flagJog) && !undo) drawConf = 1;
			drawConfirmation();
			stopTime = 0;
			pecaAniMovV(0);
			Jog->PecasConq = removePecasConquistadas();
			if(answer != -1)
				jogo.insertJog(Jog);
		}else if(row == answerRow && answer < 100){
			float mov = column - answerColumn ;
			movH = -mov*2 + pecaSelected->x;
			pecaSelected->PosTab-=mov*1; 
			cout << "MOV: " << mov<< endl;
			cout << "--------------------/JOGADA-----------------------" << endl;
			confCol = answerColumn;
			confRow = answerRow;
			if((modoJogVsJog || flagJog) && !undo) drawConf = 1;
			drawConfirmation();
			stopTime = 0;
			pecaAniMovH(0);
			Jog->PecasConq = removePecasConquistadas();
			if(answer != -1)
				jogo.insertJog(Jog);
		}else{
			cout << "JOGADA INVALIDA!" << endl;
			cout << "--------------------/JOGADA-----------------------" << endl;
			pecaAniSelect(2);
			if(answer > 100){
				negCol = answerColumn;
				negRow = answerRow;
			}
			else{
				negCol = column;
				negRow = row;
			}
			drawNeg = 1;
			negcount = 0;
			
			drawNegation();
		}
		pecaSelectedB = false;
}

void verificaJogadasPossiveis(int jog, int pos)
{
	ghosts.clear();
	string matrix = getMatrixGame();
	char s2[1024];
	sprintf(s2,"jogadasPossiveis(%d,%d,%d,%s).\n",jog, pos%10+1, pos/10+1, matrix.c_str());
	m_socket->envia(s2, strlen(s2));
	char ans[1024];
	m_socket->recebe(ans);
	cout << endl << ans << endl;
	string res;
	for(int i = 0; ans[i] != '\0'; i++)
		if(ans[i] != '-' && ans[i] != '.' && ans[i] != '[' && ans[i] != ']' && ans[i] != ',')
			res += ans[i];
	
	ghost = 1;
	ghosts = res;
}

//Starts a new game
void startNewGame(int New)
{
	//resetDeclarations
	terminajogo = 0;
	firstGame = 1;
	//resetPlayers
	flagJog=1;
	player1.clear();
	player2.clear();
	pontJog1 = 9;
	pontJog2 = 9;
	//resetCameras
	rotX = 0;
	rotY = 0;
	viewSelected = 0;
	menuFade = 100;
	if(!stopCamera) processView(0);
	//startNewGame
	if(New)
		jogo.reset();
	inicializacaoPecas();
	ingame = true;
	stopTime = 0;
	aniStartGame(0);
}

//Play the record of the game in progress
void gameRecordPlay(int type)
{
	switch(type){
		case 0:
			startNewGame(0);
			numPlayRecord = 0;
			viewSelected= 1;
			Record=false;
			RecordNext=true;
			break;
		case 1:
			if(!Record){
				if(!RecordNext){
					startNewGame(0);
					numPlayRecord = 0;
					viewSelected= 1;
				}
				Record=true;
				if(jogo.getJogo().size() != 0)
					processPlay(jogo.getJogo().at(numPlayRecord)->fromRow, jogo.getJogo().at(numPlayRecord)->fromColumn, jogo.getJogo().at(numPlayRecord)->toRow, jogo.getJogo().at(numPlayRecord)->toColumn, -1);
			}
			break;
		case 2:
			if(mouseBlock == false && numPlayRecord<jogo.getJogo().size())
				processPlay(jogo.getJogo().at(numPlayRecord)->fromRow, jogo.getJogo().at(numPlayRecord)->fromColumn, jogo.getJogo().at(numPlayRecord)->toRow, jogo.getJogo().at(numPlayRecord)->toColumn, -1);
			if(numPlayRecord==jogo.getJogo().size()-1)
				RecordNext=false;
			break;
		default:
			break;
	}
}

// ACÇÃO DO PICKING
void pickingAction(GLuint answer) {
	
	if(ingame){

		if(answer == 500){
			 menuSelected = 100;
			 menuFade=100;
			 startGame=false;
			 ingame = false;
			 if(modoCPU == 1)
			 {
				 modoCPU = 0;
				firstGame = 0;				
			 }
			 if(Record){
				Record = false;
				firstGame=0;
			 }
		}
		else if(answer == 501){
			if(jogo.getJogo().size() != 0 && !mouseBlock){
				undo = 1;
				processPlay(jogo.getJogo().back()->toRow, jogo.getJogo().back()->toColumn, jogo.getJogo().back()->fromRow, jogo.getJogo().back()->fromColumn, -1);
				checkConquest(jogo.getJogo().back()->PecasConq);
				jogo.retrieveLast();
				if(modoCPUvsJogador) oneMoreUndo = !oneMoreUndo;
				undo = 0;
			}
		}
		else if(answer == 502){
			if(stopCameraTexture == 111)
				stopCameraTexture = 112;
			else
				stopCameraTexture = 111;
			stopCamera = !stopCamera;
		}
		else{

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
				m_socket->envia(s2, strlen(s2));
				char ans[1024];
				m_socket->recebe(ans);
				cout << endl << ans << endl;
				if(ans[0] == '1')
					processPlay(row, column, answerRow, answerColumn, answer);
				else if(answer < 89)
					processPlay(row, column, answerRow, answerColumn, 300);
				else
				{
					answerColumn = (int) player1.at(i)->PosTab%10;
					answerRow = (int) player1.at(i)->PosTab/10;
					processPlay(row, column, answerRow, answerColumn, 300);
				}
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
	}
	else
	{
		switch(menuSelected){
			case 100:
				if(answer == 1){
					menuSelected = 104;
				}
				else if(answer == 2)
					menuSelected = 101; //opcoes
				else if(answer == 3)
					menuSelected = 102; //help
				else if(answer == 4)
					menuSelected = 103; //credits
				else if(answer == 5){ //exit
					m_socket->quit();
					exit(0);
				}
				break;
			case 101:
				if(answer == 2){
					if(gameScene==301)
						gameScene=303;
					else
						gameScene--;

					processScene(raiz);
				}
				else if(answer == 3){
					if(gameScene==303)
						gameScene=301;
					else
						gameScene++;
					processScene(raiz);
				}
				else if(answer == 5){
					if(gameTime == 304)
						gameTime = 306;
					else
						gameTime--;
					
					switch(gameTime){
						case 304:
							timeplay=30;
							break;
						case 305:
							timeplay=60;
							break;
						case 306:
							timeplay=90;
							break;
					}
				}
				else if(answer == 6){
					if(gameTime == 306)
						gameTime = 304;
					else
						gameTime++;

					switch(gameTime){
						case 304:
							timeplay=30;
							break;
						case 305:
							timeplay=60;
							break;
						case 306:
							timeplay=90;
							break;
					}
				}
				else if(answer == 7){
					if(switchFullScreen == 208){
						switchFullScreen = 207;
						glutFullScreen();
					}
					else{
						glutReshapeWindow(DIMX, DIMY); 
						glutPositionWindow(INITIALPOS_X,INITIALPOS_Y);
						switchFullScreen = 208;
					}
				}
				else if(answer == 8){
					menuSelected = 100;
				}
				break;
			case 102:
				if(answer == 1)
					menuSelected=100;
				break;
			case 103:
				if(answer == 1)
					menuSelected=100;
				break;
			case 104:
				if(answer == 1)
					menuSelected=100;
				if(answer == 2){
					modoCPUvsJogador=0;
					modoCPU=0;
					modoJogVsJog=1;
					startNewGame(1);
					menuSelected = 100;
				}
				if(answer == 3)
					menuSelected=105;
				if(answer == 4)
					menuSelected=106;
				break;
			case 105:
				if(answer==1){
					menuSelected=104;
					easySelected=false;
					mediumSelected = false;
					hardSelected= false;
				}
				if(answer==2){
					easySelected=true;
					mediumSelected = false;
					hardSelected= false;
				}
				if(answer==3){
					easySelected = false;
					mediumSelected = true;
					hardSelected= false;
				}
				if(answer==4){
					easySelected = false;
					mediumSelected = false;
					hardSelected= true;
				}
				if(answer == 5){
					modoCPU=0;
					modoCPUvsJogador = 1;
					pedeJogadaCPU=1;
					modoJogVsJog=0;
					if(easySelected){
						CPUMode2=0;
						startNewGame(1);
						easySelected = false;
						menuSelected = 100;
					}
					else if(mediumSelected){
						CPUMode2=1;
						startNewGame(1);
						mediumSelected = false;
						menuSelected = 100;
					}
					else if(hardSelected){
						CPUMode2=2;
						startNewGame(1);
						hardSelected = false;
						menuSelected = 100;
					}

				}
				break;
			case 106:
				if(answer==1){
					menuSelected=104;
					easySelected=false;
					mediumSelected = false;
					hardSelected= false;
					easySelected2 = false;
					mediumSelected2 = false;
					hardSelected2= false;
				}
				if(answer==2){
					easySelected=true;
					mediumSelected = false;
					hardSelected= false;
				}
				if(answer==3){
					easySelected = false;
					mediumSelected = true;
					hardSelected= false;
				}
				if(answer==4){
					easySelected = false;
					mediumSelected = false;
					hardSelected= true;
				}
				if(answer==5){
					easySelected2=true;
					mediumSelected2 = false;
					hardSelected2= false;
				}
				if(answer==6){
					easySelected2 = false;
					mediumSelected2 = true;
					hardSelected2= false;
				}
				if(answer==7){
					easySelected2 = false;
					mediumSelected2 = false;
					hardSelected2= true;
				}
				if(answer==8){
					modoCPUvsJogador = 0;
					modoCPU = 1;
					modoJogVsJog=0;
					pedeJogadaCPU=1;
					if((easySelected && easySelected2) ||  (easySelected && mediumSelected2) ||  (easySelected && hardSelected2) ||  (mediumSelected && easySelected2) || (mediumSelected && mediumSelected2) || (mediumSelected && hardSelected2) || (hardSelected && easySelected2) || (hardSelected && mediumSelected2) || (hardSelected && hardSelected2)){
						
						if(easySelected){
							CPUMode1=0;
						}
						else if(mediumSelected){
							CPUMode1=1;
						}
						else if(hardSelected){
							CPUMode1=2;
						}
						
						if(easySelected2){
							CPUMode2=0;
						}
						else if(mediumSelected2){
							CPUMode2=1;
						}
						else if(hardSelected2){
							CPUMode2=2;
						}
						startNewGame(1);
						easySelected=false;
						mediumSelected = false;
						hardSelected= false;
						easySelected2 = false;
						mediumSelected2 = false;
						hardSelected2= false;
						menuSelected = 100;
					}
				}
				break;
			case 110:
				if(answer==1){
					modoCPU = 0;
					modoCPUvsJogador = 0;
					gameRecordPlay(1);
				}
				else if(answer == 2)
					menuSelected = 100;

				break;
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

/* Mouse handling */
struct g_mouseState{
	bool leftButton;
	bool rightButton;
	bool middleButton;
	int x;
	int y;
} MouseState;

int press = 0;
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

			if(ingame && !terminajogo){
				drawWidget(GL_SELECT);
				drawScene(GL_SELECT);
				raiz->draw();
			}
			else if(!ingame && !terminajogo)
				drawMenu(GL_SELECT);
			else
				terminaJogo();
			
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

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
     case 27:		// tecla de escape termina o programa
		 mouseBlock=false;
		 if(menuSelected != 100 && !ingame){
			menuSelected = 100;
			easySelected=false;
			mediumSelected = false;
			hardSelected= false;
			easySelected2 = false;
			mediumSelected2 = false;
			hardSelected2= false;
		 }
		 else if(ingame){
			 menuSelected = 100;
			 menuFade=100;
			 startGame=false;
			 ingame = false;
			 if(modoCPU == 1)
			 {
				 modoCPU = 0;
				firstGame = 0;				
			 }
			 if(Record){
				Record = false;
				firstGame=0;
			 }
		 }
		 else if(firstGame!=0){
			 menuFade=0;
			 ingame=true;
			 startGame=true;
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
	case 'u':
		gameRecordPlay(0);
		break;
	case 'n':
		gameRecordPlay(2);
		break;
	case 'r':
		modoCPU = 0;
		modoCPUvsJogador = 0;
		gameRecordPlay(1);
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
			checkConquest(jogo.getJogo().back()->PecasConq);
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
		glLightf(GL_LIGHT0+i, GL_LINEAR_ATTENUATION, 0.15f);
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
	pixmap2.readBMPFile("textures/menu.bmp");
	pixmap2.setTexture(100);
	pixmap2.readBMPFile("textures/opcoes.bmp");
	pixmap2.setTexture(101);
	pixmap2.readBMPFile("textures/regras.bmp");
	pixmap2.setTexture(102);
	pixmap2.readBMPFile("textures/creditos.bmp");
	pixmap2.setTexture(103);
	pixmap2.readBMPFile("textures/gameMode.bmp");
	pixmap2.setTexture(104);
	pixmap2.readBMPFile("textures/difficultyPvsCPU.bmp");
	pixmap2.setTexture(105);
	pixmap2.readBMPFile("textures/difficultyCPUvsCPU.bmp");
	pixmap2.setTexture(106);
	pixmap2.readBMPFile("textures/piece1.bmp");
	pixmap2.setTexture(107);
	pixmap2.readBMPFile("textures/piece2.bmp");
	pixmap2.setTexture(108);
	pixmap2.readBMPFile("textures/end.bmp");
	pixmap2.setTexture(110);
	pixmap2.readBMPFile("textures/cameraTurnOn.bmp");
	pixmap2.setTexture(111);
	pixmap2.readBMPFile("textures/cameraTurnOff.bmp");
	pixmap2.setTexture(112);
	pixmap2.readBMPFile("textures/undo.bmp");
	pixmap2.setTexture(113);
	pixmap2.readBMPFile("textures/menuB.bmp");
	pixmap2.setTexture(114);


	//options
	pixmap2.readBMPFile("textures/switch_on.bmp");
	pixmap2.setTexture(207);
	pixmap2.readBMPFile("textures/switch_off.bmp");
	pixmap2.setTexture(208);

	pixmap2.readBMPFile("textures/set1.bmp");
	pixmap2.setTexture(301);
	pixmap2.readBMPFile("textures/set2.bmp");
	pixmap2.setTexture(302);
	pixmap2.readBMPFile("textures/noset.bmp");
	pixmap2.setTexture(303);
	pixmap2.readBMPFile("textures/30s.bmp");
	pixmap2.setTexture(304);
	pixmap2.readBMPFile("textures/60s.bmp");
	pixmap2.setTexture(305);
	pixmap2.readBMPFile("textures/90s.bmp");
	pixmap2.setTexture(306);

	pixmap2.readBMPFile("textures/player1.bmp");
	pixmap2.setTexture(401);
	pixmap2.readBMPFile("textures/player2.bmp");
	pixmap2.setTexture(402);
	pixmap2.readBMPFile("textures/cpu1.bmp");
	pixmap2.setTexture(403);
	pixmap2.readBMPFile("textures/cpu2.bmp");
	pixmap2.setTexture(404);

	inicializacaoTexturas();

	t3dInit();
	inicializacaoPecas();

	matrixViewPlayer[0][0] = cos(angView);
	matrixViewPlayer[0][2] = -sin(angView);
	matrixViewPlayer[2][0] = sin(angView);
	matrixViewPlayer[2][2] = cos(angView);

	/*glNewList(1,GL_COMPILE);
		raiz->draw();
	glEndList();*/

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
	
	if(m_socket->socketConnect() == false) 
	{
		system("pause");
		return -1;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize (DIMX, DIMY);
	glutInitWindowPosition (INITIALPOS_X, INITIALPOS_Y);
	main_window = glutCreateWindow ("Hasami Shogi");

	raiz = loadScene(&cena);
	
	glutDisplayFunc(display);
	GLUI_Master.set_glutReshapeFunc(reshape);
	GLUI_Master.set_glutKeyboardFunc (keyboard);
	GLUI_Master.set_glutMouseFunc(processMouse);
	glutMotionFunc(processMouseMoved);
	glutPassiveMotionFunc(processPassiveMouseMoved);   
	GLUI_Master.set_glutSpecialFunc( NULL );
	GLUI_Master.set_glutIdleFunc(myGlutIdle);
   
	inicializacao();
	glutMainLoop();

	m_socket->quit();
	return 0;
}
