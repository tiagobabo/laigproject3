#include "SceneLoader.h"

RGBpixmap pixmap;
vector<Texture*> textures;

int exists(Texture* text)
{
	for(int i = 0; i < textures.size(); i++)
	{
		if(textures.at(i)->file == text->file)
			return i+1;
	}
	char *FileExt = const_cast<char*> ( text->file.c_str());
	pixmap.readBMPFile(FileExt);
	textures.push_back(text);
	pixmap.setTexture(textures.size());
	return textures.size();
}

void ReduceToUnit(float vector[3])					// Reduces A Normal Vector (3 Coordinates)
{									// To A Unit Normal Vector With A Length Of One.
	float length;							// Holds Unit Length
	// Calculates The Length Of The Vector
	length = (float)sqrt((vector[0]*vector[0]) + (vector[1]*vector[1]) + (vector[2]*vector[2]));

	if(length == 0.0f)						// Prevents Divide By 0 Error By Providing
		length = 1.0f;						// An Acceptable Value For Vectors To Close To 0.

	vector[0] /= length;						// Dividing Each Element By
	vector[1] /= length;						// The Length Results In A
	vector[2] /= length;						// Unit Normal Vector.
}


void calcNormal(float v[3][3], float out[3])				// Calculates Normal For A Quad Using 3 Points
{
	float v1[3],v2[3];						// Vector 1 (x,y,z) & Vector 2 (x,y,z)
	static const int x = 0;						// Define X Coord
	static const int y = 1;						// Define Y Coord
	static const int z = 2;						// Define Z Coord

	// Finds The Vector Between 2 Points By Subtracting
	// The x,y,z Coordinates From One Point To Another.

	// Calculate The Vector From Point 1 To Point 0
	v1[x] = v[0][x] - v[1][x];					// Vector 1.x=Vertex[0].x-Vertex[1].x
	v1[y] = v[0][y] - v[1][y];					// Vector 1.y=Vertex[0].y-Vertex[1].y
	v1[z] = v[0][z] - v[1][z];					// Vector 1.z=Vertex[0].y-Vertex[1].z
	// Calculate The Vector From Point 2 To Point 1
	v2[x] = v[1][x] - v[2][x];					// Vector 2.x=Vertex[0].x-Vertex[1].x
	v2[y] = v[1][y] - v[2][y];					// Vector 2.y=Vertex[0].y-Vertex[1].y
	v2[z] = v[1][z] - v[2][z];					// Vector 2.z=Vertex[0].z-Vertex[1].z
	// Compute The Cross Product To Give Us A Surface Normal
	out[x] = v1[y]*v2[z] - v1[z]*v2[y];				// Cross Product For Y - Z
	out[y] = v1[z]*v2[x] - v1[x]*v2[z];				// Cross Product For X - Z
	out[z] = v1[x]*v2[y] - v1[y]*v2[x];				// Cross Product For X - Y

	ReduceToUnit(out);						// Normalize The Vectors
}

void loadMaterial(Material* m)
{
	glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMaterialfv(GL_FRONT, GL_SHININESS, &m->shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  m->specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   m->diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT,   m->ambient);
	glMaterialfv(GL_FRONT, GL_EMISSION,   m->emission);
}

//constructores das classes
Node::Node(string id, string type)
{
	this->id = id;
	this->type = type;
	this->texture = NULL;
	this->material = NULL;
	this->visi=-1;
	this->visi_enable=1;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, &transformations[0][0]);
}

Object::Object(string id, string type):Node(id,type) {}

CompoundObject::CompoundObject(string id, string type):Node(id,type) {}

Box::Box(string id, string type, float dx, float dy, float dz)
	:Object(id,type)
{
	this->dx=dx;
	this->dy=dy;
	this->dz=dz;

}

Triangle::Triangle(string id, string type, float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3)
	:Object(id, type)
{
	this->x1= x1;
	this->x2= x2;
	this->x3= x3;
	this->y1= y1;
	this->y2= y2;
	this->y3= y3;
	this->z1= z1;
	this->z2= z2;
	this->z3= z3;
}

Rectangle::Rectangle(string id, string type, float x1, float y1, float x2, float y2)
	:Object(id, type)
{
	this->x1=x1;
	this->y1=y1;
	this->x2=x2;
	this->y2=y2;
}

Cylinder::Cylinder(string id, string type, float base, float top, float height, float slices, float stacks)
	:Object(id,type)
{
	this->base = base;
	this->top = top;
	this-> height = height;
	this->slices = slices;
	this->stacks = stacks;
}

Disk::Disk(string id, string type, float inner, float outer, float slices, float loops)
	:Object(id, type)
{
	this->inner = inner;
	this->outer = outer;
	this->slices = slices;
	this->loops = loops;
}

Sphere::Sphere(string id, string type, float radius, float slices, float stacks)
	:Object(id, type)
{
	this->radius = radius;
	this->slices = slices;
	this->stacks = stacks;
}

//Fim da construção das classes

//Funções partilhadas da classe Node

void Node::setTexture(string t)
{
	this->textureid = t;
}

void Node::setMaterial(string m)
{
	this->materialid = m;
}

void Node::setVisi(int v)
{
	this->visi=v;
}

string Node::getTextureId()
{
	return this->textureid;
}

string Node::getMaterialId()
{
	return this->materialid;
}

void Node::setId(string id)
{
	this->id = id;
}

void CompoundObject::addNode(Node* node)
{
	this->nodes.push_back(node);
}

void CompoundObject::addId(string node)
{
	this->ids.push_back(node);
}


void CompoundObject::draw()
{

		vector<Node*>::iterator it;
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glMultMatrixf(&this->transformations[0][0]);
	for(it=nodes.begin() ; it < nodes.end(); it++)
	{
		glMatrixMode(GL_MODELVIEW);
		if((*it)->visi_enable==1){
			if((*it)->visi==1)
				(*it)->draw();
		}
		else
			(*it)->draw();
	}
	glPopMatrix();
}

void Box::draw()
{
	float s=1;
	float t=1;
	if(this->texture !=  NULL&& this->texture->id != "clear"){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, exists(this->texture));
		s=this->texture->length_s;
		t=this->texture->length_t;
	}
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);

	// estao a faltar 4 faces...
	//direita
	glBegin(GL_POLYGON);
 	    glNormal3d(1.0,0.0,0.0);
		glTexCoord2f((this->dz)/s,0.0); glVertex3d(this->dx/2,-this->dy/2,-this->dz/2);
		glTexCoord2f((this->dz)/s,(this->dy)/t); glVertex3d(this->dx/2,this->dy/2,-this->dz/2);
		glTexCoord2f(0.0,(this->dy)/t); glVertex3d(this->dx/2,this->dy/2,this->dz/2);
		glTexCoord2f(0.0,0.0); glVertex3d(this->dx/2,-this->dy/2,this->dz/2);
	glEnd();

	//esquerda
	glBegin(GL_POLYGON);
 	    glNormal3d(-1.0,0.0,0.0);
		glTexCoord2f(0.0,0.0); glVertex3d(-this->dx/2,-this->dy/2,-this->dz/2);
		glTexCoord2f((this->dz)/s,0.0); glVertex3d(-this->dx/2,-this->dy/2,this->dz/2);
		glTexCoord2f((this->dz)/s,(this->dy)/t); glVertex3d(-this->dx/2,this->dy/2,this->dz/2);
		glTexCoord2f(0.0,(this->dy)/t); glVertex3d(-this->dx/2,this->dy/2,-this->dz/2);		
	glEnd();

	//frente
	glBegin(GL_POLYGON);
 	    glNormal3d(0.0,0.0,1.0);
		glTexCoord2f((this->dx)/s,0.0); glVertex3d(this->dx/2,-this->dy/2,this->dz/2);
		glTexCoord2f((this->dx)/s,(this->dy)/t); glVertex3d(this->dx/2,this->dy/2,this->dz/2);
		glTexCoord2f(0.0,(this->dy)/t); glVertex3d(-this->dx/2,this->dy/2,this->dz/2);
		glTexCoord2f(0.0,0.0); glVertex3d(-this->dx/2,-this->dy/2,this->dz/2);
	glEnd();

	//tras
	glBegin(GL_POLYGON);
 	    glNormal3d(0.0,0.0,-1.0);
		glTexCoord2f(0.0,0.0); glVertex3d(this->dx/2,-this->dy/2,-this->dz/2);
		glTexCoord2f((this->dx)/s,0.0); glVertex3d(-this->dx/2,-this->dy/2,-this->dz/2);
		glTexCoord2f((this->dx)/s,(this->dy)/t); glVertex3d(-this->dx/2,this->dy/2,-this->dz/2);
		glTexCoord2f(0.0,(this->dy)/t); glVertex3d(this->dx/2,this->dy/2,-this->dz/2);
	glEnd();

	//cima
	glBegin(GL_POLYGON);
	    glNormal3d(0.0,1.0,0.0);
		glTexCoord2f((this->dx)/s,0.0); glVertex3d(this->dx/2,this->dy/2,this->dz/2);
		glTexCoord2f((this->dx)/s,(this->dz)/t); glVertex3d(this->dx/2,this->dy/2,-this->dz/2);
		glTexCoord2f(0.0,(this->dz)/t); glVertex3d(-this->dx/2,this->dy/2,-this->dz/2);
		glTexCoord2f(0.0,0.0); glVertex3d(-this->dx/2,this->dy/2,this->dz/2);
	glEnd();

	//baixo
	glBegin(GL_POLYGON);
	    glNormal3d(0.0,-1.0,0.0);
		glTexCoord2f(0.0,0.0); glVertex3d(this->dx/2,-this->dy/2,this->dz/2);
		glTexCoord2f((this->dx)/s,0.0); glVertex3d(-this->dx/2,-this->dy/2,this->dz/2);
		glTexCoord2f((this->dx)/s,(this->dz)/t); glVertex3d(-this->dx/2,-this->dy/2,-this->dz/2);
		glTexCoord2f(0.0,(this->dz)/t); glVertex3d(this->dx/2,-this->dy/2,-this->dz/2);
	glEnd();

	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Triangle::draw()
{
	float s=1;
	float t=1;
	if(this->texture !=  NULL&& this->texture->id != "clear"){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, exists(this->texture));
		s=this->texture->length_s;
		t=this->texture->length_t;
	}
	if(this->material != NULL)
		loadMaterial(this->material);
	float pontos[3][3] = {{this->x1,this->y1,this->z1},{this->x2,this->y2,this->z2},{this->x3,this->y3,this->z3}};
	float normal[3];
	calcNormal(pontos, normal);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	glBegin(GL_TRIANGLES);
	glNormal3d(normal[0],normal[1],normal[2]);
		glTexCoord2f(0.0,0.0); glVertex3d(this->x1, this->y1, this->z1);
		glTexCoord2f(abs((this->x2-this->x1)/s*normal[2]+(this->x2-this->x1)/s*normal[1]+(this->z2-this->z1)/s*normal[0]),0.0); glVertex3d(this->x2, this->y2, this->z2);
		glTexCoord2f(abs((this->x3-this->x1)/s*normal[2]+(this->x3-this->x1)/s*normal[1]+(this->z3-this->z1)/s*normal[0]),abs((this->y3-this->y1)/t*normal[2]+(this->x3-this->x1)/t*normal[1]+(this->y3-this->y1)/t*normal[0])); glVertex3d(this->x3, this->y3, this->z3);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}

void Rectangle::draw()
{
	float s=1;
	float t=1;
	if(this->texture !=  NULL && this->texture->id != "clear"){
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, exists(this->texture));
		s=this->texture->length_s;
		t=this->texture->length_t;
	}
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	glBegin(GL_POLYGON);
	if( (this->y2 < this->y1) && (this->x2 < this->x1)){
		glNormal3d(0.0,0.0,-1.0);
		glTexCoord2f(0.0,0.0); glVertex3d(this->x2, this->y2, 0.0);
		glTexCoord2f((this->x1-this->x2)/s,0.0); glVertex3d(this->x1, this->y2, 0.0);
		glTexCoord2f((this->x2-this->x1)/s,(this->y2-this->y1)/t); glVertex3d(this->x1, this->y1, 0.0);
		glTexCoord2f(0.0,(this->y2-this->y1)/t); glVertex3d(this->x2, this->y1, 0.0);
	}
	else{
		glNormal3d(0.0,0.0,1.0);
		glTexCoord2f(0.0,0.0); glVertex3d(this->x1, this->y1, 0.0);
		glTexCoord2f((this->x2-this->x1)/s,0.0); glVertex3d(this->x2, this->y1, 0.0);
		glTexCoord2f((this->x2-this->x1)/s,(this->y2-this->y1)/t); glVertex3d(this->x2, this->y2, 0.0);
		glTexCoord2f(0.0,(this->y2-this->y1)/t); glVertex3d(this->x1, this->y2, 0.0);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
}
void Cylinder::draw()
{
	float s=1;
	float t=1;
	GLUquadric* glQ2;
	glQ2 = gluNewQuadric();
	if(this->texture !=  NULL&& this->texture->id != "clear"){
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(glQ2, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, exists(this->texture));
		s=this->texture->length_s;
		t=this->texture->length_t;
	}
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	gluCylinder(glQ2, this->base, this->top, this->height, this->slices, this->stacks);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	gluQuadricTexture(glQ2, GL_FALSE);
}

void Disk::draw()
{
	float s=1;
	float t=1;
	GLUquadric* glQ2;
	glQ2 = gluNewQuadric();
	if(this->texture !=  NULL && this->texture->id != "clear"){
		gluQuadricTexture(glQ2, GL_TRUE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, exists(this->texture));
		s=this->texture->length_s;
		t=this->texture->length_t;
	}
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	gluDisk(glQ2, this->inner, this->outer, this->slices, this->loops);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluQuadricTexture(glQ2, GL_FALSE);
}
void Sphere::draw()
{
	float s=1;
	float t=1;
	GLUquadric* glQ2;
	glQ2 = gluNewQuadric();
	if(this->texture !=  NULL&& this->texture->id != "clear"){
		gluQuadricTexture(glQ2, GL_TRUE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, exists(this->texture));
		s=this->texture->length_s;
		t=this->texture->length_t;
	}
	if(this->material != NULL)
		loadMaterial(this->material);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(&this->transformations[0][0]);
	gluSphere(glQ2, this->radius, this->slices, this->stacks);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	gluQuadricTexture(glQ2, GL_FALSE);
}
