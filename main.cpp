#include <stdlib.h>
#include <sstream>
#include <math.h>
#include <locale.h>
#include <string>
#include <iomanip>
#include <iostream>

#define SW 40
using namespace std;

int printNbytes(FILE *archivo,int start, int num_loops, char separator);
void bytesInArray(unsigned char palabra, unsigned char *arr, int pos);
int byteArrayToDecimal(int start, int end, unsigned char *arr);
string precedence(int value);
void protocol(int option,bool jmp,FILE *archivo,unsigned char *arr);

int main(){
    string ip;
    stringstream ss;
    int i,aux_code,aux_protocol;
    unsigned char palabra,bits16[16]={};
    setlocale(LC_ALL,"");
    FILE *archivo;
    
    if ((archivo = fopen("Test Packages\\ethernet_arp_reply.bin","rb+")) == NULL)
        cout<<"Error en la apertura. Es posible que el fichero no exista \n";
    else{
        cout<<"\n"<<setw(35)<<"ETHERNET \n"<<endl;

        cout<<left<<setw(SW)<<"Dirección MAC origen:"<<setw(5)<<"";
        fseek(archivo,printNbytes(archivo,ftell(archivo),6,':'),SEEK_SET);

        cout<<endl<<left<<setw(SW)<<"Dirección MAC destino:"<<setw(5)<<"";
        printNbytes(archivo,ftell(archivo),6,':');

        fseek(archivo,13, SEEK_SET);
        palabra=fgetc(archivo);
        cout<<endl<<left<<setw(SW)<<"Tipo de código:"<<setw(5)<<"";
        printf ("%02x - ",palabra);

        aux_code = palabra;
        switch(aux_code){
            case 0:
            	cout<<"IPv4"<<endl;
                palabra=fgetc(archivo);
                cout<<left<<setw(SW)<<"Versión:"<<setw(5)<<"";
                bytesInArray(palabra,bits16,7);cout<<byteArrayToDecimal(4,7,bits16)<<endl;
            	cout<<left<<setw(SW)<<"Tamaño de Cabecera:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,3,bits16)<< " palabras"<<endl;
                palabra=fgetc(archivo);
                cout<<left<<setw(SW)<<"Tipo de Servicio:"<<endl;
                bytesInArray(palabra,bits16,7);
                cout<<right<<setw(SW)<<"Prioridad:"<<setw(5)<<"";
                cout<<precedence(byteArrayToDecimal(5,7,bits16))<<endl;
                cout<<right<<setw(SW)<<"Retardo:"<<setw(5)<<"";
                if(bits16[4]=='0')
                    cout<<"0 - Normal"<<endl;
                else
                    cout<<"1 - Bajo"<<endl;
                cout<<right<<setw(SW)<<"Rendimiento:"<<setw(5)<<"";
                if(bits16[3]=='0')
                    cout<<"0 - Normal"<<endl;
                else
                    cout<<"1 - Alto"<<endl;
                cout<<right<<setw(SW)<<"Fiabilidad:"<<setw(5)<<"";
                if(bits16[2]=='0')
                    cout<<"0 - Normal"<<endl;
                else
                    cout<<"1 - Alta"<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,15);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(SW)<<"Longitud Total:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,15,bits16)<<" bytes" <<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,15);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(SW)<<"Identificador:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,15,bits16)<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,15);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(SW)<<"Banderas:"<<setw(5)<<""<<endl;
                cout<<right<<setw(SW)<<"Reservado:"<<setw(5)<<"";
                cout<<bits16[15]<<endl;
                cout<<right<<setw(SW)<<"Divisibilidad:"<<setw(5)<<"";
                if(bits16[14]=='0')
                    cout<<"0 - No divisible"<<endl;
                else
                    cout<<"1 - Divisible (DF)"<<endl;
                cout<<right<<setw(SW)<<"Último fragmento:"<<setw(5)<<"";
                if(bits16[13]=='0')
                    cout<<"0 - Último fragmento"<<endl;
                else
                    cout<<"1 - Fragmento intermedio (MF)"<<endl;
                cout<<left<<setw(SW)<<"Posición de Fragmento:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,12,bits16)<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<right<<setw(SW)<<"Tiempo de Vida:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,7,bits16)<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<right<<setw(SW)<<"Protocolo:"<<setw(5)<<"";
                aux_protocol=byteArrayToDecimal(0,7,bits16);
                protocol(aux_protocol,true,archivo,bits16);
                break;
            case 6:
            case 53:
                if(aux_code == 6)
                    cout<<"ARP"<<endl;
                else
                    cout<<"RARP"<<endl;
                fseek(archivo,14, SEEK_SET);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,15);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(SW)<<"Tipo de hardware:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,15,bits16);
                switch(byteArrayToDecimal(0,15,bits16)){
                    case 1:
                        cout<<" - Ethernet (10 Mb)"<<endl;
                        break;
                    case 6:
                        cout<<" - IEEE 802 Networks"<<endl;
                        break;
                    case 7:
                        cout<<" - ARCNET"<<endl;
                        break;
                    case 15:
                        cout<<" - Frame Relay"<<endl;
                        break;
                    case 16:
                        cout<<" - Asynchronous Transfer Mode (ATM)"<<endl;
                        break;
                    case 17:
                        cout<<" - HDLC"<<endl;
                        break;
                    case 18:
                        cout<<" - Fibre Channel"<<endl;
                        break;
                    case 19:
                        cout<<" - Asynchronous Transfer Mode (ATM)"<<endl;
                        break;
                    case 20:
                        cout<<" - Serial Line"<<endl;
                        break;
                    default:
                        cout<<" - Other."<<endl;
                }
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,15);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(SW)<<"Tipo de protocolo:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,15,bits16);
                switch(byteArrayToDecimal(0,15,bits16)){
                    case 2048:
                        cout<<" - IPv4"<<endl;
                        break;
                    case 2054:
                        cout<<" - ARP"<<endl;
                        break;
                    case 32821:
                        cout<<" - RARP"<<endl;
                        break;
                    case 34525:
                        cout<<" - IPv6"<<endl;
                        break;
                    default:
                        cout<<"Error. Tipo de protocolo"<<endl;
                }
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(SW)<<"Longitud de la dirección de hardware:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,7,bits16)<<" bytes"<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(SW)<<"Longitud de la dirección de protocolo:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,7,bits16)<<" bytes"<<endl;
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,15);
                palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
                cout<<left<<setw(SW)<<"Código de operación:"<<setw(5)<<"";
                cout<<byteArrayToDecimal(0,15,bits16);
                switch(byteArrayToDecimal(0,15,bits16)){
                	case 1:
                		cout<<" - Solicitud ARP"<<endl;
                		break;
                	case 2:
                		cout<<" - Respuesta ARP"<<endl;
                		break;
                	case 3:
                		cout<<" - Solicitud RARP"<<endl;
                		break;
                	case 4:
                		cout<<" - Respuesta RARP"<<endl;
                		break;
                	default:
                		cout<<"Error. Código de operación."<<endl;
                		
				}
                palabra = fgetc(archivo);
                cout<<left<<setw(SW)<<"Dirección hardware del emisor (MAC):"<<setw(5)<<"";
                fseek(archivo,printNbytes(archivo,ftell(archivo),6,':'),SEEK_SET);
                cout<<endl<<left<<setw(SW)<<"Dirección IP del emisor:"<<setw(5)<<"";
	            for(i=0;i<4;i++){
	            	palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
	                cout<<byteArrayToDecimal(0,7,bits16)<<".";
	            }            
                palabra = fgetc(archivo);
                cout<<endl<<left<<setw(SW)<<"Dirección hardware del receptor (MAC):"<<setw(5)<<"";
                fseek(archivo,printNbytes(archivo,ftell(archivo),6,':'),SEEK_SET);    
                cout<<endl<<left<<setw(SW)<<"Dirección IP del receptor:"<<setw(5)<<"";
	            for(i=0;i<4;i++){
	            	palabra=fgetc(archivo);bytesInArray(palabra,bits16,7);
	                cout<<byteArrayToDecimal(0,7,bits16)<<".";
	            }     

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


int printNbytes(FILE *archivo,int start, int num_loops,char separator){
    unsigned char palabra;
    fseek(archivo,start, SEEK_SET);

    for(int i=0;i<num_loops;i++){
        palabra = fgetc(archivo);
        printf("%02x%c",palabra,separator);
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
            priority="011: Relámpago";
            break;
        case 4:
            priority="100: Invalidación relámpago";
            break;
        case 5:
            priority="101: Procesando llamada critica y de emergencia";
            break;
        case 6:
            priority="110: Control de trabajo de internet";
            break;
        case 7:
            priority="111: Control de red"; 
            break;
        default:
            priority="Error. Prioridad incorrecta.";
    }

    return priority;
}

void protocol(int option,bool jmp,FILE *archivo,unsigned char *arr){
    unsigned char palabra;
    int value;

    switch(option){
        case 1:
            if(!jmp)
                cout <<"ICMP v4"<<endl;
            else{
                cout <<"ICMP v4"<<endl;
                fseek(archivo,34,SEEK_SET);
                palabra = fgetc(archivo);bytesInArray(palabra,arr,7);
                cout<<right<<setw(SW)<<"Tipo:"<<setw(5)<<"";
                value = byteArrayToDecimal(0,7,arr);
                cout<<value;
                switch(value){
                    case 0:
                        cout<<" - Respuesta de ECO"<<endl;
                        break;
                    case 3:
                        cout<<" - Destino inaccesible"<<endl;
                        break;
                    case 4:
                        cout<<" - Disminución de tráfico desde el origen"<<endl;
                        break;
                    case 5:
                        cout<<" - Redireccionar - cambio de ruta"<<endl;
                        break;
                    case 8:
                        cout<<" - Solicitud de ECO"<<endl;
                        break;
                    case 11:
                        cout<<" - Tiempo excedido para un datagrama"<<endl;
                        break;
                    case 12:
                        cout<<" - Problema de parámetros"<<endl;
                        break;
                    case 13:
                        cout<<" - Solicitud de marca de tiempo"<<endl;
                        break;
                    case 14:
                        cout<<" - Respuesta de marca de tiempo"<<endl;
                        break;
                    case 15:
                        cout<<" - Solicitud de información - obsoleto-"<<endl;
                        break;
                    case 16:
                        cout<<" - Respuesta de información - obsoleto-"<<endl;
                        break;
                    case 17:
                        cout<<" - Solicitud de mascara de dirección"<<endl;
                        break;
                    case 18:
                        cout<<" - Respuesta de mascara de dirección"<<endl;
                        break;
                    default:
                        cout<<"Error. Tipo"<<endl;
                }
                palabra = fgetc(archivo);bytesInArray(palabra,arr,7);
                cout<<right<<setw(SW)<<"Código:"<<setw(5)<<"";
                value = byteArrayToDecimal(0,7,arr);
                cout<<value;
                switch(value){
                    case 0:
                        cout<<" - No se puede llegar a la red"<<endl;
                        break;
                    case 1:
                        cout<<" - No se puede llegar al host o aplicación de destino"<<endl;
                        break;
                    case 2:
                        cout<<" - El destino no dispone del protocolo solicitados"<<endl;
                        break;
                    case 3:
                        cout<<" - No se puede llegar al puerto destino o la aplicación destino no esta libre"<<endl;
                        break;
                    case 4:
                        cout<<" - Se necesita aplicar fragmentación, para el flag correspondiente indica lo contrario"<<endl;
                        break;
                    case 5:
                        cout<<" - La ruta de origen es correcta"<<endl;
                        break;
                    case 6:
                        cout<<" - No se conoce la red destino"<<endl;
                        break;
                    case 7:
                        cout<<" - No se conoce el host destino"<<endl;
                        break;
                    case 8:
                        cout<<" - El host origen esta aislado"<<endl;
                        break;
                    case 9:
                        cout<<" - La comunicación con la red destino esta prohibida por razones administrativas"<<endl;
                        break;
                    case 10:
                        cout<<" - La comunicación con el host destino esta prohibida por razones administrativas"<<endl;
                        break;
                    case 11:
                        cout<<" - No se puede llegar a la red de destino debido al tipo de servicio"<<endl;
                        break;
                    case 12:
                        cout<<" - No se puede llegar a la host de destino debido al tipo de servicio"<<endl;
                        break;
                    default:
                        cout<<"Error. Código."<<endl;
                }
                palabra = fgetc(archivo);
                cout<<right<<setw(SW)<<"Checksum:"<<setw(5)<<"";
                printNbytes(archivo,ftell(archivo),2,' ');
            }
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