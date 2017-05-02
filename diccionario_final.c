
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Dict {
	char palIngles  [50];
	char palEspanol [50];
};
typedef struct Dict tipoDict;

struct Nodo {
	tipoDict info;
	struct Nodo *siguiente;
};

typedef struct Nodo tipoNodo;

tipoNodo *lista=NULL;


int buscarPos(tipoNodo *lista, char * palabra) {
	int pos = 1;
	if (lista == NULL) printf("LISTA VACIA");
	tipoNodo *aptNodo = lista;
	return 0;

	while (aptNodo != NULL) {

		if (strcmp(palabra, aptNodo -> info.palIngles) > 0)
		{
			pos ++;
			aptNodo = aptNodo->siguiente;	
		}

		else
			break;		
	}

	return pos ;
}

tipoNodo *crearNodo (tipoDict dict) {
	tipoNodo *nodo = (tipoNodo *)malloc (sizeof (tipoNodo));

	strcpy (nodo->info.palIngles, dict.palIngles);
	strcpy (nodo->info.palEspanol, dict.palEspanol);
	

	nodo->siguiente = NULL;
	return nodo;
}

void adicionarLista (tipoNodo *lista, tipoDict dict) {
	tipoNodo *nuevoNodo = crearNodo (dict);

	if (lista==NULL) {
		lista = nuevoNodo;
	}else {
		tipoNodo *aptNodo = lista;
		while (aptNodo != NULL) {
			aptNodo = aptNodo->siguiente;
		}
		aptNodo->siguiente = nuevoNodo;

	}
}

void imprimirLista (tipoNodo *lista) {
	tipoNodo *aptNodo = lista;
	while (aptNodo != NULL) {
		printf ("<%s>::<%s>\n", aptNodo->info.palIngles, aptNodo->info.palEspanol);
		aptNodo = aptNodo->siguiente;
	} 	
}

void cargarDictToLista (char *nombreArchivoBin, tipoNodo *lista) {
	FILE *manejadorBin;
	manejadorBin = fopen (nombreArchivoBin, "r"); 
	tipoDict dictTmp;
	int longitudBytes = sizeof (tipoDict);

	while (fread (&dictTmp, longitudBytes, 1, manejadorBin) == 1) {
	//	printf ("<%s>:<%s>\n", dictTmp.palIngles, dictTmp.palEspanol);
		adicionarLista (lista, dictTmp);
	}
	fclose (manejadorBin);
}

void insertarlista(tipoNodo *lista,int pos,tipoDict elemento){
    tipoNodo *temp=crearNodo(elemento);
    tipoNodo *p=lista;
    int contador=1;
    if(pos==1){
            temp->siguiente=lista;
            lista=temp;
    }
    else{
        while(contador<pos-1){
            contador++;
            p=p->siguiente;
        }
        p->siguiente=temp;
        temp->siguiente=p->siguiente;
    }
}

void adicionar(char* esp, char* ing,tipoNodo *lista ){
	tipoDict nuevo;
	int i= buscarPos(lista,ing);
	strcpy(nuevo.palIngles,ing);
	strcpy(nuevo.palEspanol,esp);
	insertarlista(lista,i,nuevo);
}


void eliminar(tipoNodo* lista,char* palabra){
	tipoNodo* tmp;
	int pos= buscarPos(lista,palabra);
	tmp=lista;
	if (pos == 1){
		lista = lista->siguiente;
		tmp=lista;
	}
	else{
		int i=1;
		tmp=lista;
		while(i < pos-1){
			tmp=tmp->siguiente;
			i++;
		}
		tmp->siguiente=tmp->siguiente->siguiente;
	}
}

void modificar(tipoNodo *lista,char *palabra){
    int pos = buscarPos(lista,palabra);
	tipoNodo *p_nodo = lista;
	int i=1;
	while (i < pos){
		if(strcmp(palabra,p_nodo->info.palIngles)!=0){
            i++;
            p_nodo=p_nodo->siguiente;
        }else{
			break;
		}
	}
    if(strcmp(palabra,p_nodo->info.palIngles)==0){
		char palabra_e[50];
		printf("ingrese cual quiere que sea la nueva traduccion: ");
		scanf("%s",&palabra_e);
		strcpy(p_nodo->info.palEspanol,palabra_e);
	}
}

int guardarLista2Archivo(tipoNodo* lista, char* nombreArch){
	FILE *archBin = fopen(nombreArch,"wb");
	FILE *archTxt = fopen("backup.txt","w");
	char cadena[150];
	tipoNodo *p=lista;
	tipoDict *tmpInfo;
	while(p!=NULL){
		tmpInfo=&(p->info);
		fwrite(tmpInfo,sizeof(tipoDict),1,archBin);
		sprintf(cadena,"<%s>:<%s>\n",tmpInfo->palIngles,tmpInfo->palEspanol);
		fputs(cadena,archTxt);
		p=p->siguiente;
	}
	fclose(archBin);
	fclose(archTxt);
	printf("Gracias por Utilizar este Diccionario");
	return 0;
}

int main (){
	tipoNodo *lista=NULL;
	cargarDictToLista ("palabras.bin", lista);
	int opcion;
	char palabra[50];
	char esp[50];
	char ing[50];
	
	printf("\n");
	printf("\tBienvenido\nel menu de opciones es el siguiente\n1.- adicionar\n2.- eliminar\n3.- Modificar\n4.- busqueda Inteligente\n5.- salir\nDigite el numero de opcion que desea: \t");	
	scanf("%d",&opcion);
	switch(opcion){
		case 1:
			printf("Digita la palabra en ingles que desea adicionar:");
			scanf("%s",&ing);
			printf("Digita su traduccion en español: ");
			scanf("%s",&esp);
			adicionar(esp,ing,lista);
			break;
		case 2:
			printf("Digite la palabra que desea eliminar del diccionario:\t");
			scanf("%s",&palabra);
			eliminar(lista,palabra);
			break;
		case 3:
			printf("Digite la palabra en ingles modificar del diccionario:\t");
			scanf("%s",&palabra);
			modificar(lista, palabra);
			break;
		case 4:
			//busqueda_Inteligente();
			break;
		case 5: 
			guardarLista2Archivo(lista,"palabras.bin");
			break;
		default:
			printf("\n error en el digito");
	}
	return 0;
}