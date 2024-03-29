#define getCurrentTime GetTickCount
#define gameRatio 9		//default 9
#define gameSpeed 0.5	//default 1
#define viewSpeed 2		//default 2
int viewSelected = 0;	//default 0
bool ingame = false;	//default false
int menuFade = 100;		//default 100
int terminajogo = 0;	//default 0
int menuSelected = 100;	//default 100
int firstGame = 0;
int gameScene = 301; //default 301
int gameTime = 304;
int switchRecord = 207; //default on
int switchFullScreen = 208; //default off
int pontJog1 = 9;
int pontJog2 = 9;
int numPlayRecord = 0;
bool Record = false;
bool RecordNext = false;
int gameWon = 401; // default 401 (player1)
int menuTexture = 114;
int undoTexture = 113;
int stopCameraTexture = 111;
int stopCamera = 0;

//Game Modes
bool easySelected = false;
bool mediumSelected = false;
bool hardSelected = false;
bool easySelected2 = false;
bool mediumSelected2 = false;
bool hardSelected2 = false;
bool startGame = false;
int modoCPU = 0;
int modoCPUvsJogador = 0;
int pedeJogadaCPU = 1;
int CPUMode1 = 0;
int CPUMode2 = 0;
int continuaCPU = 0;
int timeplay = 30;
time_t start,endtime;
int stopTime = 0;
int modoJogVsJog = 0;
int escSelected = 0;
int oneMoreUndo = 0;
int undo = 0;


float gameMatrix[gameRatio][gameRatio] = {{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0}};

float matrixViewPlayer[4][4] = {{0,0,0,0},
								{0,1,0,0},
								{0,0,0,0},
								{0,0,0,1}
								};
float angView = 0;

//flag Jogador [1] -> jogador1
bool flagJog = true;

class Peca{
public:
	bool active;
	float x;
	float y;
	float z;
	float PosTab;
	Peca(float x, float y, float z, float pos){
		active = true;
		this->x=x;
		this->y=y;
		this->z=z;
		this->PosTab = pos;
	}

};

vector<Peca*> player1;
vector<Peca*> player2;
//Peca a efectuar o movimento
Peca* pecaSelected;
bool pecaSelectedB = false;
vector<Peca*> pecaConquest;
//Movimentos a efectuar
float movH;
float movV;
bool mouseBlock = false;
bool animBlock = false;
bool Pview = true;

class Jogada{
public:
	float Jog;
	float fromRow;
	float fromColumn;
	float toRow;
	float toColumn;
	vector<Peca*> PecasConq;
	Jogada(float Jog,float fromRow, float fromColumn, float toRow, float toColumn){
		this->Jog = Jog;
		this->fromRow = fromRow;
		this->fromColumn = fromColumn;
		this->toRow = toRow;
		this->toColumn = toColumn;
	}
};

class Jogo{
	vector<Jogada*> Jogadas;
public:
	void insertJog(Jogada* jog);
	void printJogo(void);
	vector<Jogada*> getJogo(void);
	void retrieveLast(void);
	void reset(void);
	Jogo(){}

};

void Jogo::insertJog(Jogada* jog){
	this->Jogadas.push_back(jog);
}

void Jogo::printJogo(void){
	for(int i = 0; i<this->Jogadas.size();i++){
		cout << ":::::Jogada "<< i <<":::::"<< endl;
		cout << "- Jogador" << this->Jogadas.at(i)->Jog << " -"  << endl;
		cout << "FROM: " << "[" <<this->Jogadas.at(i)->fromRow << "|" << this->Jogadas.at(i)->fromColumn<< "]" << endl;
		cout << "TO:   " << "[" <<this->Jogadas.at(i)->toRow << "|" << this->Jogadas.at(i)->toColumn<< "]" << endl;
		cout << "numPlayRecord: " << numPlayRecord<< endl;
	}
}

vector<Jogada*> Jogo::getJogo(void){
	return this->Jogadas;
}


void Jogo::retrieveLast(void){
	this->Jogadas.pop_back();

}

void Jogo::reset(void){
	this->Jogadas.clear();
}