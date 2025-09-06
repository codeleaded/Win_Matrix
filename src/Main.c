//#include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
//#include "C:/Wichtig/System/Static/Library/Random.h"
#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/Random.h"

#define DROP_MIN_LENGTH 10
#define DROP_MAX_LENGTH 20

#define DROP_MAX_COUNT 	200

typedef struct Drop {
	Vec2 p;
	int l;
} Drop;
typedef struct Matrix {
	Vector v;
	int w;
	int h;
	char* Data;
} Matrix;

char Char_RandomChar(){
	return Random_i32_MinMax(48,126);
}

Drop Drop_New(){
	Drop d = { (Vec2){ 
		Random_i32_MinMax(0,GetWidth() / GetAlxFont()->CharSizeX),
		Random_f64_MinMax(-(GetHeight() / GetAlxFont()->CharSizeY),0.0f)
	},Random_i32_MinMax(DROP_MIN_LENGTH,DROP_MAX_LENGTH) };
	return d;
}

Matrix Matrix_New(){
	Matrix m;
	m.v = Vector_New(sizeof(Drop));
	m.w = GetWidth();
	m.h = GetHeight();
	m.Data = CStr_New(m.w * m.h);

	for(int i = 0;i<m.w * m.h;i++){
		m.Data[i] = Char_RandomChar();
	}

	return m;
}
void Matrix_Add(Matrix* m){
	Drop d = Drop_New();
	Vector_Push(&m->v,&d);
}
void Matrix_Update(Matrix* m,float Speed){
	for(int i = 0;i<m->v.size;i++){
		Drop* d = (Drop*)Vector_Get(&m->v,i);
		d->p = Vec2_Add(d->p,(Vec2){ 0.0f,Speed });

		if((d->p.y-d->l)>GetHeight()/GetAlxFont()->CharSizeY){
			Vector_Remove(&m->v,i);
			i--;
			continue;
		}
	}
	for(int i = 0;i<DROP_MAX_COUNT - m->v.size;i++){
		Matrix_Add(m);
	}
}
void Matrix_Render(Matrix* m){
	for(int i = 0;i<m->v.size;i++){
		Drop* d = (Drop*)Vector_Get(&m->v,i);

		for(int j = 0;j<d->l;j++){
			int Index = (int)(d->p.y - j) * GetWidth() + (int)d->p.x;
			if(Index>=0 && Index<m->w * m->h){
				char c = m->Data[Index];
				float g = 1.0f - (float)j / (float)d->l;
				RenderChar(c,(int)d->p.x * GetAlxFont()->CharSizeX,(int)(d->p.y - j) * GetAlxFont()->CharSizeY,Pixel_toRGBA(0.0f,g,0.0f,1.0f));//Pixel_toRGBA(0.0f,1.0f,0.0f,g)
			}
		}
	}
}
void Matrix_Free(Matrix* m){
	free(m->Data);
	Vector_Free(&m->v);
}


Matrix m;

void Setup(AlxWindow* w){
	ResizeAlxFont(50,50);

	m = Matrix_New();
}

void Update(AlxWindow* w){
    Matrix_Update(&m,10.0f * w->ElapsedTime);

	Clear(BLACK);
	Matrix_Render(&m);
}

void Delete(AlxWindow* w){
    Matrix_Free(&m);
}

int main(){
    if(Create("The Matrix",2500,1300,1,1,Setup,Update,Delete))
        Start();
    return 0;
}