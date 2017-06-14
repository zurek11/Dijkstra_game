#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define INFINITY INT_MAX;
#define PARENT(x) (x/2)
#define LCHILD(x) (x*2)
#define RCHILD(x) (x*2+1)

typedef struct coordinates{
    int x,y;
    int searched;
}COORDINATES;

typedef struct vertex{
    int x,y;
    int preX,preY;
    int searched;
    int type;
    int distance;
}VERTEX;

VERTEX *heap[2000];
VERTEX *graph[1000][1000];

int lastHeap=0;
int path[10000];
int pathLength=2;
int genActive=0;

void heapInsert(VERTEX *v) {
    lastHeap++;
    int counter=lastHeap;
    heap[lastHeap]=v;
    while(v->distance < heap[PARENT(counter)]->distance) {
        heap[counter]=heap[PARENT(counter)];
        counter=PARENT(counter);
    }
    heap[counter]=v;
}

void heapify(int i){
    VERTEX *actual;
    int min;

    if (LCHILD(i)<=lastHeap && heap[LCHILD(i)]->distance < heap[i]->distance)min=LCHILD(i);
    else min=i;

    if(RCHILD(i)<=lastHeap && heap[RCHILD(i)]->distance < heap[min]->distance)min=RCHILD(i);
    if(min!=i){
        actual=heap[min];
        heap[min]=heap[i];
        heap[i]=actual;
        heapify(min);
    }
}

VERTEX *heapDelete() {
    VERTEX *actual;
    if(lastHeap<1)return NULL;
    actual=heap[1];
    heap[1]=heap[lastHeap];
    lastHeap--;
    heapify(1);
    return actual;
}

void printfpath(int srcX, int srcY, int destX, int destY){
    int counter=pathLength;

    if ((graph[destX][destY]->x!=srcX ) || (graph[destX][destY]->y!=srcY))printfpath(srcX,srcY,graph[destX][destY]->preX, graph[destX][destY]->preY);
    if(path[counter-2]!=graph[destX][destY]->y || path[counter-1]!=graph[destX][destY]->x){
        path[pathLength++]=graph[destX][destY]->y;
        path[pathLength++]=graph[destX][destY]->x;
    }
}

void direction(VERTEX *v, char **map, int map_h, int map_w){
    int i,j,k;
    v->searched=1;

    //RIGHT
    if (v->x+1<map_h && graph[v->x+1][v->y]->searched==0 && map[v->x+1][v->y]!='N'){
        if(graph[v->x+1][v->y]->distance > graph[v->x+1][v->y]->type+v->distance){
            graph[v->x+1][v->y]->distance=graph[v->x+1][v->y]->type+ v->distance;
            graph[v->x+1][v->y]->preX=v->x;
            graph[v->x+1][v->y]->preY=v->y;
            heapInsert(graph[v->x+1][v->y]);
        }
    }
    //DOWN
    if (v->y-1>=0 && graph[v->x][v->y-1]->searched==0 && map[v->x][v->y-1]!='N'){
        if( graph[v->x][v->y-1]->distance > graph[v->x][v->y-1]->type+v->distance){
            graph[v->x][v->y-1]->distance=graph[v->x][v->y-1]->type+v->distance;
            graph[v->x][v->y-1]->preX=v->x;
            graph[v->x][v->y-1]->preY=v->y;
            heapInsert(graph[v->x][v->y-1]);
        }
    }
    //LEFT
    if (v->x-1>=0 && graph[v->x-1][v->y]->searched==0 && map[v->x-1][v->y]!='N'){
        if(graph[v->x - 1][v->y]->distance > graph[v->x-1][v->y]->type+v->distance){
            graph[v->x-1][v->y]->distance=graph[v->x-1][v->y]->type+v->distance;
            graph[v->x-1][v->y]->preX=v->x;
            graph[v->x-1][v->y]->preY=v->y;
            heapInsert(graph[v->x-1][v->y]);
        }
    }
    //UP
    if (v->y+1<map_w && graph[v->x][v->y+1]->searched==0 && map[v->x][v->y+1]!='N'){
        if(graph[v->x][v->y+1]->distance > graph[v->x][v->y+1]->type+v->distance){
            graph[v->x][v->y+1]->distance=graph[v->x][v->y+1]->type+v->distance;
            graph[v->x][v->y+1]->preX=v->x;
            graph[v->x][v->y+1]->preY=v->y;
            heapInsert(graph[v->x][v->y+1]);
        }
    }

    if(genActive==0)return;
    for(i=48;i<58;++i){
        if(map[v->x][v->y]==i){
            for(j=0;j<map_h;++j) {
                for(k=0;k<map_w;++k) {
                    if(map[j][k]==map[v->x][v->y] && graph[j][k]->searched==0) {
                        if(graph[j][k]->distance > v->distance){
                            graph[j][k]->distance=v->distance;
                            graph[j][k]->preX=v->x;
                            graph[j][k]->preY=v->y;
                            heapInsert(graph[j][k]);
                        }
                    }
                }
            }
        }
    }
}

int dijkstra(int srcX, int srcY, int destX, int destY, char **map_d, int map_h, int map_w){
    int i,j;
    VERTEX *vrchol;

    lastHeap=0;
    for(i=0;i<1000;++i){
        heap[i]=(VERTEX *)malloc(sizeof(VERTEX));
        heap[i]->distance=0;
    }
    for (i=0;i<map_h;i++){
        for (j=0;j<map_w;j++){
            free(graph[i][j]);
            graph[i][j]=(VERTEX *)malloc(sizeof(VERTEX));
            graph[i][j]->x=i;
            graph[i][j]->y=j;
            graph[i][j]->preX=0;
            graph[i][j]->preY=0;
            graph[i][j]->searched=0;
            graph[i][j]->distance=INFINITY;
            if(map_d[i][j]=='H') graph[i][j]->type=2;
            else graph[i][j]->type=1;
        }
    }
    graph[srcX][srcY]->distance=0;
    graph[srcX][srcY]->searched=1;
    graph[srcX][srcY]->x=srcX;
    graph[srcX][srcY]->y=srcY;
    graph[srcX][srcY]->preX=0;
    graph[srcX][srcY]->preY=0;

    heapInsert(graph[srcX][srcY]);

    while(lastHeap!=0 && graph[destX][destY]->searched != 1){
        vrchol=heapDelete();
        direction(vrchol,map_d,map_h,map_w);
    }
    return graph[destX][destY]->distance;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty){
    int i, j;
    pathLength=2;

    COORDINATES *dragon=(COORDINATES*)malloc(sizeof(COORDINATES));
    COORDINATES *generator=(COORDINATES*)malloc(sizeof(COORDINATES));
    COORDINATES *princess1=(COORDINATES*)malloc(sizeof(COORDINATES));
    COORDINATES *princess2=(COORDINATES*)malloc(sizeof(COORDINATES));
    COORDINATES *princess3=(COORDINATES*)malloc(sizeof(COORDINATES));
    princess1->searched=0;
    princess2->searched=0;
    princess3->searched=0;
    generator->x=0;
    generator->y=0;
    for (i=0;i<n;++i){
        for (j=0;j<m;++j){
            graph[i][j]=(VERTEX*)malloc(sizeof(VERTEX));
            if (mapa[i][j]=='D'){
                dragon->x=i;
                dragon->y=j;
            }
            if (mapa[i][j]=='G'){
                generator->x=i;
                generator->y=j;
            }
            if (mapa[i][j]=='P'){
                if((princess1->searched==1) && (princess2->searched==1) && (princess3->searched==0)){
                    princess3->x=i;
                    princess3->y=j;
                    princess3->searched=1;
                }
                if((princess1->searched==1) && (princess2->searched==0)){
                    princess2->x=i;
                    princess2->y=j;
                    princess2->searched=1;
                }
                if(princess1->searched==0){
                    princess1->x=i;
                    princess1->y=j;
                    princess1->searched=1;
                }
            }
        }
    }

    if (generator->x==0 && generator->y==0){    //Map without generator
        dijkstra(0,0,dragon->x,dragon->y,mapa,n,m);
        printfpath(0,0,dragon->x,dragon->y);
        dijkstra(dragon->x,dragon->y,princess1->x,princess1->y,mapa,n,m);
        printfpath(dragon->x,dragon->y,princess1->x,princess1->y);
        dijkstra(princess1->x,princess1->y,princess2->x,princess2->y,mapa,n,m);
        printfpath(princess1->x,princess1->y,princess2->x,princess2->y);
        dijkstra(princess2->x,princess2->y,princess3->x,princess3->y,mapa,n,m);
        printfpath(princess2->x,princess2->y,princess3->x,princess3->y);
    }
    else {      //first dragon
        if (dijkstra(0,0,generator->x,generator->y,mapa,n,m)+dijkstra(generator->x,generator->y,dragon->x,dragon->y,mapa,n,m) < dijkstra(0,0,dragon->x,dragon->y,mapa,n,m)) {
            dijkstra(0,0,dragon->x,dragon->y,mapa,n,m);
            printfpath(0,0,dragon->x,dragon->y);
            dijkstra(dragon->x,dragon->y,generator->x,generator->y,mapa,n,m);
            printfpath(dragon->x,dragon->y,generator->x,generator->y);
            genActive=1;
            dijkstra(generator->x,generator->y,princess1->x,princess1->y,mapa,n,m);
            printfpath(generator->x,generator->y,princess1->x,princess1->y);
            dijkstra(princess1->x,princess1->y,princess2->x,princess2->y,mapa,n,m);
            printfpath(princess1->x,princess1->y,princess2->x,princess2->y);
            dijkstra(princess2->x,princess2->y,princess3->x,princess3->y,mapa,n,m);
            printfpath(princess2->x,princess2->y,princess3->x,princess3->y);
        }
        else {  //first generator
            dijkstra(0,0,generator->x,generator->y,mapa,n,m);
            printfpath(0,0,generator->x,generator->y);
            genActive=1;
            dijkstra(generator->x,generator->y,dragon->x,dragon->y,mapa,n,m);
            printfpath(generator->x,generator->y,dragon->x,dragon->y);
            dijkstra(dragon->x,dragon->y,princess1->x,princess1->y,mapa,n,m);
            printfpath(dragon->x,dragon->y,princess1->x,princess1->y);
            dijkstra(princess1->x,princess1->y, princess2->x,princess2->y,mapa,n,m);
            printfpath(princess1->x,princess1->y,princess2->x,princess2->y);
            dijkstra(princess2->x,princess2->y,princess3->x, princess3->y, mapa,n,m);
            printfpath(princess2->x,princess2->y,princess3->x,princess3->y);
        }
    }
    *dlzka_cesty=pathLength/2;
    genActive=0;
    return path;
}