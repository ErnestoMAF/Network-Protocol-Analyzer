#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <locale.h>
#include <string>
#include <iomanip>
#include <iostream>

using namespace std;

int printNbytes(FILE *archivo,int start, int num_loops);
void bytesInArray(unsigned char palabra, unsigned char *arr, int pos);
int byteArrayToDecimal(int start, int end, unsigned char *arr);
string precedence(int value);
void protocol(int option,bool jmp);

int main(){
    int i,aux_code;
    unsigned char palabra,bits16[16]={};
    setlocale(LC_ALL,"");
    FILE *archivo;
    
    if ((archivo = fopen("Test Packages\\ethernet_ipv4_icmp.bin","rb+")) == NULL)
        cout<<"Error en la apertura. Es posible que el fichero no exista \n";
    else{
        cout<<"\n"<<setw(28)<<"ETHERNET \n"<<endl;

        cout<<left<<setw(25)<<"Dirección MAC origen:"<<setw(5)<<"";
        fseek(archivo,printNbytes(archivo,ftell(archivo),6),SEEK_SET);

        cout<<endl<<left<<setw(25)<<"Dirección MAC destino:"<<setw(5)<<"";
        printNbytes(archivo,ftell(archivo),6);

        fseek(archivo,13, SEEK_SET);
        palabra=fgetc(archivo);
        cout<<endl<<left<<setw(25)<<"Tipo de código:"<<setw(5)<<"";
        printf ("%02x - ",palabra);

        aux_code = palabra;
        switch(aux_code){
            case 0:
            	cout<<"IPv4"<<endl;
                palabra=fgetc(archivo);
                cout<<left<<setw(25)<<"Versión:"<<setw(5)<<"";
                bytesInArray(palabra,bits16,7);cout<<byteArrayToDecimal(4,7,bits16)<<endl;
            	cout<<left<<setw(25)<<"Tamaño de Cabecera:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,3,bits16)<< " palabras"<<endl;
                palabra=fgetc(archivo);
                cout<<left<<setw(25)<<"Tipo de Servicio:"<<endl;
                bytesInArray(palabra,bits16,7);
                cout<<right<<setw(25)<<"Prioridad:"<<setw(5)<<"";
                cout<<precedence(byteArrayToDecimal(5,7,bits16))<<endl;
                cout<<right<<setw(25)<<"Retardo:"<<setw(5)<<"";
                if(bits16[4]=='0')
                    cout<<"0 - Normal"<<endl;
                else
                    cout<<"1 - Bajo"<<endl;
                cout<<right<<setw(25)<<"Rendimiento:"<<setw(5)<<"";
                if(bits16[3]=='0')
                    cout<<"0 - Normal"<<endl;
                else
                    cout<<"1 - Alto"<<endl;
                cout<<right<<setw(25)<<"Fiabilidad:"<<setw(5)<<"";
                if(bits16[2]=='0')
                    cout<<"0 - Normal"<<endl;
                else
                    cout<<"1 - Alta"<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,15);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(25)<<"Longitud Total:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,15,bits16)<<" bytes" <<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,15);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(25)<<"Identificador:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,15,bits16)<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,15);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(25)<<"Banderas:"<<setw(5)<<""<<endl;
                cout<<right<<setw(25)<<"Reservado:"<<setw(5)<<"";
                cout<<bits16[15]<<endl;
                cout<<right<<setw(25)<<"Divisibilidad:"<<setw(5)<<"";
                if(bits16[14]=='0')
                    cout<<"0 - No divisible"<<endl;
                else
                    cout<<"1 - Divisible (DF)"<<endl;
                cout<<right<<setw(25)<<"Ultimo fragmento:"<<setw(5)<<"";
                if(bits16[13]=='0')
                    cout<<"0 - Último fragmento"<<endl;
                else
                    cout<<"1 - Fragmento intermedio (MF)"<<endl;
                cout<<left<<setw(25)<<"Posición de Fragmento:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,12,bits16)<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<right<<setw(25)<<"Tiempo de Vida:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,7,bits16)<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<right<<setw(25)<<"Protocolo:"<<setw(5)<<"";
                protocol(byteArrayToDecimal(0,7,bits16),false);
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
            	cout<<"Error."<<endl;
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

void bytesInArray(unsigned char palabra, unsigned char *arr, int pos){
    for(int i=7;i>=0;i--)
        *(arr+pos--)=((palabra & (1 << i)) ? '1' : '0');
}

int byteArrayToDecimal(int start, int end, unsigned char *arr){
    int sum = 0,cont = 0;

    for(int i=start;i<end;i++){
        if(*(arr+i)=='1')
            sum += pow(2,cont);
        cont++;
    }

    return sum;
}

string precedence(int value){
    string priority;

    switch(value){
        case 0:
            priority="000: De rutina";
            break;
        case 1:
            priority="001: Prioritario";
            break;
        case 2:
            priority="010: Inmediato";
            break;
        case 3:
            priority="011: Relampago";
            break;
        case 4:
            priority="100: Invalidacion relampago";
            break;
        case 5:
            priority="101: Procesando llamada critica y de emergencia";
            break;
        case 6:
            priority="110: Control de trabajo de Internet";
            break;
        case 7:
            priority="111: Control de Red"; 
            break;
        default:
            priority="Incorrecta";
    }

    return priority;
}

void protocol(int option,bool jmp){
    unsigned char palabra;

    switch(option){
        case 1:
            if(!jmp)
                cout <<"ICMP v4"<<endl;
            break;
        case 6:
            if(!jmp)
                cout <<"TCP"<<endl;
            break;               
        case 17:
            if(!jmp)
                cout <<"UDP"<<endl;
            break;
        case 58:
            if(!jmp)
                cout <<"ICMP v6"<<endl;
            break;
        case 118:
            if(!jmp)
                cout <<"STP"<<endl;
            break;
        case 121:
            if(!jmp)
                cout <<"SMP"<<endl;
            break;
        default:
            cout <<"Error. No existe protocolo."<<endl;
    }
}