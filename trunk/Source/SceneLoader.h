#include <cstdlib>
#include <GL\glui.h>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iterator>

#include "RGBpixmap.h"
#include "..\tinyxml.h"

using namespace std;

class Light
{
public:
	string id;
	bool enabled;
	float position[4];
	float ambient[4];
	float specular[4];
	float diffuse[4];

	Light(string id, bool enabled)
	{
		this->id = id;
		this->enabled = enabled;
		for(int i = 0; i < 4; i++)
		{
			this->position[i] = 0;
			this->ambient[i] = 0;
			this->specular[i] = 0;
			this->diffuse[i] = 0;
		}
	}
};

class Texture
{
public:
	string id;
	string file;
	float length_s;
	float length_t;
	Texture(string id, string file, float length_s, float length_t)
	{
		this->id = id;
		this->file = file;
		this->length_s = length_s;
		this->length_t = length_t;
	}
};

class Material
{
public:
	string id;
	float emission[4];
	float ambient[4];
	float diffuse[4];
	float specular[4];
	float shininess;

	Material(string id)
	{
		this->id = id;
		shininess = 0;
		for(int i = 0; i < 4; i++)
		{
			this->emission[i] = 0;
			this->ambient[i] = 0;
			this->specular[i] = 0;
			this->diffuse[i] = 0;
		}
	}
};


class Node
{
private:
	string id, type;
	string materialid;
	string textureid;
public:
	vector<Node*> nodes;
	vector<string> ids;
	Texture* texture;
	Material* material;
	float transformations[4][4];
	Node(string id, string type);
	virtual void draw() = 0;
	virtual Node* clone() = 0;
	string getID() {return id;};
	string getType() {return type;};
	void setTexture(string t);
	void setMaterial(string m);
	void setTransformations(float trans[4][4]);
	void setId(string id);
	string getTextureId();
	string getMaterialId();
	float** getTransformations();
	void setVisi(int v);
	int visi;
	int visi_enable;
};

class Object : public Node
{
private:
public:
	Object(string id, string type);
	virtual void draw() = 0;
	virtual Node* clone() = 0;
}; 

class Box: public Object
{
private:
	float dx,dy,dz;
public:
	Box(string id, string type, float dx,float dy, float dz);
	Node* clone() { return new Box(*this); }
	void draw();
};

class Triangle: public Object
{
private:
	float x1,x2,x3,y1,y2,y3,z1,z2,z3;
public:
	Triangle(string id, string type, float x1, float x2, float x3, float y1, float y2, float y3, float z1, float z2, float z3);
	Node* clone() { return new Triangle(*this); }
	void draw();
};

class Rectangle: public Object
{
private:
	float x1,y1,x2,y2;
public:
	Rectangle(string id, string type, float x1, float y1, float x2, float y2);
	Node* clone() { return new Rectangle(*this); }
	void draw();
};

class Cylinder: public Object
{
private:
	float base, top, height, slices, stacks;
public:
	Cylinder(string id, string type, float base, float top, float height, float slices, float stacks);
	Node* clone() { return new Cylinder(*this); }
	void draw();
};

class Sphere: public Object
{
private:
	float radius, slices, stacks;
public:
	Sphere(string id, string type, float radius, float slices, float stacks);
	Node* clone() { return new Sphere(*this); }
	void draw();
};

class Disk: public Object
{
private:
	float inner, outer, slices, loops;
public:
	Disk(string id, string type, float inner, float outer, float slices, float loops);
	Node* clone() { return new Disk(*this); }
	void draw();
};

class CompoundObject : public Node
{
private:
	
public:
	CompoundObject(string id, string type);
	void addId(string id);
	void addNode(Node* node);
	void draw();
	Node* clone() { return new CompoundObject(*this); }
	CompoundObject* clone2() { return new CompoundObject(*this); }

};

class Scene
{
public:
	TiXmlElement* sgxElement; 
	TiXmlElement* texturesElement;
	TiXmlElement* materialsElement;
	TiXmlElement* lightsElement;
	TiXmlElement* objectsElement;
	TiXmlElement* illuminationElement;

	//globals
	int maxlights;
	int maxtextures; 
	int maxmaterials; 
	int maxobjects;
	string root;

	//view
	float near;
	float far;
	float axisscale;

	//matrix de transformacao inicial
	float m[4][4];

	//lights
	bool doublesided, local;
	float ambient[4];
	float background[4];

	vector<Light*> lights;
	vector<Texture*> textures;
	vector<Material*> materials;

	Scene(){
		sgxElement=NULL; 
		texturesElement=NULL;
		materialsElement=NULL;
		lightsElement=NULL;
		objectsElement=NULL;
		illuminationElement=NULL;
	}

};

Node* loadScene(Scene* s);
