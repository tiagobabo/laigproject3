#define gameRatio 9		//default 9
#define gameSpeed 0.5	//default 0.5
#define viewSpeed 2		//default 2
int viewSelected = 0;	//default 0
bool fullscreen = false;//default false
bool ingame = false;	//default false
int menuFade = 100;		//default 100
int terminajogo = 0;	//default 0
int menuSelected = 100;	//default 100
int firstGame = 0;
int gameResolution = 202; //default 640x480
int switchRecord = 208; //default off
int switchFullScreen = 208; //default off
int pontJog1 = 9;
int pontJog2 = 9;
int numPlayRecord = 0;
bool Record = false;
bool RecordNext = false;


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