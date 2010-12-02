class Peca{

public:
	float x;
	float y;
	float z;
	float PosTab;
	float AniX, AniY, AniZ;
	Peca(float x, float y, float z, float pos){
		this->x=x;
		this->y=y;
		this->z=z;
		this->PosTab = pos;
		this->AniX = 0;
		this->AniY = 0;
		this->AniZ = 0;
	}

};