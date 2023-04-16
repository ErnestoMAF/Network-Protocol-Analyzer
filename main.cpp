#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <locale.h>
#include <string>
#include <iomanip>
#include <iostream>

using namespace std;

int printNbytes(FILE *archivo,int start, int num_loops);

int main(){
    int i,aux_code;
    unsigned char palabra;
    setlocale(LC_ALL,"");
    FILE *archivo;
    
    if ((archivo = fopen("Test Packages\\ethernet_ipv4_icmp.bin","rb+")) == NULL)
        cout<<"Error en la apertura. Es posible que el fichero no exista \n";
    else{
        cout<<"\n"<<setw(28)<<"ETHERNET \n"<<endl;

        cout<<left<<setw(25)<<"Direccion MAC origen:"<<setw(5)<<"";
        fseek(archivo,printNbytes(archivo,ftell(archivo),6),SEEK_SET);

        cout<<endl<<left<<setw(25)<<"Direccion MAC destino:"<<setw(5)<<"";
        printNbytes(archivo,ftell(archivo),6);

        fseek(archivo,13, SEEK_SET);
        palabra=fgetc(archivo);
        cout<<endl<<left<<setw(25)<<"Tipo de código:"<<setw(5)<<"";
        printf ("%02x - ",palabra);

        aux_code = palabra;
        switch(aux_code){
            case 0:
            	cout<<"IPv4"<<endl;
            	break;
            case 6:
            	cout<<"ARP"<<endl;
            	break;
            case 53:
            	cout<<"RARP"<<endl;
            	break;
            case 221:
            	cout<<"IPv6"<<endl;
            	break;
            default:
                aux_code = -1;
            	cout<<"error"<<endl;
        }
        
        cout<<"\nDatos: \n";
        while (!feof(archivo)){
            palabra=fgetc(archivo);
            printf ("%02x:",palabra);
        }
    }
    return 0;
}


int printNbytes(FILE *archivo,int start, int num_loops){
    unsigned char palabra;
    fseek(archivo,start, SEEK_SET);

    for(int i=0;i<num_loops;i++){
        palabra = fgetc(archivo);
        printf("%02x:",palabra);
    }
   
    return ftell(archivo);
}