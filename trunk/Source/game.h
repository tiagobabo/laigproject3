#define gameRatio 9
#define gameSpeed 0.5
#define viewSpeed 2

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
	float x;
	float y;
	float z;
	float PosTab;
	Peca(float x, float y, float z, float pos){
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
//Movimentos a efectuar
float movH;
float movV;
bool mouseBlock = false;
bool Pview = true;

class Jogada{
public:
	float Jog;
	float fromRow;
	float fromColumn;
	float toRow;
	float toColumn;
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
	}
}

vector<Jogada*> Jogo::getJogo(void){
	return this->Jogadas;
}


void Jogo::retrieveLast(void){
	this->Jogadas.pop_back();
}
